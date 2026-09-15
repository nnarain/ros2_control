cmake_minimum_required(VERSION 3.16)
project(transport_interface LANGUAGES CXX)

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)

add_library(transport_interface INTERFACE)
target_include_directories(transport_interface INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(transport_interface INTERFACE pluginlib::pluginlib)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
endif()

install(
  DIRECTORY include/
  DESTINATION include
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
