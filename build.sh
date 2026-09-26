#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/Debug"

# mac stuff
TOOLCHAIN_BIN=$(find /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins \
    -maxdepth 1 -iname "com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.*" \
    -exec echo {}/tools/bin \; | head -n1)

export PATH="$TOOLCHAIN_BIN:$PATH"

build() {
    cd "$BUILD_DIR"
    make -j7 all
}

compile_db() { # so vscode doesnt cry
    cd "$BUILD_DIR"

    echo "Regenerating compile_commands.json..."

    rm -f compile_commands.json
    make clean
    bear -- make -j7 all

    sed -i '' 's/-fcyclomatic-complexity//g' compile_commands.json

    echo "compile_commands.json updated."
}

flash() {
    cd "$BUILD_DIR"

    arm-none-eabi-objcopy -O binary apss_spi.elf apss_spi.bin

    st-flash --connect-under-reset write apss_spi.bin 0x08000000
}

case "${1:-build}" in
    build)
        build
        ;;
    compile-db)
        compile_db
        ;;
    flash)
        flash
        ;;
    both)
        build
        flash
        ;;
    *)
        echo "Usage: $0 [build|compile-db|flash|both]"
        ;;
esac