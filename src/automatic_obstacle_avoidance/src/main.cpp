#include <ros/ros.h>
#include "drone_controller.hpp"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "drone_control_node");
    ros::NodeHandle nh("~");
    
    int uav_id = 1;
    nh.param("uav_id", uav_id, 1);
    
    DroneController drone(nh, uav_id);
    ros::Duration(1.0).sleep();
    
    ROS_INFO("========================================");
    ROS_INFO("Starting fixed flight sequence");
    ROS_INFO("========================================");
    
    // Step 1: Auto takeoff to 1 meter
    ROS_INFO("\n>>> Step 1: Auto takeoff to 1m");
    if (!drone.autoTakeoff(1.0)) {
        ROS_ERROR("Takeoff failed, exiting");
        return -1;
    }
    
    ros::Duration(2.0).sleep();
    
    // Step 2: Go to (2,2,2) using inertial frame
    ROS_INFO("\n>>> Step 2: Inertial frame - go to (2,2,2)");
    drone.gotoXYZYaw(2, 2, 2, 0);
    ros::Duration(5.0).sleep();
    
    // Step 3: Move forward 2 meters (body frame)
    ROS_INFO("\n>>> Step 3: Body frame - move forward 2m");
    drone.moveForward(2);
    ros::Duration(3.0).sleep();
    
    // Step 4: Turn to 45 degrees
    ROS_INFO("\n>>> Step 4: Turn to 45 degrees");
    drone.turnTo(45);
    ros::Duration(3.0).sleep();
    
    // Step 5: Land
    ROS_INFO("\n>>> Step 5: Land");
    drone.autoLand();
    
    ROS_INFO("========================================");
    ROS_INFO("Flight sequence completed");
    ROS_INFO("========================================");
    
    return 0;
}