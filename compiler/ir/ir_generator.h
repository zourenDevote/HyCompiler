/**
 * Author:  Devotes
 * Date:    20220616
 *
 * FixLog:
 *
 */
#ifndef COMPILER_IR_GENERATOR_H
#define COMPILER_IR_GENERATOR_H

#include "sy_ir_builder.h"
#include "../ast_visitor.h"
#include "../utility/hy_utility.h"
#include "../semantic/dag_ast_builder.h"
#include <map>
#include <vector>
#include <set>

namespace IR{

    /* 数组符号 */
    struct ArrayVarSymbol{
        ArrayVarSymbol(const std::string& name, IRType ty, const std::vector<int>& dims, VALUE V);

        std::string name;
        IRType ty;
        std::vector<int> dims;
        VALUE V;
    };

    // 记录
    struct Record{
        Record(){
            condidx = 0;
            ifidx = 0;
            whileidx = 0;
            bbidx = 0;
        }

        int condidx;
        int ifidx;
        int bbidx;
        int whileidx;

    };

    struct SyScope{
    public:
        explicit SyScope(int level = 0) : level(level){}

        // 父作用域
        SyScope* parent{nullptr};

        // 是否有跳转指令
        bool hasJump{false};

        // 添加数组符号，获得数组符号的属性
        ArrayVarSymbol* getArraySymbol(const std::string& name);
        void addArraySymbol(ArrayVarSymbol* arr);

        // 添加普通符号，获得普通符号
        VALUE getSymbol(const std::string& name);
        void  addSymbol(const std::string& name, const VALUE& pointer);

        // 当前作用区域普通变量变量存储
        std::map<std::string, VALUE> symbol_table{};

        // 当前作用域数组维度存储
        std::map<std::string, ArrayVarSymbol*> array_symbol_table{};

        int level;
    };

    class IRGenerate : public AstVisitor{
    public:
        IRGenerate(const MODULE& M);

        // 进入作用域
        void entryScope();

        // 离开作用域
        void leaveScope();

        // 辅助方法
        void visitStmt(const std::shared_ptr<Node>& node);
        VALUE getIntV(int v);
        VALUE getFloatV(float v);
        unsigned getOccurTime(const std::string& id);

        // 辅助数组参数索引
        void calcuArrRef(const std::vector<SmartExpr>& express, const std::vector<int>& dims);

        /// 辅助条件转换的方法
        void dealWithCondition(const SmartExpr& node);

        bool hasFuncCall(const SmartExpr& node);

        /// removeSubExpr -- 移除子表达式
        void removeSubExpr(const std::string& subExpr);

        /// calcuExprStr -- 计算表达式Hash值
        std::string calcuExprStr(const SmartExpr& node);

        // 生成IR
        void visit(const std::shared_ptr<CompUnitNode>& node) override;
        void visit(const std::shared_ptr<VarDef>& node) override;
        void visit(const std::shared_ptr<InitializedNode>& node) override;
        void visit(const std::shared_ptr<FuncDef>& node) override;
        void visit(const std::shared_ptr<CommonFormalParaNode>& node) override;
        void visit(const std::shared_ptr<ArrayFormalParaNode>& node) override;
        void visit(const std::shared_ptr<CodeBlockNode>& node) override;
        void visit(const std::shared_ptr<AssignStmtNode>& node) override;
        void visit(const std::shared_ptr<IfStmtNode>& node) override;
        void visit(const std::shared_ptr<WhileStmtNode>& node) override;
        void visit(const std::shared_ptr<ExpressionStmtNode>& node) override;
        void visit(const std::shared_ptr<ContinueStmtNode>& node) override;
        void visit(const std::shared_ptr<BreakStmtNode>& node) override;
        void visit(const std::shared_ptr<ReturnStmtNode>& node) override;
        void visit(const std::shared_ptr<ExpressionNode>& node) override;
        void visit(const std::shared_ptr<UnaryExpNode>& node) override;
        void visit(const std::shared_ptr<FuncCallNode>& node) override;
        void visit(const std::shared_ptr<ArrayRefLValueNode>& node) override;
        void visit(const std::shared_ptr<ArrayVarDef>& node) override;
        void visit(const std::shared_ptr<CommonLValueNode>& node) override;
        void visit(const std::shared_ptr<BinaryOpExprNode>& node) override;
        void visit(const std::shared_ptr<FloatValueNode>& node) override;
        void visit(const std::shared_ptr<IntValueNode>& node) override;

    private:
        // 针对与函数内联的特殊化判断操作
        bool hasMustConvert(const SmartExpr& expr);


        bool isLogicOp(Operator op);
        void clearData();

        // 存储整个IR指令模块
        MODULE module;

        // 当前函数模块
        FUNC_DEFINE currentFunc;

        // 函数的记录
        std::map<std::string, FUNC_MSG_PTR> func_record;

        // 全局作用域
        SyScope* globalScope{nullptr};

        // 当前作用域
        SyScope* currentScope{nullptr};

        // IR构造器
        SyIRBuilder builder;

        // 变量V
        VALUE V;

        // 与变量处理有关的一个标志
        bool isLoad{true};
        bool isArrayLoad{true};

        //辅助数组初始化的数据结构
        int deep = 0;
        int count = 0;
        int total = 0;
        std::vector<int> current_array_dims;

        // 节省内存空间
        std::map<int, VALUE> int_map_record;
        std::map<float, VALUE> float_map_record;

        // 当前基本快
        BASIC_BLOCK currentBlock;

        // 初始化列表
        Type current_type{UNDEF};
        std::vector<VALUE> vecInitial{};
        //0表示可以忽略，1表示需要进行初始化
        std::vector<int> needInitial{};

        // BasicBlock标签自增下标的记录
        Record record;

        // Break和Continue语句相关
        std::vector<BASIC_BLOCK> beforeBB_stack{};
        std::vector<BASIC_BLOCK> nextBB_stack{};

        // 变量声明的指令
        std::vector<INSTRUCTION> var_alloca_list{};

        // 辅助 A && B和 A || B条件判断的字段
        std::map<std::string, unsigned> varRecordMap;
        std::set<std::string> globalVarRecordMap;

        std::vector<std::string> para_names;

        /// 用于回填操作
        std::map<SmartExpr , std::vector<std::shared_ptr<IR::InstThreeOp>>> trueMap;
        std::map<SmartExpr , std::vector<std::shared_ptr<IR::InstThreeOp>>> falseMap;
        std::map<std::shared_ptr<IR::InstThreeOp>, IR::BASIC_BLOCK> inst2BbMap{};

        /// 辅助公共子表达式消除的数值
        std::map<std::string, IR::VALUE> gvnMap;
        std::vector<std::map<std::string, IR::VALUE>> tempMapStack;
        bool isTemp{false};

        Scope* currentNumberScope{nullptr};
        int scopeNumber{0};

        /// const 变量的Load消除
        std::map<std::string, VALUE> constVarMap;

        /// value对应
        std::map<std::string, VALUE> varValueMap;

        /// 结果只受传入参数影响的函数集合
        std::set<std::string> impactByParrFuncSet;
    };
}

#endif //COMPILER_IR_GENERATOR_H
