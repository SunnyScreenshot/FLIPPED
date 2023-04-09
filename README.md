<div align="center">
  <p>
      <h1>
      <a href="https://github.com/XMuli/Flipped">
          <img src="snapshot/Flipped.svg"  alt="Flipped" />
      </a>
      <br/>
      Flipped
    </h1>
    <br/>
    <h4>Simple and beautiful cross-platform screenshot software.</h4>
  </p>
  <p>
    <a href="https://github.com/XMuli/Flipped/actions/workflows/Windows.yml">
      <img src="https://img.shields.io/github/workflow/status/XMuli/Flipped/Windows?style=flat&logo=windows" alt="Windows Build Status" />
    </a>
    <a href="https://github.com/XMuli/Flipped/actions/workflows/MacOS.yml">
      <img src="https://img.shields.io/github/workflow/status/XMuli/Flipped/MacOS?style=flat&logo=apple" alt="MacOS Build Status" />
    </a>
    <a href="https://github.com/XMuli/Flipped/actions/workflows/Linux.yml">
      <img src="https://img.shields.io/github/workflow/status/XMuli/Flipped/Linux?style=flat&logo=linux" alt="GNU/Linux Build Status" />
    </a>
    <a href="https://flameshot.org/docs/installation/development-build/">
      <img src="https://img.shields.io/github/languages/code-size/XMuli/Flipped" alt="code-size" />
    </a>
    <a href="https://github.com/XMuli/Flipped/releases">
      <img src="https://img.shields.io/github/releaseFlipped/Flipped.svg" alt="Latest Stable Release" />
    </a>
    <a href="https://github.com/XMuli/Flipped/releases">
      <img src="https://img.shields.io/github/downloads/XMuli/Flipped/total" alt="Total Downloads" />
    </a>
  <a href="https://github.com/XMuli/Flipped">
      <img src="https://img.shields.io/github/release/XMuli/Flipped.svg?label=docs" alt="Docs" />
    </a>
  </p>
  <p align="right"><br><a href="README.md">English</a> | <a href="README.zh_CN.md">简体中文</a></p>
</div>
[TOC]

<br>

## Features

- Multi-screen screenshot, time-lapse screenshot, custom screenshot
- Pinning the picture
- Intelligent window recognition（Windows & Linux）
- Draw Rectangle, Ellipse, Arrow, Custom Path, Mosaic, Text, Serial Number
- Undo, Redo (multi-level), Save, Cancel, Copy
- Plugin Framework

<br>

## Preview

### Voide

- [P1] [FLIPPED-MACOS operation demonstration](https://www.bilibili.com/video/BV1rX4y1D7EZ?p=1)
- [P2] [FLIPPED-WINDOWS operation demonstration](https://www.bilibili.com/video/BV1rX4y1D7EZ?p=2)
- [P3] [FLIPPED-LINUX (ubuntu 20.04) operation demonstration](https://www.bilibili.com/video/BV1rX4y1D7EZ?p=3)

<br>

### Snapshoot

- **MacOS**

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121457071.jpg" width="100%"/>

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121524707.jpg" width="100%"/>

- **Windows**

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121616530.jpg" width="100%"/>

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121524281.jpg" width="100%"/>

- **Linux**

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121447431.jpg" width="100%"/>

<br>

<font color=#D0087E size=4 face="幼圆"> **Other:** More snapshoot effects can be → [here](. \snapshot) preview </font>

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121515818.jpg" width="30%"/> <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202303121522838.jpg" width="30%"/>

<br>

## Keyboard shortcuts

| Keys                                                         | Description              | Mode  |
| ------------------------------------------------------------ | ------------------------ | ----- |
| <kbd>←</kbd>, <kbd>↓</kbd>, <kbd>↑</kbd>, <kbd>→</kbd> ( <kbd>A</kbd>, <kbd>S</kbd>, <kbd>W</kbd>, <kbd>D</kbd> ) | Move selection 1px       | Local |
| <kbd>Ctrl</kbd> + <kbd>←</kbd>, <kbd>↓</kbd>, <kbd>↑</kbd>, <kbd>→</kbd> | Extended selection 1 px  | Local |
| <kbd>Alt</kbd> + <kbd>←</kbd>, <kbd>↓</kbd>, <kbd>↑</kbd>, <kbd>→</kbd> | Shrink selection 1 px    | Local |
| <kbd>Shift</kbd> + <kbd>←</kbd>, <kbd>↓</kbd>, <kbd>↑</kbd>, <kbd>→</kbd> | Move selection 10 px     | Local |
| <kbd>Shift</kbd> + <kbd>Ctrl</kbd> + <kbd>←</kbd>, <kbd>↓</kbd>, <kbd>↑</kbd>, <kbd>→</kbd> | Extended selection 10 px | Local |
| <kbd>Shift</kbd> + <kbd>Alt</kbd> + <kbd>←</kbd>, <kbd>↓</kbd>, <kbd>↑</kbd>, <kbd>→</kbd> | Shrink selection 10 px   | Local |
| <kbd>Shift</kbd> + <kbd>F4</kbd>                             | 快速保存截图             | 局部  |
| <kbd>Esc</kbd>                                               | 退出                     | 局部  |
|                                                              |                          |       |
| <kbd>F6</kbd>                                                | 窗口智能截图             | 全局  |
| <kbd>F7</kbd>                                                | 延时截图                 | 全局  |
| <kbd>F8</kbd>                                                | 全屏截图                 | 全局  |

<br>

## Compilation

### Dependencies

- Qt >= 5.15.2
- CMake >= 3.16
- MSVC >= 2019 | MinGW >=  8.1.0 | GCC >= 9.4 | Clang >= 12.0

​	NOTE: This is a successfully compiled dependency version, lower versions have not been tested.

<br>

### Windows

- **Kit Tools:** Windows 10 & Qt 5.15.2 & CMake 3.24.1 & MSVC 2019 ( or MinGW 8.1.0)

- **Compile Step:**

  ```bash
  # ******************** MSVC 2019 ********************
  #『Step1』
  # x86:
  # After adding "C:\Qt\5.15.2\msvc2019\bin" to the path, execute echo %PATH% in the terminal to make it take effect immediately.
  "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
  cmake -G "Visual Studio 16 2019" -A Win32 ..
  devenv Flipped.sln /Build "Release|Win32"
  
  # x64:
  # After adding "C:\Qt\5.15.2\msvc2019_64\bin" to the path, execute echo %PATH% in the terminal to make it take effect immediately.
  "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
  cmake -G "Visual Studio 16 2019" -A x64 ..
  devenv Flipped.sln /Build "Release|x64"
  
  #『Step2』
  Visual Studio 2019 open `Flipped.sln`
  
  #『Step3』
  windeployqt  bin/Flipped.exe --no-translations
  
  # ******************** MinGW 8.1.0 ********************
  QtCreator opens the `CMakeLists.txt` file in the root directory of the source code
  ```

<br>

### MacOS / Linux

- **Kit Tools:** 

  - **MacOS:** MacOS 10.15 & Qt 5.15.2 & CMake 3.24 & Clang 12.0
  - **Linux:** Ubuntu 20.04 & Qt 5.15.2 & CMake 3.24 & GCC 9.4

- **Compile Step:**

  ```bash
  git clone --recursive https://github.com/XMuli/Flipped.git
  cd Flipped
  mkdir build & cd build
  cmake ..
  make -j16
  ```

<br>

## Author

[![alt text](https://img.shields.io/badge/QQ-XMuli-brightgreen)](https://sighttp.qq.com/authd?IDKEY=31f3ef7312b39e2c8dc822ae2f4c3b3118e1a6f31cc83373) : Chat with me directly~                   |    [![alt text](https://img.shields.io/badge/GitHub-XMuli-brightgreen)](https://github.com/XMuli) : View my homepage

[![alt text](https://img.shields.io/badge/Blog-%E5%81%95%E8%87%A7%E7%9A%84%E5%B0%8F%E7%AB%99-ff69b4)](https://ifmet.cn/) : Curious about my nest           |    [![alt text](https://img.shields.io/badge/Blog-国内镜像-ff69b4)](https://blog.csdn.net/qq_33154343) ：Views 100W+

<br>

## Contributors

If it helps you, or find it useful, <font color=#FE7207  size=4 face="幼圆">you can click on the item's <font color=#D0087E size=4 face="幼圆">**⭐Star** **🍴 Fork** </font> of the two icons, conveniently lift the hand between, said a point of praise the hand,</font> There is a fragrance in your hand；The next best thing is to buy me a cold Coke.   → [project-flipped](https://github.com/XMuli/FlippedPackage)

<br>

<details>
    <summary> <b>Of course you can also give a cold Coke [Donate/Reward ← Click to expand QR code]</b></summary>
  <p> - If you have something to learn from the project, you can also invite me to share a glass of Fat House Ice and Coke. - </p>
  <pre><img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202302282339037.png" width="80%"/></pre>
</details>

<br>

## Feedback & How to contribute

Feedback & How to contribute

You are very welcome to join us! You can [open an issue](https://github.com/XMuli/FlippedPackage/issues) ; for any bug, suggestion, feature idea, or to help improve this software. Or help improve the project by submitting a Pull Request.

<br>

You can also add  [![alt text](https://img.shields.io/badge/QQ_Groups-418103279-brightgreen)](https://qm.qq.com/cgi-bin/qm/qr?k=jsD03QzMohGZm0TqYAFh3BvKOpCGlTcT&jump_from=webapi&authKey=DMUwiCQ6ta95PoH8JmtZ+Jz9y7Ozg3yinEsxmm92rNXZRVeMPD7NRgjU+dmUI8Xu) of the exchange group.

<br>

## Package Download

<font color=#D0087E> → Offline Installer Download [Releases](https://github.com/XMuli/FlippedPackage/releases); </font>

<font color=#D0087E>→ **Feedback BUG/SUGGEST, user community, etc., and the latest version of the installer get, in QQ group:[418103279 ](https://qm.qq.com/cgi-bin/qm/qr?k=jsD03QzMohGZm0TqYAFh3BvKOpCGlTcT&jump_from= webapi&authKey=DMUwiCQ6ta95PoH8JmtZ+Jz9y7Ozg3yinEsxmm92rNXZRVeMPD7NRgjU+dmUI8Xu)** </font>

<br>

## Series Address

[QtExamples](https://github.com/XMuli/QtExamples) Welcome `star` ⭐ and `fork` 🍴 to this series of `C++ / QT / DTK` studies, with a table of contents for learning from the beginning to the end

[ExCMake](https://github.com/XMuli/ExCMake) Welcome `star` ⭐ and `fork` 🍴 to this series of `CMake` studies, with a table of contents for learning from the beginning to the end
