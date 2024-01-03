add_library(GeometricTools INTERFACE)
add_library(Engine::GeometricTools ALIAS GeometricTools)
target_include_directories(GeometricTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
