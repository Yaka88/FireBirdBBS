# FireBird BBS - MIPS32 Little Endian Cross-Compilation

This repository contains the FireBird BBS system cross-compiled for mipsel (MIPS32 little-endian) architecture.

## Build Specifications

- **Target Architecture**: mipsel-linux-gnu (MIPS32 rel2, little-endian)
- **Compiler**: gcc-mipsel-linux-gnu 12.4.0  
- **Linking**: Static linking
- **Stack Protection**: Disabled (-fno-stack-protector)
- **Build Date**: 2025-11-24

## Built Binaries

All binaries are located in `mipsel-binaries/bin/` and also available in `mipsel-binaries.tar.gz`:

1. **bbsd** (1.4MB) - Main BBS daemon
2. **chatd** (820KB) - Chat server daemon  
3. **thread** (615KB) - Thread management utility
4. **expire** (614KB) - Post expiration utility

All binaries are:
- Statically linked (no runtime dependencies)
- Stripped (debug symbols removed)
- ELF 32-bit LSB executable for GNU/Linux 3.2.0+

## Build Process

### Compilation Fixes Applied

1. **struct commands definition**: Added missing structure definition to `bbs.h`
2. **varargs.h → stdarg.h**: Converted deprecated varargs to modern stdarg in:
   - `lib/libBBS/dlm.c`
   - `src/screen.c`
3. **Line ending fixes**: Fixed CRLF issues in `src/five.c`
4. **Multiple definitions**: Recompiled with `-fcommon` flag for affected files

### Dependencies

The build uses stub implementations for:
- **termcap/ncurses functions** (tputs, tgoto, tgetstr, etc.)
- **libcrypt functions** (crypt, crypt_r)

**Note**: These stub implementations provide basic functionality. For full production use, you should link against proper mipsel versions of ncurses and libcrypt libraries.

## Perl Requirement

The configuration mentions Perl (`PERL=/usr/bin/perl` in `configure.inf`). For full functionality on the target system, you will need:
- Perl 5.x compiled for mipsel architecture
- Perl scripts in `util/mail2bbs/` directory require Perl interpreter

## Installation on Target System

1. Extract the archive:
   ```bash
   tar -xzf mipsel-binaries.tar.gz
   cd mipsel-binaries/bin
   ```

2. Copy binaries to appropriate locations as defined in Makefile:
   ```bash
   # Default BBSHOME=/home/bbs
   cp bbsd /home/bbs/bin/
   cp chatd /home/bbs/bin/
   cp thread /home/bbs/bin/
   cp expire /home/bbs/bin/
   ```

3. Set proper permissions:
   ```bash
   chown bbs:bbs /home/bbs/bin/*
   chmod 550 /home/bbs/bin/*
   ```

## Verification

Verify the binaries on your mipsel system:
```bash
file bbsd
# Output: bbsd: ELF 32-bit LSB executable, MIPS, MIPS32 rel2 version 1 (SYSV), statically linked...

./bbsd --help  # Test execution
```

## Known Limitations

1. **Stub Libraries**: Terminal control and password hashing use minimal stub implementations
2. **Network Functions**: gethostbyname/gethostbyaddr warnings - runtime glibc NSS modules may be needed
3. **Dynamic Loading**: dlopen warning - dynamic module loading may not work in static build
4. **Perl Not Included**: Perl interpreter for mipsel must be provided separately

## Build Environment

- Ubuntu 24.04 LTS
- Cross-compiler toolchain: gcc-mipsel-linux-gnu
- Build system: GNU Make

## License

See LICENSE file for the original FireBird BBS license information.

## Support

For issues related to:
- Original BBS functionality: Contact original FireBird BBS developers
- Cross-compilation: Check this repository's issues
- Deployment on mipsel systems: Ensure proper runtime environment

## Security Notice

⚠️ **WARNING**: The stub implementation of `crypt()` is NOT SECURE and should not be used in production. Replace with proper libcrypt compiled for mipsel for any security-sensitive deployment.
