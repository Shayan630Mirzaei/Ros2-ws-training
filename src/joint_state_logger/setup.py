from setuptools import find_packages, setup

package_name = 'joint_state_logger'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='shayan630',
    maintainer_email='shayan630@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'state_logger_node = joint_state_logger.state_logger_node:main',
            'supervisor_node   = joint_state_logger.supervisor_node:main'
        ],
    },
)
