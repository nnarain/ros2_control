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

#ifndef TRANSPORT_INTERFACE__MOCK_CAN_TRANSPORT_HPP_
#define TRANSPORT_INTERFACE__MOCK_CAN_TRANSPORT_HPP_

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>

#include "transport_interface/can_transport.hpp"

namespace transport_interface
{

/**
 * @brief In-memory loopback CAN transport for tests and demos.
 *
 * send() delivers the frame synchronously to the callback registered for that
 * arbitration ID. Deterministic, thread-safe, no hardware required.
 */
class MockCanTransport : public CanTransport
{
public:
  // --- lifecycle ---
  return_type on_init(const TransportInfo & info) override;
  return_type on_configure() override;
  return_type on_activate() override;
  return_type on_deactivate() override;
  return_type on_shutdown() override;
  TransportStatus get_status() const override;

  // --- CanTransport ---
  void register_frame_callback(const std::string & arbitration_id, FrameCallback cb) override;
  bool send(const CanFrame & frame, bool wait_for_lock) override;

private:
  std::mutex cb_mutex_;
  std::unordered_map<std::string, FrameCallback> callbacks_;

  mutable std::atomic<bool> link_up_{false};
  mutable std::atomic<uint64_t> frames_in_{0};
  mutable std::atomic<uint64_t> frames_out_{0};
  mutable std::atomic<uint64_t> errors_{0};
};

}  // namespace transport_interface

#endif  // TRANSPORT_INTERFACE__MOCK_CAN_TRANSPORT_HPP_
