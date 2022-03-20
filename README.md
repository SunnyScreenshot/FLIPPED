

# PicShot

Lightweight screenshot tool

<br>
![](https://img.shields.io/github/license/XMuli/PicShot) 

![](https://img.shields.io/github/v/release/XMuli/PicShot?style=flat&color=birightgreen) 

![](https://img.shields.io/badge/powered%20by-XMuli-ff69b4) 

![](https://img.shields.io/github/stars/XMuli/PicShot?style=social) 

 ![](https://img.shields.io/github/forks/XMuli/PicShot?style=social&label=Fork)

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



## Build (MSCVC)

```bash
mkdir build
cd build

# 打开 “x64_x86 Cross Tools Command Prompt for VS 2019"
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_PREFIX_PATH=C:/Qt/Qt5.12.11/5.12.11/msvc2017 -G "Visual Studio 16 2019" -A Win32 ..

使用 Visual Studio 2019 打开 .sln 解决方案

#----------------------------------------------------------------------------------------------
# 本意
Qt 5.12.11 + MSVC 2019 + Win10 21H1 + CMake 3.21.1 使用命令行生成一个 Qt 程序


# 微软
devenv："C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.com"  编译项目实例
vcvarsall.bat "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" 初始化 msvc 交叉编译的环境（x86/x64）

msvc交叉编译:使用vcvarsall.bat设置命令行编译环境（https://blog.csdn.net/10km/article/details/51722353）
%VS140COMNTOOLS%/VC下就有vcvarsall.bat,用于生成命令行编译环境。
如果要在命令行生成 32位代码，就执行vcvarsall x86
如果要在32位系统下生成64位代码，就执行vcvarsall x86_amd64
如果要在64位系统下生成32位代码，就执行vcvarsall x86或vcvarsall amd64_x86
到了VS2015，已经支持arm平台了，所以如果要生成arm平台的代码，就执行vcvarsall x86_arm 如果你的操作系统是64位的也可以 vcvarsall amd64_arm

# cmd 编译 Qt 64 位项目 
添加 "C:\Qt\Qt5.12.11\5.12.11\msvc2017_64\bin" 到 path 后，终端执行 echo %PATH% 使其立即生效
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G "Visual Studio 16 20179" -A x64 ..
devenv GitHubAction.sln /Build "Release|x64"

# cmd 编译 Qt x86 位项目 
添加 "C:\Qt\Qt5.12.11\5.12.11\msvc2017\bin" 到 path 后，终端执行 echo %PATH% 使其立即生效
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
cmake -G "Visual Studio 16 2019" -A Win32 ..
devenv GitHubAction.sln /Build "Release|Win32"
windeployqt --no-angle --no-opengl-sw Release/xxx.exe
```

