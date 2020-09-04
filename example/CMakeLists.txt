cmake_minimum_required(VERSION 3.0)
project(example)

SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/include)

SET(EXAMPLE_SOURCES

  ${CMAKE_CURRENT_SOURCE_DIR}/src/basic/01_lookupActuators.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/basic/02a_enableSingleActuator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/basic/02b_enableActuatorsInBatch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/basic/03a_currentControl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/basic/03b_velocityControl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/basic/03c_positionControl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced/04_actuatorSetting.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced/05a_feedback_sync.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced/05b_feedback_asyc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced/06a_auto_homing.cpp
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
	SET(LIB_DIR_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/lib/windows_x64/release)
	SET(LIB_DIR_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/lib/windows_x64/debug)
	link_directories(${LIB_DIR_RELEASE})
	link_directories(${LIB_DIR_DEBUG})
	SET(COPY_LIBRARY ${LIB_DIR_RELEASE}/actuatorController.dll)
	SET(COPY_LIBRARYD ${LIB_DIR_DEBUG}/actuatorControllerd.dll)
#	if(CMAKE_BUILD_TYPE MATCHES "Release")
#		SET(COPY_LIBRARY ${LIB_DIR_RELEASE}/actuatorController.dll)
#	else()
#		SET(COPY_LIBRARY ${LIB_DIR_DEBUG}/actuatorControllerd.dll)
#	endif()
elseif(UNIX)
	SET(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/lib/linux_x86_64)
	link_directories(${LIB_DIR})
endif()

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/asio)
include_directories(${INCLUDE_DIR}/serial/include)


add_compile_options(-std=c++11)
add_compile_options(-fPIC)
#add_compile_options(-DQT_NO_VERSION_TAGGING)
add_compile_options(-pthread)
add_compile_options(-DASIO_STANDALONE)
add_compile_options(-DASIO_HAS_STD_CHRONO)

foreach(EXAMPLE ${EXAMPLE_SOURCES})
    get_filename_component(EX_NAME ${EXAMPLE} NAME_WE)
    add_executable(${EX_NAME} ${EXAMPLE})
    if(WIN32)
        target_link_libraries(${EX_NAME} debug actuatorControllerd optimized ActuatorController)
        add_custom_command(TARGET ${EX_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${COPY_LIBRARY}" "${COPY_LIBRARYD}"
        $<TARGET_FILE_DIR:${EX_NAME}>
        )
    elseif(UNIX)
        target_link_libraries(${EX_NAME} ActuatorController pthread)
    endif()
endforeach(EXAMPLE ${EXAMPLE_SOURCES})

