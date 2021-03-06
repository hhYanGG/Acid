if(APPLE)
	find_path(OPENAL_INCLUDE_DIR al.h)
else()
	set(OPENAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libraries/openal/include" PARENT_SCOPE)
endif()

find_library(OPENAL_LIBRARY NAMES openal OpenAL32 OpenAL PATHS "${PROJECT_SOURCE_DIR}/Libraries/openal/lib/" PARENT_SCOPE)
message(STATUS "Found OpenAL: ${OPENAL_LIBRARY}")
