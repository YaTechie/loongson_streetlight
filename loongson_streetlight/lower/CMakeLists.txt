project ("lower")

file(GLOB ROOT_SETTING ${CMAKE_CURRENT_SOURCE_DIR}/*.ini)
file(GLOB ROOT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/stdafx*;${CMAKE_CURRENT_SOURCE_DIR}/targetver*;${CMAKE_CURRENT_SOURCE_DIR}/main*)
file(GLOB ROOT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h;${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB ROOT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp;${CMAKE_CURRENT_SOURCE_DIR}/*.c)

source_group("Main" FILES ${ROOT_MAIN})
source_group("Precompile Source File" FILES "${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/cmake_pch.cxx")
source_group("Header Files" FILES ${ROOT_HEADER})
source_group("Source Files" FILES ${ROOT_SOURCE})

include_directories("${CMAKE_SOURCE_DIR}/3rd")

message(STATUS "------------ ${PROJECT_NAME} -------------")
message(STATUS "${PROJECT_NAME}_header: ${ROOT_HEADER}")
message(STATUS "${PROJECT_NAME}_source: ${ROOT_SOURCE}")

add_executable(${PROJECT_NAME} ${ROOT_SOURCE} ${ROOT_HEADER} ${ROOT_SETTING})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h)

set( ALL_COPY_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ini
)

if (UNIX)
		
	foreach( file_i ${ALL_COPY_FILES})

		message(STATUS "copy:${file_i} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}.")
		
		add_custom_command(
			TARGET
			${PROJECT_NAME}
			PRE_BUILD
			COMMAND echo "copy ${file_i} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
			COMMAND ${CMAKE_COMMAND} -E 
			copy_if_different
			${file_i}
			"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
			)
		
	endforeach( file_i )

else ()
	
	foreach( file_i ${ALL_COPY_FILES})

		message(STATUS "copy:${file_i} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>.")
		
		add_custom_command(
			TARGET
			${PROJECT_NAME}
			PRE_BUILD
			COMMAND echo "copy ${file_i} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>"
			COMMAND ${CMAKE_COMMAND} -E 
			copy_if_different
			${file_i}
			"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>"
			)
		
	endforeach( file_i )

endif()
