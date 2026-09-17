//
// transport_interface.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Sep 14 2026
//

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
  /// Name of the transport instance
  std::string name;
  /// Type of the transport
  std::string type;
  /// Name of the plugin implementing this transport
  std::string plugin_name;
  /// Arbitrary key-value parameters for the transport
  std::unordered_map<std::string, std::string> parameters;
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

  // TODO(nnarain): vibes, not well defined.
  uint64_t frames_in = 0;
  uint64_t frames_out = 0;
  uint64_t errors = 0;
};

/**
 * @brief Base class for all transport plugins (CAN, Modbus, EtherNet/IP, ...).
 *
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
