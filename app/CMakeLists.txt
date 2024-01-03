project(ChessSim)

add_executable(
		${PROJECT_NAME}
		main.cpp
        ChessApp.cpp)

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/light_wood.png
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/light_wood.png)

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/dark_wood.png
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/dark_wood.png)

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/resources
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)

set(MODELS_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models")
string(REPLACE "/" "\/" ESCAPED_MODELS_PATH ${MODELS_PATH})
target_compile_definitions(${PROJECT_NAME}
		PRIVATE
		MODELS_DIR="${ESCAPED_MODELS_PATH}"
		TINYOBJLOADER_IMPLEMENTATION)



add_library(ChessApp ChessApp.cpp)
add_library(Engine::ChessApp ALIAS ChessApp)
add_library(ChessEngine ChessEngine.cpp)
add_library(Engine::ChessEngine ALIAS ChessEngine)
add_library(ChessPiece ChessPiece.cpp)
add_library(Engine::ChessPiece ALIAS ChessPiece)
target_include_directories(ChessApp PUBLIC resources)
target_link_libraries(ChessApp PUBLIC Rendering GeometricTools GLFWApplication ChessEngine ChessPiece tinyobjloader)


target_link_libraries(${PROJECT_NAME} PRIVATE ChessApp)

#target_compile_definitions(${PROJECT_NAME} PRIVATE
#		TEXTURES_DIR=\"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/\")
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)



