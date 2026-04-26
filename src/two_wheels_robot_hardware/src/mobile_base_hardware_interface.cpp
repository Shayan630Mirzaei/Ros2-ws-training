#include<two_wheels_robot_hardware/mobile_base_hardware_interface.hpp>  // first we built header in include folder in this package,
                                                                        // then we call it here

#include "hardware_interface/system_interface.hpp"
namespace mobile_base_hardware{

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    hardware_interface::CallbackReturn Mobile_base_hw_itf::on_init(const hardware_interface::HardwareInfo &info) 
    {
       if ( hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS)
       {
        return hardware_interface::CallbackReturn::ERROR;
       }
       info_           = info;
       left_motor_id_  = 10;
       right_motor_id_ = 20;
       port_           = "/dev/ttyACM0";
       driver_         = std::make_shared<XL330Driver>(port_);

       return hardware_interface::CallbackReturn::SUCCESS;
    }

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    hardware_interface::CallbackReturn Mobile_base_hw_itf::on_configure(const rclcpp_lifecycle::State &previous_state)
    {
        (void)previous_state;
        if (driver_->init() != 0)
        {
            return hardware_interface::CallbackReturn::ERROR;
        }

        return hardware_interface::CallbackReturn::SUCCESS;  
     }

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    hardware_interface::CallbackReturn  Mobile_base_hw_itf::on_activate(const rclcpp_lifecycle::State &previous_state)
     {
        (void)previous_state;
        driver_->activateWithVelocityMode(left_motor_id_);
        driver_->activateWithVelocityMode(right_motor_id_);
        return hardware_interface::CallbackReturn::SUCCESS;  
     }

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    hardware_interface::CallbackReturn Mobile_base_hw_itf::on_deactivate(const rclcpp_lifecycle::State &previous_state)
    {
        (void)previous_state;
       driver_->deactivate(left_motor_id_);
       driver_->deactivate(left_motor_id_);
       return hardware_interface::CallbackReturn::SUCCESS;  

    }

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    hardware_interface::return_type Mobile_base_hw_itf::read(const rclcpp::Time &time, const rclcpp::Duration &period)
    {
        (void)time;
        double left_vel  = driver_->getVelocityRadianPerSec(left_motor_id_);
        double right_vel = driver_->getVelocityRadianPerSec(right_motor_id_);

        set_state("base_left_wheel_joint/velocity", left_vel);
        set_state("base_right_wheel_joint/velocity", right_vel);

        set_state("base_left_wheel_joint/position",  get_state("base_left_wheel_joint/position")  + left_vel * period.seconds());
        set_state("base_right_wheel_joint/position", get_state("base_right_wheel_joint/position") + right_vel * period.seconds());

        return hardware_interface::return_type::OK;   // <-- REQUIRED

    }




    hardware_interface::return_type Mobile_base_hw_itf::write(const rclcpp::Time &time, const rclcpp::Duration &period)
    {
        (void)time;
        return hardware_interface::return_type::OK;   // <-- REQUIRED
       
    }


}    