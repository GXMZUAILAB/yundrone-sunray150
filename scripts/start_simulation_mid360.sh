#!/bin/bash
# 功能：使用 tmux 启动 Sunray 无人机仿真与控制系统

SESSION="yundrone-env"

# 检查 tmux 是否安装
if ! command -v tmux &> /dev/null; then
    echo "[ERROR] tmux not found. Please install: sudo apt install tmux"
    exit 1
fi

# 如果会话已存在，先关闭
tmux kill-session -t $SESSION 2>/dev/null

# 创建新会话，第一个窗口运行 roscore
tmux new-session -d -s $SESSION -n "roscore"
tmux send-keys -t $SESSION:roscore "roscore" C-m
sleep 3  # 等待 roscore 完全启动

# 窗口2：仿真器
tmux new-window -t $SESSION -n "simulator"
tmux send-keys -t $SESSION:simulator "roslaunch sunray_simulator sunray150_with_mid360.launch" C-m

# 窗口3：外部融合
tmux new-window -t $SESSION -n "external_fusion"
tmux send-keys -t $SESSION:external_fusion "roslaunch sunray_uav_control external_fusion.launch external_source:=2" C-m

# 窗口4：控制节点
tmux new-window -t $SESSION -n "control_node"
tmux send-keys -t $SESSION:control_node "roslaunch sunray_uav_control sunray_control_node.launch" C-m

# 窗口5：终端控制
tmux new-window -t $SESSION -n "terminal_control"
tmux send-keys -t $SESSION:terminal_control "roslaunch sunray_uav_control terminal_control.launch" C-m

# 窗口6：通信桥接
tmux new-window -t $SESSION -n "bridge"
tmux send-keys -t $SESSION:bridge "roslaunch sunray_communication_bridge sunray_communication_bridge_sim.launch uav_id:=\"1\" uav_simulation_num:=\"1\"" C-m

# 附加到会话
tmux attach-session -t $SESSION