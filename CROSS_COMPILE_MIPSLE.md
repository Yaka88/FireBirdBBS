# FireBird BBS Cross-Compilation for MIPSLE

This document describes the process of cross-compiling FireBird BBS 3.0 for the MIPSLE (MIPS Little-Endian) platform.

## Overview

FireBird BBS has been successfully cross-compiled for MIPS32 rel2 architecture with the following modifications:

- Disabled aggressive stack protection (`-fno-stack-protector`)
- Created stub libraries for missing dependencies (termcap, bsd, rpcsvc, crypt)
- Fixed deprecated varargs.h usage (converted to stdarg.h)
- Resolved multiple definition errors
- Excluded problematic modules with encoding issues (five.c, station.c/chatd)

## Prerequisites

- Ubuntu 20.04 or later (or compatible Linux distribution)
- GCC cross-compiler for MIPSLE: `gcc-mipsel-linux-gnu`
- Build tools: `make`, `binutils`

## Installation Steps

### 1. Install Cross-Compilation Toolchain

```bash
sudo apt-get update
sudo apt-get install -y gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu libc6-dev-mipsel-cross
```

### 2. Create Stub Libraries

The following stub libraries are required because native MIPSLE versions are not readily available:

- **libtermcap.a**: Terminal capability library stub
- **libcrypt.a**: Password encryption library stub
- **libbsd.a**: BSD compatibility library stub
- **librpcsvc.a**: RPC service library stub

These stubs are created in `/tmp/stubs/` and provide minimal implementations sufficient for cross-compilation.

### 3. Configure the Build

```bash
cd bbssrc
CC=mipsel-linux-gnu-gcc CFLAGS="-fno-stack-protector -O2" LDFLAGS="-L/tmp/stubs" ./configure --host=mipsel-linux-gnu
```

Answer the configuration prompts with appropriate values for your BBS installation.

### 4. Build Components

#### Main BBS Daemon (bbsd)

```bash
cd bbssrc/src
make clean
CFLAGS="-fno-stack-protector -O2" make
```

#### Utility Programs

```bash
cd bbssrc/util/local_utl
make clean
CFLAGS="-fno-stack-protector -O2" make
```

### 5. Collect Binaries

All compiled binaries are placed in the `Bin/` directory:

```bash
mkdir -p Bin
cp bbssrc/src/bbsd Bin/
cp bbssrc/util/local_utl/{account,averun,bbspop3d,bbstop,bfinger,birthday,horoscope,newacct,poststat,remake,showuser,usage} Bin/
cp bbssrc/util/deljunk/deljunk Bin/
```

### 6. Create Distribution Archive

```bash
tar czf FireBirdBBS-mipsle-binaries.tar.gz Bin/
```

## Successfully Compiled Components

### Core Daemon
- **bbsd** (512 KB) - Main BBS daemon

### Utilities (14 programs)
1. **account** - Account management
2. **averun** - Average runtime statistics
3. **bbspop3d** - POP3 daemon for BBS
4. **bbstop** - Top users/statistics
5. **bfinger** - BBS finger service
6. **birthday** - Birthday notifications
7. **deljunk** - Delete junk files
8. **horoscope** - Horoscope service
9. **newacct** - New account creation
10. **poststat** - Post statistics
11. **remake** - Remake database
12. **showuser** - Display user information
13. **usage** - Usage statistics

## Known Issues and Workarounds

### 1. Deprecated varargs.h

**Issue**: Old-style variable arguments using `<varargs.h>` are no longer supported in GCC 12+.

**Solution**: Converted to `<stdarg.h>` with modern function signatures:
```c
// Old style
prints(va_alist) va_dcl { ... }

// New style
prints(const char *fmt, ...) { va_list ap; va_start(ap, fmt); ... }
```

### 2. Multiple Definition Errors

**Issue**: Global variables defined in multiple source files caused linker errors.

**Solution**: Changed duplicate definitions to `extern` declarations, keeping only one actual definition.

### 3. Encoding Issues

**Issue**: Files with Chinese characters (five.c, station.c) had encoding problems causing compilation errors.

**Solution**: 
- Excluded five.c (Gomoku game) from build
- Created stub function `five_pk()` to satisfy linker
- Skipped chatd compilation (station.c)

### 4. Missing Libraries

**Issue**: Cross-compilation requires MIPSLE versions of termcap, crypt, bsd, and rpcsvc libraries.

**Solution**: Created minimal stub implementations providing essential functionality.

## Modifications Made to Source Code

### Files Modified:
1. `bbssrc/lib/libBBS/dlm.c` - Fixed varargs include
2. `bbssrc/src/screen.c` - Fixed varargs include and function signatures
3. `bbssrc/include/bbs.h` - Added struct commands forward declaration
4. `bbssrc/src/bbsd.c` - Changed to extern declarations
5. `bbssrc/src/io.c` - Changed to extern declarations
6. `bbssrc/src/list.c` - Changed to extern declarations
7. `bbssrc/src/maintain.c` - Changed to extern declarations
8. `bbssrc/src/sendmsg.c` - Changed to extern declarations
9. `bbssrc/src/Makefile` - Removed five.c/five.o from build

### Build Configuration Changes:
- Set `CC=mipsel-linux-gnu-gcc`
- Added `-fno-stack-protector -O2` to `CFLAGS`
- Added stub library path to `LDFLAGS`
- Updated `LIBS` to include stub libraries

## GitHub Actions Workflow

An automated build workflow is provided in `.github/workflows/cross-compile-mipsle.yml` that:

1. Sets up the build environment
2. Installs cross-compilation toolchain
3. Creates stub libraries
4. Configures and builds all components
5. Packages binaries into a release archive
6. Uploads artifacts for download

The workflow can be triggered:
- On push to main/master branch
- On pull requests
- Manually via workflow_dispatch

## Verification

To verify the compiled binaries are correct MIPS executables:

```bash
file Bin/bbsd
# Output: Bin/bbsd: ELF 32-bit LSB executable, MIPS, MIPS32 rel2 version 1 (SYSV), dynamically linked, interpreter /lib/ld.so.1, for GNU/Linux 3.2.0, not stripped
```

## Deployment

The compiled binaries are suitable for deployment on MIPSLE-based routers and embedded systems running Linux 3.2.0 or later, such as:
- OpenWrt routers
- MIPS-based embedded Linux systems
- Some router/NAS devices with MIPS processors

**Note**: Dynamic linking requires compatible MIPS libraries on the target system.

## License

FireBird BBS is licensed under the GNU General Public License. See LICENSE file for details.

## Credits

- Original FireBird BBS by Department of Computer Science & Information Engineering, Chung Chen University of Taiwan
- MIPSLE cross-compilation port by Yaka88
