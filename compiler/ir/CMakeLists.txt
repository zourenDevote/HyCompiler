project(hy_ir)

add_library(${PROJECT_NAME} sy_ir.cpp sy_ir.h sy_ir_builder.h sy_ir_builder.cpp ir_generator.cpp ir_generator.h)

target_link_libraries(${PROJECT_NAME} dagbuilder)
