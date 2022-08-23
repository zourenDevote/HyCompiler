link_directories(lib)

include_directories(antlr4-runtime)

add_subdirectory(antlr_generate)
add_subdirectory(utility)
add_subdirectory(ast)
add_subdirectory(error)
add_subdirectory(semantic)
add_subdirectory(ir)
add_subdirectory(pass)
add_subdirectory(asm)
add_subdirectory(preopti)

add_executable(${PROJECT_NAME} compiler.cpp cxxopts.hpp)

target_link_libraries(${PROJECT_NAME} antlr_parser antlr4-runtime new_ast_creator semantic_check pass asm dagbuilder preopti)
