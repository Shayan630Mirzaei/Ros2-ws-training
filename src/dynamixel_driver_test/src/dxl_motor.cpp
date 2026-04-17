#include"rclcpp/rclcpp.hpp"
#include "dynamixel_driver/xl330_driver.hpp"
#include<thread>
#include<iostream>

using namespace std::chrono_literals;
using namespace std; 



int main()
{
    auto driver = XL330Driver("/dev/ttyACM0");
    driver.init();
    std::this_thread::sleep_for(1s);

    // Test velocity control
    int dxl_id1 = 10;
    int dxl_id2 = 20;


// Test velocity
    // driver.activateWithVelocityMode(dxl_id1);
    // driver.activateWithVelocityMode(dxl_id2);
    
    // std::this_thread::sleep_for(10s);

    // driver.setTargetVelocityRadianPerSec(dxl_id1,1.0);
    // driver.setTargetVelocityRadianPerSec(dxl_id2,-1.0);

    // std::this_thread::sleep_for(10s);
    // double velocity1 =driver.getVelocityRadianPerSec(dxl_id1);
    // double velocity2 =driver.getVelocityRadianPerSec(dxl_id2);

    // cout << "velocity of motor 1 is:"<< velocity1 << endl;
    // cout << "velocity of motor 2 is:"<< velocity2 << endl;

    

    // driver.deactivate(dxl_id1);
    // driver.deactivate(dxl_id2);


 // Test position mode
    driver.activateWithPositionMode(dxl_id1);
    driver.activateWithPositionMode(dxl_id2);
    std::this_thread::sleep_for(5s);
    driver.setTargetPositionRadian(dxl_id1, 6.14);
    driver.setTargetPositionRadian(dxl_id2, -6.14);
    double pose1 = driver.getPositionRadian(dxl_id1);
    double pose2 = driver.getPositionRadian(dxl_id2);
    std::this_thread::sleep_for(20s);

    cout << "position of motor 1 is: " << pose1 << endl;
    cout << "position of motor 2 is: " << pose2 << endl;
    driver.deactivate(dxl_id1);
    driver.deactivate(dxl_id2);



        return 0;
}