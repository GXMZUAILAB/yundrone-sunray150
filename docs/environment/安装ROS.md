# 安装无人机对应版本的ROS

> 前言： **在安装ROS之前，请根据无人机的款式或者型号在官网找到对应的ROS版本。** 由于我们无人机使用的是云纵科技旗下的Sunray-150-三维激光雷达款，所以之后所描述的内容均针对Sunray-150-三维激光雷达款

## 步骤一
在官网查找无人机的ROS版本以及ROS对应的系统版本 [Sunray-150-三维激光雷达款](https://wiki.yundrone.cn/docs/pei-zhi-can-shu)

Sunray-150-三维激光雷达款的ROS版本为**ROS1 Neotic**，对应系统版本为**Ubuntu 20.04**
## 步骤二
确定本机的ubuntu系统版本
```bash
lsb_release -a # 使用指令查询
```
- 系统为 20.04：参考 [CSDN](https://blog.csdn.net/weixin_29231221/article/details/159372416)
- 系统为 20.04以上：由于 ROS1 Neotic 最高只支持20.04，所以20.04以上的版本不能直接安装 ROS1 Neotic，请继续阅读下面的内容

## 解决方法
- 直接重装系统（在windows安装虚拟机的还好，是双系统的就麻烦了）
- 使用 Docker 安装
1. 拉取正确的 ROS Noetic 镜像：
```bash
docker pull osrf/ros:noetic-desktop-full
```
> Tips: 换国内源 or 科学上网
2. 创建Docker容器：
```bash
docker run -it \
    --name sunray_ros \
    --network host \
    --privileged \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -v ~/sunray_env:/root/sunray_env \
    osrf/ros:noetic-desktop-full \
    /bin/bash
```
3. 授权 Docker 容器访问 X11 图形显示（用于仿真环境的GUI加载）
```bash
xhost +local:docker
```
> Tips: 之后可使用以下指令进入容器：
```bash
docker start -ai sunray_ros
```
3. **再按照 ROS Noetic 的安装方法正常安装即可。**