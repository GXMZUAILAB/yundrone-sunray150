# yundrone-sunray150

## 项目简介
云纵无人机Sunray150开发项目
## 设备依赖
1. 设备品牌 - 型号：Sunray150 - 三维激光雷达款
2. 设备说明：[硬件整体介绍-云纵科技](https://wiki.yundrone.cn/docs/Sunray150-ying-jian-zheng-ti-jie-shao)
## 环境配置
参考 docs/environment/ 目录下的文件说明部署步骤
## 使用方法
```bash
# 启动 ros
roscore

# 启动 sunray_communciation_bridge (用于无人机与地面站的数据交互)
roslaunch sunray_communication_bridge sunray_communication_bridge.launch uav_id:="1" uav_experiment_num:="1" # 真机
roslaunch sunray_communication_bridge sunray_communication_bridge_sim.launch uav_id:="1" uav_experiment_num:="1" # 仿真

# 启动 px4 仿真 (真机无需执行)
roslaunch sunray_simulator sunray_sim_1uav.launch

# 发送外部定位 
roslaunch sunray_uav_control external_fusion.launch external_source:=2 

# 启动控制节点 
roslaunch sunray_uav_control sunray_control_node.launch 

# 启动键盘控制节点 (真机无需执行)
roslaunch sunray_uav_control terminal_control.launch
```
## 一键启动脚本使用方法
```
#前提:要在docker中安装ROS以及仿真平台(使用one-click-start-docker.sh),或者在原机器或WSL中安装(使用one-click-start-origin.sh)


#将脚本复制到你的用户文件夹,env文件复制到/root/中
#赋予执行权限
chmod +x one-click-start-origin.sh
(docker则使用
chmod +x one-click-start-docker.sh)
#执行启动
./one-click-start.sh
(docker则运行
./one-click-start-docker.sh)
```
## 维护人员
| 姓名  | 邮箱                |
| --- | ----------------- |
| 冼佳炜 | 3188074406@qq.com |
| 李帅相 | 2425057190@qq.com |



