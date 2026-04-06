#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from action_msgs.msg import GoalStatusArray
from  std_srvs.srv import Trigger

class SupervisorNode(Node):
    def __init__(self):
        super().__init__('supervisor_node')
        self.get_logger().info("====================================================================================") 
        self.get_logger().info("Shayan Supervisor node started")
        self.get_logger().info("====================================================================================") 

        plan_execute_status   = self.create_subscription(GoalStatusArray,'/move_action/_action/status',self.status_callback,10)
        self.start_log_client = self.create_client(Trigger,'start_logging') 
        self.stop_log_client  = self.create_client(Trigger,'stop_logging') 
        
        self.uuid_6dof        = None

    def status_callback(self,msg:GoalStatusArray):
        self.get_logger().info("====================================================================================") 
        length = len(msg.status_list)
        self.get_logger().info(f"Number of goals in status_list: {length}")
        self.get_logger().info("====================================================================================") 

        for goal in msg.status_list:
            if goal.status ==2:
               self.uuid_6dof = tuple(goal.goal_info.goal_id.uuid)
               self.get_logger().info("====================================================================================") 
               self.get_logger().info("Goal accepted and running") 
               self.get_logger().info(f'UUID is: {self.uuid_6dof}') 
               self.get_logger().info(f'Status is: {goal.status}') 
               self.start_log_func()   
               self.get_logger().info('6dof Robot data logging started') 
               self.get_logger().info("====================================================================================")


        for goal in msg.status_list:
            uuid = goal.goal_info.goal_id.uuid
            if self.uuid_6dof is not None and tuple(uuid) == self.uuid_6dof and goal.status==4:
                self.stop_log_func()
                self.uuid_6dof= None
                self.get_logger().info("====================================================================================")
                self.get_logger().info('End of Sim - Shayan 6dof Robot') 
                self.get_logger().info('6dof Robot data logging stopped') 
                self.get_logger().info("====================================================================================")

                

    def start_log_func(self):
        Request = Trigger.Request()
        self.start_log_client.call_async(Request)


    def stop_log_func(self):
        Request = Trigger.Request()
        self.stop_log_client.call_async(Request)

def main(args=None):
    rclpy.init(args=args)
    node = SupervisorNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
