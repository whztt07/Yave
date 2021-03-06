cmake_minimum_required(VERSION 3.11)
project(yave)

function(enable_unity_build UB_SUFFIX SOURCE_VARIABLE_NAME)
	set(FILES ${${SOURCE_VARIABLE_NAME}})

	# Exclude all translation units from compilation
	set_source_files_properties(${FILES} PROPERTIES HEADER_FILE_ONLY true)

	# Generate a unique filename for the unity build translation unit
	set(UNIT_BUILD_FILE ${CMAKE_CURRENT_BINARY_DIR}/blob_${UB_SUFFIX}.cpp)
	file(WRITE ${UNIT_BUILD_FILE} "// Unity Build generated by CMake\n")

	# Add include statement for each translation unit
	foreach(SOURCE_FILE ${FILES})
		get_filename_component(FILE_EXT "${SOURCE_FILE}" EXT)
		if(NOT "${FILE_EXT}" MATCHES "\.h(pp)?")
			file(APPEND ${UNIT_BUILD_FILE} "#include <${SOURCE_FILE}>\n")
		endif()
	endforeach(SOURCE_FILE)

	# Complement list of translation units with the name of ub
	set(${SOURCE_VARIABLE_NAME} ${${SOURCE_VARIABLE_NAME}} ${UNIT_BUILD_FILE} PARENT_SCOPE)
endfunction(enable_unity_build)


find_package(vulkan REQUIRED)

option(YAVE_BUILD_YAVE "Build yave" ON)
option(YAVE_BUILD_EDITOR "Build editor" ON)

# add y subtree
add_subdirectory(y)

add_subdirectory(external/spirv_cross)

# setup includes
include_directories(${y_SOURCE_DIR})
include_directories(${yave_SOURCE_DIR})
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(external)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

# Yave's core file
file(GLOB_RECURSE YAVE_FILES
		"yave/*.h"
		"yave/*.cpp"
	)

# ECS files
file(GLOB_RECURSE ECS_FILES
		"ecs/*.h"
		"ecs/*.cpp"
	)

# Editor files
file(GLOB_RECURSE EDITOR_FILES
		"editor/*.cpp"
		"editor/*.h"
		"external/imgui/*.cpp"
		"external/imgui/*.h"
	)

# Shader files, they are here so the IDE can find them
file(GLOB_RECURSE SHADER_FILES
		"shaders/*.frag"
		"shaders/*.vert"
		"shaders/*.geom"
		"shaders/*.comp"
		"shaders/*.glsl"
	)



if(CMAKE_BUILD_TYPE STREQUAL Release)
	enable_unity_build(yave YAVE_FILES)
endif()


if(YAVE_BUILD_YAVE)
	add_library(yave STATIC ${YAVE_FILES} ${SHADER_FILES})

	target_link_libraries(yave ${Vulkan_LIBRARY})
	target_link_libraries(yave y spirv-cross-core stdc++fs)

	enable_unity_build(ecs ECS_FILES)
	add_executable(ecs "ecs/main.cpp" ${ECS_FILES})
	target_link_libraries(ecs y)
endif()

if(YAVE_BUILD_EDITOR)
	add_executable(editor ${EDITOR_FILES})

	target_link_libraries(editor yave)
	target_link_libraries(editor y)

	find_package(assimp)
	if(assimp_FOUND)
		target_link_libraries(editor "${ASSIMP_LIBRARY_DIRS}/libassimp.dll.a")
		include_directories(SYSTEM ${ASSIMP_INCLUDE_DIRS}) # SYSTEM to kill random warnings
	else()
		message("Assimp not found")
		add_definitions(-DEDITOR_NO_ASSIMP)
	endif()
endif()








