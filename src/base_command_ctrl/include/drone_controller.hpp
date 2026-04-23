#ifndef DRONE_CONTROLLER_HPP
#define DRONE_CONTROLLER_HPP

#include <ros/ros.h>
#include <sunray_msgs/UAVSetup.h>
#include <sunray_msgs/UAVControlCMD.h>
#include <sunray_msgs/UAVState.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Vector3.h>
#include <string>

class DroneController
{
public:
    /**
     * @brief Constructor
     * @param nh ROS NodeHandle
     * @param uav_id UAV ID (default 1)
     */
    DroneController(ros::NodeHandle& nh, int uav_id = 1);

    // ---------- Basic commands ----------
    void arm();
    void disarm();
    void setCmdControl();
    void setOffboardMode();
    void setRCControl();
    void rebootPX4();
    void emergencyKill();
    void takeoff();
    void land();
    void hover();
    void returnToLaunch();

    // ---------- Inertial frame (world) control ----------
    void gotoXYZ(double x, double y, double z);
    void gotoXYZYaw(double x, double y, double z, double yaw_deg);
    void gotoGlobal(double latitude, double longitude, double altitude, double yaw_deg);
    void turnTo(double yaw_deg);   // turn in place to absolute yaw

    // ---------- Body frame control ----------
    void moveBody(double forward, double right, double up, double yaw_deg = 0.0);
    void moveForward(double distance, double yaw_deg = 0.0);
    void moveBackward(double distance, double yaw_deg = 0.0);
    void moveRight(double distance, double yaw_deg = 0.0);
    void moveLeft(double distance, double yaw_deg = 0.0);
    void moveUp(double distance, double yaw_deg = 0.0);
    void moveDown(double distance, double yaw_deg = 0.0);
    void turnBody(double yaw_deg);   // relative yaw rotation

    // ---------- Advanced control ----------
    void setVelocity(double vx, double vy, double vz, double yaw_rate_deg = 0.0);
    void gotoPoint(double x, double y, double z, double yaw_deg = 0.0);

    // ---------- Wait functions with timeout ----------
    bool waitForConnection(double timeout = 10.0);
    bool waitForArmed(double timeout = 5.0);
    bool waitForTakeoff(double target_height = 1.0, double timeout = 10.0);
    bool waitForLand(double timeout = 15.0);

    // ---------- One-key actions ----------
    bool autoTakeoff(double target_height = 1.0, bool wait = true);
    bool autoLand(bool wait = true);

    // ---------- Get status ----------
    bool isArmed() const { return armed_; }
    bool isConnected() const { return connected_; }
    std::string getControlMode() const { return control_mode_; }
    int getLandedState() const { return landed_state_; }
    geometry_msgs::Point getPosition() const { return position_; }

private:
    void stateCallback(const sunray_msgs::UAVState::ConstPtr& msg);
    void sendSetupCmd(uint8_t cmd, const std::string& control_mode = "", const std::string& px4_mode = "");
    void sendControlCmd(uint8_t cmd,
                        const geometry_msgs::Point* pos = nullptr,
                        double yaw_rad = 0.0,
                        double yaw_rate_rad = 0.0,
                        double latitude = 0.0, double longitude = 0.0, double altitude = 0.0);

    ros::NodeHandle nh_;
    ros::Publisher setup_pub_;
    ros::Publisher cmd_pub_;
    ros::Subscriber state_sub_;

    // UAV state
    bool connected_;
    bool armed_;
    std::string control_mode_;
    int landed_state_;
    geometry_msgs::Point position_;
    geometry_msgs::Vector3 velocity_;
    geometry_msgs::Vector3 attitude_; // roll, pitch, yaw (rad)
    std::string mode_;
    std::string move_mode_;
    int uav_id_;
    std::string uav_name_;
};

#endif // DRONE_CONTROLLER_HPP