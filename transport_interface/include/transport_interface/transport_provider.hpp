//
// transport_provider.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Sep 14 2026
//

#ifndef TRANSPORT_INTERFACE__TRANSPORT_PROVIDER_HPP_
#define TRANSPORT_INTERFACE__TRANSPORT_PROVIDER_HPP_

#include <memory>
#include <string>

#include "transport_interface/transport_interface.hpp"

namespace transport_interface
{

/**
 * @brief Interface for providing transport instances by name.
 */
class TransportProvider
{
public:
  virtual ~TransportProvider() = default;

  virtual std::shared_ptr<TransportInterface> get_transport(const std::string & name) const = 0;

  template <typename T>
  std::shared_ptr<T> get_transport(const std::string & name) const
  {
    return std::dynamic_pointer_cast<T>(get_transport(name));
  }
};

}  // namespace transport_interface

#endif  // TRANSPORT_INTERFACE__TRANSPORT_PROVIDER_HPP_
