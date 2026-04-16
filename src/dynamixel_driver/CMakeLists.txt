cmake_minimum_required(VERSION 3.8)
project(dynamixel_driver)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(dynamixel_sdk REQUIRED)

include_directories(include)                                        # shayan added this line after creating package
add_executable(dxl_motor src/dxl_motor.cpp)                         # shayan added this line after creating package
ament_target_dependencies(dxl_motor rclcpp dynamixel_sdk)           # shayan added this line after creating package

install(TARGETS                                                     # shayan added this line after creating package
dxl_motor
DESTINATION lib/${PROJECT_NAME}
)

ament_package()
