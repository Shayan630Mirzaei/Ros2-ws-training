#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist
from turtlesim.msg     import Pose

from turtlesim.srv import SetPen
from std_srvs.srv  import Empty

class pen_controller_node(Node):
    
    def __init__(self):
        super().__init__('prac1_turtutle_controller')
        self.get_logger().info('Node has started')

        self.vel_publisher = self.create_publisher(Twist,"/turtle1/cmd_vel",10)
        pose_subscr        = self.create_subscription(Pose,'/turtle1/pose',self.pose_call_back,1)
        self.pen_client    = self.create_client(SetPen,'/turtle1/set_pen')
        self.reset_client   = self.create_client(Empty,'/reset')

        # self.timer         = self.create_timer(0.5,self.timer_function)

    def change_pen(self,r,g,b,width,off):
        Request = SetPen.Request()
        Request.r = r
        Request.g = g
        Request.b = b
        Request.width = width
        Request.off = off
        future = self.pen_client.call_async(Request)
        future.add_done_callback(self.handle_service)

    def reset_function(self):
        Request = Empty.Request()
        future  = self.reset_client.call_async(Request)
        future.add_done_callback(self.handle_service)



    def handle_service(self,futur):
        try:
           self.get_logger().info('Service done succesfully')
        except Exception as e:
            self.get_logger().error(str(e))     


    # def timer_function(self):
    #     msg = Twist()
    #     msg.linear.x  = 1.0
    #     msg.angular.z = 0.0
    #     self.vel_publisher.publish(msg)


    def pose_call_back(self, msg:Pose):
        vel = Twist()
        if msg.x<7:
            vel.linear.x = 1.0

        if msg.x>7:
            vel.angular.z = 0.5
            vel.linear.x = 0.5
            self.change_pen(255,0,0,5,0)
            self.get_logger().info(f"x Velocity is : {vel.linear.x}")
      
        if msg.x<2:
            self.reset_function()

        self.vel_publisher.publish(vel)

        




def main(args=None):
    rclpy.init(args=args)
    Node = pen_controller_node()
    rclpy.spin(Node)
    rclpy.shutdown()
