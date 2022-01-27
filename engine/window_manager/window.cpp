//@	{
//@ "target":{"name":"window.o", "pkgconfig_libs":["x11"]}
//@	}

#include "./window.hpp"

#define GLFW_EXPOSE_NATIVE_X11

#include <GLFW/glfw3native.h>
#include <cstring>
#include <memory>
