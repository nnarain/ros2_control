#!/usr/bin/env bash
# Entry point for the transport-plugins prototype container.
# Sources ROS, builds the modified packages once (guarded), sources the overlay.
set -e

source /opt/ros/rolling/setup.bash

# The repo root is the colcon workspace. Build only the packages this POC
# touches; everything else (controller_manager, etc.) comes from the
# ros-rolling-ros2-control binaries already installed.
PACKAGES=(transport_interface hardware_interface socketcan_transport ros2_control_transport_demo)

if [ ! -f install/setup.bash ]; then
  colcon build \
    --packages-select "${PACKAGES[@]}" \
    --cmake-args -DCMAKE_BUILD_TYPE=Release
fi

source install/setup.bash

exec "$@"
