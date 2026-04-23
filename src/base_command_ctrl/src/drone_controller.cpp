#include "drone_controller.hpp"
#include <cmath>
#include <ros/ros.h>

static const double DEG_TO_RAD = M_PI / 180.0;

DroneController::DroneController(ros::NodeHandle& nh, int uav_id)
    : nh_(nh),
      connected_(false),
      armed_(false),
      control_mode_(""),
      landed_state_(0),
      uav_id_(uav_id)
{
    uav_name_ = "/uav" + std::to_string(uav_id_);
    setup_pub_ = nh_.advertise<sunray_msgs::UAVSetup>(uav_name_ + "/sunray/setup", 1);
    cmd_pub_ = nh_.advertise<sunray_msgs::UAVControlCMD>(uav_name_ + "/sunray/uav_control_cmd", 1);
    state_sub_ = nh_.subscribe(uav_name_ + "/sunray/uav_state", 10, &DroneController::stateCallback, this);
    ros::Duration(1.0).sleep();
    ROS_INFO("DroneController initialized for %s", uav_name_.c_str());
}

void DroneController::stateCallback(const sunray_msgs::UAVState::ConstPtr& msg)
{
    connected_ = msg->connected;
    armed_ = msg->armed;
    control_mode_ = msg->control_mode;
    landed_state_ = msg->landed_state;
    
    // 手动赋值位置、速度、姿态（数组 -> geometry_msgs 类型）
    position_.x = msg->position[0];
    position_.y = msg->position[1];
    position_.z = msg->position[2];
    
    velocity_.x = msg->velocity[0];
    velocity_.y = msg->velocity[1];
    velocity_.z = msg->velocity[2];
    
    attitude_.x = msg->attitude[0];
    attitude_.y = msg->attitude[1];
    attitude_.z = msg->attitude[2];
    
    mode_ = msg->mode;
    move_mode_ = msg->move_mode;
}

void DroneController::sendSetupCmd(uint8_t cmd, const std::string& control_mode, const std::string& px4_mode)
{
    sunray_msgs::UAVSetup msg;
    msg.header.stamp = ros::Time::now();
    msg.cmd = cmd;
    if (!control_mode.empty()) msg.control_mode = control_mode;
    if (!px4_mode.empty()) msg.px4_mode = px4_mode;
    setup_pub_.publish(msg);
}

void DroneController::sendControlCmd(uint8_t cmd,
                                     const geometry_msgs::Point* pos,
                                     double yaw_rad,
                                     double yaw_rate_rad,
                                     double latitude, double longitude, double altitude)
{
    sunray_msgs::UAVControlCMD msg;
    msg.header.stamp = ros::Time::now();
    msg.cmd = cmd;
    if (pos) {
        msg.desired_pos[0] = pos->x;
        msg.desired_pos[1] = pos->y;
        msg.desired_pos[2] = pos->z;
    }
    msg.desired_yaw = yaw_rad;
    msg.desired_yaw_rate = yaw_rate_rad;
    msg.latitude = latitude;
    msg.longitude = longitude;
    msg.altitude = altitude;
    cmd_pub_.publish(msg);
}

// ---------- Basic commands ----------
void DroneController::arm() { sendSetupCmd(sunray_msgs::UAVSetup::ARM); ROS_INFO("ARM command sent"); }
void DroneController::disarm() { sendSetupCmd(sunray_msgs::UAVSetup::DISARM); ROS_INFO("DISARM command sent"); }
void DroneController::setCmdControl() { sendSetupCmd(sunray_msgs::UAVSetup::SET_CONTROL_MODE, "CMD_CONTROL", ""); ROS_INFO("Switch to CMD_CONTROL mode"); }
void DroneController::setOffboardMode() { sendSetupCmd(sunray_msgs::UAVSetup::SET_PX4_MODE, "", "OFFBOARD"); ROS_INFO("Switch to OFFBOARD mode"); }
void DroneController::setRCControl() { sendSetupCmd(sunray_msgs::UAVSetup::SET_CONTROL_MODE, "RC_CONTROL", ""); ROS_INFO("Switch to RC_CONTROL mode"); }
void DroneController::rebootPX4() { sendSetupCmd(sunray_msgs::UAVSetup::REBOOT_PX4); ROS_INFO("Reboot PX4 command sent"); }
void DroneController::emergencyKill() { sendSetupCmd(sunray_msgs::UAVSetup::EMERGENCY_KILL); ROS_INFO("EMERGENCY KILL command sent"); }
void DroneController::takeoff() { sendControlCmd(sunray_msgs::UAVControlCMD::Takeoff); ROS_INFO("Takeoff command sent"); }
void DroneController::land() { sendControlCmd(sunray_msgs::UAVControlCMD::Land); ROS_INFO("Land command sent"); }
void DroneController::hover() { sendControlCmd(sunray_msgs::UAVControlCMD::Hover); ROS_INFO("Hover command sent"); }
void DroneController::returnToLaunch() { sendControlCmd(sunray_msgs::UAVControlCMD::Return); ROS_INFO("Return-to-launch command sent"); }

// ---------- Inertial frame control ----------
void DroneController::gotoXYZ(double x, double y, double z)
{
    geometry_msgs::Point pos;
    pos.x = x; pos.y = y; pos.z = z;
    sendControlCmd(sunray_msgs::UAVControlCMD::XyzPos, &pos);
    ROS_INFO("Go to XYZ (%.2f, %.2f, %.2f) m", x, y, z);
}

void DroneController::gotoXYZYaw(double x, double y, double z, double yaw_deg)
{
    geometry_msgs::Point pos;
    pos.x = x; pos.y = y; pos.z = z;
    sendControlCmd(sunray_msgs::UAVControlCMD::XyzPosYaw, &pos, yaw_deg * DEG_TO_RAD);
    ROS_INFO("Go to XYZ (%.2f, %.2f, %.2f) m, yaw %.1f deg", x, y, z, yaw_deg);
}

void DroneController::gotoGlobal(double latitude, double longitude, double altitude, double yaw_deg)
{
    sendControlCmd(sunray_msgs::UAVControlCMD::GlobalPos, nullptr, yaw_deg * DEG_TO_RAD, 0.0, latitude, longitude, altitude);
    ROS_INFO("Go to global: lat=%.6f, lon=%.6f, alt=%.2f m, yaw %.1f deg", latitude, longitude, altitude, yaw_deg);
}

void DroneController::turnTo(double yaw_deg)
{
    gotoXYZYaw(position_.x, position_.y, position_.z, yaw_deg);
}

// ---------- Body frame control ----------
void DroneController::moveBody(double forward, double right, double up, double yaw_deg)
{
    geometry_msgs::Point pos;
    pos.x = forward; pos.y = right; pos.z = up;
    sendControlCmd(sunray_msgs::UAVControlCMD::XyzPosYawBody, &pos, yaw_deg * DEG_TO_RAD);
    ROS_INFO("Body move: forward %.2f, right %.2f, up %.2f, yaw %.1f deg", forward, right, up, yaw_deg);
}

void DroneController::moveForward(double distance, double yaw_deg) { moveBody(distance, 0, 0, yaw_deg); }
void DroneController::moveBackward(double distance, double yaw_deg) { moveBody(-distance, 0, 0, yaw_deg); }
void DroneController::moveRight(double distance, double yaw_deg) { moveBody(0, distance, 0, yaw_deg); }
void DroneController::moveLeft(double distance, double yaw_deg) { moveBody(0, -distance, 0, yaw_deg); }
void DroneController::moveUp(double distance, double yaw_deg) { moveBody(0, 0, distance, yaw_deg); }
void DroneController::moveDown(double distance, double yaw_deg) { moveBody(0, 0, -distance, yaw_deg); }
void DroneController::turnBody(double yaw_deg) { moveBody(0, 0, 0, yaw_deg); }

// ---------- Advanced control ----------
void DroneController::setVelocity(double vx, double vy, double vz, double yaw_rate_deg)
{
    sunray_msgs::UAVControlCMD msg;
    msg.header.stamp = ros::Time::now();
    msg.cmd = sunray_msgs::UAVControlCMD::XyzPosYaw; // placeholder, may need dedicated cmd
    msg.desired_vel[0] = vx;
    msg.desired_vel[1] = vy;
    msg.desired_vel[2] = vz;
    msg.desired_yaw_rate = yaw_rate_deg * DEG_TO_RAD;
    cmd_pub_.publish(msg);
    ROS_INFO("Velocity command: vx=%.2f, vy=%.2f, vz=%.2f m/s", vx, vy, vz);
}

void DroneController::gotoPoint(double x, double y, double z, double yaw_deg)
{
    geometry_msgs::Point pos;
    pos.x = x; pos.y = y; pos.z = z;
    sendControlCmd(sunray_msgs::UAVControlCMD::Point, &pos, yaw_deg * DEG_TO_RAD);
    ROS_INFO("Waypoint point command: (%.2f, %.2f, %.2f) m", x, y, z);
}

// ---------- Wait functions ----------
bool DroneController::waitForConnection(double timeout)
{
    ros::Time start = ros::Time::now();
    while (ros::ok() && !connected_) {
        if ((ros::Time::now() - start).toSec() > timeout) {
            ROS_ERROR("Wait for connection timeout");
            return false;
        }
        ros::Duration(0.5).sleep();
        ros::spinOnce();
    }
    ROS_INFO("UAV connected");
    return true;
}

bool DroneController::waitForArmed(double timeout)
{
    ros::Time start = ros::Time::now();
    while (ros::ok() && !armed_) {
        if ((ros::Time::now() - start).toSec() > timeout) {
            ROS_ERROR("Wait for arm timeout");
            return false;
        }
        ros::Duration(0.5).sleep();
        ros::spinOnce();
    }
    ROS_INFO("UAV armed");
    return true;
}

bool DroneController::waitForTakeoff(double target_height, double timeout)
{
    // wait for landed_state == 2 (airborne)
    ros::Time start = ros::Time::now();
    while (ros::ok() && landed_state_ != 2) {
        if ((ros::Time::now() - start).toSec() > timeout) {
            ROS_ERROR("Wait for takeoff timeout (landed_state not 2)");
            return false;
        }
        ros::Duration(0.5).sleep();
        ros::spinOnce();
    }
    ROS_INFO("UAV airborne");

    // optionally wait for altitude
    start = ros::Time::now();
    while (ros::ok() && std::fabs(position_.z - target_height) > 0.2) {
        if ((ros::Time::now() - start).toSec() > 5.0) {
            ROS_WARN("Altitude target timeout, current z=%.2f m", position_.z);
            break;
        }
        ros::Duration(0.5).sleep();
        ros::spinOnce();
    }
    ROS_INFO("Takeoff complete, height=%.2f m", position_.z);
    return true;
}

bool DroneController::waitForLand(double timeout)
{
    ros::Time start = ros::Time::now();
    while (ros::ok() && landed_state_ != 1) {
        if ((ros::Time::now() - start).toSec() > timeout) {
            ROS_ERROR("Wait for land timeout");
            return false;
        }
        ros::Duration(0.5).sleep();
        ros::spinOnce();
    }
    ROS_INFO("Land complete");
    return true;
}

// ---------- One-key actions ----------
bool DroneController::autoTakeoff(double target_height, bool wait)
{
    ROS_INFO("========== Auto Takeoff Start ==========");
    if (!waitForConnection()) return false;
    setCmdControl();
    ros::Duration(0.5).sleep();
    setOffboardMode();
    ros::Duration(0.5).sleep();
    arm();
    if (wait) {
        if (!waitForArmed()) return false;
    } else {
        ros::Duration(1.0).sleep();
    }
    takeoff();
    if (wait) {
        if (!waitForTakeoff(target_height)) return false;
    } else {
        ros::Duration(5.0).sleep();
    }
    ROS_INFO("Auto takeoff finished");
    return true;
}

bool DroneController::autoLand(bool wait)
{
    ROS_INFO("========== Auto Land Start ==========");
    land();
    if (wait) {
        return waitForLand();
    } else {
        ros::Duration(5.0).sleep();
        return true;
    }
}