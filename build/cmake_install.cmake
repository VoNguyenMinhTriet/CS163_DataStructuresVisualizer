# Install script for directory: C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Project")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/ucrt64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/AudioDevice.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/AudioStream.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/AutomationEventList.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/BoundingBox.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Camera2D.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Camera3D.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Color.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/FileData.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/FileText.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Font.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Functions.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Gamepad.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Image.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Keyboard.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Material.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Matrix.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Mesh.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/MeshUnmanaged.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Model.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/ModelAnimation.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Mouse.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Music.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Ray.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/RayCollision.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/RaylibException.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/raylib-cpp-utils.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/raylib-cpp.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/raylib.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/raymath.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Rectangle.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/RenderTexture.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/ShaderUnmanaged.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Shader.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Sound.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Text.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Texture.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/TextureUnmanaged.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Touch.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Vector2.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Vector3.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Vector4.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/VrStereoConfig.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Wave.hpp"
    "C:/Users/ASUS/CS163_DataStructuresVisualizer/extern/include/raylib-cpp/Window.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/ASUS/CS163_DataStructuresVisualizer/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
