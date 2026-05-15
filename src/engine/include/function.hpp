#pragma once

#include "entity.hpp"
#include "registry.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

/**
 * @class FunctionPlot
 * @brief Samples and renders a mathematical function f(x) as a polyline.
 *
 * Pass any expression string using `x` as the variable:
 *
 *   FunctionPlot plot(registry, "x^2");
 *   FunctionPlot plot(registry, "sin(x) * 2");
 *   FunctionPlot plot(registry, "1/x");          // gap at x=0 handled automatically
 *   FunctionPlot plot(registry, "exp(-x^2)");    // Gaussian
 *
 * Supported syntax:
 *   Operators  :  +  -  *  /  ^  (unary minus)
 *   Variable   :  x
 *   Constants  :  pi   e
 *   Functions  :  sin  cos  tan  asin  acos  atan
 *                 exp  ln  log  log2  log10
 *                 sqrt  abs  floor  ceil  round
 *   Grouping   :  ( )
 *
 * NaN / Inf results (division by zero, sqrt of negative, etc.) are skipped
 * silently — the curve simply has a gap at those x values.
 *
 * Call setExpression() to change the function at runtime. All other setters
 * also trigger an immediate rebuild.
 *
 * Typical usage:
 *   FunctionPlot plot(registry, "sin(x)", -6.28f, 6.28f);
 *
 *   // game loop:
 *   plot.draw();
 */
class FunctionPlot
{
public:
    /**
     * @param registry   Source for the square mesh and basic shader.
     * @param expression Math expression string (use 'x' as the variable).
     * @param xMin       Left bound of the sampling domain.
     * @param xMax       Right bound of the sampling domain.
     * @param samples    Number of sample points (more = smoother curve).
     * @param color      RGBA colour of the curve.
     * @param thickness  World-space thickness of each line segment.
     */
    FunctionPlot(Registry&          registry,
                 const std::string& expression,
                 float              xMin      = -10.0f,
                 float              xMax      =  10.0f,
                 int                samples   =  300,
                 glm::vec4          color     =  glm::vec4(1.0f, 0.75f, 0.2f, 1.0f),
                 float              thickness =  0.04f);

    // -------------------------------------------------------------------------
    // Setters  (each triggers a rebuild)
    // -------------------------------------------------------------------------
    void setExpression(const std::string& expression);
    void setRange(float xMin, float xMax);
    void setSamples(int samples);
    void setColor(glm::vec4 color);
    void setThickness(float thickness);

    // -------------------------------------------------------------------------
    // Accessors
    // -------------------------------------------------------------------------
    const std::string& getExpression() const { return m_expression; }
    bool               isValid()       const { return m_valid;      }
    int                segmentCount()  const { return static_cast<int>(m_segments.size()); }

    // -------------------------------------------------------------------------
    // Per-frame
    // -------------------------------------------------------------------------
    void draw() const;

    /** Segments render at this z — in front of graph edges, behind vertices. */
    static constexpr float Z_LAYER = -0.05f;

private:
    // ---- rebuild ------------------------------------------------------------
    void  rebuild();

    // ---- expression evaluator -----------------------------------------------
    float evaluate(float x) const;

    // Parser state — mutable so evaluate() can be const
    mutable std::string m_src;
    mutable size_t      m_pos      = 0;
    mutable float       m_currentX = 0.0f;

    void        skipWS()            const;
    float       parseNumber()       const;
    std::string parseIdentifier()   const;

    float parseExpr()    const;   // handles + and -
    float parseTerm()    const;   // handles * and /
    float parsePower()   const;   // handles ^ (right-associative)
    float parseUnary()   const;   // handles unary minus / plus
    float parsePrimary() const;   // number | variable | constant | func(expr) | (expr)

    // ---- render data --------------------------------------------------------
    std::vector<Entity> m_segments;

    Mesh*   m_mesh;
    Shader* m_shader;

    // ---- settings -----------------------------------------------------------
    std::string m_expression;
    float       m_xMin, m_xMax;
    int         m_samples;
    glm::vec4   m_color;
    float       m_thickness;
    bool        m_valid = false;
};
