# yundrone-sunray150无人机开发

## 项目简介
云纵无人机Sunray150开发项目

开发清单:
- 无人机基础指令控制
- 视觉识别
- 自动避障

## 设备依赖
- 设备品牌 - 型号：Sunray150 - 三维激光雷达款
- 设备说明：[硬件整体介绍-云纵科技](https://wiki.yundrone.cn/docs/Sunray150-ying-jian-zheng-ti-jie-shao)
## 环境配置
- ROS1 Neotic
- 无人机仿真平台(Gazebo)

参考 docs/environment/ 目录下的文件说明部署步骤
## 使用方法
### 编译官方仓库代码
```bash
cd ~/Sunray
./build.sh # 选择构建UAV模块
```
### 运行本仓库代码
```bash
cd yundrone-sunray150 

./scripts/start_simulation.sh # 启动仿真环境

catkin_make # 编译项目
source devel/setup.bash # 加载环境变量
roslaunch automatic_obstacle_avoidance demo1.launch # 启动项目
```

## 维护人员
| 姓名  | 邮箱                |
| --- | ----------------- |
| 冼佳炜 | 3188074406@qq.com |
| 李帅相 | 2425057190@qq.com |