# Contributing to MDNGE

As-salamu alaykum, This document outlines how to set up your environment and the standards we follow to keep the engine buildable and maintainable for everyone.

---

## 1. System Prerequisites

All dependencies for this project are listed in `manifest.json`. Ultimately, it is up to the developer how to manage and install these dependencies, provided they are structured as shown below:

```text
$PROJECT_ROOT/deps
├── include
│   ├── glad
│   ├── glfw
│   ├── glm
│   ├── imgui
│   ├── lua
│   ├── miniaudio
│   ├── sol
│   ├── spdlog
│   └── stb
├── lib
│   ├── libglfw3.a
│   ├── liblua.a
│   └── libspdlog.a
└── src
    ├── glad
    ├── glfw-3.4
    ├── glm
    ├── imgui-1.92.6
    ├── lua-5.4.0
    ├── spdlog-1.17.0
    └── stb-master

# AND

$PROJECT_ROOT/src
└── vendor
    ├── glad.c
    └── imgui
        ├── backends
        │   ├── imgui_impl_glfw.cpp
        │   ├── imgui_impl_glfw.h
        │   ├── imgui_impl_opengl3.cpp
        │   └── imgui_impl_opengl3.h
```

The `requirements.sh` script is provided to handle all necessary dependency resolution. Note that this script is currently designed for **Linux with X11** only. macOS and wayland users may need to modify the build process for specific dependencies (such as `lua` and `spdlog`). If you are contributing from Windows, the use of **WSL** is recommended.
All told, the developer must be able to get the exact structure shown above regardless of the platform or the package management system.

---

## 2. Getting Started

1.  **Clone the Repository:**

    ```bash
    # Via SSH (Recommended)
    git clone git@github.com:Tarek-L/MDNGE.git

    # Via HTTPS
    git clone https://github.com/Tarek-L/MDNGE
    ```

2.  **Configure & Build:**
    - You are free to use any Text Editor or IDE (Vim, MVS, etc.).
    - Local test builds are managed by the contributor. Official release builds are performed on the maintainer's machines.

---

## 3. Coding Standards

- **Language:** C++20.
- **Naming Conventions:**
  - `CamelCase` for Classes and Structs.
  - `camelCase` for variables and functions.
  - `m_camelCase` for private class members.
- **Logging:** Use `spdlog` for all console output and debugging.
- **Headers:** Use `#pragma once` for header guards.

---

## 4. Git Workflow

1.  **Branching:** Never commit directly to `main`. Create a feature branch using the format: `type/short-description` (e.g., `feature/add-renderer-system`).
2.  **Pull Requests:** All code must be reviewed by at least two other team members before being merged into `main`.
3.  **Commit Messages:** We follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) specification. Include your GitHub username in parentheses at the end of the PR title.

### PR Title Types:

- `feat`: A new feature
- `impr`: An improvement to an existing feature
- `fix`: A bug fix
- `docs`: Documentation changes
- `style`: Formatting, missing semi-colons, etc. (no code logic changes)
- `refactor`: Code changes that neither fix a bug nor add a feature
- `perf`: Code changes that improve performance
- `build`: Changes to the build system or external dependencies
- `chore`: Routine tasks or maintenance

**Example:** `feat: add vertex buffer abstraction (@github_username)`

---
