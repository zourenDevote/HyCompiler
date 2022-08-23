project(pass)

AUX_SOURCE_DIRECTORY(./ SRC)

add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} hy_ir)
