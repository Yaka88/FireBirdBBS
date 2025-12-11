# FireBirdBBS MIPS 32-bit Little Endian 交叉编译说明

## 概述

本文档说明如何将 FireBirdBBS 交叉编译为 32 位 MIPS Little Endian 平台的程序。

FireBirdBBS 是一个用 C 语言编写的字符界面 BBS 站系统，原本运行在 Intel x86 32 位平台（约 2000 年代），使用 GB18030 中文编码。

## 编译要求

### 系统要求
- Linux 构建环境（Ubuntu 18.04 或更高版本推荐）
- 足够的磁盘空间（至少 500MB）
- 互联网连接（用于安装交叉编译工具链）

### 自动安装的依赖
编译脚本会自动安装以下工具：
- gcc-mipsel-linux-gnu
- g++-mipsel-linux-gnu  
- binutils-mipsel-linux-gnu

## 编译特性

### 目标平台
- **架构**: MIPS 32-bit Little Endian (MIPSEL)
- **ABI**: o32 (32-bit ABI)
- **字节序**: Little Endian

### 编译选项
```
MIPS_CFLAGS="-mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2 -U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32"
```

说明：
- `-mips32`: 使用 MIPS32 指令集
- `-mabi=32`: 使用 32 位 ABI
- `-EL`: Little Endian 字节序
- `-fno-stack-protector`: 禁用栈保护
- `-fcommon`: 允许多个定义（旧代码兼容性）
- `-O2`: 优化级别 2
- `-U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32`: 确保使用 32 位 time_t 和文件偏移

### 库编译方式

#### 静态链接的库
以下库采用静态编译，直接链接到可执行文件中：
- **libtermcap.a** - 终端控制库
- **libncurses.a** - 屏幕控制库
- **libcrypt.a** - 加密库
- **libBBS.a** - BBS 核心功能库

#### 动态链接的库
系统 C 库采用动态链接：
- **libc.so** - C 标准库
- **libm.so** - 数学库
- **libdl.so** - 动态加载库
- **libpthread.so** - 线程库
- 其他必要的系统库

动态库文件会被复制到 `lib/` 目录，程序使用 RPATH 设置确保优先使用本地库文件而非系统库。

## 使用方法

### 编译步骤

1. 克隆或下载本仓库：
```bash
git clone https://github.com/Yaka88/FireBirdBBS.git
cd FireBirdBBS
```

2. 赋予编译脚本执行权限：
```bash
chmod +x build-mipsle.sh
```

3. 运行编译脚本：
```bash
./build-mipsle.sh
```

### 编译过程

脚本将执行以下步骤：

1. **安装工具链** - 自动安装 MIPS 交叉编译工具
2. **设置环境** - 创建构建目录和输出目录
3. **配置构建** - 设置编译器和编译选项
4. **编译 libBBS** - 构建 BBS 核心库
5. **编译系统库** - 处理 termcap、ncurses、libcrypt
6. **编译主程序** - 编译 bbsd、chatd、thread 等可执行文件
7. **复制依赖** - 提取并复制 libc 动态库依赖
8. **精简二进制** - 使用 strip 减小文件大小
9. **打包** - 创建 tar.gz 压缩包

### 输出文件

编译完成后会生成：

- `FireBirdBBS-mipsle-YYYYMMDD.tar.gz` - 编译包（日期标记）
- `output-mipsle/bin/` - 可执行文件目录
  - bbsd - 主 BBS 服务程序
  - chatd - 聊天服务程序
  - thread - 线程处理工具
  - 其他工具程序
- `output-mipsle/lib/` - 动态库目录
  - libc.so.* 及其他必要的动态库

## 部署到目标系统

### 方法 1: 使用压缩包

1. 将生成的 tar.gz 文件传输到目标 MIPS 系统
2. 解压：
```bash
tar xzf FireBirdBBS-mipsle-YYYYMMDD.tar.gz
```

3. 进入 bin 目录：
```bash
cd bin
```

4. 运行程序：
```bash
./bbsd
```

### 方法 2: 直接复制

直接复制 `output-mipsle/bin` 和 `output-mipsle/lib` 目录到目标系统。

### 注意事项

1. **RPATH 设置**: 程序已配置 RPATH 为 `$ORIGIN/../lib`，确保程序会首先搜索相对路径的 lib 目录中的动态库。

2. **文件权限**: 确保可执行文件有执行权限：
```bash
chmod +x bin/*
```

3. **目录结构**: 保持 bin 和 lib 的相对位置关系，不要单独移动 bin 目录。

4. **系统兼容性**: 目标系统需要是 MIPS 32-bit Little Endian 架构的 Linux 系统。

## 编码说明

本项目保留原有的 **GB18030** 中文编码格式，未转换为 UTF-8。源代码中的中文字符串和注释均保持原始编码，以确保与原始系统兼容。

## 时间格式

本项目使用 **32 位 time_t** 格式，与原始 2000 年代 x86 32 位平台保持一致，避免 64 位 time_t 兼容性问题。

## 技术细节

### 静态库构建
- libBBS: 从源码编译，包含 BBS 核心功能
- 其他库: 使用仓库中已有的库文件

### RPATH 设置
程序链接时使用以下 LDFLAGS：
```
-Wl,-rpath,'$ORIGIN/../lib'
```
这确保程序在运行时会在自身所在目录的上级目录下的 lib 子目录中查找动态库。

### 栈保护
使用 `-fno-stack-protector` 禁用栈保护，以保持与原始代码的兼容性并减少依赖。

## 故障排除

### 工具链安装失败
如果自动安装失败，手动安装：
```bash
sudo apt-get update
sudo apt-get install gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu
```

### 编译错误
1. 检查是否有足够的磁盘空间
2. 确保网络连接正常
3. 查看脚本输出的具体错误信息

### 运行时库找不到
确保 lib 目录与 bin 目录保持正确的相对位置关系。可以使用以下命令检查：
```bash
ldd bin/bbsd
```

### 架构不匹配
使用 file 命令验证二进制文件架构：
```bash
file bin/bbsd
```
应该显示: `ELF 32-bit LSB executable, MIPS, MIPS32 ...`

## 版本信息

- **项目**: FireBirdBBS 3.0
- **编译目标**: MIPS 32-bit Little Endian
- **编译脚本**: build-mipsle.sh
- **文档版本**: 1.0

## 许可证

遵循 FireBirdBBS 原有许可证。

## 联系方式

如有问题，请提交 Issue 到项目仓库：
https://github.com/Yaka88/FireBirdBBS

---

最后更新: 2024-12-11
