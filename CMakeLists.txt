cmake_minimum_required(VERSION 3.20)

project(lim
    VERSION  0.0.1
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD          17                     )
set(CMAKE_CXX_STANDARD_REQUIRED ON                     )
set(CMAKE_CXX_EXTENSIONS        OFF                    )
set(SKIP_UNINSTALL_TARGET       ON  CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS           OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS      OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS             OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL                OFF CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_17           ON  CACHE BOOL "" FORCE)
set(GLM_ENABLE_FAST_MATH        ON  CACHE BOOL "" FORCE)

# In lim's CMakeLists.txt or wherever it's added
if (NOT TARGET glm)
    add_subdirectory(extern/lim/extern/glm/glm)
endif()

add_subdirectory(extern/glfw)

add_library(lim
    source/common/string.cpp
    source/notify/notify.cpp
    source/render/render.cpp
    source/window/window.cpp
    source/kernel/kernel.cpp
)

target_include_directories(lim PRIVATE source)

target_link_libraries(lim PRIVATE d3d9)
target_link_libraries(lim PRIVATE glm::glm)
target_link_libraries(lim PRIVATE glfw)
