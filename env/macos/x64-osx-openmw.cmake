# The tree's libraries on macOS (a port of the Linux triplet for AMDRadeonRDNA4's rig): static, release only, x86_64.
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES x86_64)
set(VCPKG_OSX_DEPLOYMENT_TARGET 15.0)
set(VCPKG_BUILD_TYPE release)
set(VCPKG_C_FLAGS "-g1 -fno-omit-frame-pointer")
set(VCPKG_CXX_FLAGS "-g1 -fno-omit-frame-pointer")
# OpenSceneGraph's GL2 profile, which the rasterizer is written against.
set(osg_OPENGL_PROFILE GL2)
