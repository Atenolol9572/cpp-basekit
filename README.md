# 项目结构说明
本项目采用 CMake 构建，包含主程序和自定义日志模块 `myLog`。<br>
以下是精简后的项目目录结构：
### 目录结构
```text
.
├── CMakeLists.txt              # 项目主构建配置
├── README.md                   # 项目说明文档
├── myLog/
│   ├── CMakeLists.txt          # 日志模块构建配置
│   ├── include/
│   │   └── myLogLib.h          # 日志库对外接口头文件
│   └── src/
│       ├── myLog.cpp           # 日志模块实现
│       ├── myLog.h             # 日志模块头文件
│       └── myLogLib.cpp        # 日志库接口实现
├── lib/
│   ├── Debug_Windows_MSVC_v1900_x64/
│   │   └── myLogLib_debug.lib  # Debug模式(MSVC)生成的静态库
│   └── Release_Windows_MSVC_v1900_x64/
│       └── myLogLib.lib        # Release模式(MSVC)生成的静态库
├── bin/
│   ├── Debug_Windows_MSVC_v1900_x64/
│   │   └── cpp-basekit.exe     # Debug模式(MSVC)生成的可执行文件
│   └── Release_Windows_MSVC_v1900_x64/
│       └── cpp-basekit.exe     # Release模式(MSVC)生成的可执行文件
├── src/
│   └── main.cpp                # 主程序入口
```
### 致谢
项目使用的是 [CLion](https://www.jetbrains.com/clion/) 编译调试，感谢 JetBrains 提供如此强大好用的 IDE，也感谢所有开源工具和开发者社区的支持！
### 结语
欢迎感兴趣的朋友提 Issue、提交 PR 或一起完善这个项目。