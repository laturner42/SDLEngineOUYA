
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
APP_STL := stlport_static
APP_CPPFLAGS := -std=gnu++0x
APP_USE_CPP0X := true
#NDK_TOOLCHAIN_VERSION := clang3.3
#LOCAL_STATIC_LIBRARIES += libstlport_static
#LOCAL_C_INCLUDES += external/stlport/stlport bionic/ bionic/libstdc++/include
#APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1 -std=c++11 -Wno-literal-suffix -fsigned-char