cmake_minimum_required(VERSION 3.13)

add_library(heavy INTERFACE)
target_sources(heavy INTERFACE
    Heavy_prog.cpp
    HeavyContext.cpp
    HvControlBinop.c
    HvControlCast.c
    HvControlDelay.c
    HvControlIf.c
    HvControlRandom.c
    HvControlSlice.c
    HvControlSystem.c
    HvControlUnop.c
    HvControlVar.c
    HvHeavy.cpp
    HvLightPipe.c
    HvMessage.c
    HvMessagePool.c
    HvMessageQueue.c
    HvSignalDel1.c
    HvSignalLine.c
    HvSignalPhasor.c
    HvSignalRPole.c
    HvSignalSamphold.c
    HvSignalTabread.c
    HvSignalTabwrite.c
    HvSignalVar.c
    HvTable.c
    HvUtils.c

)
target_include_directories(heavy INTERFACE
 ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(heavy INTERFACE)


