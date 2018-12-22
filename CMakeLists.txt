# This CMakeList.txt currently only supports compilation of
#a program on windows or linux, but only with the dependancies
#for oclPixelGameEngine.h

# NOTE: THIS CMAKELIST WILL NOT INSTALL DEPENDANCIES, IT WILL JUST FIND THEM AND
#COMPILE / LINK THEM RESPECTIVELY, YOU NEED TO INSTALL THEM YOURSELF

# Any issues, submit an issue, or contact the author, "Ben (plane000)#8618" on Discord

# Currently linked / compiled by default is:
#Threads (pthread), OpenGL, GLX, libPNG, X11


cmake_minimum_required(VERSION 3.7)
project(oclPixelGameEngine) # You can change this with your project name!

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CMakeFiles/) # Don't worry what this does, it just gives more compatability options with libraries!
cmake_policy(SET CMP0037 OLD) # This just allows the / charicter in path names

set(ExecutableName output) # You can change this to whatever you wish the executable outputed to be, don't worry about a .exe, i'll do that!
set(SourceDir src) # Change src/ to wherever your code is

# You can uncomment these 2 lines if you wish to have some headers included
# set(IncludeDir include/) # Change include/ to your include directory
# include_directories(${IncludeDir})

# Uncomment this if you have your source in a folder called src/
# file(GLOB_RECURSE SourceFiles
#     ${SourceDir}/*.cpp
# )

# Comment this line if you wish to compile your own programs source code
set(SourceFiles OneLoneCoder_PGE_SpriteTransforms.cpp)

# From this point on, you don't need to worry about what the code does, it just manages dependancies, linking and compilation
#enjoy tinkering!

# Find thread package
set(THREADS_PREFER_PTHREAD_FLAD ON)
find_package(Threads REQUIRED)

# Find GL and GLX package
find_package(OpenGL REQUIRED)


if (UNIX)
    find_package(X11 REQUIRED)
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})

    # Set platform spesific output names
    set(Executable ${ExecutableName}.o)
endif (UNIX)
if (WIN32)
    target_include_directories(${WinSDK})

    # Set platform spesific output names
    set(Executable ${ExecutableName}.exe)
endif (WIN32)

add_executable(${Executable} 
    ${SourceFiles}
    # Add more directories, files or CMake variables here to have them compile too!
)

target_link_libraries(${Executable}
    Threads::Threads
    OpenGL::OpenGL
    OpenGL::GL
    OpenGL::GLX
)

if (UNIX) 
    target_link_libraries(${Executable} 
        ${X11_LIBRARIES}
        PNG::PNG
    )
endif (UNIX)
if (WIN32)
    target_link_libraries(${Executable} 
        ${WinSDK}
    )
endif (WIN32)
