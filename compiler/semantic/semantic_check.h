/*
 * Author:   Devotes
 * Date:     2022/5/31
 *
 * FixLog:
 *
 */

#ifndef COMPILER_SEMANTIC_CHECK_H
#define COMPILER_SEMANTIC_CHECK_H

#include "../common.h"
#include "../ast_visitor.h"
#include <vector>
#include "symbol.h"

/************************************************************************************
 * <语义检查模块>:
 * 1. 检查IF和WHILE语句中的条件是否为布尔类型
 * 2. 对于赋值和参数传递，语义分析器必须检查类型是否兼容
 * 3. 对于每一个声明，语义分析器必须检查所使用的名称是否已经声明过
 * 4. 对于每一个表达式，语义分析器必须计算得到的类型，还需要判断表达式是否是常量
 * 5. 对于表达式语句中每次出现的名称，语义分析器必须检查名称是否已经声明，并且期望的用途是否符合声明
 *************************************************************************************/

class SemanticChecker: AstVisitor{
public:
    /* 语义检查 */
    SemanticChecker();
    SemanticChecker(std::ostream& out);

    /* 重置语义分析器 */
    void resetChecker();

    /* 输出错误信息，如果没有，则直接返回 */
    void outErrorMessage();

    /* 是否有语义错误 */
    bool hasCheckError()                const { return has_error; }

    /* 1.检查变量函数是否重复定义 */
    bool checkReDefine(const std::shared_ptr<VarDef>&       node);
    bool checkReDefine(const std::shared_ptr<ArrayVarDef>&  node);
    bool checkReDefine(const std::shared_ptr<FuncDef>&      node);

    /* 2.检查是否使用未定义的变量 */
    bool checkVarDecl(const std::shared_ptr<ArrayRefLValueNode>& node);
    bool checkVarDecl(const std::shared_ptr<CommonLValueNode>&   node);

    /* 3.检查数组维度是否越界 */
    bool checkArrRefOutOfRange(const std::shared_ptr<ArrayRefLValueNode>& node);

    /* 4.函数匹配检查 */
    bool checkFuncMatch(const std::shared_ptr<FuncCallNode>& node);

    /* 5.数组初始化列表检查 */
    bool checkInitial(const std::shared_ptr<InitializedNode>& node);

    /* 9. 语义检查辅助方法 */
    bool checkCanEmit(const std::shared_ptr<ExpressionNode>& node, double &v);
    Type checkExprType(const std::shared_ptr<ExpressionNode>& node);
    void visitStmtNode(const std::shared_ptr<Node>& node);
    int condAlwaysTrueOrFalse(const std::shared_ptr<ExpressionNode>& node);

    /* 打印符号表 */
    void printSymbol(){ sym_table_manager.printSymbolTable(); }

    /* visitor */
    virtual void visit(const std::shared_ptr<CompUnitNode>&         node)   override;
    virtual void visit(const std::shared_ptr<VarDef>&               node)   override;
    virtual void visit(const std::shared_ptr<ArrayVarDef>&          node)   override;
    virtual void visit(const std::shared_ptr<InitializedNode>&      node)   override;
    virtual void visit(const std::shared_ptr<FuncDef>&              node)   override;
    virtual void visit(const std::shared_ptr<CommonFormalParaNode>& node)   override;
    virtual void visit(const std::shared_ptr<ArrayFormalParaNode>&  node)   override;
    virtual void visit(const std::shared_ptr<CodeBlockNode>&        node)   override;
    virtual void visit(const std::shared_ptr<IfStmtNode>&           node)   override;
    virtual void visit(const std::shared_ptr<WhileStmtNode>&        node)   override;
    virtual void visit(const std::shared_ptr<BreakStmtNode>&        node)   override;
    virtual void visit(const std::shared_ptr<ContinueStmtNode>&     node)   override;
    virtual void visit(const std::shared_ptr<ReturnStmtNode>&       node)   override;
    virtual void visit(const std::shared_ptr<ExpressionStmtNode>&   node)   override;
    virtual void visit(const std::shared_ptr<AssignStmtNode>&       node)   override;
    virtual void visit(const std::shared_ptr<ExpressionNode>&       node)   override;
    virtual void visit(const std::shared_ptr<CommonLValueNode>&     node)   override;
    virtual void visit(const std::shared_ptr<ArrayRefLValueNode>&   node)   override;
    virtual void visit(const std::shared_ptr<IntValueNode>&         node)   override;
    virtual void visit(const std::shared_ptr<FloatValueNode>&       node)   override;
    virtual void visit(const std::shared_ptr<FuncCallNode>&         node)   override;
    virtual void visit(const std::shared_ptr<UnaryExpNode>&         node)   override;
    virtual void visit(const std::shared_ptr<BinaryOpExprNode>&     node)   override;
private:

    SymbolTableManager          sym_table_manager;              // 符号表管理器
    std::ostream&               out;                            // 输出流
    bool                        has_error{false};               // 语义检查标志
    std::vector<std::string>    messages;                       // end message
    Type                        arrayType{UNDEF};               // 当前数组类型用于辅助全局变量的初始化
    bool                        isCorrect;                      // 辅助优化的字段
};


#endif //COMPILER_SEMANTIC_CHECK_H
