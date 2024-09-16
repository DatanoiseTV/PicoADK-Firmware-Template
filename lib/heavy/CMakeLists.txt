cmake_minimum_required(VERSION 3.13)

add_library(heavy INTERFACE)
target_sources(heavy INTERFACE
${CMAKE_CURRENT_LIST_DIR}/HeavyContext.cpp
${CMAKE_CURRENT_LIST_DIR}/Heavy_prog.cpp
${CMAKE_CURRENT_LIST_DIR}/HvControlBinop.c
${CMAKE_CURRENT_LIST_DIR}/HvControlCast.c
${CMAKE_CURRENT_LIST_DIR}/HvControlDelay.c
${CMAKE_CURRENT_LIST_DIR}/HvControlRandom.c
${CMAKE_CURRENT_LIST_DIR}/HvControlSlice.c
${CMAKE_CURRENT_LIST_DIR}/HvControlSystem.c
${CMAKE_CURRENT_LIST_DIR}/HvControlUnop.c
${CMAKE_CURRENT_LIST_DIR}/HvControlVar.c
${CMAKE_CURRENT_LIST_DIR}/HvHeavy.cpp
${CMAKE_CURRENT_LIST_DIR}/HvLightPipe.c
${CMAKE_CURRENT_LIST_DIR}/HvMessage.c
${CMAKE_CURRENT_LIST_DIR}/HvMessagePool.c
${CMAKE_CURRENT_LIST_DIR}/HvMessageQueue.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalDel1.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalLine.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalPhasor.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalRPole.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalSamphold.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalTabread.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalTabwrite.c
${CMAKE_CURRENT_LIST_DIR}/HvSignalVar.c
${CMAKE_CURRENT_LIST_DIR}/HvTable.c
${CMAKE_CURRENT_LIST_DIR}/HvUtils.c
)
target_include_directories(heavy INTERFACE
 ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(heavy INTERFACE)


