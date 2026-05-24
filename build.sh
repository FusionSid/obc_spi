#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/Debug"

echo "Building STM32 project..."

cd "$BUILD_DIR"

rm -f compile_commands.json

bear -- make -j7 --silent all

sed -i '' 's/-fcyclomatic-complexity//g' compile_commands.json

echo ""
echo "Build complete."
echo "compile_commands.json updated."