/**
 * Author: Devotes
 * Date:   2022/5/28
 *
 *
 * FixLog:
 *
 */

#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include "../common.h"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <list>

class       ExpressionNode;
class       InitializedNode;
class       CodeBlockNode;
class       FormalParameter;
class       AssignStmtNode;
class       Node;

typedef     std::shared_ptr<Node>             SmartNode;
typedef     std::shared_ptr<ExpressionNode>   SmartExpr;
typedef     std::shared_ptr<InitializedNode>  InitialNode;
typedef     std::vector<SmartExpr>            ArrayDefSuffix;


#define EXT_NODE(V) : public Node
#define EXT_EXPR(V) : public ExpressionNode

/***********************************************
 * AstNode:
 *      parent -> 父节点
 *      type -> ast节点类型
 ***********************************************/
struct Node : public std::enable_shared_from_this<Node> {
public:
    typedef std::shared_ptr<Node> Parent;
    /// 构造函数
    explicit Node(const position& pos): pos(pos){}
    /// AST结点基类虚函数方法
   virtual NodeType type() = 0;
    virtual void toAstInfoString(std::stringstream &ss, int tab) = 0;
    virtual SmartNode deepCopy() = 0;
    /// 位置定义
    position        pos;
    Parent          parent{nullptr};
};

/***********************************************
 * 顶层编译单元节点。
 ***********************************************/
class CompUnitNode EXT_NODE(CompUnitNode) {
public:
    typedef std::vector<SmartNode> CompElems;
    /// 构造函数
    explicit CompUnitNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return COMP_UNIT; }
    void addElem(const SmartNode& elem) {
        elem->parent = shared_from_this();
        elems.push_back(elem); }
    /// 编译单元元素
    CompElems elems{};
};

/***********************************************
 * 变量定义节点。
 ***********************************************/
class VarDef EXT_NODE(VarDef) {
public:
    explicit VarDef(const position& pos, const std::string id): Node(pos),
                                                       id(id){}
    NodeType type() override { return VAR_DEF; }
    void toAstInfoString (std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void setInitialNode  (const SmartExpr& init);

    SmartExpr      initial{nullptr};        /* 初始化列表 */
    bool           is_const{false};         /* 是否是常量 */
    Type           var_type;                /* 变量类型 */
    std::string    id;                      /* 变量名 */
};

/***********************************************
 * 数组变量定义节点
 ***********************************************/
class ArrayVarDef EXT_NODE(ArrayVarDef){
public:
    typedef std::vector<SmartExpr> ArrayDimVec;
    ArrayVarDef(const position& pos, const std::string& id):Node(pos),
                                                            id(id){}
    NodeType type() override{ return ARRAY_VAR_DEF; }
    void   toAstInfoString  (std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void   addArrayDimExpr  (const SmartExpr& node);
    void   setInitialNode   (const InitialNode& init);

    Type                var_type;           /* 数组变量类型 */
    bool                is_const{false};    /* 标识是否是常量声明 */
    std::string         id;                 /* 数组名称 */
    ArrayDimVec         arrayDimVec;        /* 数组后缀 */
    InitialNode         initial{nullptr};   /* 初始化列表 */
};

/***********************************************
 * 初始化列表节点: InitializedNode
 ***********************************************/
class InitializedNode EXT_NODE(InitializedNode) {
public:
    typedef std::vector<InitialNode> InitExprList;
    explicit InitializedNode(const position& pos): Node(pos){}

    NodeType type() override { return INITIALIZED_NODE; }
    void toAstInfoString (std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void addInitialExp   (const InitialNode& initexp);
    void setExpr         (const SmartExpr& exp);

    SmartExpr           expr{nullptr};
    InitExprList        inits{};
};

/***********************************************
 * 函数定义AST节点: FuncDef
 ***********************************************/
struct CallChain;
class FuncCallNode;
class FuncDef EXT_NODE(FuncDef) {
public:
    typedef std::shared_ptr<FormalParameter> FormalPara;
    typedef std::vector<FormalPara>  Parameters;
    FuncDef(const position& pos, const std::string& func_name, Type ret_type): Node(pos), func_name(func_name), ret_type(ret_type){}

    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return FUNC_DEF; }
    void addParameter(const FormalPara& para);
    void setCodeBlock(const SmartNode& code_block);

    Type                    ret_type;                   /* 函数返回值类型 */
    std::string             func_name;                  /* 函数名 */
    Parameters              parameters{};               /* 参数列表 */
    SmartNode               block{nullptr};             /* 对应的语句块 */
    std::vector<CallChain>  callChains;                 /* 调用链，指向展开位置 */
    std::set<std::shared_ptr<FuncDef>> funcChains;      /* 函数链 */

    bool onlyHasSimpleStmt{false};          /* 是否只有一条简单的语句 */
    bool hasStdFuncCallIn{false};           /* 函数中是否有标准库函数的调用 */
    bool hasParaAssign{false};              /* 函数参数是否被赋值 */
    bool hasArrayPara{false};               /* 形参参数中是否有数组 */
    bool hasRet{true};                      /* 函数是否有返回值 */
    bool resultOnlyImpactByPara{false};     /* 是否只受传入参数影响 */
    bool hasUseGlobalVar{false};            /* 是否使用全局变量 */
    bool hasCallImpactStdFunc{false};       /* 是否调用有影响的标准库函数 */
};

struct CallChain{
    std::shared_ptr<FuncDef> funcDef{nullptr};
    std::shared_ptr<FuncCallNode> funcCall{nullptr};
};

/***********************************************
 * 形式参数AST节点: FormalParameter
 ***********************************************/
class FormalParameter: public Node{
public:
    FormalParameter(Type ty, const std::string& name, const position& pos) : Node(pos), ty(ty), name(name){}
    NodeType type() override = 0;
    SmartNode deepCopy() override = 0;
    void toAstInfoString(std::stringstream &ss, int tab) override = 0;
    std::string name;
    Type ty;
};

/***********************************************
 * 普通形参节点: CommonFormalParaNode
 * 传递方式以值传递
 ***********************************************/
class CommonFormalParaNode: public FormalParameter{
public:
    CommonFormalParaNode(const position& pos, Type type, const std::string& name): FormalParameter(type, name, pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override{ return COMMON_FORMAL_PARA; }
};

/***********************************************
 * 数组形参节点: ArrayFormalParaNode
 * 传递方式以指针传递或者引用传递
 ***********************************************/
class ArrayFormalParaNode: public FormalParameter{
public:
    ArrayFormalParaNode(const position& pos, Type type, const std::string& name): FormalParameter(type, name, pos) {}
    NodeType type() override{ return ARRAY_FORMAL_PARA; }
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void addArrayDimExpr(const SmartExpr& node);
    ArrayDefSuffix  arrayDims{};
};

/***********************************************
 * 语句块AST节点: CodeBlockNode
 ***********************************************/
class CodeBlockNode EXT_NODE(CodeBlockNode){
public:
    typedef std::vector<SmartNode> Stmts;
    explicit CodeBlockNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return CODE_BLOCK; }
    void addStmt(const SmartNode& stmt) { stmt->parent = shared_from_this(); stmts.push_back(stmt); }
    void copyStmts(const std::vector<SmartNode>& cstmts);
    void replaceStmt(const SmartNode& src, const SmartNode& now);
    void insertToStmt(const SmartNode& loc, const SmartNode& stmt);
    void removeStmt(const SmartNode& stmt);

    /// 语句
    Stmts               stmts{};
};

/***********************************************
 * 条件跳转语句AST节点: IfStmtNode
 ***********************************************/
class IfStmtNode EXT_NODE(IfStmtNode) {
public:
    IfStmtNode(const position& pos, const SmartExpr& expr): Node(pos),
                                                            l_or_expr(expr){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return IF_STATEMENT; }
    void setLocParent();
    void setCorrectStmt(const SmartNode& stmt) { stmt->parent = shared_from_this(); correct_stmt = stmt; }
    void setElseStmt(const SmartNode& stmt) { stmt->parent = shared_from_this(); else_stmt = stmt; }

    SmartExpr l_or_expr;
    SmartNode correct_stmt {nullptr};
    SmartNode else_stmt {nullptr};
};

/***********************************************
 * 循环语句节点: WhileStmtNode
 ***********************************************/
class WhileStmtNode EXT_NODE(WhileStmtNode){
public:
    WhileStmtNode(const position& pos, const SmartExpr& exp): Node(pos),
                                                              l_or_exp(exp){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void setLocParent();
    void setStmt(const SmartNode& s) { s->parent = shared_from_this(); stmt = s; }
    NodeType type() override { return WHILE_STATEMENT; }

    SmartExpr l_or_exp;
    SmartNode stmt{nullptr};
};

/***********************************************
 * Break语句AST节点: BreakStmtNode
 ***********************************************/
class BreakStmtNode EXT_NODE(BreakStmtNode) {
public:
    explicit BreakStmtNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return BREAK_STATEMENT; }
};

/***********************************************
 * Continue语句AST节点: ContinueStmtNode
 ***********************************************/
class ContinueStmtNode EXT_NODE(ContinueStmtNode) {
public:
    explicit ContinueStmtNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return CONTINUE_STATEMENT; }
};

/***********************************************
 * Return语句AST节点: ReturnStmtNode
 ***********************************************/
class ReturnStmtNode EXT_NODE(ReturnStmtNode) {
public:
    explicit ReturnStmtNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return RETURN_STATEMENT; }
    void setExp(const SmartExpr& exp);
    SmartExpr expr{nullptr};
};

/***********************************************
 * 表达式语句AST节点: ExpressionStmtNode
 ***********************************************/
class ExpressionStmtNode EXT_NODE(ExpressionStmtNode) {
public:
    explicit ExpressionStmtNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return EXPRESSION_STATEMENT; }
    void setExpr(const SmartExpr& exp);
    SmartExpr expr{nullptr};
};

/***********************************************
 * 赋值语句AST节点: AssignStmtNode
 ***********************************************/
class AssignStmtNode EXT_NODE(AssignStmtNode) {
public:
    explicit AssignStmtNode(const position& pos);
    NodeType type() override{ return ASSIGN_STATEMENT; }
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;

    void setLhs(const SmartExpr& expr);
    void setRhs(const SmartExpr& expr);

    SmartExpr lhs{nullptr};
    SmartExpr rhs{nullptr};
};

/***********************************************
 * 表达式AST节点: ExpressionNode
 ***********************************************/
class ExpressionNode : public Node{
public:
    explicit ExpressionNode(const position& pos): Node(pos){}
    void toAstInfoString(std::stringstream &ss, int tab) override = 0;
    SmartNode deepCopy() override = 0;
    NodeType type() override = 0;
    virtual ~ExpressionNode() = default;
    Type expr_type{UNDEF};
};

/***********************************************
 * 普通左值变量AST节点: CommonLvalueNode
 ***********************************************/
class CommonLValueNode EXT_EXPR(CommonLValueNode) {
public:
    CommonLValueNode(const position& pos, const std::string& id): ExpressionNode(pos),
                                                                  id(id){}
    NodeType type() override { return COMMON_LVALUE; }
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    std::string id;
    bool mustConvert{false};
};

/***********************************************
 * 数组引用变量AST节点: ArrayRefLvalueNode
 ***********************************************/
class ArrayRefLValueNode EXT_EXPR(ArrayRefLValueNode) {
public:
    typedef std::vector<SmartExpr> ArrayIndex;
    ArrayRefLValueNode(const position& pos, const std::string& id): ExpressionNode(pos), id(id){}
    NodeType type() override{ return ARRAY_REF_LVALUE; }
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void addArrayRef (const SmartExpr& node);

    std::string id;
    ArrayIndex  arrayRef;
    bool mustConvert{false};
};

/***********************************************
 * 整型字面值的AST节点: IntValueNode
 ***********************************+************/
class IntValueNode EXT_EXPR(IntValueNode) {
public:
    IntValueNode(const position& pos, int v): ExpressionNode(pos), v(v){}
    NodeType type() override{ return INT_ITER_NODE; }
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    int v;
};

/***********************************************
 * 一元运算符的AST节点: FloatValueNode
 ***********************************************/
class UnaryExpNode EXT_EXPR(UnaryExpNode) {
public:
    UnaryExpNode(const position& pos, Operator op): ExpressionNode(pos), unary_op(op){}
    void setExprNode(const SmartExpr& n);
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override{ return UNART_NODE; }

    Operator unary_op;
    SmartExpr node{nullptr};
};


/***********************************************
 * 浮点型字面值的AST节点: FloatValueNode
 ***********************************************/
class FloatValueNode EXT_EXPR(FloatValueNode) {
public:
    FloatValueNode(const position& pos, float v): ExpressionNode(pos), v(v){}
    NodeType type() override{ return FLOAT_ITER_NODE; }
    void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;

    float v;
};

/***********************************************
 * 函数调用的AST节点: FuncCallNode
 ***********************************************/
class FuncCallNode EXT_EXPR(FuncCallNode) {
public:
    typedef std::vector<SmartExpr> FuncPParams;
    FuncCallNode(const position& pos, const std::string& func_name): ExpressionNode(pos), func_name(func_name){}
    void  toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    NodeType type() override { return FUNC_CALL_NODE; }
    void addParam(const SmartExpr& param) { param->parent = shared_from_this(); pparams.push_back(param); }

    std::string         func_name;
    FuncPParams         pparams{};
    bool                mustConvert{false};             /* 是否需要类型转换 */
    std::vector<Type>   realTypes;                      /* 实际的类型 */
};

/***********************************************
 * 二元运算符表达式的AST节点: BinaryOpExprNode
 ***********************************************/
class BinaryOpExprNode EXT_EXPR(BinaryOpExprNode) {
public:
    BinaryOpExprNode(const position& pos, Operator oper): ExpressionNode(pos), oper(oper){}
    NodeType type() override{ return BINARY_OP_NODE; }
        void toAstInfoString(std::stringstream &ss, int tab) override;
    SmartNode deepCopy() override;
    void setLhs(const SmartExpr& lhs);
    void setRhs(const SmartExpr& rhs);

    Operator oper;
    SmartExpr lhs{nullptr};
    SmartExpr rhs{nullptr};
};

#endif //COMPILER_AST_H
