#include <two_wheels_robot_hardware/mobile_base_hardware_interface.hpp>

#include "hardware_interface/system_interface.hpp"

namespace mobile_base_hardware
{

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hardware_interface::CallbackReturn Mobile_base_hw_itf::on_init(const hardware_interface::HardwareComponentInterfaceParams & params)
{
    info_ = params.hardware_info;

    left_motor_id_  = std::stoi(info_.hardware_parameters["left_motor_id"]);
    right_motor_id_ =  std::stoi(info_.hardware_parameters["right_motor_id"]);;
    port_           = info_.hardware_parameters["dynamixel_port"];  // "/dev/ttyACM0"

    driver_         = std::make_shared<XL330Driver>(port_);

    return hardware_interface::CallbackReturn::SUCCESS;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hardware_interface::CallbackReturn Mobile_base_hw_itf::on_configure(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;

    if (driver_->init() != 0)
    {
        return hardware_interface::CallbackReturn::ERROR;
    }


    return hardware_interface::CallbackReturn::SUCCESS;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hardware_interface::CallbackReturn Mobile_base_hw_itf::on_activate(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;

    left_position_  = 0.0;
    right_position_ = 0.0;

    left_velocity_  = 0.0;
    right_velocity_ = 0.0;

    left_command_   = 0.0;
    right_command_  = 0.0;

    driver_->activateWithVelocityMode(left_motor_id_);
    driver_->activateWithVelocityMode(right_motor_id_);

    return hardware_interface::CallbackReturn::SUCCESS;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hardware_interface::CallbackReturn Mobile_base_hw_itf::on_deactivate(const rclcpp_lifecycle::State & previous_state)
{
    (void)previous_state;

    driver_->deactivate(left_motor_id_);
    driver_->deactivate(right_motor_id_);

    return hardware_interface::CallbackReturn::SUCCESS;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
std::vector<hardware_interface::StateInterface>Mobile_base_hw_itf::export_state_interfaces()
{
    std::vector<hardware_interface::StateInterface> state_interfaces;

    state_interfaces.emplace_back("base_left_wheel_joint","position",&left_position_);
    state_interfaces.emplace_back("base_left_wheel_joint","velocity",&left_velocity_);

    state_interfaces.emplace_back("base_right_wheel_joint","position",&right_position_);
    state_interfaces.emplace_back("base_right_wheel_joint","velocity",&right_velocity_);

    return state_interfaces;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
std::vector<hardware_interface::CommandInterface>Mobile_base_hw_itf::export_command_interfaces()
{
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    command_interfaces.emplace_back("base_left_wheel_joint","velocity",&left_command_);
    command_interfaces.emplace_back("base_right_wheel_joint","velocity",&right_command_);

    return command_interfaces;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hardware_interface::return_type Mobile_base_hw_itf::read(const rclcpp::Time & time,const rclcpp::Duration & period)
{
    (void)time;

    left_velocity_  = driver_->getVelocityRadianPerSec(left_motor_id_);
    right_velocity_ = driver_->getVelocityRadianPerSec(right_motor_id_) * -1.0;

    if (abs(left_velocity_)<0.03){left_velocity_=0;};    // compensating inaccuracy of motors
    if (abs(right_velocity_)<0.03){right_velocity_=0;};

    left_position_ += left_velocity_ * period.seconds();
    right_position_ += right_velocity_ * period.seconds();

    RCLCPP_INFO(get_logger(),"left com: %lf",left_command_);
    RCLCPP_INFO(get_logger(),"left vel: %lf",left_velocity_);
    //RCLCPP_INFO(get_logger(),"left pos: %lf",left_position_);
    RCLCPP_INFO(get_logger(),"right com: %lf",right_command_);
    RCLCPP_INFO(get_logger(),"right vel: %lf",right_velocity_);
    return hardware_interface::return_type::OK;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hardware_interface::return_type Mobile_base_hw_itf::write(const rclcpp::Time & time, const rclcpp::Duration & period)
{
    (void)time;
    (void)period;

    driver_->setTargetVelocityRadianPerSec(left_motor_id_,left_command_);
    driver_->setTargetVelocityRadianPerSec(right_motor_id_,-1.0 * right_command_); // -1 is due to two motors shout have oposit direction, otherwise robot don't move forward
       
    return hardware_interface::return_type::OK;
}

} // namespace mobile_base_hardware

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(mobile_base_hardware::Mobile_base_hw_itf,hardware_interface::SystemInterface)