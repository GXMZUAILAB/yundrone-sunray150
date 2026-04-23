cmake_minimum_required(VERSION 3.0.2)
project(base_command_ctrl)

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
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. 生成公共库
add_library(${PROJECT_NAME}
  src/drone_controller.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
)

# 2. 生成第一个可执行文件 demo1
add_executable(demo1 src/demo1.cpp)
target_link_libraries(demo1
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
add_dependencies(demo1 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})