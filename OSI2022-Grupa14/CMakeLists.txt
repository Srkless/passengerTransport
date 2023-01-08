# CMakeList.txt : CMake project for OSI2022-Grupa14, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (OSI2022-Grupa14 "OSI2022-Grupa14.cpp" "OSI2022-Grupa14.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OSI2022-Grupa14 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
