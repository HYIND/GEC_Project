# 基于GEC6818开发板实现的小型系统

该系统包含相册、相机、音乐、视频和游戏（木板挡小球）功能。

![](https://github.com/HYIND/GEC_Project/blob/master/screenshots/pic1.jpg)


该工程使用CMake组织，安装cmake后，使用终端进入build目录，运行以下命令。（运行前请确保配置了arm-linux-gcc的交叉编译环境）

cmake .
make
然后将在bin文件夹下生成最新的可执行文件myproject。

project文件夹为项目资源和可执行文件，project/myproject为可执行文件。
project/photo文件夹存储图片和照片。
project/video文件夹存储视频。
project/music文件夹存储音乐。
project/windows_pic和project/game_pic 存储程序运行的相关bmp资源

相关的资源文件可自行替换