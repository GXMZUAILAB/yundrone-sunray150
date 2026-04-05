#!/bin/bash

gnome-terminal -- bash -c "echo '标签页1';sudo docker start sunray_workspace;xhost +;sudo docker exec -it sunray_workspace /bin/bash -c 'source /root/env&&roscore';exec bash" 
sleep 3
gnome-terminal -- bash -c "echo '标签页2';sudo docker exec -it sunray_workspace /bin/bash -c 'source /root/env&&roslaunch sunray_simulator sunray_sim_1uav.launch';exec bash"
sleep 0.1
gnome-terminal -- bash -c "echo '标签页3';sudo docker exec -it sunray_workspace /bin/bash -c 'source /root/env&&roslaunch sunray_uav_control external_fusion.launch external_source:=2';exec bash"
sleep 0.1
gnome-terminal -- bash -c "echo '标签页4';sudo docker exec -it sunray_workspace /bin/bash -c 'source /root/env&&roslaunch sunray_uav_control sunray_control_node.launch';exec bash"
sleep 0.1
gnome-terminal -- bash -c "echo '标签页5';sudo docker exec -it sunray_workspace /bin/bash -c 'source /root/env&&roslaunch sunray_uav_control terminal_control.launch';exec bash"
