CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

CreateSharedLibrary(photon-lib
DIRS
	.
FEATURES
	cxx_auto_type cxx_range_for
LINK
	humblepeer
	sha1
	websockets
	${CMAKE_THREAD_LIBS_INIT}
)

source_group("Source" REGULAR_EXPRESSION ".")
source_group("" FILES CMakeLists.txt)