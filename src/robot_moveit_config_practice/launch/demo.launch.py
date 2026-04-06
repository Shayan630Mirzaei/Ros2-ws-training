from moveit_configs_utils import MoveItConfigsBuilder
from moveit_configs_utils.launches import generate_demo_launch
from launch_ros.actions import Node

def generate_launch_description():
    # MoveIt setup
    moveit_config = MoveItConfigsBuilder(
        "shayan_robot", package_name="robot_moveit_config_practice"
    ).to_moveit_configs()
    
    # Generate the default MoveIt demo launch
    moveit_launch = generate_demo_launch(moveit_config)

    # Add your custom nodes
    supervisor_node = Node(
        package="joint_state_logger",
        executable="supervisor_node",
        output="screen"
    )

    state_logger_node = Node(
        package="joint_state_logger",
        executable="state_logger_node",
        output="screen"
    )

    # Append custom nodes to the MoveIt launch
    moveit_launch.add_action(supervisor_node)
    moveit_launch.add_action(state_logger_node)

    return moveit_launch
