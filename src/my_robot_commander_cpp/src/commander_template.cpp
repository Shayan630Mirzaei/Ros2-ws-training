#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <example_interfaces/msg/float64_multi_array.hpp>
#include "robot_interfaces/srv/go_to_pose.hpp"


using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;
using namespace std;
using rclcpp::Node;
using std::shared_ptr;
using FloatArray = example_interfaces::msg::Float64MultiArray;
using PoseServ   = robot_interfaces::srv::GoToPose;


// Shayan:: as practice, I use subscribtion for "goToJointTarget". For this, I use topic system with availble massage. That is "data" field avaible in "example_interfaces/msg/float64_multi_array"
//                       I use service for      "goToPoseTarget". For this, I have build a service interface named ""

class Commander
{
public:
       Commander(shared_ptr<Node> node)
       {
              node_ = node;
              arm_  = make_shared<MoveGroupInterface>(node_, "arm");
              arm_->setMaxVelocityScalingFactor(1.0);
              arm_->setMaxAccelerationScalingFactor(1.0);
              joint_cmd_sub_ = node_->create_subscription<FloatArray>("joint_command",10,std::bind(&Commander::jointCmdCallback, this, std::placeholders::_1));
             // pose_cmd_srv_  = node_->create_service<PoseServ>       ("pose_command",      std::bind(&Commander::poseCmdCallback,  this, std::placeholders::_1, std::placeholders::_2));
              pose_cmd_srv_ = node_->create_service<PoseServ>("pose_command",[this](PoseServ::Request::SharedPtr request,PoseServ::Response::SharedPtr response){this->poseCmdCallback(*request, *response);});
       }

       void goToNamedTarget(const string &name)
       {
              arm_->setStartStateToCurrentState();
              arm_->setNamedTarget(name);
              planAndExecute(arm_);
       }

       void goToJointTarget(const vector<double> &joints)
       {
              arm_->setStartStateToCurrentState();
              arm_->setJointValueTarget(joints);
              planAndExecute(arm_);
       }

       void goToPoseTarget(double x, double y, double z, double roll, double pitch, double yaw, bool cartesian_path = false)
       {
              tf2::Quaternion q;
              q.setRPY(roll, pitch, yaw);
              q = q.normalize();

              geometry_msgs::msg::PoseStamped target_pose;
              target_pose.header.frame_id = "base_link";
              target_pose.pose.position.x = x;
              target_pose.pose.position.y = y;
              target_pose.pose.position.z = z;
              target_pose.pose.orientation.x = q.getX();
              target_pose.pose.orientation.y = q.getY();
              target_pose.pose.orientation.z = q.getZ();
              target_pose.pose.orientation.w = q.getW();

              arm_->setStartStateToCurrentState();

              if (!cartesian_path)
              {
                     arm_->setPoseTarget(target_pose);
                     planAndExecute(arm_);
              }
              else
              {
                     vector<geometry_msgs::msg::Pose> waypoints;
                     waypoints.push_back(target_pose.pose);
                     moveit_msgs::msg::RobotTrajectory trajectory;
                     double fraction = arm_->computeCartesianPath(waypoints, 0.01, 0.0, trajectory);
                     if (fraction == 1)
                     {
                            arm_->execute(trajectory);
                     }
              }
       }

private:
       shared_ptr<Node> node_;
       shared_ptr<MoveGroupInterface> arm_;
       rclcpp::Subscription<FloatArray>::SharedPtr joint_cmd_sub_ ;
       rclcpp::Service<PoseServ>::SharedPtr pose_cmd_srv_ ;

       void planAndExecute(shared_ptr<MoveGroupInterface> &interface)
       {
       MoveGroupInterface::Plan plan;

       auto result = interface->plan(plan);

       if (result == moveit::core::MoveItErrorCode::SUCCESS)
       {
              RCLCPP_INFO(node_->get_logger(), "Plan successful. Executing...");
              interface->execute(plan);
       }
       else
       {
              RCLCPP_ERROR(node_->get_logger(), "Shayan!Shayan!--->Planning failed!");
       }
       }

       void jointCmdCallback (const FloatArray &msg)
       {
         auto joints = msg.data;
         if (joints.size()==6){
              goToJointTarget(joints);
         }

       }

       void poseCmdCallback(const PoseServ::Request &request,PoseServ::Response &response)
       {
             goToPoseTarget(request.x, request.y, request.z,request.roll, request.pitch, request.yaw,request.cartesian);
             response.success = true;
             response.message = "Pose command executed by Shayan Commander";
       }
        
};

int main(int argc, char **argv)
{
       rclcpp::init(argc, argv);
       auto node = make_shared<Node>("Shayan_Commander");
       auto myCommander = Commander(node);
       rclcpp::spin(node);
       rclcpp::shutdown();
       return 0;
}