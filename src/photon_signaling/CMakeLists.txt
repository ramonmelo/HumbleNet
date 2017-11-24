CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

set(HUMBLENET_SERVER_URL "ws://localhost:8080/ws" CACHE STRING "The peer server to use for the test apps")

GenerateFlatBuffer(test_peer_flatbuffers test_peer.fbs)

set(PROJECT_LINK_LIBS
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Common-cpp/lib/libCommon-cpp_release_macosx.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Photon-cpp/lib/libPhoton-cpp_release_macosx.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/LoadBalancing-cpp/lib/libLoadBalancing-cpp_release_macosx.a)

if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation)

   set(PROJECT_LINK_LIBS ${COREFOUNDATION_LIBRARY} ${PROJECT_LINK_LIBS})
endif (APPLE)

CreateTool(photon_humble_peer
DIRS
    .
    src
FILES
    photon_humble_peer.cpp
    ${test_peer_flatbuffers}
REFERENCE
    test_peer.fbs
DEFINES
    HUMBLENET_SERVER_URL=\"${HUMBLENET_SERVER_URL}\"
FEATURES
    cxx_auto_type cxx_range_for
INCLUDES
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
LINK
    humblenet
    flatbuffers
    ${PROJECT_LINK_LIBS}
    -g
PROPERTIES
    FOLDER HumbleNet/PhotonSignaling
)
