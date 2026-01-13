# 构建指南

## Windows 系统构建

### 使用 Visual Studio

1. 打开命令提示符或PowerShell，进入项目目录
2. 创建构建目录：
```bash
mkdir build
cd build
```

3. 生成Visual Studio项目文件：
```bash
cmake .. -G "Visual Studio 16 2019" -A x64
```

4. 编译项目：
```bash
cmake --build . --config Release
```

5. 运行程序：
```bash
.\bin\Release\path_planning_engine.exe
```

### 使用 MinGW

1. 创建构建目录：
```bash
mkdir build
cd build
```

2. 生成Makefile：
```bash
cmake .. -G "MinGW Makefiles"
```

3. 编译：
```bash
mingw32-make
```

4. 运行：
```bash
.\bin\path_planning_engine.exe
```

## Linux/macOS 系统构建

1. 创建构建目录：
```bash
mkdir build
cd build
```

2. 生成Makefile：
```bash
cmake ..
```

3. 编译：
```bash
make -j4
```

4. 运行：
```bash
./bin/path_planning_engine
```

## 运行测试

编译完成后，运行测试程序：

**Windows:**
```bash
.\bin\path_planning_test.exe
```

**Linux/macOS:**
```bash
./bin/path_planning_test
```

## 常见问题

### 问题1: CMake找不到编译器
**解决方案**: 确保已安装C++编译器，并添加到系统PATH中。

### 问题2: 找不到数据文件
**解决方案**: 确保数据文件在 `data/` 目录下，或者使用绝对路径指定数据文件。

### 问题3: 编译错误
**解决方案**: 确保使用C++17或更高版本的编译器。
