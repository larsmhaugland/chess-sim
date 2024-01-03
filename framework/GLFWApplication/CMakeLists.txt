add_library(GLFWApplication GLFWApplication.cpp)
add_library(Engine::GLFWApplication ALIAS GLFWApplication)

target_include_directories(GLFWApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GLFWApplication PUBLIC glfw glad)

