#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from sensor_msgs.msg import JointState
from std_srvs.srv    import Trigger

import os
import csv
from datetime import datetime


class StateLoggerNode(Node):

    def __init__(self):
        super().__init__('state_logger_node')
        self.get_logger().info("State logger node started")

        self.logging_active = False
        self.csv_file       = None
        self.csv_writer     = None
        self.joint_names    = None

        self.state_sub = self.create_subscription(JointState,'/joint_states',self.state_callback,10)
        self.start_srv = self.create_service(Trigger,'start_logging',self.start_logging_callback)
        self.stop_srv  = self.create_service(Trigger,'stop_logging',self.stop_logging_callback)
  
    def state_callback(self, msg: JointState):
        if not self.logging_active:
            return
        if self.joint_names is None:
            self.joint_names = msg.name
            header = ["time"]
            header += [f"{name}_pos" for name in self.joint_names]
            header += [f"{name}_vel" for name in self.joint_names]
            self.csv_writer.writerow(header)
            self.get_logger().info("CSV header written")

        time_sec   = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9
        positions  = list(msg.position)
        velocities = list(msg.velocity)
        row        = [time_sec] + positions + velocities
        self.csv_writer.writerow(row)

        
    def start_logging_callback(self, request, response):
        if self.logging_active:
            response.success = False
            response.message = "Logging already active"
            return response

        log_dir         = os.path.expanduser("~/joint_logs")
        os.makedirs(log_dir, exist_ok=True)
        timestamp       = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        file_path       = os.path.join(log_dir, f"joint_states_{timestamp}.csv")
        self.csv_file   = open(file_path, 'w', newline='')
        self.csv_writer = csv.writer(self.csv_file)

        self.joint_names    = None
        self.logging_active = True
        self.get_logger().info(f"Started logging to {file_path}")

        response.success = True
        response.message = "Logging started"
        return response

    def stop_logging_callback(self, request, response):
        if not self.logging_active:
            response.success = False
            response.message = "Logging is not active"
            return response

        self.logging_active = False

        if self.csv_file:
            self.csv_file.close()
            self.csv_file = None
            self.csv_writer = None

        self.get_logger().info("Stopped logging")

        response.success = True
        response.message = "Logging stopped"
        return response

    def destroy_node(self):
        if self.csv_file:
            self.csv_file.close()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = StateLoggerNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
