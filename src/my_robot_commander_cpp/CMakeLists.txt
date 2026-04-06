cmake_minimum_required(VERSION 3.8)
project(my_robot_commander_cpp)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(robot_interfaces REQUIRED)  # <-- my custom service package for goToPoseTarget


# first, declare the executable
add_executable(test_moveit src/test_moveit.cpp)
ament_target_dependencies(test_moveit rclcpp moveit_ros_planning_interface)


add_executable(commander src/commander_template.cpp)
ament_target_dependencies(commander rclcpp moveit_ros_planning_interface example_interfaces robot_interfaces)

# install executable
install(TARGETS 
  test_moveit
  commander
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
