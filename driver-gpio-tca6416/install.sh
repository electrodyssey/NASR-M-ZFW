#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ZEPHYR_GPIO="$SCRIPT_DIR/../../zephyr/drivers/gpio"
ZEPHYR_BINDINGS="$SCRIPT_DIR/../../zephyr/dts/bindings/gpio"

set -e

echo "TCA6416A GPIO expander out-of-tree driver installer"
echo "----------------------------------------------------"
echo "This script installs the TCA6416A driver into the Zephyr tree by:"
echo "  1. Copying gpio_tca6416a.c      -> zephyr/drivers/gpio/"
echo "  2. Copying Kconfig.tca6416a     -> zephyr/drivers/gpio/"
echo "  3. Copying ti,tca6416a.yaml     -> zephyr/dts/bindings/gpio/"
echo "  4. Patching zephyr/drivers/gpio/CMakeLists.txt"
echo "  5. Patching zephyr/drivers/gpio/Kconfig"
echo ""

echo "Copying driver files..."
cp -v "$SCRIPT_DIR/ti,tca6416a.yaml"  "$ZEPHYR_BINDINGS/"
cp -v "$SCRIPT_DIR/gpio_tca6416a.c"   "$ZEPHYR_GPIO/"
cp -v "$SCRIPT_DIR/Kconfig.tca6416a"  "$ZEPHYR_GPIO/"

# Add to CMakeLists.txt if not already present
CMAKE_FILE="$ZEPHYR_GPIO/CMakeLists.txt"
CMAKE_LINE="zephyr_library_sources_ifdef(CONFIG_GPIO_TCA6416A   gpio_tca6416a.c)"
if grep -qF "CONFIG_GPIO_TCA6416A" "$CMAKE_FILE"; then
    echo "CMakeLists.txt already contains TCA6416A entry, skipping."
else
    sed -i "s|zephyr_library_sources_ifdef(CONFIG_GPIO_TCA6424A gpio_tca6424a.c)|&\n$CMAKE_LINE|" "$CMAKE_FILE"
    echo "CMakeLists.txt updated."
fi

# Add to Kconfig if not already present
KCONFIG_FILE="$ZEPHYR_GPIO/Kconfig"
KCONFIG_LINE='source "drivers/gpio/Kconfig.tca6416a"'
if grep -qF "Kconfig.tca6416a" "$KCONFIG_FILE"; then
    echo "Kconfig already contains TCA6416A entry, skipping."
else
    sed -i "s|source \"drivers/gpio/Kconfig.tca6424a\"|$KCONFIG_LINE\n&|" "$KCONFIG_FILE"
    echo "Kconfig updated."
fi

echo "Done."
