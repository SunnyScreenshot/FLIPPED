

# PicShot

Lightweight screenshot tool



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





