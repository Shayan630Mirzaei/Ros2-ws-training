cmake_minimum_required(VERSION 3.8)
project(two_wheels_robot_hardware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)                             # shayan added this line after wrote mobile_base_hardware_interface.cpp
find_package(hardware_interface REQUIRED)                           # shayan added this line after wrote mobile_base_hardware_interface.cpp
find_package(pluginlib REQUIRED)                                    # shayan added this line after wrote mobile_base_hardware_interface.cpp
find_package(dynamixel_sdk REQUIRED)

include_directories(include)                                        # shayan added this line after creating package


add_library(
  ${PROJECT_NAME}
  SHARED
  src/mobile_base_hardware_interface.cpp
)

ament_target_dependencies(${PROJECT_NAME} rclcpp rclcpp_lifecycle hardware_interface pluginlib dynamixel_sdk)
pluginlib_export_plugin_description_file(hardware_interface two_wheels_robot_hardware_interface.xml)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
)

install(
  FILES two_wheels_robot_hardware_interface.xml
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY include
  DESTINATION include
)

ament_export_libraries(
  ${PROJECT_NAME}
)

ament_export_dependencies(
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
)

ament_package()
