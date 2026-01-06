# FireBirdBBS MIPS 32-bit Little Endian Cross-Compilation

Chinese version: [README-MIPSLE.md](README-MIPSLE.md)

## Overview

This document explains how to cross-compile FireBirdBBS into binaries for 32-bit MIPS Little Endian platforms.

FireBirdBBS is a text-based BBS system written in C. The original target was 32-bit Intel x86 systems (circa 2000s) and the project uses GBK encoding for Chinese text.

## Build Requirements

### System
- A Linux build host (Ubuntu 18.04 or newer recommended)
- Sufficient disk space (at least 500 MB)
- Internet access to install the cross compilation toolchain

### Dependencies installed by the script
The build script installs the following packages automatically if needed:

- `gcc-mipsel-linux-gnu`
- `g++-mipsel-linux-gnu`
- `binutils-mipsel-linux-gnu`

## Build Features

### Target platform
- Architecture: MIPS 32-bit Little Endian (MIPSEL)
- ABI: o32 (32-bit)
- Endianness: Little Endian

### Compiler flags
```
MIPS_CFLAGS="-mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2 -U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32"
```

Explanation:
- `-mips32`: use MIPS32 instruction set
- `-mabi=32`: use 32-bit ABI
- `-EL`: little-endian
- `-fno-stack-protector`: disable stack protector for legacy compatibility
- `-fcommon`: allow older common-link behavior
- `-O2`: optimization level
- `-U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32`: enforce 32-bit time_t and file offsets

### Libraries and linking

The build uses static linking for several libraries where appropriate:

- `libtermcap.a` — terminal control
- `libncurses.a` — screen control
- `libcrypt.a` — encryption
- `libBBS.a` — BBS core library built from source

System libraries such as `libc`, `libm` and `libdl` may also be included as needed.

## How to build

### Steps

1. Clone or download the repository:

```bash
git clone https://github.com/Yaka88/FireBirdBBS.git
cd FireBirdBBS
```

2. Make the build script executable:

```bash
chmod +x build-mipsle.sh
```

3. Run the build script:

```bash
./build-mipsle.sh
```

### What the script does

1. Install the cross compilation toolchain
2. Prepare build and output directories
3. Configure compilation environment and flags
4. Build `libBBS` from source
5. Handle termcap/ncurses/libcrypt libraries
6. Compile main executables (`bbsd`, `chatd`, `thread`, etc.)
7. Copy runtime files
8. Strip binaries to reduce size
9. Create a tar.gz package

## Output

After building, you will find:

- `FireBirdBBS-mipsle-YYYYMMDD.tar.gz` — the packaged build
- `output-mipsle/bin/` — executables (e.g. `bbsd`, `chatd`, `thread`)
- `output-mipsle/lib/` — runtime libraries (if present)

## Deploying to target

### Method 1: tarball

Transfer the tarball to the MIPS target and extract:

```bash
tar xzf FireBirdBBS-mipsle-YYYYMMDD.tar.gz
cd bin
./bbsd
```

### Method 2: copy directories

Copy `output-mipsle/bin` and `output-mipsle/lib` to the target, preserving relative locations.

Notes:
- Binaries use RPATH set to `$ORIGIN/../lib` so they prefer `../lib` relative to their location.
- Ensure executables have execute permissions:

```bash
chmod +x bin/*
```

## Encoding

- Source files keep the original GBK encoding to stay compatible with legacy text and behavior.

## Time format

- The project uses 32-bit `time_t` to match original 32-bit x86 semantics.

## Technical details

- `libBBS` is built from source; other libraries are taken from bundled/prebuilt static archives where available.
- RPATH is set using `-Wl,-rpath,'$ORIGIN/../lib'` so shared libraries are searched under `../lib`.
- Stack protector is disabled with `-fno-stack-protector` for compatibility.

## Troubleshooting

### Toolchain install failed
Install manually:

```bash
sudo apt-get update
sudo apt-get install gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu
```

### Compilation errors

1. Check disk space
2. Ensure network connectivity
3. Review the script output for specific errors

### Missing runtime libraries on target

Keep `bin` and `lib` relative layout; check with:

```bash
ldd bin/bbsd
```

### Architecture mismatch

Verify with:

```bash
file bin/bbsd
```

It should report MIPS 32-bit LSB executable.

## Version

- Project: FireBirdBBS 3.0
- Target: MIPS 32-bit Little-Endian
- Build script: `build-mipsle.sh`
- Document version: v0.3

## License

Follow the original FireBirdBBS project's license.

## Contact

Open an issue at: https://github.com/Yaka88/FireBirdBBS
