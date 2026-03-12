#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ZEPHYR_CLOCK="$SCRIPT_DIR/../../zephyr/drivers/clock_control"
ZEPHYR_BINDINGS="$SCRIPT_DIR/../../zephyr/dts/bindings/clock"
ZEPHYR_INCLUDE="$SCRIPT_DIR/../../zephyr/include/zephyr/drivers/clock_control"

set -e

echo "LMK03328 clock generator out-of-tree driver installer"
echo "------------------------------------------------------"
echo "This script installs the LMK03328 driver into the Zephyr tree by:"
echo "  1. Copying lmk03328.c            -> zephyr/drivers/clock_control/"
echo "  2. Copying Kconfig.lmk03328      -> zephyr/drivers/clock_control/"
echo "  3. Copying ti,lmk03328.yaml      -> zephyr/dts/bindings/clock/"
echo "  4. Copying lmk03328.h            -> zephyr/include/zephyr/drivers/clock_control/"
echo "  5. Patching zephyr/drivers/clock_control/CMakeLists.txt"
echo "  6. Patching zephyr/drivers/clock_control/Kconfig"
echo ""

echo "Copying driver files..."
cp -v "$SCRIPT_DIR/dts/bindings/clock/ti,lmk03328.yaml"  "$ZEPHYR_BINDINGS/"
cp -v "$SCRIPT_DIR/lmk03328.c"                            "$ZEPHYR_CLOCK/"
cp -v "$SCRIPT_DIR/Kconfig.lmk03328"                      "$ZEPHYR_CLOCK/"

mkdir -p "$ZEPHYR_INCLUDE"
cp -v "$SCRIPT_DIR/include/lmk03328.h"                    "$ZEPHYR_INCLUDE/"

# Add to CMakeLists.txt if not already present
CMAKE_FILE="$ZEPHYR_CLOCK/CMakeLists.txt"
CMAKE_LINE="zephyr_library_sources_ifdef(CONFIG_LMK03328 lmk03328.c)"
if grep -qF "CONFIG_LMK03328" "$CMAKE_FILE"; then
    echo "CMakeLists.txt already contains LMK03328 entry, skipping."
else
    sed -i "s|zephyr_library_sources_ifdef(CONFIG_CLOCK_CONTROL_WCH_RCC clock_control_wch_rcc.c)|&\n$CMAKE_LINE|" "$CMAKE_FILE"
    echo "CMakeLists.txt updated."
fi

# Add to Kconfig if not already present
KCONFIG_FILE="$ZEPHYR_CLOCK/Kconfig"
KCONFIG_LINE='source "drivers/clock_control/Kconfig.lmk03328"'
if grep -qF "Kconfig.lmk03328" "$KCONFIG_FILE"; then
    echo "Kconfig already contains LMK03328 entry, skipping."
else
    sed -i "s|endif # CLOCK_CONTROL|$KCONFIG_LINE\n\nendif # CLOCK_CONTROL|" "$KCONFIG_FILE"
    echo "Kconfig updated."
fi

echo "Done."
