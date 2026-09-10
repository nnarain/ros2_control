# ROS2 Control Transport Plugins — Prototype (POC)

Transport as a first-class plugin type in `ros2_control`: **controller, hardware, transport**.

A transport (CAN bus, Modbus line, EtherCAT master) is a shared communication resource
declared **once** in the URDF as `<ros2_control name="can0" type="transport">`, loaded by
the resource manager via pluginlib, and consumed by any number of hardware components that
resolve it **by name** through `HardwareComponentParams::transport_provider` — the
framework-level replacement for process-wide singleton connection managers.

Upstream context: [ros2_control #1956](https://github.com/ros-controls/ros2_control/issues/1956)
("Comms"), #2811 / PR #2984 (grouped activation). Design notes in the Obsidian vault:
`Projects/Project Ideas/ROS2 Control Transport Plugins/`.

## Repo layout

This repo is a mirror of upstream `ros-controls/ros2_control` (branch `master`) with the
POC on branch `feat/transport-plugins`. The repo root **is** the colcon workspace.

```
transport_interface/         NEW — framework: TransportInterface base, CanTransport,
                                    ModbusTransport, TransportProvider, MockCanTransport
socketcan_transport/         NEW — real async SocketCAN transport plugin (rx thread + callbacks)
ros2_control_transport_demo/ NEW — demo robot: one shared can0, two motor actuators
hardware_interface/          MODIFIED — RM integration (see below)
```

### What changed in hardware_interface

| File | Change |
|---|---|
| `include/.../types/hardware_component_params.hpp` | new `transport_provider` field (registry view) |
| `include/.../types/hardware_component_interface_params.hpp` | same field on the params `on_init()` receives |
| `src/hardware_component_interface.cpp` | propagate provider into `on_init()` params |
| `include/.../component_parser.hpp` + `src/component_parser.cpp` | `parse_transport_resources_from_urdf()`; hardware parse skips `type="transport"` blocks |
| `include/.../resource_manager.hpp` + `src/resource_manager.cpp` | 4th pluginlib loader + `transports_` registry, two-pass load (transports first), `ResourceManagerTransportProvider`, `get_transport()` / `transport_names()`, transport lifecycle + shutdown |
| `test/test_transport.cpp` + `test/test_hardware_components/` | RM-level test: shared transport, loopback round-trip |

## Build & test

The container **is** the build env (no local ROS install needed):

```bash
# Build the image, run an interactive shell
docker build -t transport-poc .
docker run --rm -it --network=host -v $(pwd):/workspaces/ros2_control transport-poc

# Inside the container the entrypoint has already built; to rebuild/test:
colcon build --packages-select transport_interface hardware_interface socketcan_transport ros2_control_transport_demo --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
colcon test --packages-select transport_interface hardware_interface --ctest-args -R "transport"
colcon test-result --verbose
```

VS Code: open this folder → "Reopen in Container" (`.devcontainer/devcontainer.json`).

## Run the demo (mock transport — no hardware)

```bash
source install/setup.bash
ros2 launch ros2_control_transport_demo transport_demo.launch.py
```

In another shell:

```bash
ros2 control list_hardware_interfaces
# left_wheel_joint/position   [state]   left_motor
# left_wheel_joint/velocity   [command] left_motor
# right_wheel_joint/position  [state]   right_motor
# right_wheel_joint/velocity  [command] right_motor
```

The two actuators share the single `can0` MockCanTransport. Commands written to
`left_wheel_joint/velocity` loop back through the transport and appear as
`left_wheel_joint/position` state (see the transport demo actuator's frame callback).

To use a real bus instead of the mock, edit `ros2_control_transport_demo/description/transport_demo.urdf`:
swap the transport plugin to `socketcan_transport/SocketCanTransport` and add
`<param name="interface">can0</param>`, then `sudo ip link set can0 up type can bitrate 500000`.

## Tests

- `transport_interface/test/test_transport.cpp` — pluginlib load of MockCanTransport, loopback
  delivery, typed `get_transport<T>()` downcast semantics.
- `hardware_interface/test/test_transport.cpp` — resource manager loads a URDF with one
  `type="transport"` block + two consumer actuators; asserts the transport is registered and
  shared, and that a command round-trips through it as state.

## Status

Code-complete prototype, mirrors the agreed design (see vault reference note
`ROS2 Control Transport Plugins — Reference Implementation.md`). Built against upstream
master @ `eb41c996`. **Not yet compiled** on a ROS 2 container — the Dockerfile above is the
build path (works on any Docker host / the Apps VM; the Hermes VM has no Docker).
