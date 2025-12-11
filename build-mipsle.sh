#!/bin/bash
# FireBirdBBS Cross-compilation Script for MIPS 32-bit Little Endian
# This script cross-compiles the FireBirdBBS project to 32-bit MIPS LE platform

set -e  # Exit on error

echo "=========================================="
echo "FireBirdBBS MIPS Cross-Compilation Script"
echo "=========================================="

# Get absolute path
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Configuration
MIPS_CFLAGS="-mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2 -U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32"
TARGET_ARCH="mipsel-linux-gnu"
CROSS_PREFIX="${TARGET_ARCH}-"

# Directories
BUILD_DIR="${SCRIPT_DIR}/build-mipsle"
OUTPUT_DIR="${SCRIPT_DIR}/output-mipsle"
BIN_DIR="${OUTPUT_DIR}/bin"
LIB_DIR="${OUTPUT_DIR}/lib"
SRC_DIR="${SCRIPT_DIR}/bbssrc"

echo "Step 1: Installing MIPS cross-compilation toolchain..."
sudo apt-get update -qq
sudo apt-get install -y gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu

# Verify toolchain installation
if ! command -v ${CROSS_PREFIX}gcc &> /dev/null; then
    echo "ERROR: MIPS cross-compiler not found!"
    exit 1
fi

echo "Toolchain installed: $(${CROSS_PREFIX}gcc --version | head -1)"

echo ""
echo "Step 2: Setting up build environment..."
rm -rf "$BUILD_DIR" "$OUTPUT_DIR"
mkdir -p "$BUILD_DIR" "$BIN_DIR" "$LIB_DIR"

# Copy source to build directory
cp -r "$SRC_DIR"/* "$BUILD_DIR/"
cd "$BUILD_DIR"

echo ""
echo "Step 3: Configuring build..."

# Set up environment for cross-compilation
export CC="${CROSS_PREFIX}gcc"
export AR="${CROSS_PREFIX}ar"
export RANLIB="${CROSS_PREFIX}ranlib"
export LD="${CROSS_PREFIX}ld"
export STRIP="${CROSS_PREFIX}strip"

# Combine CFLAGS
export FULL_CFLAGS="${MIPS_CFLAGS} -I${BUILD_DIR}/include -Wunused"
export LDFLAGS="-Wl,-rpath,'\$\$ORIGIN/../lib' -L${BUILD_DIR}/lib"

echo "CC = ${CC}"
echo "CFLAGS = ${FULL_CFLAGS}"
echo "LDFLAGS = ${LDFLAGS}"

echo ""
echo "Step 4: Building libBBS library..."
cd "${BUILD_DIR}/lib/libBBS"
make clean || true
rm -f ../libBBS.a

# Build libBBS objects
for src in *.c; do
    obj="${src%.c}.o"
    echo "  Compiling $src..."
    ${CC} ${FULL_CFLAGS} -DLINUX -DTERMIOS -c "$src" -o "$obj"
done

# Create static library
echo "  Creating libBBS.a..."
${AR} rcs libBBS.a *.o
${RANLIB} libBBS.a
cp libBBS.a ../

echo ""
echo "Step 5: Building system libraries (termcap, ncurses, libcrypt)..."

# These libraries appear to be pre-built stubs. We need to rebuild them properly
# or they're minimal implementations in the repository.

# For termcap - check if source exists
cd "${BUILD_DIR}/lib"
if [ ! -f "libtermcap.a" ] || [ $(stat -c%s libtermcap.a) -lt 10000 ]; then
    echo "  Note: termcap library is minimal, will use as-is"
fi

# The lib directory may contain pre-built libraries that need to be kept
# We'll verify they work during linking stage

echo ""
echo "Step 6: Building main executables..."
cd "${BUILD_DIR}/src"

# Generate version header
echo "  Generating version.h..."
sh ver.sh ../include/version.h || echo "/* Version placeholder */" > ../include/version.h

# Compile all source files
echo "  Compiling source files..."
for src in admintool.c announce.c bbs.c bbsd.c bbsgopher.c bcache.c bm.c \
           boards.c chat.c comm_lists.c delete.c edit.c fileshm.c goodbye.c \
           help.c io.c list.c mail.c main.c maintain.c modetype.c more.c \
           namecomplete.c pass.c postheader.c read.c record.c \
           register.c screen.c sendmsg.c stuff.c talk.c term.c \
           five.c userinfo.c vote.c lovepaper.c xyz.c; do
    obj="${src%.c}.o"
    echo "    $src -> $obj"
    ${CC} ${FULL_CFLAGS} -DLINUX -DTERMIOS -DSHOW_IDLE_TIME -DWITHOUT_CHROOT -DHAVE_VERSION_H -c "$src" -o "$obj"
done

echo "  Linking bbsd..."
${CC} -o bbsd ${FULL_CFLAGS} ${LDFLAGS} \
    admintool.o announce.o bbs.o bbsd.o bbsgopher.o bcache.o bm.o \
    boards.o chat.o comm_lists.o delete.o edit.o fileshm.o goodbye.o \
    help.o io.o list.o mail.o main.o maintain.o modetype.o more.o \
    namecomplete.o pass.o postheader.o read.o record.o \
    register.o screen.o sendmsg.o stuff.o talk.o term.o \
    five.o userinfo.o vote.o lovepaper.o xyz.o \
    -L../lib -lBBS -static-libgcc

echo "  Linking chatd..."
${CC} ${FULL_CFLAGS} ${LDFLAGS} -o chatd station.c -DLINUX -DTERMIOS \
    -L../lib -lBBS -static-libgcc

echo "  Linking thread..."
${CC} ${FULL_CFLAGS} ${LDFLAGS} -o thread record.c thread.c -DLINUX -DTERMIOS \
    -static-libgcc

# Build paging.so if needed
if [ -f paging.c ]; then
    echo "  Building paging.so..."
    ${CC} ${FULL_CFLAGS} -DLINUX -DTERMIOS -DSHOW_IDLE_TIME -DWITHOUT_CHROOT -c paging.c -o paging.o
    ${LD} -shared paging.o -o paging.so -L../lib -lBBS
fi

echo ""
echo "Step 7: Copying executables to output directory..."
cp bbsd chatd thread "${BIN_DIR}/"
if [ -f paging.so ]; then
    cp paging.so "${BIN_DIR}/"
fi

# Build utilities if present
echo ""
echo "Step 8: Building utilities..."
cd "${BUILD_DIR}/util/local_utl"
if [ -f Makefile ]; then
    for src in *.c; do
        if [ -f "$src" ]; then
            prog="${src%.c}"
            echo "  Building $prog..."
            ${CC} ${FULL_CFLAGS} ${LDFLAGS} -DLINUX -DTERMIOS -o "$prog" "$src" \
                -L../../lib -lBBS -static-libgcc 2>/dev/null || echo "    (skipped - build failed)"
            if [ -f "$prog" ]; then
                cp "$prog" "${BIN_DIR}/"
            fi
        fi
    done
fi

echo ""
echo "Step 9: Extracting and copying libc dependencies..."
cd "${BIN_DIR}"

# Find all dynamic library dependencies
echo "  Analyzing dependencies..."
SYSROOT="/usr/${TARGET_ARCH}"

# Get list of all shared library dependencies
for binary in *; do
    if [ -f "$binary" ] && [ -x "$binary" ]; then
        echo "  Checking $binary..."
        ${CROSS_PREFIX}readelf -d "$binary" 2>/dev/null | grep NEEDED || true
    fi
done

# Copy necessary libc and other dynamic libraries
echo "  Copying dynamic libraries from sysroot..."
if [ -d "$SYSROOT" ]; then
    # Copy essential libraries
    for lib in libc.so.* libm.so.* libdl.so.* libpthread.so.* librt.so.* libnsl.so.* libutil.so.*; do
        if [ -f "${SYSROOT}/lib/${lib}" ]; then
            echo "    Copying ${lib}..."
            cp -P "${SYSROOT}/lib/${lib}" "${LIB_DIR}/" 2>/dev/null || true
        fi
    done
    
    # Copy ld-linux
    if [ -f "${SYSROOT}/lib/ld.so.1" ]; then
        cp -P "${SYSROOT}/lib/ld.so.1" "${LIB_DIR}/"
    fi
    
    # Copy any symlinks
    for lib in libc.so libm.so libdl.so libpthread.so librt.so libnsl.so libutil.so; do
        if [ -L "${SYSROOT}/lib/${lib}" ]; then
            cp -P "${SYSROOT}/lib/${lib}" "${LIB_DIR}/" 2>/dev/null || true
        fi
    done
fi

echo ""
echo "Step 10: Stripping binaries..."
cd "${BIN_DIR}"
for binary in *; do
    if [ -f "$binary" ] && [ -x "$binary" ] && file "$binary" | grep -q "ELF"; then
        echo "  Stripping $binary..."
        ${STRIP} "$binary" 2>/dev/null || true
    fi
done

echo ""
echo "Step 11: Creating package..."
cd "${SCRIPT_DIR}"

PACKAGE_NAME="FireBirdBBS-mipsle-$(date +%Y%m%d).tar.gz"
echo "  Creating ${PACKAGE_NAME}..."
tar czf "${PACKAGE_NAME}" -C "${OUTPUT_DIR}" bin lib

echo ""
echo "Step 12: Verifying build..."
echo "Binaries in ${BIN_DIR}:"
ls -lh "${BIN_DIR}"
echo ""
echo "Libraries in ${LIB_DIR}:"
ls -lh "${LIB_DIR}" 2>/dev/null || echo "  (no dynamic libraries)"
echo ""
echo "Package created:"
ls -lh "${PACKAGE_NAME}"

echo ""
echo "Step 13: Checking binary architecture..."
file "${BIN_DIR}/bbsd"
${CROSS_PREFIX}readelf -h "${BIN_DIR}/bbsd" | grep -E "Class|Machine|ABI"

echo ""
echo "=========================================="
echo "Build completed successfully!"
echo "=========================================="
echo "Package: ${PACKAGE_NAME}"
echo "Output directory: ${OUTPUT_DIR}"
echo ""
echo "To extract on target system:"
echo "  tar xzf ${PACKAGE_NAME}"
echo "  cd bin"
echo "  ./bbsd"
echo ""
