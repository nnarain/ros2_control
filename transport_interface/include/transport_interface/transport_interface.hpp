// Copyright 2026 ros2_control Development Team
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

#ifndef TRANSPORT_INTERFACE__TRANSPORT_INTERFACE_HPP_
#define TRANSPORT_INTERFACE__TRANSPORT_INTERFACE_HPP_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace transport_interface
{

/// Return values for transport lifecycle calls.
enum class return_type
{
  OK = 0,
  ERROR = 1,
};

/// Everything parsed from one <ros2_control name="can0" type="transport"> block.
struct TransportInfo
{
  std::string name;                                        // "can0"
  std::string type;                                        // "transport"
  std::string plugin_name;                                 // "transport_interface/MockCanTransport"
  std::unordered_map<std::string, std::string> parameters; // interface, bitrate, ...
};

/// Observability shared by every bus type.
struct TransportStatus
{
  enum class State
  {
    UNCONFIGURED,
    INACTIVE,
    ACTIVE,
    ERROR,
    FINALIZED,
  } state = State::UNCONFIGURED;

  bool link_up = false;
  uint64_t frames_in = 0;
  uint64_t frames_out = 0;
  uint64_t errors = 0;
};

/**
 * @brief Virtual base class for all transport plugins (CAN, Modbus, EtherNet/IP, ...).
 *
 * Lifecycle only — deliberately NO read()/write(). Real-world transports are often
 * asynchronous (a SocketCAN driver runs a receive thread pushing callbacks); a
 * synchronous cycle on the base would force async transports into a mold that
 * doesn't fit. Bus-specific I/O vocabulary lives on subclasses (CanTransport,
 * ModbusTransport, ...).
 */
class TransportInterface
{
public:
  virtual ~TransportInterface() = default;

  /// Initialization from data parsed from the <ros2_control type="transport"> tag.
  virtual return_type on_init(const TransportInfo & info) = 0;

  virtual return_type on_configure() = 0;
  virtual return_type on_activate() = 0;
  virtual return_type on_deactivate() = 0;
  virtual return_type on_shutdown() = 0;

  /// Common observability: link state, frame counts, errors.
  virtual TransportStatus get_status() const = 0;

  const std::string & get_name() const { return info_.name; }

protected:
  TransportInfo info_;
};

}  // namespace transport_interface

#endif  // TRANSPORT_INTERFACE__TRANSPORT_INTERFACE_HPP_
