# 项目结构说明
本项目采用 CMake 构建，包含主程序和自定义日志模块 `myLog`。<br>
本项目所设计的库接口为 C 语言风格接口，具有Windows下**MSVC**、**MinGW**与**Cygwin**的完整构建支持（Linux GCC 尚未实测，预期兼容）。支持将头文件与库文件安装到标准目录，方便第三方项目引用。<br>
以下是精简后的项目目录结构：
### 目录结构
```text
.
├── CMakeLists.txt                                      # 项目主构建配置
├── README.md                                           # 项目说明文档
├── myLog/
│   ├── CMakeLists.txt                                  # 日志模块构建配置
│   ├── include/
│   │   └── myLogApi.h                                  # 日志库对外接口头文件
│   └── src/
│       ├── myLog.cpp                                   # 日志模块实现
│       ├── myLog.h                                     # 日志模块头文件
│       └── myLogApi.cpp                                # 日志库接口实现
├── lib/
│   ├── Debug_Windows_GCC_v11.5.0_Cygwin_x64/
│   │   └── libmyLogd.a                                 # Debug模式(Cygwin)生成的静态库
│   ├── Debug_Windows_GCC_v13.1.0_MinGW_x64/
│   │   └── libmyLogd.a                                 # Debug模式(MinGW)生成的静态库
│   ├── Debug_Windows_MSVC_v1900_x64/
│   │   └── myLogd.lib                                  # Debug模式(MSVC)生成的静态库
│   ├── Release_Windows_GCC_v11.5.0_Cygwin_x64/
│   │   └── libmyLog.a                                  # Release模式(Cygwin)生成的静态库
│   ├── Release_Windows_GCC_v13.1.0_MinGW_x64/
│   │   └── libmyLog.a                                  # Release模式(MinGW)生成的静态库
│   └── Release_Windows_MSVC_v1900_x64/
│       └── myLog.lib                                   # Release模式(MSVC)生成的静态库
├── bin/
│   ├── Debug_Windows_GCC_v11.5.0_Cygwin_x64/
│   │   └── cpp-basekit.exe                             # Debug模式(Cygwin)生成的可执行文件
│   ├── Debug_Windows_GCC_v13.1.0_MinGW_x64/
│   │   └── cpp-basekit.exe                             # Debug模式(MinGW)生成的可执行文件
│   ├── Debug_Windows_MSVC_v1900_x64/
│   │   └── cpp-basekit.exe                             # Debug模式(MSVC)生成的可执行文件
│   ├── Release_Windows_GCC_v11.5.0_Cygwin_x64/
│   │   └── cpp-basekit.exe                             # Release模式(Cygwin)生成的可执行文件
│   ├── Release_Windows_GCC_v13.1.0_MinGW_x64/
│   │   └── cpp-basekit.exe                             # Release模式(MinGW)生成的可执行文件
│   └── Release_Windows_MSVC_v1900_x64/
│       └── cpp-basekit.exe                             # Release模式(MSVC)生成的可执行文件
├── install/
│   ├── include/
│   │   ├── myLibrary_Windows_GCC_v11.5.0_Cygwin_x64    # 安装后的头文件目录(Cygwin), 供外部项目include使用
│   │   ├── myLibrary_Windows_GCC_v13.1.0_MinGW_x64     # 安装后的头文件目录(MinGW), 供外部项目include使用
│   │   └── myLibrary_Windows_MSVC_v1900_x64            # 安装后的头文件目录(MSVC), 供外部项目include使用
│   ├── lib/
│   │   ├── Debug_Windows_GCC_v11.5.0_Cygwin_x64        # Debug模式安装的库文件输出目录(Cygwin), 供外部项目链接
│   │   ├── Debug_Windows_GCC_v13.1.0_MinGW_x64         # Debug模式安装的库文件输出目录(MinGW), 供外部项目链接
│   │   ├── Debug_Windows_MSVC_v1900_x64                # Debug模式安装的库文件输出目录(MSVC), 供外部项目链接
│   │   ├── Release_Windows_GCC_v11.5.0_Cygwin_x64      # Release模式安装的库文件输出目录(Cygwin), 供外部项目链接
│   │   ├── Release_Windows_GCC_v13.1.0_MinGW_x64       # Release模式安装的库文件输出目录(MinGW), 供外部项目链接
│   │   └── Release_Windows_MSVC_v1900_x64              # Release模式安装的库文件输出目录(MSVC), 供外部项目链接
├── src/
│   └── main.cpp                                        # 主程序入口
```
### 致谢
项目使用的是 [CLion](https://www.jetbrains.com/clion/) 编译调试，感谢 JetBrains 提供如此强大好用的 IDE，也感谢所有开源工具和开发者社区的支持！
### 结语
欢迎感兴趣的朋友提 Issue、提交 PR 或一起完善这个项目。