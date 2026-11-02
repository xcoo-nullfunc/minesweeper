# 扫雷/Minesweeper

zh [en](README.md)

## 写在前面

这个项目算是我第一个较为正式的项目，我对于一些东西不太熟悉，可以说我几乎只专注于代码本身了，在一些问题上可能还有些疏忽

该项目从Godot官方提供的模板([godot-cpp-template](https://github.com/godotengine/godot-cpp-template))开始开发，运行在[GodotEngine](https://github.com/godotengine/godot) 4.7.2

## 编译/构建

### 动态库构建

本项目使用C++20标准构建，

本项目使用SCons构建

```
scons
```

项目里保留了CMakeLists.txt作为备用

构建安卓平台的动态库需要`28.1.13356709`版本的NDK
