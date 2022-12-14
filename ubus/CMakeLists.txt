cmake_minimum_required(VERSION 2.6)

PROJECT(ubus C)
ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -g3 -Wmissing-declarations)

OPTION(BUILD_LUA "build Lua plugin" ON)
OPTION(BUILD_EXAMPLES "build examples" ON)
OPTION(ENABLE_SYSTEMD "systemd support" ON)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(UBUS_UNIX_SOCKET "/var/run/ubus.sock")
SET(UBUS_MAX_MSGLEN 1048576)

INCLUDE_DIRECTORIES("xxx/json-c/include/json-c")
INCLUDE_DIRECTORIES("xxx/json-c/include")
LINK_DIRECTORIES("xxx/json-c/lib")

INCLUDE_DIRECTORIES(xxx/libubox/include/libubox)
INCLUDE_DIRECTORIES(xxx/libubox/include)
LINK_DIRECTORIES(xxx/libubox/lib)

SET(CMAKE_INSTALL_PREFIX "xxx/ubus")
SET(DESTINATION "xxx/ubus")

ADD_DEFINITIONS( -DUBUS_UNIX_SOCKET="${UBUS_UNIX_SOCKET}")
ADD_DEFINITIONS( -DUBUS_MAX_MSGLEN=${UBUS_MAX_MSGLEN})

IF(APPLE)
  INCLUDE_DIRECTORIES(/opt/local/include)
  LINK_DIRECTORIES(/opt/local/lib)
ENDIF()

ADD_LIBRARY(ubus SHARED libubus.c libubus-io.c libubus-obj.c libubus-sub.c libubus-req.c)
TARGET_LINK_LIBRARIES(ubus ubox)

ADD_EXECUTABLE(ubusd ubusd.c ubusd_id.c ubusd_obj.c ubusd_proto.c ubusd_event.c)
TARGET_LINK_LIBRARIES(ubusd ubox)

#find_library(json NAMES json-c json)
ADD_EXECUTABLE(cli cli.c)
SET_TARGET_PROPERTIES(cli PROPERTIES OUTPUT_NAME ubus)
TARGET_LINK_LIBRARIES(cli ubus ubox blobmsg_json json-c)

#ADD_SUBDIRECTORY(lua)
#ADD_SUBDIRECTORY(examples)

INSTALL(TARGETS ubus cli
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
INSTALL(TARGETS ubusd
    RUNTIME DESTINATION sbin
)

INSTALL(FILES ubusmsg.h ubus_common.h libubus.h DESTINATION include)

# FIXME: this works but certainly can be done better:
SET(UBUSD_BINARY "${CMAKE_INSTALL_PREFIX}/sbin/ubusd")

# do this after the installs so we have the proper paths
#IF(ENABLE_SYSTEMD)
#  INCLUDE(FindPkgConfig)
#  PKG_CHECK_MODULES(SYSTEMD systemd)
#  IF(SYSTEMD_FOUND)
#    ADD_SUBDIRECTORY(systemd)
#  ENDIF()
#ENDIF()