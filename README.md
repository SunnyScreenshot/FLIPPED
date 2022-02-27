

# PicShot

Lightweight screenshot tool

<br>
![](https://img.shields.io/github/license/XMuli/PicShot) ![](https://img.shields.io/github/v/release/XMuli/PicShot?style=flat&color=birightgreen) ![](https://img.shields.io/badge/powered%20by-XMuli-ff69b4)  ![](https://img.shields.io/github/stars/XMuli/PicShot?style=social) ![](https://img.shields.io/github/forks/XMuli/PicShot?style=social&label=Fork)

![](https://img.shields.io/github/workflow/status/XMuli/PicShot/Windows?style=flat&logo=windows) ![](https://img.shields.io/github/workflow/status/XMuli/PicShot/MacOS?style=flat&logo=apple) ![](https://img.shields.io/github/workflow/status/XMuli/PicShot/Linux?style=flat&logo=linux) ![](https://img.shields.io/github/languages/code-size/XMuli/PicShot)  ![](https://img.shields.io/github/downloads/XMuli/PicShot/total)

<br>

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

# 打开 “x64_x86 Cross Tools Command Prompt for VS 2019"
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_PREFIX_PATH=C:/Qt/Qt5.12.11/5.12.11/msvc2017 -G "Visual Studio 16 2019" -A Win32 ..

# 使用 Visual Studio 2019 打开 .sln 解决方案
```





