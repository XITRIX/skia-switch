#!/bin/sh
# Copyright 2026 The NXKit Authors
# Use of this source code is governed by a BSD-style license in the LICENSE file.
set -eu

skia_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$skia_root"
sdk_root=${DEVKITPRO:-/opt/devkitpro}
if [ ! -x "$sdk_root/devkitA64/bin/aarch64-none-elf-g++" ] ||
   [ ! -f "$sdk_root/portlibs/switch/include/freetype2/ft2build.h" ]; then
    echo "Set DEVKITPRO to a complete devkitPro SDK with Switch portlibs." >&2
    exit 1
fi
export DEVKITPRO="$sdk_root"
build_dir=${1:-out/horizon-vulkan}

# Keep the public release ABI: development builds add GPU_TEST_UTILS fields.
# HarfBuzz and ICU stay bundled; other system libraries come from Switch portlibs.
bin/gn gen "$build_dir" --args='is_official_build=true is_debug=false is_trivial_abi=true target_os="horizon" target_cpu="arm64" skia_enable_graphite=true skia_enable_ganesh=false skia_use_vulkan=true skia_use_gl=false skia_use_egl=false skia_use_dawn=false skia_use_metal=false skia_enable_tools=false skia_use_system_harfbuzz=false skia_enable_android_utils=true'
ninja -C "$build_dir" -j "${SKIA_BUILD_JOBS:-${NXKIT_BUILD_JOBS:-8}}" \
    skia skparagraph skshaper skunicode_core skunicode_icu icu skcms
