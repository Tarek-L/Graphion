#!/usr/bin/bash

# this script is written for linux so you may want to use it or use wsl, it can work on mac and wayland after some small changes

set -e

PROJECT_ROOT=$(pwd)
DEPS_DIR="$PROJECT_ROOT/deps"
SRC_DIR="$DEPS_DIR/src"
INC_DIR="$DEPS_DIR/include"
LIB_DIR="$DEPS_DIR/lib"

mkdir -p "$SRC_DIR" "$INC_DIR" "$LIB_DIR" 

while getopts "c" opt; do
  case $opt in
    c)
      echo "cleaning"
      rm -fr "$PROJECT_ROOT"/deps/lib/*
      rm -fr "$PROJECT_ROOT"/deps/src/*
      rm -fr "$PROJECT_ROOT"/deps/include/*
      rm -fr "$PROJECT_ROOT"/src/vendor
      exit 0
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

echo "--- Starting Dependency Fetch ---"

fetch_and_unpack() {
    local url=$1
    local folder=$2
    local tarball="temp_archive.tar.gz"

    echo "Fetching $folder..."
    cd "$SRC_DIR"
    curl -L "$url" -o "$tarball"
    
    if [[ "$url" == *.zip ]]; then
        unzip -q "$tarball"
        rm "$tarball"
    else
        tar -xzf "$tarball"
        rm "$tarball"
    fi
}

# 1. LUA
fetch_and_unpack "https://www.lua.org/ftp/lua-5.4.7.tar.gz" "lua"
cd "$SRC_DIR/lua-5.4.7"
make linux 

mkdir -p "$INC_DIR/lua"
cp src/liblua.a "$LIB_DIR/"
cp src/lua.h src/luaconf.h src/lualib.h src/lauxlib.h src/lua.hpp "$INC_DIR/lua/"
echo "Lua installed to include/lua/"

# 2. GLM 
fetch_and_unpack "https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip" "glm"
mkdir -p "$INC_DIR/glm"
cp -r "$SRC_DIR/glm/glm"/* "$INC_DIR/glm/"
echo "GLM installed to include/glm/"

# 3. SPDLOG
fetch_and_unpack "https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz" "spdlog"

cd "$SRC_DIR/spdlog-1.17.0"
mkdir -p build && cd build

cmake .. -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_BUILD_TESTS=OFF -DSPDLOG_USE_STD_FORMAT=ON
cmake --build . --config Release -j$(nproc)

mkdir -p "$INC_DIR/spdlog"
cp -r ../include/spdlog/* "$INC_DIR/spdlog/"
cp $(find . -name "libspdlog.a") "$LIB_DIR/"

echo "spdlog compiled and installed to include/spdlog/ and lib/libspdlog.a"

# 4. STB 
echo "Fetching stb..."
fetch_and_unpack "https://github.com/nothings/stb/archive/refs/heads/master.tar.gz" "stb"

mkdir -p "$INC_DIR/stb"

cp "$SRC_DIR/stb-master/stb_image.h" "$INC_DIR/stb/"
cp "$SRC_DIR/stb-master/stb_image_write.h" "$INC_DIR/stb/"

echo "stb headers installed to include/stb/"

# 5. GLFW

echo "Fetching and building GLFW 3.4..."
fetch_and_unpack "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" "glfw"

cd "$SRC_DIR/glfw-3.4"
mkdir -p build && cd build

cmake .. \
    -DGLFW_BUILD_WAYLAND=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DGLFW_BUILD_EXAMPLES=OFF \
    -DGLFW_BUILD_TESTS=OFF \
    -DGLFW_BUILD_DOCS=OFF \
    -DCMAKE_BUILD_TYPE=Release

cmake --build . -j$(nproc)

mkdir -p "$INC_DIR/glfw"

cp -r ../include/GLFW/* "$INC_DIR/GLFW"

cp $(find . -name "libglfw3.a") "$LIB_DIR/"

echo "GLFW 3.4 installed to include/GLFW/ and lib/libglfw3.a"

# 6. GLAD
# go to https://glad.dav1d.de/ to generate the required zip or you you can use python
# see that you have to unzip the glad.zip zip generated in the deps/src dir before running this script
# API + profile:
# 3.3 core 
# extenstions: 
# GL_ARB_debug_output
# GL_ARB_multisample
# GL_ARB_texture_float 

mkdir -p "$INC_DIR/glad"
mkdir -p "$INC_DIR/KHR"
cd "$SRC_DIR"
unzip glad.zip -d glad
rm glad.zip
cd "$PROJECT_ROOT"
cp -r "$SRC_DIR/glad/include/glad"/* "$INC_DIR/glad/"
cp -r "$SRC_DIR/glad/include/KHR"/* "$INC_DIR/KHR/"

mkdir -p "$PROJECT_ROOT/src/vendor"
cp "$SRC_DIR/glad/src/glad.c" "$PROJECT_ROOT/src/vendor/"

echo "GLAD headers installed to include/glad/ and source to src/vendor/glad"

# 7. MINIAUDIO

echo "Fetching miniaudio..."
mkdir -p "$INC_DIR/miniaudio"

curl -L "https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h" \
    -o "$INC_DIR/miniaudio/miniaudio.h"

echo "miniaudio header installed to include/miniaudio/"

# 8. SOL2

echo "Fetching sol2..."
mkdir -p "$INC_DIR/sol"

curl -L "https://github.com/ThePhD/sol2/releases/download/v3.3.0/sol.hpp" \
    -o "$INC_DIR/sol/sol.hpp"

echo "sol2 header installed to include/sol/sol.hpp"

# 9. IMGUI 

echo "Fetching and preparing ImGui v1.92.6 for OpenGL 4.6..."
fetch_and_unpack "https://github.com/ocornut/imgui/archive/refs/tags/v1.92.6.tar.gz" "imgui"

mkdir -p "$INC_DIR/imgui"
mkdir -p "$PROJECT_ROOT/src/vendor/imgui/backends"

cd "$SRC_DIR/imgui-1.92.6"

cp *.h "$INC_DIR/imgui/"

cp *.cpp "$PROJECT_ROOT/src/vendor/imgui/"
cp backends/imgui_impl_glfw.{h,cpp} "$PROJECT_ROOT/src/vendor/imgui/backends/"
cp backends/imgui_impl_opengl3.{h,cpp} "$PROJECT_ROOT/src/vendor/imgui/backends/"

echo "ImGui headers installed to include/imgui/ and source to src/vendor/imgui/"

echo "--- Done. Dependencies organized in $DEPS_DIR ---"

