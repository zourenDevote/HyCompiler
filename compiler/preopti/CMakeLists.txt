
# ast 针对层面的优化
project(preopti)


add_library(${PROJECT_NAME} func_inline.h func_inline.cpp symbol_table.h symbol_table.cpp inst_combine.h inst_combine.cpp func_execute_analysis.h func_execute_analysis.cpp)

