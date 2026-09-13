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

#include "transport_interface/mock_can_transport.hpp"

#include <cstring>
#include <cstdio>
#include <utility>
#include <iostream>

#include "pluginlib/class_list_macros.hpp"

namespace transport_interface
{

return_type MockCanTransport::on_init(const TransportInfo & info)
{
  info_ = info;
  return return_type::OK;
}

return_type MockCanTransport::on_configure() { return return_type::OK; }

return_type MockCanTransport::on_activate()
{
  link_up_ = true;
  return return_type::OK;
}

return_type MockCanTransport::on_deactivate()
{
  link_up_ = false;
  return return_type::OK;
}

return_type MockCanTransport::on_shutdown() { return return_type::OK; }

TransportStatus MockCanTransport::get_status() const
{
  TransportStatus status;
  status.state = link_up_.load() ? TransportStatus::State::ACTIVE : TransportStatus::State::INACTIVE;
  status.link_up = link_up_.load();
  status.frames_in = frames_in_.load();
  status.frames_out = frames_out_.load();
  status.errors = errors_.load();
  return status;
}

void MockCanTransport::register_frame_callback(
  const std::string & arbitration_id, FrameCallback cb)
{
  std::cout << "Registering frame callback for arb ID '" << arbitration_id << "'" << std::endl;
  std::lock_guard<std::mutex> lock(cb_mutex_);
  callbacks_[arbitration_id] = std::move(cb);
}

bool MockCanTransport::send(const CanFrame & frame, bool /*wait_for_lock*/)
{
  std::cout << "Sending frame on arb ID 0x" << std::hex << frame.id << std::dec << std::endl;
  char key_buf[16];
  std::snprintf(key_buf, sizeof(key_buf), "0x%X", frame.id);
  const std::string key = key_buf;

  FrameCallback cb;
  {
    std::lock_guard<std::mutex> lock(cb_mutex_);
    const auto it = callbacks_.find(key);
    if (it != callbacks_.end())
    {
      cb = it->second;
    }
  }

  if (cb)
  {
    cb(frame);          // synchronous loopback
    frames_in_.fetch_add(1);
    frames_out_.fetch_add(1);
    return true;
  }

  errors_.fetch_add(1);  // no consumer for this arbitration ID
  return false;
}

}  // namespace transport_interface

PLUGINLIB_EXPORT_CLASS(
  transport_interface::MockCanTransport, transport_interface::TransportInterface)
