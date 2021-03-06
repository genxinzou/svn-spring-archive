INCLUDE_DIRECTORIES(streflop lua/include luabind)

AUX_SOURCE_DIRECTORY(lua/src lualibfiles)
ADD_LIBRARY(lua STATIC ${lualibfiles})
TARGET_LINK_LIBRARIES(lua streflop)

AUX_SOURCE_DIRECTORY(luabind/src luabindfiles)
ADD_LIBRARY(luabind STATIC ${luabindfiles})
TARGET_LINK_LIBRARIES(luabind lua)

ADD_DEFINITIONS(-D_SZ_ONE_DIRECTORY)
AUX_SOURCE_DIRECTORY(7zip 7zipfiles)
ADD_LIBRARY(7zip STATIC ${7zipfiles})

AUX_SOURCE_DIRECTORY(hpiutil2 hpifiles)
ADD_LIBRARY(hpiutil2 STATIC ${hpifiles})
TARGET_LINK_LIBRARIES(hpiutil2 z)

IF (UNIX)
	ADD_LIBRARY(minizip STATIC  minizip/unzip minizip/zip minizip/ioapi)
ELSE (UNIX)
	ADD_LIBRARY(minizip STATIC  minizip/unzip minizip/zip minizip/iowin32 minizip/ioapi)
ENDIF (UNIX)
TARGET_LINK_LIBRARIES(minizip z)

ADD_SUBDIRECTORY(streflop)