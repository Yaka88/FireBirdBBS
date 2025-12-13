#!/bin/bash
# FireBirdBBS bbsnet and telnet Cross-compilation Script for MIPS 32-bit Little Endian
# This script cross-compiles bbsnet from existing source and telnet from inetutils source to MIPS LE platform

set -e  # Exit on error

echo "=========================================="
echo "FireBirdBBS bbsnet and telnet MIPS Cross-Compilation Script"
echo "=========================================="

# Get absolute path
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Configuration
MIPS_CFLAGS="-mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2 -U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32"
TARGET_ARCH="mipsel-linux-gnu"
CROSS_PREFIX="${TARGET_ARCH}-"

# Directories
BUILD_DIR="${SCRIPT_DIR}/build-bbsnet-telnet"
OUTPUT_DIR="${SCRIPT_DIR}/output-bbsnet-telnet"
BIN_DIR="${OUTPUT_DIR}/bin"
SRC_DIR="${SCRIPT_DIR}/bbssrc"

INETUTILS_VERSION="2.3"
INETUTILS_URL="https://ftp.gnu.org/gnu/inetutils/inetutils-${INETUTILS_VERSION}.tar.gz"
INETUTILS_DIR="${BUILD_DIR}/inetutils-${INETUTILS_VERSION}"

echo "Step 1: Installing MIPS cross-compilation toolchain..."
sudo apt-get update -qq
sudo apt-get install -y gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu wget

# Verify toolchain installation
if ! command -v ${CROSS_PREFIX}gcc &> /dev/null; then
    echo "ERROR: MIPS cross-compiler not found!"
    exit 1
fi

echo "Toolchain installed: $(${CROSS_PREFIX}gcc --version | head -1)"

echo ""
echo "Step 2: Setting up build environment..."
rm -rf "$BUILD_DIR" "$OUTPUT_DIR"
mkdir -p "$BUILD_DIR" "$BIN_DIR"

# Copy bbsnet source to build directory
cp -r "$SRC_DIR/util/bbsnet" "$BUILD_DIR/"
cd "$BUILD_DIR"

echo ""
echo "Step 3: Configuring build..."

# Set up environment for cross-compilation
export CC="${CROSS_PREFIX}gcc"
export AR="${CROSS_PREFIX}ar"
export RANLIB="${CROSS_PREFIX}ranlib"
export LD="${CROSS_PREFIX}ld"
export STRIP="${CROSS_PREFIX}strip"

# Combine CFLAGS - don't convert charset, keep as-is
export FULL_CFLAGS="${MIPS_CFLAGS} -I${SRC_DIR}/include -Wunused"
export LDFLAGS="-static -L${SRC_DIR}/lib"

echo "CC = ${CC}"
echo "CFLAGS = ${FULL_CFLAGS}"
echo "LDFLAGS = ${LDFLAGS}"

echo ""
echo "Step 4: Building bbsnet..."
cd "${BUILD_DIR}/bbsnet"

# Fix line endings if dos2unix is available
if command -v dos2unix &> /dev/null; then
    echo "  Fixing line endings..."
    dos2unix *.c *.h 2>/dev/null || true
fi

# Build bbsnet with cross-compiler and static libs
make clean || true
make CC="${CC}" CFLAG="-c ${FULL_CFLAGS} -DLINUX" LIBS="-L${SRC_DIR}/lib -lncurses -ltermcap -lcrypt" LIBSDIR="" ARCH="-DLINUX" bbs

# Copy bbs to bin dir
cp bbs "${BIN_DIR}/"

echo ""
echo "Step 5: Downloading and building inetutils telnet..."
cd "${BUILD_DIR}"
wget "${INETUTILS_URL}"
tar xzf "inetutils-${INETUTILS_VERSION}.tar.gz"
cd "${INETUTILS_DIR}"

# Configure for cross-compilation, static build, only telnet client
./configure --host=${TARGET_ARCH} --enable-static --disable-shared --disable-servers --disable-clients --enable-telnet CC="${CC}" CFLAGS="${FULL_CFLAGS}" LDFLAGS="${LDFLAGS}"

make -j$(nproc) telnet

# Copy telnet to bin dir
cp telnet/telnet "${BIN_DIR}/"

echo ""
echo "Step 6: Stripping binaries..."
cd "${BIN_DIR}"
for binary in *; do
    if [ -f "$binary" ] && [ -x "$binary" ] && file "$binary" | grep -q "ELF"; then
        echo "  Stripping $binary..."
        ${STRIP} "$binary" 2>/dev/null || true
    fi
done

echo ""
echo "Step 7: Creating single package file..."
cd "${SCRIPT_DIR}"

PACKAGE_NAME="FireBirdBBS-bbsnet-telnet-mipsle.tar.gz"
echo "  Creating ${PACKAGE_NAME}..."
tar czf "${PACKAGE_NAME}" -C "${OUTPUT_DIR}" bin

echo ""
echo "Step 8: Build verification..."
echo "Binaries in ${BIN_DIR}:"
ls -lh "${BIN_DIR}"
echo ""
echo "Package created:"
ls -lh "${PACKAGE_NAME}"

echo ""
echo "=========================================="
echo "Build completed successfully!"
echo "=========================================="
echo "Package: ${PACKAGE_NAME}"
echo "Output directory: ${OUTPUT_DIR}"
echo "  - bin/: bbs (bbsnet) and telnet executables (statically linked)"
echo ""
echo "To extract on target system:"
echo "  tar xzf ${PACKAGE_NAME}"
echo "  cd bin"
echo "  ./bbs"
echo "  ./telnet hostname port"
echo ""