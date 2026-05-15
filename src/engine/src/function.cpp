#include "function.hpp"
#include "renderer.hpp"

#include <logging.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <cctype>
#include <stdexcept>
#include <limits>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E  2.71828182845904523536
#endif

// =============================================================================
// Constructor
// =============================================================================

FunctionPlot::FunctionPlot(Registry&          registry,
                           const std::string& expression,
                           float              xMin,
                           float              xMax,
                           int                samples,
                           glm::vec4          color,
                           float              thickness)
    : m_expression(expression)
    , m_xMin(xMin)
    , m_xMax(xMax)
    , m_samples(samples)
    , m_color(color)
    , m_thickness(thickness)
{
    m_mesh   = registry.getMesh("square");
    m_shader = registry.getShader("basic");

    ASSERT(m_mesh,   "FunctionPlot: Registry did not return a 'square' mesh");
    ASSERT(m_shader, "FunctionPlot: Registry did not return a 'basic' shader");

    rebuild();
}

// =============================================================================
// Setters
// =============================================================================

void FunctionPlot::setExpression(const std::string& expression)
{
    m_expression = expression;
    rebuild();
}

void FunctionPlot::setRange(float xMin, float xMax)
{
    ASSERT(xMin < xMax, "FunctionPlot::setRange — xMin must be less than xMax");
    m_xMin = xMin;
    m_xMax = xMax;
    rebuild();
}

void FunctionPlot::setSamples(int samples)
{
    ASSERT(samples >= 2, "FunctionPlot::setSamples — need at least 2 samples");
    m_samples = samples;
    rebuild();
}

void FunctionPlot::setColor(glm::vec4 color)
{
    m_color = color;
    // Recolor existing segments without a full resample
    for (auto& seg : m_segments)
        seg.color = color;
}

void FunctionPlot::setThickness(float thickness)
{
    m_thickness = thickness;
    rebuild();
}

// =============================================================================
// Draw
// =============================================================================

void FunctionPlot::draw() const
{
    if (!m_valid) return;

    for (const auto& seg : m_segments)
        Renderer::draw(seg);
}

// =============================================================================
// Rebuild — sample the function and build line-segment entities
// =============================================================================

void FunctionPlot::rebuild()
{
    m_segments.clear();
    m_valid = false;

    if (m_expression.empty()) return;

    // Validate the expression with a test evaluation before sampling
    try {
        evaluate(0.0f);
    }
    catch (const std::exception& ex) {
        LOG_WARN("FunctionPlot: invalid expression '{}' — {}", m_expression, ex.what());
        return;
    }

    m_valid = true;
    m_segments.reserve(m_samples - 1);

    // --- Sample all x values -------------------------------------------------
    struct Point { float x, y; };
    std::vector<Point> pts;
    pts.reserve(m_samples);

    float step = (m_xMax - m_xMin) / static_cast<float>(m_samples - 1);

    for (int i = 0; i < m_samples; ++i) {
        float x = m_xMin + i * step;
        float y = NAN;

        try { y = evaluate(x); }
        catch (...) { /* malformed at this x — leave as NaN */ }

        // Treat non-finite results as gaps (handles 1/0, log(-1), etc.)
        if (!std::isfinite(y)) y = NAN;

        pts.push_back({ x, y });
    }

    // --- Build one Entity per consecutive pair of valid points ---------------
    for (int i = 0; i + 1 < static_cast<int>(pts.size()); ++i) {
        const Point& a = pts[i];
        const Point& b = pts[i + 1];

        // Skip segments that cross a discontinuity / undefined region
        if (std::isnan(a.y) || std::isnan(b.y)) continue;

        glm::vec2 pa = { a.x, a.y };
        glm::vec2 pb = { b.x, b.y };

        glm::vec2 delta  = pb - pa;
        float     length = glm::length(delta);

        if (length < 1e-7f) continue; // coincident points — nothing to draw

        float     angle  = glm::degrees(std::atan2(delta.y, delta.x));
        glm::vec2 mid    = (pa + pb) * 0.5f;

        Entity seg(m_mesh, m_shader, m_color);
        seg.transform.position  = glm::vec3(mid, Z_LAYER);
        seg.transform.rotationZ = angle;
        seg.transform.scale     = glm::vec3(length, m_thickness, 1.0f);

        m_segments.push_back(std::move(seg));
    }

    LOG_INFO("FunctionPlot: '{}' built {} segments", m_expression, m_segments.size());
}

// =============================================================================
// Evaluator entry point
// =============================================================================

float FunctionPlot::evaluate(float x) const
{
    m_src      = m_expression;
    m_pos      = 0;
    m_currentX = x;

    float result = parseExpr();

    skipWS();
    if (m_pos != m_src.size())
        throw std::runtime_error(
            std::string("unexpected character '") + m_src[m_pos] + "' at position " +
            std::to_string(m_pos));

    return result;
}

// =============================================================================
// Tokenizer helpers
// =============================================================================

void FunctionPlot::skipWS() const
{
    while (m_pos < m_src.size() && std::isspace(static_cast<unsigned char>(m_src[m_pos])))
        ++m_pos;
}

float FunctionPlot::parseNumber() const
{
    size_t start = m_pos;

    while (m_pos < m_src.size() && std::isdigit(static_cast<unsigned char>(m_src[m_pos])))
        ++m_pos;

    if (m_pos < m_src.size() && m_src[m_pos] == '.') {
        ++m_pos;
        while (m_pos < m_src.size() && std::isdigit(static_cast<unsigned char>(m_src[m_pos])))
            ++m_pos;
    }

    // Scientific notation  (e.g. 1e-4, 2.5E+10)
    if (m_pos < m_src.size() && (m_src[m_pos] == 'e' || m_src[m_pos] == 'E')) {
        size_t ePos = m_pos++;
        if (m_pos < m_src.size() && (m_src[m_pos] == '+' || m_src[m_pos] == '-'))
            ++m_pos;
        if (m_pos < m_src.size() && std::isdigit(static_cast<unsigned char>(m_src[m_pos]))) {
            while (m_pos < m_src.size() && std::isdigit(static_cast<unsigned char>(m_src[m_pos])))
                ++m_pos;
        } else {
            // 'e' was not part of a number — roll back to before 'e'
            m_pos = ePos;
        }
    }

    return std::stof(m_src.substr(start, m_pos - start));
}

std::string FunctionPlot::parseIdentifier() const
{
    size_t start = m_pos;
    while (m_pos < m_src.size() &&
           (std::isalpha(static_cast<unsigned char>(m_src[m_pos])) || m_src[m_pos] == '_'))
        ++m_pos;
    return m_src.substr(start, m_pos - start);
}

// =============================================================================
// Recursive descent parser
//
//   expr    →  term  ( ( '+' | '-' )  term  )*
//   term    →  power ( ( '*' | '/' )  power )*
//   power   →  unary ( '^'  unary )*                 [right-associative]
//   unary   →  ( '-' | '+' ) unary  |  primary
//   primary →  NUMBER
//            |  'x'
//            |  'pi'  |  'e'
//            |  IDENT '(' expr ')'
//            |  '(' expr ')'
// =============================================================================

float FunctionPlot::parseExpr() const
{
    float result = parseTerm();
    skipWS();

    while (m_pos < m_src.size() && (m_src[m_pos] == '+' || m_src[m_pos] == '-')) {
        char op = m_src[m_pos++];
        float rhs = parseTerm();
        result = (op == '+') ? result + rhs : result - rhs;
        skipWS();
    }

    return result;
}

float FunctionPlot::parseTerm() const
{
    float result = parsePower();
    skipWS();

    while (m_pos < m_src.size() && (m_src[m_pos] == '*' || m_src[m_pos] == '/')) {
        char op = m_src[m_pos++];
        float rhs = parsePower();
        if (op == '/' && rhs == 0.0f) return std::numeric_limits<float>::quiet_NaN();
        result = (op == '*') ? result * rhs : result / rhs;
        skipWS();
    }

    return result;
}

float FunctionPlot::parsePower() const
{
    float base = parseUnary();
    skipWS();

    if (m_pos < m_src.size() && m_src[m_pos] == '^') {
        ++m_pos;
        float exp = parseUnary(); // right-associative: recurse into unary, not power
        return std::pow(base, exp);
    }

    return base;
}

float FunctionPlot::parseUnary() const
{
    skipWS();

    if (m_pos < m_src.size() && m_src[m_pos] == '-') {
        ++m_pos;
        return -parseUnary();
    }
    if (m_pos < m_src.size() && m_src[m_pos] == '+') {
        ++m_pos;
        return  parseUnary();
    }

    return parsePrimary();
}

float FunctionPlot::parsePrimary() const
{
    skipWS();

    if (m_pos >= m_src.size())
        throw std::runtime_error("unexpected end of expression");

    // ---- Number -------------------------------------------------------------
    if (std::isdigit(static_cast<unsigned char>(m_src[m_pos])) || m_src[m_pos] == '.')
        return parseNumber();

    // ---- Parenthesised sub-expression ---------------------------------------
    if (m_src[m_pos] == '(') {
        ++m_pos;
        float val = parseExpr();
        skipWS();
        if (m_pos >= m_src.size() || m_src[m_pos] != ')')
            throw std::runtime_error("expected closing ')'");
        ++m_pos;
        return val;
    }

    // ---- Identifier: variable, constant, or function call -------------------
    if (std::isalpha(static_cast<unsigned char>(m_src[m_pos])) || m_src[m_pos] == '_') {
        std::string id = parseIdentifier();

        // Variable
        if (id == "x")  return m_currentX;

        // Constants
        if (id == "pi") return static_cast<float>(M_PI);
        if (id == "e")  return static_cast<float>(M_E);

        // All remaining identifiers are functions — require '(' arg ')'
        skipWS();
        if (m_pos >= m_src.size() || m_src[m_pos] != '(')
            throw std::runtime_error("expected '(' after '" + id + "'");
        ++m_pos;

        float arg = parseExpr();

        skipWS();
        if (m_pos >= m_src.size() || m_src[m_pos] != ')')
            throw std::runtime_error("expected ')' after argument of '" + id + "'");
        ++m_pos;

        // Trig
        if (id == "sin")   return std::sin(arg);
        if (id == "cos")   return std::cos(arg);
        if (id == "tan")   return std::tan(arg);
        if (id == "asin")  return std::asin(arg);
        if (id == "acos")  return std::acos(arg);
        if (id == "atan")  return std::atan(arg);

        // Exponential / logarithmic
        if (id == "exp")   return std::exp(arg);
        if (id == "ln")    return std::log(arg);
        if (id == "log")   return std::log10(arg);
        if (id == "log2")  return std::log2(arg);
        if (id == "log10") return std::log10(arg);

        // Misc
        if (id == "sqrt")  return std::sqrt(arg);
        if (id == "abs")   return std::abs(arg);
        if (id == "floor") return std::floor(arg);
        if (id == "ceil")  return std::ceil(arg);
        if (id == "round") return std::round(arg);

        throw std::runtime_error("unknown function: '" + id + "'");
    }

    throw std::runtime_error(
        std::string("unexpected character '") + m_src[m_pos] + "'");
}
