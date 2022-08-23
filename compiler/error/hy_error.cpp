/**
 * Author: Devotes
 * Date:   2022/6/1
 *
 * FixLog:
 *  20220612 将错误报告修改为中文提示
 */

#include "hy_error.h"
#include <sstream>

using namespace std;

std::string message_error(const position &pos, const std::string& msg, ErrorCode error_code){
    stringstream ss;
    ss<<"<"<<pos.file_name<<">:["<<pos.start_line<<":"<<pos.start_column;
    ss<<"] ";

    switch (error_code) {
        case REDEFINE:{
            ss<<"'"<<msg<<"' 错误! 这个符号在当前作用区域已经声明!";
            break;
        }
        case ARRAY_DIM_NOT_CONST:{
            ss<<"'"<<msg<<"' 错误! 不能使用编译时无法确定的变量定义数组维度!";
            break;
        }
        case ARRAR_DIM_DEFINE_WITH_FLOAT:{
            ss<<"'"<<msg<<"' 错误! 浮点数类型不允许出现在数组维度定义中!";
            break;
        }
        case NO_INIT_VAL:{
            ss<<"'"<<msg<<"' 错误! 常量定义必须初始化!";
            break;
        }
        case VAR_USE_WITHOUT_DECL:{
            ss<<"'"<<msg<<"' 错误! 使用的变量未声明!";
            break;
        }
        case COND_ERROR:{
            ss<<"'"<<msg<<"' 错误! 非法的条件表达式！";
            break;
        }
        case ARRAY_OFF_OUTRANGE:{
            ss<<"'"<<msg<<"' 错误! 数组引用超出范围!";
            break;
        }
        case ARRAY_DIM_LT_ZERO:{
            ss<<"'"<<msg<<"' 错误! 数组维度定义必须是一个大于0的数!";
            break;
        }
        case BAD_BREAK:{
            ss<<"'"<<msg<<"' 错误! break语句使用错误!";
            break;
        }
        case BAD_CONTINUE:{
            ss<<"'"<<msg<<"' 错误! continue语句使用错误!";
            break;
        }
        case EXPR_TYPE_ERROR:{
            ss<<"'"<<msg<<"' 错误! 表达式类型错误!";
            break;
        }
        case FUNC_NOT_MATCH_ANY:{
            ss<<"'"<<msg<<"' 错误! 没有匹配到任何函数原型!";
            break;
        }
        case ASSIGN_ERROR:{
            ss<<"'"<<msg<<"' 错误! 赋值语句右边是VOID类型!";
            break;
        }
        case ASSIGN_CONST_VAR:{
            ss<<"'"<<msg<<"' 错误! 常量不能再次赋值!";
            break;
        }
        case ASSIGN_WITH_VOID:{
            ss<<"'"<<msg<<"' 错误! 变量初始化为VOID类型!";
            break;
        }
        default:
            break;
    }

    return ss.str();
}


std::string message_warning(const position &pos, const std::string& msg, WarningCode warn_code){

    stringstream ss;
    ss<<"<"<<pos.file_name<<">:["<<pos.start_line<<":"<<pos.start_column;
    ss<<"] ";

    return ss.str();
}