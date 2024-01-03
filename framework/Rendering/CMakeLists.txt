		#Add rendering
add_library(Rendering Shader.cpp IndexBuffer.cpp VertexArray.cpp VertexBuffer.cpp RenderCommands.h Camera.h PerspectiveCamera.h OrthographicCamera.h OrthographicCamera.cpp TextureManager.cpp TextureManager.h)
target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
add_library(Engine::Rendering ALIAS Rendering)
target_link_libraries(Rendering PUBLIC glad glfw glm stb)
