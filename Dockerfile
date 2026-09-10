# ROS2 Control Transport Plugins — prototype dev container
# Pinned Rolling image; ros2_control binaries provide the deps for building the
# modified packages from source as an overlay.
FROM ros:rolling-ros-base

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-colcon-common-extensions \
    python3-vcstool \
    ros-rolling-ros2-control \
    ros-rolling-ros2-controllers \
    ros-rolling-ros2-control-test-assets \
    && rm -rf /var/lib/apt/lists/*

# The repo root IS the colcon workspace (mirror of ros-controls/ros2_control).
# Entry point: source ROS, colcon-build the modified packages once, exec cmd.
COPY scripts/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
