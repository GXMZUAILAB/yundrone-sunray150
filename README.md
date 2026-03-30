# yundrone-sunray150

## 项目简介
云纵无人机Sunray150开发项目
## 设备依赖
设备品牌 - 型号：Sunray150 - 三维激光雷达款
## 环境配置
参考 docs/environment/ 目录下的文件说明部署步骤
## 使用方法
```bash
# 启动 ros
roscore

# 启动 sunray_communciation_bridge (用于无人机与地面站的数据交互)
roslaunch sunray_communication_bridge sunray_communication_bridge.launch

# 启动 px4 仿真 (真机无需执行)
roslaunch sunray_simulator sunray_sim_1uav.launch

# 发送外部定位 
roslaunch sunray_uav_control external_fusion.launch external_source:=2 

# 启动控制节点 
roslaunch sunray_uav_control sunray_control_node.launch 

# 启动键盘控制节点 (真机无需执行)
roslaunch sunray_uav_control terminal_control.launch
```
## 维护人员
| 姓名  | 邮箱                |
| --- | ----------------- |
| 冼佳炜 | 3188074406@qq.com |
| 李相帅 | 2425057190@qq.com |



