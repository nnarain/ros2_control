// Copyright 2025 ros2_control Development Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef HARDWARE_INTERFACE__TYPES__HARDWARE_COMPONENT_PARAMS_HPP_
#define HARDWARE_INTERFACE__TYPES__HARDWARE_COMPONENT_PARAMS_HPP_

#include <memory>
#include <string>
#include "hardware_interface/hardware_info.hpp"
#include "rclcpp/rclcpp.hpp"
#include "transport_interface/transport_provider.hpp"

namespace hardware_interface
{

/**
 * @brief Parameters required for the initialization of a specific hardware component plugin.
 * Typically used for init, initialise and load methods of hardware components.
 * This struct is typically not accessible to user.
 * This struct is typically populated with data from ResourceManagerParams.
 */
struct HardwareComponentParams
{
  HardwareComponentParams() = default;
  /**
   * @brief Reference to the HardwareInfo struct for this specific component,
   * parsed from the URDF. The HardwareInfo object's lifetime must be guaranteed
   * by the caller (e.g., ResourceManager) for the duration this struct is used.
   */
  hardware_interface::HardwareInfo hardware_info;

  /**
   * @brief A logger instance taken from resource manager
   */
  rclcpp::Logger logger = rclcpp::get_logger("resource_manager");

  /**
   * @brief Shared pointer to the rclcpp::Clock to be used by this hardware component.
   * Typically, this is the same clock used by the ResourceManager/ControllerManager.
   */
  rclcpp::Clock::SharedPtr clock = nullptr;

  /**
   * @brief The namespace used by the hardware component's internal node.
   * This is typically same as the controller manager's node namespace.
   */
  std::string node_namespace = "";

  /**
   * @brief Weak pointer to the rclcpp::Executor instance. Hardware components
   * can use this (after locking) to add their own internal ROS 2 nodes
   * to the ControllerManager's executor.
   */
  rclcpp::Executor::WeakPtr executor;

  /**
   * @brief Read-only view of the resource manager's transport registry.
   *
   * Populated by the resource manager at load time when the URDF declares
   * <ros2_control type="transport"> blocks. Nullptr when no transports are
   * declared — components that don't consume transports can ignore it.
   * This is the framework-level replacement for process-wide singleton
   * connection managers (CAN/Modbus/EtherCAT masters).
   */
  std::shared_ptr<transport_interface::TransportProvider> transport_provider = nullptr;
};

}  // namespace hardware_interface

#endif  // HARDWARE_INTERFACE__TYPES__HARDWARE_COMPONENT_PARAMS_HPP_
