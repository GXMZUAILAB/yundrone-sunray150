cmake_minimum_required(VERSION 3.0.2)
project(automatic_obstacle_avoidance)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    sunray_msgs
)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
    CATKIN_DEPENDS roscpp std_msgs sunray_msgs
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

# 1. 库文件
add_library(
    ${PROJECT_NAME}
    src/drone_controller.cpp
)
target_link_libraries(
    ${PROJECT_NAME}
    ${catkin_LIBRARIES}
)

add_executable(drone_control_node src/main.cpp)
target_link_libraries(drone_control_node ${PROJECT_NAME} ${catkin_LIBRARIES})
add_dependencies(drone_control_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})