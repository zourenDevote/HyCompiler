#file(GLOB AST_CREATOR_SRC ast.h ast.cpp ast_creator.h ast_creator.cpp)
file(GLOB NEW_AST_CREATOR_SRC ast.h ast.cpp ast_creator.h ast_creator.cpp)

#SET(AST_CREATOR ast_creator)
SET(NEW_AST_CREATOR new_ast_creator)

#add_library(${AST_CREATOR} ${AST_CREATOR_SRC})
add_library(${NEW_AST_CREATOR} ${NEW_AST_CREATOR_SRC})

#target_link_libraries(${AST_CREATOR} antlr_parser antlr4-runtime)
target_link_libraries(${NEW_AST_CREATOR} antlr_parser antlr4-runtime hy_utility)



