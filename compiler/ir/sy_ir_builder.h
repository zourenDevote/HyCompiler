/**
 * Author:  Devotes
 * Date:    20220615
 *
 * IRbuilder
 */

#ifndef COMPILER_SYIRBUILDER_H
#define COMPILER_SYIRBUILDER_H

#include "sy_ir.h"

namespace IR{

    class SyIRBuilder{
    public:
        SyIRBuilder() = default;

        // 创建一个浮点型常量
        static VALUE createFConstant(float v);

        // 创建一个整数型常量
        static VALUE createIConstant(int v);

        // 创建一个具名寄存器型变量
        static VALUE createFReg(const std::string& name);
        static VALUE createIReg(const std::string& name);

        // 创建一个临时寄存器型变量
        VALUE createTempFReg();
        VALUE createTempIReg();

        // 创建一个具名指针类型
        static VALUE createFPE(const std::string& name);
        static VALUE createIPE(const std::string& name);

        // 创建一个临时名整数指针类型
        VALUE createTempFPE();
        VALUE createTempIPE();

        // 创建一个标签
        static VALUE createLabel(const std::string& name, BASIC_BLOCK bb);

        // 创建一个全局变量
        static GLOBAL_VAR_DEF createFGlobalVariable(const std::string& name, float v);
        static GLOBAL_VAR_DEF createIGlobalVariable(const std::string& name, int v);

        // 创建一个全局数组
        static GLOBAL_ARRAY_DEF createFGlobalArray(const std::string& name, int width, const std::vector<float>& inits,const std::vector<int>& needs);
        static GLOBAL_ARRAY_DEF createIGlobalArray(const std::string& name, int width, const std::vector<int>& inits,const std::vector<int>& needs);

        // 创建一个函数声明
        static FUNC_DECL createFuncDecl(const std::string& name, const FUNC_MSG_PTR& ptr);

        // 创建一个函数定义
        static FUNC_DEFINE createFuncDef(const std::string& name, const FUNC_MSG_PTR& ptr, const std::vector<std::string>& param_names);

        // 创建一个基本快
        static BASIC_BLOCK createBB(const std::string& name);

        // 创建一个Alloca指针类型
        static INSTRUCTION createFAlloca(VALUE result, const IR::BASIC_BLOCK& def, int w = 4, bool isArray = false);
        static INSTRUCTION createIAlloca(VALUE result, const IR::BASIC_BLOCK& def, int w = 4, bool isArray = false);

        // 创建一条Not指令
        static INSTRUCTION createINot(VALUE arg1, VALUE result);

        // 创建一条加法指令
        static INSTRUCTION createIAdd(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条减法指令
        static INSTRUCTION createISub(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条除法指令
        static INSTRUCTION createIDiv(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条乘法指令
        static INSTRUCTION createIMul(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条取余指令
        static INSTRUCTION createIMod(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条小于等于比较指令
        static INSTRUCTION createILe(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条小于比较指令
        static INSTRUCTION createILt(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条大于等于比较指令
        static INSTRUCTION createIGe(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条大于比较指令
        static INSTRUCTION createIGt(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条等于比较指令
        static INSTRUCTION createIEqual(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条非等于比较指令
        static INSTRUCTION createINEqual(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条或运算指令
        static INSTRUCTION createIOr(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条与运算指令
        static INSTRUCTION createIAnd(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条获取指针的指令
        static INSTRUCTION createIGetPtr(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条跳转指令
        static INSTRUCTION createIJump(VALUE arg1);

        // 创建一条条件跳转指令
        static INSTRUCTION createICmp(VALUE arg1, VALUE arg2, VALUE result);

        // 创建一条参数传递指令
        static INSTRUCTION createIParam(VALUE arg1, const std::string& func, int ord, bool is_reverse);

        // 创建一条函数调用指令
        static INSTRUCTION createIFuncCall(const std::string& name, int paraNum, int intParaNum, int floatParaNum, bool reverse, VALUE ret = nullptr);

        // 创建一条返回指令
        static INSTRUCTION createIRet(VALUE arg1);

        // 创建一个变量加载指令
        static INSTRUCTION createILoad(VALUE arg1, VALUE result);

        // 创建一个变量存储指令
        static INSTRUCTION createIStore(VALUE arg1, VALUE result);

        // 创建一条整数转浮点数指令
        static INSTRUCTION createIItof(VALUE arg1, VALUE result);

        // 创建一条浮点数转整数指令
        static INSTRUCTION createIFtoi(VALUE arg1, VALUE result);

        // 创建一条memset指令
        static INSTRUCTION createIMemSet(VALUE arg1, const std::string& name, int number);

        // 创建一条留空的条件跳转指令
        static std::shared_ptr<IR::InstThreeOp> createIEmptyCmp(VALUE arg1);

        void reset(){ ssa_use = -1; }
    private:

        // SSA辅助字段
        int ssa_use{-1};
    };
}

#endif //COMPILER_SYIRBUILDER_H
