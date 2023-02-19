file(GLOB files_list ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(file ${files_list})
	if (IS_DIRECTORY ${file})
		add_subdirectory(${file})
	endif()
endforeach()