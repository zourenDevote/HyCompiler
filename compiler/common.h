/**
 * Author: Devotes
 * Date:   2022/5/14
 *
 * FixLog:
 *
 */

#ifndef COMPILER_COMMON_H
#define COMPILER_COMMON_H
#include<string>


/// locate the code pos
struct position{

    int start_line;
    int start_column;
    std::string file_name;
};

#define ENUM_DEF(A, ...) enum A : unsigned char{__VA_ARGS__};


ENUM_DEF(Type,          INT = 0,
                        FLOAT,
                        VOID,
                        UNDEF
                        )

ENUM_DEF(ExpType,       ARITHMETIC_OPERATION_AS = 20, ARITHMETIC_OPERATION_SMD,
                        LOGIC_OPERATION_EQU,          LOGIC_OPERATION_NEQU,
                        COMPARE_OPERATION,
                        AND_OPERATION,                OR_OPERATION)


ENUM_DEF(UnaryExpType,  PRIAMRY_EXPRESSION,
                        FUNC_CALL
                        )


ENUM_DEF(NodeType,      COMP_UNIT = 30,             // 编译单元
                        DECL,                       // 变量声明
                        VAR_DEF,                    // 变量定义
                        ARRAY_VAR_DEF,              // 数组变量定义
                        ARRAY_MSG_NODE,             // 数组维度，元素，信息存储节点
                        INITIALIZED_NODE,           // 初始化列表结点
                        FUNC_DEF,                   // 函数定义
                        COMMON_FORMAL_PARA,         // 普通变量形式参数
                        ARRAY_FORMAL_PARA,          // 数组变量形式参数
                        CODE_BLOCK,                 // 代码快，或者说作用域Scope
                        IF_STATEMENT,               // if语句快
                        WHILE_STATEMENT,            // while循环语句块
                        BREAK_STATEMENT,            // Block语句
                        CONTINUE_STATEMENT,         // Continue语句
                        RETURN_STATEMENT,           // Return语句
                        EXPRESSION_STATEMENT,       // 表达式语句
                        ASSIGN_STATEMENT,           // 赋值语句
                        COMMON_LVALUE,              // 普通左值结点
                        ARRAY_REF_LVALUE,           // 数组引用左值节点
                        FUNC_CALL_NODE,             // FuncCall结点
                        INT_ITER_NODE,              // 整型字面值节点
                        FLOAT_ITER_NODE,            // 浮点型字面值节点
                        BINARY_OP_NODE,             // 二元运算节点
                        UNART_NODE,                 // unary_exp运算节点
                        )


ENUM_DEF(Operator,      ADD_OP= 60,                 // add          '+'
                        SUB_OP,                     // sub          '-'
                        MUL_OP,                     // mul          '*'
                        DIV_OP,                     // div          '/'
                        MOD_OP,                     // mod          '%'
                        GT,                         // gt           '>'
                        LT,                         // lt           '<'
                        LE,                         // le           '<='
                        GE,                         // ge           '>='
                        NOT,                        // not          '!'
                        AND,                        // and          '&&'
                        OR,                         // or           '||'
                        EQUAL_OP,                   // equal        '=='
                        NEQUAL_OP,                  // no_equal     '!='
                        )


ENUM_DEF(LogLevel,      DEBUG = 100,                // 调试信息
                        INFO,                       // 普通信息
                        WARNING,                    // 警告信息
                        ERROR,                      // 错误信息
                        FETAL                       // 程序崩溃日志信息
         )

ENUM_DEF(ErrorCode,     REDEFINE = 150,             // 重新定义变量
                        ARRAY_DIM_NOT_CONST,        // 数组长度定义不是常数
                        ARRAR_DIM_DEFINE_WITH_FLOAT,// 数组长度定义使用浮点类型
                        NO_INIT_VAL,                // 常量定义没有初始化
                        VAR_USE_WITHOUT_DECL,       // 符号未定义就使用
                        FUNC_NOT_MATCH_ANY,         // 没有匹配到任何函数式
                        BAD_BREAK,                  // break使用错误
                        BAD_CONTINUE,               // continue使用错误
                        COND_ERROR,                 // 条件类型非法
                        EXPR_TYPE_ERROR,            // 表达式类型推导非法
                        ARRAY_OFF_OUTRANGE,         // 数组明显越界
                        ARRAY_DIM_LT_ZERO,          // 数组长度使用小于0的数声明
                        ASSIGN_ERROR,               // 赋值语句右部给与错误类型
                        ASSIGN_CONST_VAR,           // 常量不能够被再次赋值
                        GLOBAL_ASSIG_BY_NONE_CONST, // 全局常量
                        ASSIGN_WITH_VOID,           // 使用空的类型进行初始化
        )


ENUM_DEF(WarningCode,   COND_TYPE_WARNING = 180,    // 警告1
         )
#endif //COMPILER_COMMON_H
