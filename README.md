

# PicShot

Lightweight screenshot tool



![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg)

![macos-badge](https://github.com/xmuli/PicShot/workflows/macos/badge.svg)   ![ubuntu-badge](https://github.com/xmuli/PicShot/workflows/ubuntu/badge.svg)  ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/PicShot/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/PicShot?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/PicShot?style=social) 



## Clone

```bash
# 方式一
git clone --recursive https://github.com/xmuli/PicShot.git   // 它会自动初始化并更新每一个子模块

# 方式二
git clone https://github.com/xmuli/PicShot.git
git submodule init
git submodule update
```



## Build And Delete

```bash
1. build.bat 
2. build.bat delete 
```



## Build

### MSCVC Build

```bash
mkdir build
cd build

# 打开 “x64_x86 Cross Tools Command Prompt for VS 2017"
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_PREFIX_PATH=C:/Qt/Qt5.12.11/5.12.11/msvc2017 -G "Visual Studio 15 2017" -A Win32 ..

# 使用 Visual Studio 2017 打开 .sln 解决方案
```





