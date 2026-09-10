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

#ifndef TRANSPORT_INTERFACE__MODBUS_TRANSPORT_HPP_
#define TRANSPORT_INTERFACE__MODBUS_TRANSPORT_HPP_

#include <cstdint>

#include "transport_interface/transport_interface.hpp"

namespace transport_interface
{

/**
 * @brief The generic "Modbus transport" interface (RTU or TCP).
 *
 * Slave-addressed register I/O. A hardware component holding a typed
 * shared_ptr<ModbusTransport> can issue register reads/writes directly.
 */
class ModbusTransport : public TransportInterface
{
public:
  virtual bool read_registers(
    uint8_t slave_id, uint16_t address, uint16_t count, uint16_t * out, bool wait_for_lock) = 0;

  virtual bool write_registers(
    uint8_t slave_id, uint16_t address, const uint16_t * data, uint16_t count,
    bool wait_for_lock) = 0;
};

}  // namespace transport_interface

#endif  // TRANSPORT_INTERFACE__MODBUS_TRANSPORT_HPP_
