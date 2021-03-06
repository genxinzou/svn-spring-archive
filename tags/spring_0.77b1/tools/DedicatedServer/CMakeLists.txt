ADD_DEFINITIONS(-DSTREFLOP_X87) # should be in ../../CMakeLists.txt
ADD_DEFINITIONS(-DDEDICATED ${PIC_FLAG} -DNO_AVI)

AUX_SOURCE_DIRECTORY(../../rts/System/Net/ netfiles)
INCLUDE_DIRECTORIES(../../rts/System/Net/ ../../rts/System/ ../../rts/lib/lua/include)
INCLUDE_DIRECTORIES(../../rts/ ../../rts/Game ../../rts/lib/7zip ../../rts/System)

IF (UNIX)
	SET(platformfiles
		../../rts/System/Platform/Linux/UnixFileSystemHandler
		../../rts/System/Platform/Linux/DataDirLocater
		../../rts/System/Platform/Linux/DotfileHandler)
ELSE (UNIX)
	SET(platformfiles
		../../rts/System/Platform/Win/WinFileSystemHandler
		../../rts/System/Platform/Win/DataDirLocater
		../../rts/System/Platform/Win/RegHandler)
ENDIF (UNIX)

AUX_SOURCE_DIRECTORY(../../rts/System/FileSystem/ fsfiles)
SET(system_files 
	${fsfiles}
	${platformfiles}
	 ${netfiles}
	../../rts/System/TdfParser
	../../rts/System/Platform/FileSystem
	../../rts/System/Platform/ConfigHandler
	../../rts/System/LogOutput
	../../rts/System/BaseNetProtocol
	../../rts/System/Demo
	../../rts/System/DemoReader
	../../rts/System/AutohostInterface
	../../rts/System/UnsyncedRNG )

AUX_SOURCE_DIRECTORY(../../rts/Game/Server serverfiles)
ADD_LIBRARY(server SHARED
	${system_files}
	${serverfiles}
	../../rts/Game/GameServer
	../../rts/Game/GameSetup
	../../rts/Game/GameData
	../../rts/Game/PlayerBase
	../../rts/Game/GameVersion
	../../rts/Game/CommandMessage
	../../rts/Game/ChatMessage
	../../rts/Game/Console
	../../rts/Game/Action
	../../rts/Lua/LuaIO
	../../rts/Lua/LuaParser
	../../rts/Lua/LuaUtils
	../../rts/Map/MapParser
	../../rts/Rendering/Textures/TAPalette)
TARGET_LINK_LIBRARIES(server SDL hpiutil2 7zip minizip lua)
if (MINGW)
	TARGET_LINK_LIBRARIES (server ws2_32 boost_thread-mt boost_regex-mt)
else (MINGW)
	TARGET_LINK_LIBRARIES (server ${Boost_REGEX_LIBRARY} ${Boost_THREAD_LIBRARY})
	ADD_DEFINITIONS (-fvisibility=default ) #overwrite hidden visibility
endif (MINGW)

ADD_EXECUTABLE(dedicated main)
TARGET_LINK_LIBRARIES(dedicated server)

install (TARGETS server dedicated RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
