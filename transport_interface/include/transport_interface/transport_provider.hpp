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

#ifndef TRANSPORT_INTERFACE__TRANSPORT_PROVIDER_HPP_
#define TRANSPORT_INTERFACE__TRANSPORT_PROVIDER_HPP_

#include <memory>
#include <string>

#include "transport_interface/transport_interface.hpp"

namespace transport_interface
{

/**
 * @brief The registry view handed to every hardware component via
 * HardwareComponentParams::transport_provider.
 *
 * This is the framework-level replacement for the process-wide ConnectionManager
 * singleton: the resource manager owns the registry, and each component resolves
 * transports by name through this view.
 */
class TransportProvider
{
public:
  virtual ~TransportProvider() = default;

  /// Look up a transport by its <ros2_control name="...">. nullptr if not found.
  virtual std::shared_ptr<TransportInterface> get_transport(const std::string & name) const = 0;

  /// Typed accessor: name lookup + ONE downcast, wrapped here, called once in on_init().
  template <typename T>
  std::shared_ptr<T> get_transport(const std::string & name) const
  {
    return std::dynamic_pointer_cast<T>(get_transport(name));
  }
};

}  // namespace transport_interface

#endif  // TRANSPORT_INTERFACE__TRANSPORT_PROVIDER_HPP_
