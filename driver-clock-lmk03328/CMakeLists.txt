# Copyright (c) 2024 NASR
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_LMK03328 lmk03328.c)
zephyr_include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
