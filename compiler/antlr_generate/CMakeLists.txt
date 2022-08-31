project(antlr_parser)

link_directories(../lib)

FILE(GLOB ANTLR_GENERATE *.h *.cpp)

add_library(${PROJECT_NAME} ${ANTLR_GENERATE})

target_link_libraries(${PROJECT_NAME} antlr4-runtime)
