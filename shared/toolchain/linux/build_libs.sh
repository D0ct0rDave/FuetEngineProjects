#!/usr/bin/env bash
# Builds on Linux the libraries (git submodules in shared/src/sdks/externals; run "git submodule update --init" first) that Cubit and the engine use
# (Box2D 2.2.1 (modified for the engine) and libConfigPortable, the 2014 sources) into $PREFIX/{lib,include}.
# FreeImage and freealut are not part of that tree: they are copied from PREBUILT (the linux-deps that
# shared/toolchain/linux/build_externals.sh of FuetEngineProjects produced) when it exists.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
LIBS="$ROOT/shared/src/sdks/externals"   # git submodules: Box2D, libConfigPortable (physfs)
PREFIX="${PREFIX:-$ROOT/shared/toolchain/FuetEngine/linux-deps}"
PREBUILT="${PREBUILT:-$ROOT/../FuetEngineProjects/shared/toolchain/FuetEngine/linux-deps}"
BUILD="$PREFIX/_build"
JOBS="$(nproc)"
mkdir -p "$PREFIX/lib" "$PREFIX/include" "$BUILD/box2d" "$BUILD/libconfig"

echo "== Box2D (the 2014 copy is modified: float32 is FEReal and it includes <FuetEngine.h>)"
B2="$LIBS/Box2D/Box2D"
ENG="$ROOT/shared/src/FuetEngineFramework/FuetEngine/src"
export B2FLAGS="-O2 -fPIC -w -DLINUX -I$LIBS/Box2D -I$ENG -include $ENG/core/Linux/FECLinuxCompat.h"
find "$B2/Collision" "$B2/Common" "$B2/Dynamics" "$B2/Rope" -name '*.cpp' | \
  xargs -P "$JOBS" -I{} sh -c 'g++ $B2FLAGS -c "$1" -o "'"$BUILD"'/box2d/$(echo "$1" | md5sum | cut -c1-8)_$(basename "${1%.cpp}").o"' _ {}
ar rcs "$PREFIX/lib/libbox2d.a" "$BUILD"/box2d/*.o
mkdir -p "$PREFIX/include/Box2D" && (cd "$B2" && find . -name '*.h' -exec cp --parents {} "$PREFIX/include/Box2D/" \;)

echo "== libConfigPortable"
for f in "$LIBS"/libConfigPortable/src/*.c; do
  gcc -O2 -fPIC -w -I"$LIBS/libConfigPortable/src" -c "$f" -o "$BUILD/libconfig/$(basename "${f%.c}").o"
done
ar rcs "$PREFIX/lib/libconfigPortable.a" "$BUILD"/libconfig/*.o
cp "$LIBS"/libConfigPortable/src/*.h "$PREFIX/include/"

if [ -d "$PREBUILT" ]; then
  echo "== FreeImage, freealut, physfs (prebuilt copy from $PREBUILT)"
  for l in libfreeimage.a libalut.a libphysfs.a; do cp -n "$PREBUILT/lib/$l" "$PREFIX/lib/" 2>/dev/null || true; done
  cp -rn "$PREBUILT/include/FreeImage.h" "$PREBUILT/include/AL" "$PREBUILT/include/physfs.h" "$PREFIX/include/" 2>/dev/null || true
else
  echo "WARNING: no PREBUILT deps; build FreeImage and freealut and put them in $PREFIX"
fi
echo "Done: $PREFIX"
