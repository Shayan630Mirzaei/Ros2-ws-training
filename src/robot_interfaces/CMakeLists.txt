cmake_minimum_required(VERSION 3.8)
project(robot_interfaces)

# Compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Generate interfaces from srv files
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/GoToPose.srv"
)

# Export dependency for runtime
ament_export_dependencies(rosidl_default_runtime)

# Finish
ament_package()
