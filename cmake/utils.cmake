include(FetchContent)

function(create_task_executable TARGET_NAME SOURCE_FILE)
	add_executable(${TARGET_NAME} ${SOURCE_FILE})

	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

function(create_task_test TEST_NAME SOURCE_FILE)
	add_executable(${TEST_NAME} ${SOURCE_FILE})

	target_link_libraries(${TEST_NAME} PRIVATE gtest_main)

	add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

function(setup_gtest)
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG release-1.11.0
	)

	FetchContent_MakeAvailable(googletest)
endfunction()

macro(add_all_subdirectories)
	file(GLOB files_list ${CMAKE_CURRENT_SOURCE_DIR}/*)

	foreach(file ${files_list})
		if (IS_DIRECTORY ${file})
			add_subdirectory(${file})
		endif()
	endforeach()
endmacro()