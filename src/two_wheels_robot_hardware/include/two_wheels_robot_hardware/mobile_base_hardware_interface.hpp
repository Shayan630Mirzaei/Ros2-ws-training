#ifndef MOBILE_BASE_HARDWARE_INTERFACE_HPP
#define MOBILE_BASE_HARDWARE_INTERFACE_HPP


#include "hardware_interface/system_interface.hpp"
#include "two_wheels_robot_hardware/xl330_driver.hpp"    // for this line, you should update json file by adding directroy of two_wheels_robot_hardware 

namespace mobile_base_hardware{

    class Mobile_base_hw_itf : public hardware_interface :: SystemInterface
    {
        public:
            // lifecycle node override
            hardware_interface::CallbackReturn
                on_configure(const rclcpp_lifecycle::State &previous_state) override;

            hardware_interface::CallbackReturn
                on_activate(const rclcpp_lifecycle::State &previous_state) override;

            hardware_interface::CallbackReturn
                on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

            // System Interface override
            hardware_interface::CallbackReturn
                on_init(const hardware_interface::HardwareInfo &Info) override;
            hardware_interface::return_type
                read(const rclcpp::Time &time, const rclcpp::Duration &period);
            hardware_interface::return_type
                write(const rclcpp::Time &time, const rclcpp::Duration &period);
       
        private:
            std::shared_ptr<XL330Driver> driver_;

    }; //  class Mobile_base_hw_itf

} //  namespace mobile_base_hardware


#endif 