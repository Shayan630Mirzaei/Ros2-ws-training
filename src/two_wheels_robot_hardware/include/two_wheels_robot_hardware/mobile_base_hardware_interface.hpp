#ifndef MOBILE_BASE_HARDWARE_INTERFACE_HPP
#define MOBILE_BASE_HARDWARE_INTERFACE_HPP

#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "two_wheels_robot_hardware/xl330_driver.hpp"

namespace mobile_base_hardware
{

class Mobile_base_hw_itf : public hardware_interface::SystemInterface
{
public:
    // lifecycle callbacks override
    hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareComponentInterfaceParams & params) override;
    hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;
    hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;
    hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

    // export interfaces
    std::vector<hardware_interface::StateInterface>export_state_interfaces() override;
    std::vector<hardware_interface::CommandInterface>export_command_interfaces() override;

    // read/write
    hardware_interface::return_type read(const rclcpp::Time & time,const rclcpp::Duration & period) override;
    hardware_interface::return_type write(const rclcpp::Time & time,const rclcpp::Duration & period) override;

private:
    std::shared_ptr<XL330Driver> driver_;
    int left_motor_id_;
    int right_motor_id_;
    std::string port_;

    double left_position_  = 0.0;
    double right_position_ = 0.0;

    double left_velocity_  = 0.0;
    double right_velocity_ = 0.0;

    double left_command_   = 0.0;
    double right_command_  = 0.0;
};

} // namespace mobile_base_hardware

#endif