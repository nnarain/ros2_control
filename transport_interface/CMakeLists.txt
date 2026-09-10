cmake_minimum_required(VERSION 3.16)
project(transport_interface LANGUAGES CXX)

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)

add_library(transport_interface SHARED
  src/mock_can_transport.cpp
)
target_include_directories(transport_interface PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/transport_interface>
)
target_link_libraries(transport_interface PUBLIC pluginlib::pluginlib)

pluginlib_export_plugin_description_file(transport_interface plugins.xml)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_transport test/test_transport.cpp)
  target_link_libraries(test_transport transport_interface)
endif()

install(
  DIRECTORY include/
  DESTINATION include/transport_interface
)
install(
  TARGETS transport_interface
  EXPORT export_transport_interface
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

ament_export_include_directories(include)
ament_export_targets(export_transport_interface HAS_LIBRARY_TARGET)
ament_export_dependencies(pluginlib)
ament_package()
