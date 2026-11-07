#!/usr/bin/env bash
# Compila en Linux las librerias externas de FuetEngine que son submodulos de git.
# Resultado: $PREFIX/{lib,include}, por defecto shared/toolchain/FuetEngine/linux-deps
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
EXT="$ROOT/shared/src/sdks/externals"
PREFIX="${PREFIX:-$ROOT/shared/toolchain/FuetEngine/linux-deps}"
BUILD="${BUILD:-$PREFIX/_build}"
JOBS="$(nproc)"

mkdir -p "$PREFIX/lib" "$PREFIX/include" "$BUILD"

# --- FreeImage (Makefile propio; flags para compilar con gcc moderno) ---
echo "== FreeImage"
( cd "$EXT/FreeImage" && \
  CFLAGS="-std=gnu99 -O3 -fPIC -fexceptions -fvisibility=hidden -w -include wchar.h -D_byteswap_ulong=__builtin_bswap32" \
  CXXFLAGS="-std=gnu++14 -O3 -fPIC -fexceptions -fvisibility=hidden -w -Wno-ctor-dtor-privacy" \
  make -j"$JOBS" )
# Makefile.srcs omits LibTIFF4/tif_hash_set.c, which libtiff needs
gcc -std=gnu99 -O3 -fPIC -w -I"$EXT/FreeImage/Source/LibTIFF4" -I"$EXT/FreeImage/Source" \
    -c "$EXT/FreeImage/Source/LibTIFF4/tif_hash_set.c" -o "$BUILD/tif_hash_set.o"
ar rs "$EXT/FreeImage/libfreeimage.a" "$BUILD/tif_hash_set.o"
cp "$EXT/FreeImage/libfreeimage.a" "$PREFIX/lib/"
cp "$EXT/FreeImage/Dist/FreeImage.h" "$PREFIX/include/"

# --- libConfigPortable (C, pocos ficheros) ---
echo "== libConfigPortable"
mkdir -p "$BUILD/libconfig"
for f in "$EXT"/libConfigPortable/src/*.c; do
    gcc -O2 -fPIC -w -I"$EXT/libConfigPortable/src" -c "$f" -o "$BUILD/libconfig/$(basename "${f%.c}").o"
done
ar rcs "$PREFIX/lib/libconfigPortable.a" "$BUILD"/libconfig/*.o
cp "$EXT"/libConfigPortable/src/*.h "$PREFIX/include/"

# --- freealut (el config.h de la raiz es el de Windows: se usa el generado por cmake) ---
echo "== freealut"
cmake -S "$EXT/freealut" -B "$BUILD/freealut" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DCMAKE_C_FLAGS="-w"
mkdir -p "$BUILD/alut_static"
for f in "$EXT"/freealut/src/*.c; do
    gcc -O2 -fPIC -w -DHAVE_CONFIG_H=1 -DALUT_BUILD_STATIC -I"$BUILD/freealut" -I"$EXT/freealut/include" \
        -c "$f" -o "$BUILD/alut_static/$(basename "${f%.c}").o"
done
ar rcs "$PREFIX/lib/libalut.a" "$BUILD"/alut_static/*.o
mkdir -p "$PREFIX/include/AL" && cp "$EXT/freealut/include/AL/alut.h" "$PREFIX/include/AL/"

# --- physfs ---
echo "== physfs"
cmake -S "$EXT/physfs" -B "$BUILD/physfs" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DCMAKE_C_FLAGS="-w -Wno-error" -DPHYSFS_BUILD_SHARED=OFF -DPHYSFS_BUILD_TEST=OFF -DPHYSFS_BUILD_WX_TEST=OFF \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_INSTALL_PREFIX="$PREFIX"
cmake --build "$BUILD/physfs" -j "$JOBS"
cp "$BUILD/physfs/libphysfs.a" "$PREFIX/lib/" && cp "$EXT/physfs/physfs.h" "$PREFIX/include/"

echo "Hecho. Artefactos en $BUILD (revisar *.a) y $PREFIX"
