#!/bin/bash
# Final MIPS32 LE Build Script with Hybrid Linking
# 4 libraries static, libc dynamic

set -e

export MIPS_CC=mipsel-linux-gnu-gcc
export MIPS_CFLAGS="-mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2 -U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32"
export MIPS_STRIP=mipsel-linux-gnu-strip
export PKG_DIR=/tmp/firebbs-mipsle-hybrid

rm -rf $PKG_DIR
mkdir -p $PKG_DIR/bin $PKG_DIR/lib

BASEDIR=$(cd "$(dirname "$0")" && pwd)
LIBBBS=$BASEDIR/bbssrc/lib/libBBS.a
LIBS="$BASEDIR/bbssrc/lib/libtermcap.a $BASEDIR/bbssrc/lib/libncurses.a $BASEDIR/bbssrc/lib/libcrypt.a"
RPATH_FLAGS="-Wl,-rpath,'\$ORIGIN/../lib' -Wl,--dynamic-linker,../lib/ld-linux.so.3"

cd $BASEDIR/bbssrc/src
rm -f *.o

echo "Building main executables..."

# bbsd - compile with goodbye.o FIRST for custom cexp
echo "  bbsd..."
$MIPS_CC $MIPS_CFLAGS -c goodbye.c chat.c userinfo.c station.c talk.c announce.c boards.c delete.c edit.c help.c mail.c more.c read.c record.c stuff.c  -I../include 2>&1 | grep -i error || true
$MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o bbsd goodbye.o chat.o userinfo.o station.o talk.o announce.o boards.o delete.o edit.o help.o mail.o more.o read.o record.o stuff.o $LIBS $LIBBBS 2>&1 | grep -i error || true
$MIPS_STRIP bbsd
mv bbsd $PKG_DIR/bin/

# chatd
echo "  chatd..."
rm -f *.o
$MIPS_CC $MIPS_CFLAGS -c chat.c -I../include 2>&1 | grep -i error || true
$MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o chatd chat.o $LIBS $LIBBBS 2>&1 | grep -i error || true
$MIPS_STRIP chatd
mv chatd $PKG_DIR/bin/

# thread
echo "  thread..."
rm -f *.o
$MIPS_CC $MIPS_CFLAGS -c thread.c record.c -I../include 2>&1 | grep -i error || true
$MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o thread thread.o record.o $LIBBBS 2>&1 | grep -i error || true
$MIPS_STRIP thread
mv thread $PKG_DIR/bin/

# expire
echo "  expire..."
rm -f *.o
$MIPS_CC $MIPS_CFLAGS -c expire.c -I../include 2>&1 | grep -i error || true
$MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o expire expire.o $LIBBBS 2>&1 | grep -i error || true
$MIPS_STRIP expire
mv expire $PKG_DIR/bin/

echo "Building utilities..."
cd $BASEDIR/bbssrc/util/local_utl
for util in account averun bbstop bfinger birthday horoscope newacct poststat remake usage showuser; do
    if [ -f ${util}.c ]; then
        echo "  $util..."
        rm -f *.o
        $MIPS_CC $MIPS_CFLAGS -c ${util}.c -I../../include 2>&1 | grep -i error || true
        $MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o $util ${util}.o $LIBS $LIBBBS 2>&1 | grep -i error || true
        $MIPS_STRIP $util 2>/dev/null || true
        mv $util $PKG_DIR/bin/ 2>/dev/null || true
    fi
done

# bbspop3d
if [ -f bbspop3d.c ]; then
    echo "  bbspop3d..."
    rm -f *.o
    $MIPS_CC $MIPS_CFLAGS -c bbspop3d.c -I../../include 2>&1 | grep -i error || true
    $MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o bbspop3d bbspop3d.o $LIBBBS 2>&1 | grep -i error || true
    $MIPS_STRIP bbspop3d 2>/dev/null || true
    mv bbspop3d $PKG_DIR/bin/ 2>/dev/null || true
fi

# deljunk
cd $BASEDIR/bbssrc/util/deljunk
if [ -f deljunk.c ]; then
    echo "  deljunk..."
    rm -f *.o
    $MIPS_CC $MIPS_CFLAGS -c deljunk.c -I../../include 2>&1 | grep -i error || true
    $MIPS_CC $MIPS_CFLAGS $RPATH_FLAGS -o deljunk deljunk.o $LIBBBS 2>&1 | grep -i error || true
    $MIPS_STRIP deljunk 2>/dev/null || true
    mv deljunk $PKG_DIR/bin/ 2>/dev/null || true
fi

# Copy libc.so and ld-linux.so to lib/
echo "Copying dynamic libraries..."
MIPS_SYSROOT=/usr/mipsel-linux-gnu
cp $MIPS_SYSROOT/lib/libc.so.6 $PKG_DIR/lib/
cp $MIPS_SYSROOT/lib/ld.so.1 $PKG_DIR/lib/ld-linux.so.3

# Create package
echo "Creating package..."
cd /tmp
tar czf firebbs-mipsle-hybrid.tar.gz firebbs-mipsle-hybrid/
cp firebbs-mipsle-hybrid.tar.gz $BASEDIR/

echo "=== Build Complete ==="
echo "Main executables:"
ls -lh $PKG_DIR/bin/ | grep -E "bbsd|chatd|thread|expire"
echo ""
echo "Utilities:"
ls -lh $PKG_DIR/bin/ | grep -v -E "bbsd|chatd|thread|expire" | tail -n +2
echo ""
echo "Libraries:"
ls -lh $PKG_DIR/lib/
echo ""
echo "Package: $BASEDIR/firebbs-mipsle-hybrid.tar.gz"
ls -lh $BASEDIR/firebbs-mipsle-hybrid.tar.gz
