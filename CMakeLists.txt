cmake_minimum_required(VERSION 3.17)

project(G2O_ExtendedTools)

option(INSTALL_AFTER_BUILD "Run cmake --install separately for every target after each build. By default this option is set to OFF" OFF)

set(GAME_PATH "" CACHE PATH "This option specifies the game location. It's only used for the installation step.")
set(SERVER_PATH "" CACHE PATH "This option specifies the server location. It's only used for the installation step.")

file(GLOB SRC
	"src/api/squirrel_api.h"
	"src/api/module_api.h"
	"src/api/module_api.cpp"

	"src/g2o/*.h"
	"src/g2o/*.cpp"

	"src/pch.h"
	"src/sqmain.cpp"
)

add_library(G2O_ExtendedTools SHARED ${SRC})
target_precompile_headers(G2O_ExtendedTools PRIVATE "src/pch.h")

target_include_directories(G2O_ExtendedTools
	INTERFACE
		"include/"
	PRIVATE
		"src/"
)

add_subdirectory(dependencies)

if (NOT ${GAME_PATH} STREQUAL "")
	install(TARGETS G2O_ExtendedTools
			RUNTIME
			  DESTINATION ${GAME_PATH}
			  COMPONENT "clientModule"
	)

	if(INSTALL_AFTER_BUILD)
		add_custom_command(TARGET G2O_ExtendedTools 
			POST_BUILD
				COMMAND ${CMAKE_COMMAND} --install ${CMAKE_CURRENT_BINARY_DIR} --component "clientModule"
		)
	endif()
endif()

if (NOT ${SERVER_PATH} STREQUAL "")
	install(TARGETS G2O_ExtendedTools
			RUNTIME
			  DESTINATION ${SERVER_PATH}
			  COMPONENT "serverModule"
	)

	if(INSTALL_AFTER_BUILD)
		add_custom_command(TARGET G2O_ExtendedTools 
			POST_BUILD
				COMMAND ${CMAKE_COMMAND} --install ${CMAKE_CURRENT_BINARY_DIR} --component "serverModule"
		)
	endif()
endif()
