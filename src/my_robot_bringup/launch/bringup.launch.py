from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    # -------- Paths --------
    description_pkg = FindPackageShare("my_robot_description")
    urdf_file = PathJoinSubstitution(
        [description_pkg, "urdf", "my_robot.urdf.xacro"]
    )

    controllers_file = PathJoinSubstitution(
        [FindPackageShare("my_robot_bringup"), "config", "ros2_controllers.yaml"]
    )

    # -------- Robot State Publisher --------
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{
            "robot_description": Command(["xacro ", urdf_file])
        }],
        output="screen"
    )

    # -------- ros2_control --------
    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[controllers_file],
        remappings=[
            ("~/robot_description", "/robot_description"),
        ],
        output="screen",
    )

    # -------- Spawn Controllers --------
    joint_state_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
        output="screen",
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["arm_controller"],
        output="screen",
    )

    # -------- Launch MoveIt --------
    moveit_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare("robot_moveit_config_practice"),
                "launch",
                "move_group.launch.py"
            ])
        )
    )

    return LaunchDescription([
        robot_state_publisher,
        control_node,
        joint_state_spawner,
        arm_controller_spawner,
        moveit_launch,
    ])
