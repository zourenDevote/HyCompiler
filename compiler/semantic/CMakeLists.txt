project(semantic_check)

file(GLOB SEM_SRC semantic_check.h semantic_check.cpp symbol.h symbol.cpp pre_ir_analysis.h pre_ir_analysis.cpp)

add_library(${PROJECT_NAME} ${SEM_SRC})

target_link_libraries(${PROJECT_NAME} hy_error new_ast_creator)

add_library(dagbuilder dag_ast_builder.h dag_ast_builder.cpp)
target_link_libraries(dagbuilder new_ast_creator)


