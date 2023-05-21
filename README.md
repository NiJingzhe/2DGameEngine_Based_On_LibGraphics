# 2DGameEngine Based on LibGraphics

#### 一、介绍
作为ZJU的C大程大作业的项目

#### 二、安装教程

1. 请确保自己的`cmake`版本高于`3.8.0`
2. 我们提供了可以直接使用的`CMakeLists.txt`, 可以根据其中的注释修改后使用
3. 修改完`CMakeLists.txt`后，在当前文件夹执行 `cmake .` 即可生成`build`文件夹
4. 进入`build`文件夹，执行`make`即可获得最终目标，目标二进制文件名为`2DGameEngine.exe`
5. 请务必将`res`文件夹置于目标文件的启动目录下，否则将找不到相关资源
6. **总结：**
```bash
cmake .
cp -r ./res ./build/res
cd build
make -j18
./2DGameEngine.exe
```
#### 三、参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

