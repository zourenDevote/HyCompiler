/**
 * Author:  Devotes
 * Date:    20220617
 *
 * FixLog:
 *
 */

#include "ir_generator.h"
#include <vector>
#include <queue>
#include <stack>

using namespace std;
namespace IR{

    inline IRType convertType(Type ty, bool isArray){
        if(ty == VOID)
            return IRType::IRVoid32;
        else if(isArray){
            return ty==INT?IRType::IRIntPtr32:IRType::IRFloatPtr32;
        }else{
            return ty==INT?IRType::IRInt32:IRType::IRFloat32;
        }
    }

    ArrayVarSymbol::ArrayVarSymbol(const std::string &name, IRType ty, const std::vector<int>& dims, VALUE V) : name(name), ty(ty), dims(dims), V(V){}

    /* 作用域 */
    void SyScope::addSymbol(const std::string& name, const VALUE& pointer) {
        if(symbol_table.find(name) == symbol_table.end()){
            symbol_table.insert({name, pointer});
        }
    }

    /* 获取符号 */
    VALUE SyScope::getSymbol(const std::string &name) {
        auto p = this;
        while(p){
            if(p->symbol_table.find(name) != p->symbol_table.end())
                return p->symbol_table[name];
            p = p->parent;
        }
        return nullptr;
    }

    /* 添加数组符号 */
    void SyScope::addArraySymbol(ArrayVarSymbol* arr) {
        if(array_symbol_table.find(arr->name) == array_symbol_table.end()){
            array_symbol_table.insert({arr->name, arr});
        }
    }

    /* 获取数组符号 */
    ArrayVarSymbol* SyScope::getArraySymbol(const std::string &name) {
        auto p = this;
        while (p){
            if(p->array_symbol_table.find(name) != p->array_symbol_table.end())
                return p->array_symbol_table[name];
            p = p->parent;
        }
        return nullptr;
    }

    /* 构造函数 */
    IRGenerate::IRGenerate(const MODULE& M) : module(M), currentFunc(nullptr), builder(){
        globalScope = new SyScope();
        currentScope = globalScope;
        currentNumberScope = new Scope(++scopeNumber);

        module->func_defs.clear();
        module->global_vars.clear();
        module->func_decls.clear();


        FUNC_MSG_PTR p = make_shared<FUNC_MSG>();
        p->first = IRType::IRInt32;
        p->second = {};

        // getint
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("getint", p));
        func_record.insert({"getint", p});

        // getch
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("getch", p));
        func_record.insert({"getch", p});

        FUNC_MSG_PTR p1 = make_shared<FUNC_MSG>();
        p1->first = IRType::IRFloat32;
        p1->second = {};

        // getfloat
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("getfloat", p1));
        func_record.insert({"getfloat", p1});

        // getarray
        FUNC_MSG_PTR p2 = make_shared<FUNC_MSG>();
        p2->first = IRType::IRInt32;
        p2->second = {IRType::IRIntPtr32};
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("getarray", p2));
        func_record.insert({"getarray", p2});

        // getfarray
        FUNC_MSG_PTR p3 = make_shared<FUNC_MSG>();
        p3->first = IRType::IRInt32;
        p3->second = {IRType::IRFloatPtr32};
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("getfarray", p3));
        func_record.insert({"getfarray", p3});

        // putfloat
        FUNC_MSG_PTR p4 = make_shared<FUNC_MSG>();
        p4->first = IRType::IRVoid32;
        p4->second = {IRType::IRFloat32};
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("putfloat", p4));
        func_record.insert({"putfloat",p4});

        // putarray
        FUNC_MSG_PTR p5 = make_shared<FUNC_MSG>();
        p5->first = IRType::IRVoid32;
        p5->second = {IRType::IRInt32, IRType::IRIntPtr32};
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("putarray", p5));
        func_record.insert({"putarray", p5});

        // putfarray
        FUNC_MSG_PTR p6 = make_shared<FUNC_MSG>();
        p6->first = IRType::IRVoid32;
        p6->second = {IRType::IRInt32, IRType::IRFloatPtr32};
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("putfarray", p6));
        func_record.insert({"putfarray", p6});

        // putch
        FUNC_MSG_PTR p7 = make_shared<FUNC_MSG>();
        p7->first = IRType::IRVoid32;
        p7->second = {IRType::IRInt32};
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("putch", p7));
        func_record.insert({"putch", p7});

        // putint
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("putint", p7));
        func_record.insert({"putint", p7});

        // _sysy_starttime
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("_sysy_starttime", p7));
        func_record.insert({"_sysy_starttime", p7});

        // _sysy_stoptime
        module->func_decls.push_back(SyIRBuilder::createFuncDecl("_sysy_stoptime", p7));
        func_record.insert({"_sysy_stoptime", p7});

    }

    /* 进入作用域 */
    void IRGenerate::entryScope() {
        SyScope* s = currentScope;
        currentScope = new SyScope(s->level + 1);
        currentScope->parent = s;

        auto numberScope = new Scope(++scopeNumber);
        numberScope->parent = currentNumberScope;
        currentNumberScope = numberScope;
    }

    /* 离开作用域 */
    void IRGenerate::leaveScope() {
        SyScope* s = currentScope;
        currentScope = currentScope->parent;
        delete s;

        auto numberScope = currentNumberScope;
        currentNumberScope = currentNumberScope->parent;
        delete numberScope;
    }

    /* 辅助IR生成的部分 */
    void IRGenerate::visitStmt(const std::shared_ptr<Node>& node) {
        switch(node->type()){
            case IF_STATEMENT:
                visit(dynamic_pointer_cast<IfStmtNode>(node));
                break;
            case EXPRESSION_STATEMENT:
                visit(dynamic_pointer_cast<ExpressionStmtNode>(node));
                break;
            case WHILE_STATEMENT:
                visit(dynamic_pointer_cast<WhileStmtNode>(node));
                break;
            case ASSIGN_STATEMENT:
                visit(dynamic_pointer_cast<AssignStmtNode>(node));
                break;
            case CONTINUE_STATEMENT:
                visit(dynamic_pointer_cast<ContinueStmtNode>(node));
                break;
            case BREAK_STATEMENT:
                visit(dynamic_pointer_cast<BreakStmtNode>(node));
                break;
            case RETURN_STATEMENT:
                visit(dynamic_pointer_cast<ReturnStmtNode>(node));
                break;
            case CODE_BLOCK:
                entryScope();
                visit(dynamic_pointer_cast<CodeBlockNode>(node));
                leaveScope();
                break;
            default:
                break;
        }
    }

    bool IRGenerate::hasFuncCall(const std::shared_ptr<ExpressionNode>& node) {
        if(node->type() == FUNC_CALL_NODE)
            return true;
        else if(node->type() == BINARY_OP_NODE){
            auto binary = dynamic_pointer_cast<BinaryOpExprNode>(node);
            return hasFuncCall(binary->rhs) || hasFuncCall(binary->lhs);
        }
        return false;
    }

    void IRGenerate::removeSubExpr(const std::string &subExpr) {
        auto tempMap = gvnMap;
        for(const auto& value : tempMap){
            int v = value.first.find(subExpr);
            if(v >= 0)
                gvnMap.erase(value.first);
        }
    }

    std::string IRGenerate::calcuExprStr(const std::shared_ptr<ExpressionNode>& node) {

        switch (node->type()) {
            case NodeType::INT_ITER_NODE:{
                int v = (dynamic_pointer_cast<IntValueNode>(node))->v;
                return std::to_string(v);
            }
            case NodeType::FLOAT_ITER_NODE:{
                float v = (dynamic_pointer_cast<FloatValueNode>(node))->v;
                return std::to_string(v);
            }
            case NodeType::UNART_NODE:{
                auto unary = dynamic_pointer_cast<UnaryExpNode>(node);
                auto res = calcuExprStr(unary->node);
                if(res != "")
                    return toOper(unary->unary_op) + res;
                break;
            }
            case NodeType::BINARY_OP_NODE:{
                auto binary = dynamic_pointer_cast<BinaryOpExprNode>(node);
                auto res1 = calcuExprStr(binary->lhs);
                auto res2 = calcuExprStr(binary->rhs);
                if(res1 != "" && res2 != "")
                    return res1 + toOper(binary->oper) + res2;
                break;
            }
            case NodeType::COMMON_LVALUE:{
                auto clv = dynamic_pointer_cast<CommonLValueNode>(node);
                return currentNumberScope->getSymbolName(clv->id);
            }
            case  NodeType::ARRAY_REF_LVALUE:{
                auto arf = dynamic_pointer_cast<ArrayRefLValueNode>(node);
                string arrayRef = currentNumberScope->getSymbolName(arf->id);
                string aid = "";
                for(const auto& aindex : arf->arrayRef){
                    aid = calcuExprStr(aindex);
                    if(aid == "")
                        return "";
                    arrayRef.append("[" + aid + "]");
                }
                return arrayRef;
            }
            case NodeType::FUNC_CALL_NODE:{
                auto funcCall = dynamic_pointer_cast<FuncCallNode>(node);
                if(impactByParrFuncSet.find(funcCall->func_name) != impactByParrFuncSet.end()){
                    string funcName = funcCall->func_name;
                    funcName.append("(");
                    string res;
                    int i = 0;
                    int len = funcCall->pparams.size() - 1;
                    for(const auto& para : funcCall->pparams){
                        res = calcuExprStr(para);
                        if(res == "")
                            return "";
                        if(i < len){
                            funcName.append(res + ",");
                        }else{
                            funcName.append(res);
                        }
                        i++;
                    }
                    funcName.append(")");
                    return funcName;
                }else{
                    return "";
                }
                break;
            }
            default:
                break;
        }
        return "";
    }

    VALUE IRGenerate::getIntV(int v) {
        if(int_map_record.find(v) == int_map_record.end())
            int_map_record[v] = SyIRBuilder::createIConstant(v);
        return int_map_record[v];
    }

    VALUE IRGenerate::getFloatV(float v) {
        if(float_map_record.find(v) == float_map_record.end())
            float_map_record[v] = SyIRBuilder::createFConstant(v);
        return float_map_record[v];
    }

    void IRGenerate::calcuArrRef(const std::vector<SmartExpr>& exprs, const std::vector<int> &dims) {
        VALUE V1;
        VALUE V2;
        auto it = dims.begin();
        auto it1 = dims.begin();
        for(auto& expr: exprs){
            if(expr->type() == ARRAY_REF_LVALUE || expr->type() == FUNC_CALL_NODE || expr->type() == BINARY_OP_NODE || expr->type() == UNART_NODE){
                auto res = calcuExprStr(expr);
                if(res != ""){
                    if(gvnMap.find(res) != gvnMap.end()){
                        V = gvnMap[res];
                    }else{
                        visit(expr);
                        gvnMap.insert({res, V});
                    }
                }else{
                    visit(expr);
                }
            }else{
                visit(expr);
            }

            if(V->type() == OpType::IRImmOp){
                auto immV = dynamic_pointer_cast<ImmediateOp>(V);

                if(immV->ty == IRType::IRFloat32){
                    immV->ty = IRType::IRInt32;
                    immV->v = (int)immV->v1;
                }

                immV->v = immV->v * (*it);
                V = immV;
            }
            else{
                V1 = V;
                // 如果索引是浮点类型，可能需要一层转型指令
                if(V1->ty == IRType::IRFloat32){
                    // 转型
                    V = builder.createTempIReg();
                    currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
                    V1 = V;
                }

                V = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIMul(V1, SyIRBuilder::createIConstant(*it),V));
            }

            if(it != it1){
                if(V2->type() == OpType::IRImmOp && V->type() == OpType::IRImmOp){
                    int v = dynamic_pointer_cast<ImmediateOp>(V2)->v + dynamic_pointer_cast<ImmediateOp>(V)->v;
                    V = SyIRBuilder::createIConstant(v);
                }else{
                    V1 = V;
                    V = builder.createTempIReg();
                    currentBlock->addInst(SyIRBuilder::createIAdd(V2, V1, V));
                }
            }
            V2 = V;
            it++;
        }
        V = V2;
    }

    unsigned int IRGenerate::getOccurTime(const std::string &id) {
        if(varRecordMap.find(id) != varRecordMap.end()){
            varRecordMap[id]++;
        }else if(globalVarRecordMap.find(id) != globalVarRecordMap.end()){
            varRecordMap[id] = 1;
        }else
            varRecordMap[id] = 0;
        return varRecordMap[id];
    }

    bool IRGenerate::isLogicOp(Operator op) {
        static const set<Operator> nlogicOpSet = {ADD_OP, SUB_OP, MUL_OP, DIV_OP, MOD_OP};
        if(nlogicOpSet.find(op) == nlogicOpSet.end())
            return true;
        return false;
    }

    /// 单独的针对条件跳转的方法
    void IRGenerate::dealWithCondition(const std::shared_ptr<ExpressionNode>& node) {
        VALUE V1;
        bool dealFlag = true;
        bool isJudge = true;
        switch (node->type()) {
            case COMMON_LVALUE:{
                visit(dynamic_pointer_cast<CommonLValueNode>(node));
                break;
            }
            case ARRAY_REF_LVALUE:{
                visit(dynamic_pointer_cast<ArrayRefLValueNode>(node));
                break;
            }
            case INT_ITER_NODE:{
                visit(dynamic_pointer_cast<IntValueNode>(node));
                dealFlag = false;
                break;
            }
            case FLOAT_ITER_NODE:{
                visit(dynamic_pointer_cast<FloatValueNode>(node));
                break;
            }
            case FUNC_CALL_NODE:{
                visit(dynamic_pointer_cast<FuncCallNode>(node));
                break;
            }
            case UNART_NODE:{
                visit(dynamic_pointer_cast<UnaryExpNode>(node));
                if((dynamic_pointer_cast<UnaryExpNode>(node))->unary_op == NOT)
                    dealFlag = false;
                break;
            }
            case BINARY_OP_NODE:{
                auto binaryOp = dynamic_pointer_cast<BinaryOpExprNode>(node);
                if(binaryOp->oper == AND || binaryOp->oper == OR){
                    dealWithCondition(binaryOp->lhs);
                    dealWithCondition(binaryOp->rhs);
                }
                if(binaryOp->oper == AND){
                    isJudge = false;
                    auto b = trueMap[binaryOp->rhs][0];
                    auto k = trueMap[binaryOp->lhs];
                    auto label = SyIRBuilder::createLabel(inst2BbMap[b]->getBBName(), inst2BbMap[b]);
                    for(auto& jump : k){
                        jump->replaceOp2(label);
                        inst2BbMap[jump]->addNextBB(inst2BbMap[b]);
                        inst2BbMap[b]->addPrevBB(inst2BbMap[jump]);
                    }
                    auto temp = falseMap[binaryOp->lhs];
                    temp.insert(temp.end(), falseMap[binaryOp->rhs].begin(), falseMap[binaryOp->rhs].end());
                    trueMap[node] = trueMap[binaryOp->rhs];
                    falseMap[node] = temp;
                }else if(binaryOp->oper == OR){
                    isJudge = false;
                    auto b = falseMap[binaryOp->rhs][0];
                    auto k = falseMap[binaryOp->lhs];
                    auto label = SyIRBuilder::createLabel(inst2BbMap[b]->getBBName(), inst2BbMap[b]);
                    for(auto& jump : k){
                        jump->replaceOp3(label);
                        inst2BbMap[jump]->addNextBB(inst2BbMap[b]);
                        inst2BbMap[b]->addPrevBB(inst2BbMap[jump]);
                    }
                    auto temp = trueMap[binaryOp->lhs];
                    temp.insert(temp.end(), trueMap[binaryOp->rhs].begin(), trueMap[binaryOp->rhs].end());
                    falseMap[node] = falseMap[binaryOp->rhs];
                    trueMap[node] = temp;
                }else
                    visit(binaryOp);

                if(isLogicOp(binaryOp->oper))
                    dealFlag = false;
                break;
            }
            default:
                break;
        }


        if(dealFlag){
            V1 = V;
            V = builder.createTempIReg();
            if(V1->ty == IRType::IRFloat32)
                currentBlock->addInst(SyIRBuilder::createINEqual(V1, SyIRBuilder::createFConstant(0.0), V));
            else
                currentBlock->addInst(SyIRBuilder::createINEqual(V1, SyIRBuilder::createIConstant(0), V));

        }

        if(V->ty == IRType::IRFloat32){
            V1 = V;
            V = builder.createTempIReg();
            currentBlock->addInst(SyIRBuilder::createIFtoi(V1,V));
        }

        if(isTemp){
            tempMapStack.push_back(gvnMap);
            isTemp = false;
        }

        if(isJudge){
            V->setCmpValue();
            auto emptyJump = SyIRBuilder::createIEmptyCmp(V);
            currentBlock->addInst(emptyJump);
            inst2BbMap.insert({emptyJump, currentBlock});

            currentBlock = SyIRBuilder::createBB("cond" + to_string(++record.condidx));
            clearData();
            currentFunc->addBB(currentBlock);
            trueMap.insert({node, {emptyJump}});
            falseMap.insert({node, {emptyJump}});
        }

    }

    bool IRGenerate::hasMustConvert(const SmartExpr &expr) {
        switch (expr->type()) {
            case FUNC_CALL_NODE:{
                return dynamic_pointer_cast<FuncCallNode>(expr)->mustConvert;
            }
            case BINARY_OP_NODE:{
                auto binary = dynamic_pointer_cast<BinaryOpExprNode>(expr);
                return hasMustConvert(binary->lhs) || hasMustConvert(binary->rhs);
            }
            case UNART_NODE:{
                auto unary = dynamic_pointer_cast<UnaryExpNode>(expr);
                return hasMustConvert(unary->node);
            }
            case COMMON_LVALUE:{
                return dynamic_pointer_cast<CommonLValueNode>(expr)->mustConvert;
            }
            case ARRAY_REF_LVALUE:{
                return dynamic_pointer_cast<ArrayRefLValueNode>(expr)->mustConvert;
            }
            default:
                return false;
        }
    }

    /* IR生成的部分 */
    void IRGenerate::visit(const std::shared_ptr<CompUnitNode>& node) {
        for(auto& elem: node->elems){
            switch (elem->type()) {
                case VAR_DEF: visit(dynamic_pointer_cast<VarDef>(elem)); break;
                case FUNC_DEF: visit(dynamic_pointer_cast<FuncDef>(elem)); break;
                case ARRAY_VAR_DEF: visit(dynamic_pointer_cast<ArrayVarDef>(elem)); break;
                default: break;
            }
        }
    }

    void IRGenerate::clearData() {
      gvnMap.clear();
      varValueMap.clear();
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<VarDef>& node) {
        currentNumberScope->addSymbol(node->id);
        if(currentScope == globalScope){
            GLOBAL_VAR_DEF global;
            globalVarRecordMap.insert(node->id);
            if(node->var_type == FLOAT){
                float v = dynamic_pointer_cast<FloatValueNode>(node->initial)->v;
                global = SyIRBuilder::createFGlobalVariable(node->id, v);
                currentScope->addSymbol(node->id, SyIRBuilder::createFPE(node->id));
                if(node->is_const){
                  VALUE V1 = SyIRBuilder::createFConstant(v);
                  constVarMap.insert({node->id, V1});
                }
            }
            else{
                int v = dynamic_pointer_cast<IntValueNode>(node->initial)->v;
                global = SyIRBuilder::createIGlobalVariable(node->id, v);
                currentScope->addSymbol(node->id, SyIRBuilder::createIPE(node->id));
                if(node->is_const){
                  VALUE V1 = SyIRBuilder::createIConstant(v);
                  constVarMap.insert({node->id, V1});
                }
            }
            module->global_vars.push_back(global);
        }else{
            unsigned i = getOccurTime(node->id);
            string id = node->id;
            if(i != 0){
                id.append(to_string(i));
            }
            // @fixme 如果这种设计有问题的话，到时候再来改动
            if(node->var_type == FLOAT){
                V = SyIRBuilder::createFPE(id);
                var_alloca_list.push_back(SyIRBuilder::createFAlloca(V, currentBlock));
            }else{
                V = SyIRBuilder::createIPE(id);
                var_alloca_list.push_back(SyIRBuilder::createIAlloca(V, currentBlock));
            }
            VALUE V1 = V;
            if(node->initial){
                VALUE V2 = V;
                visit(node->initial);
                if(node->var_type == FLOAT && V->ty == IRType::IRInt32){
                    VALUE V3 = V;
                    V = builder.createTempFReg();
                    currentBlock->addInst(SyIRBuilder::createIItof(V3, V));
                }else if(node->var_type == INT && V->ty == IRType::IRFloat32){
                    VALUE V3 = V;
                    V = builder.createTempIReg();
                    currentBlock->addInst(SyIRBuilder::createIFtoi(V3, V));
                }
                if(node->is_const)
                    constVarMap.insert({id, V});
                currentBlock->addInst(SyIRBuilder::createIStore(V, V2));
            }
            currentScope->addSymbol(node->id, V1);
        }
    }

    /* @fixme 这个数组初始化的方式可能需要改动，暂时先这样处理 */
    void IRGenerate::visit(const std::shared_ptr<ArrayVarDef>& node) {

        currentNumberScope->addSymbol(node->id);
        vector<int> dims{};
        total = 1;
        auto it = node->arrayDimVec.rbegin();
        auto end = node->arrayDimVec.rend();

        // 求取元素个数
        for(auto& dim: node->arrayDimVec)
            total *= (dynamic_pointer_cast<IntValueNode>(dim))->v;

        // 求取每一维度的位宽
        dims.push_back(4);
        for(;(it+1) != end; it++){
            dims.insert(dims.begin(), dims.front() * (dynamic_pointer_cast<IntValueNode>(*it))->v);
        }

        //计算初始化节点所需
        current_array_dims = dims;
        for(auto &n:current_array_dims){
            n/=4;
        }
        current_type = node->var_type;

        bool isInit = false;
        vecInitial.clear();
        needInitial.clear();
        count=0;
        if (node->initial) {
            isInit = true;
            visit(node->initial);
        }

        if (currentScope == globalScope) {
            GLOBAL_ARRAY_DEF global;
            globalVarRecordMap.insert(node->id);
            if(node->var_type == FLOAT){
                V = SyIRBuilder::createFPE(node->id);
                currentScope->addArraySymbol(new ArrayVarSymbol(node->id, IRType::IRFloat32, dims, V));
                std::vector<float> inits;
                if(node->initial){
                    for(auto& v: vecInitial)
                        inits.push_back(((ImmediateOp*)v.get())->v1);
                }else{
                    for(auto& v: vecInitial)
                        inits.push_back(0.0);
                }
                global = SyIRBuilder::createFGlobalArray(node->id, total*4, inits,needInitial);

            }else{
                V = SyIRBuilder::createIPE(node->id);
                currentScope->addArraySymbol(new ArrayVarSymbol(node->id, IRType::IRInt32, dims, V));
                std::vector<int> inits;
                if(node->initial){
                    for(auto& v: vecInitial)
                        inits.push_back(((ImmediateOp*)v.get())->v);
                }else{
                    for(auto& v: vecInitial)
                        inits.push_back(0);
                }
                global = SyIRBuilder::createIGlobalArray(node->id, total*4, inits,needInitial);
            }
            module->addGlobalArrayDef(global);
        } else {

            unsigned i = getOccurTime(node->id);
            string id = node->id;
            if(i != 0){
                id.append(to_string(i));
            }

            if(node->var_type == FLOAT){
                V = SyIRBuilder::createFPE(id);
                ArrayVarSymbol* symbol = new ArrayVarSymbol(node->id, IRType::IRFloat32, dims, V);
                currentScope->addArraySymbol(symbol);
                var_alloca_list.push_back(SyIRBuilder::createFAlloca(V, currentBlock, total*4, true));
                if(node->initial){
                    if(node->initial->expr == nullptr && node->initial->inits.empty()){
                      currentBlock->addInst(SyIRBuilder::createIMemSet(V, dynamic_pointer_cast<IR::Reg>(V)->getRegName(), total));
                    }else{
                        int i = 0;
//                      int index = 0;
                        VALUE V1;
                        for(auto&v: vecInitial){
//                          if(needInitial[index++]){
                            V1 = SyIRBuilder::createIConstant(i);
                            V = builder.createTempFPE();
                            currentBlock->addInst(SyIRBuilder::createIGetPtr(symbol->V, V1, V));
                            currentBlock->addInst(SyIRBuilder::createIStore(v, V));
//                          }
                            i = i+4;
                        }
                    }
                }
            }else{
                V = SyIRBuilder::createIPE(id);
                ArrayVarSymbol* symbol = new ArrayVarSymbol(node->id, IRType::IRInt32, dims, V);
                currentScope->addArraySymbol(symbol);
                var_alloca_list.push_back(SyIRBuilder::createIAlloca(V, currentBlock, total*4, true));
                if(node->initial){
                    if(node->initial->expr == nullptr && node->initial->inits.empty()){
                        currentBlock->addInst(SyIRBuilder::createIMemSet(V, dynamic_pointer_cast<IR::Reg>(V)->getRegName(), total));
                    }else{
                        int i = 0;
  //                      int index = 0;
                        VALUE V1;
                        for(auto&v: vecInitial){
  //                          if(needInitial[index++]){
                          V1 = SyIRBuilder::createIConstant(i);
                          V = builder.createTempIPE();
                          currentBlock->addInst(SyIRBuilder::createIGetPtr(symbol->V, V1, V));
                          currentBlock->addInst(SyIRBuilder::createIStore(v, V));
  //                          }
                          i = i+4;
                        }
                    }
                }
            }
        }
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<InitializedNode>& node) {
        if (node->expr) {//若是表达式则直接求值
            visit(node->expr);
            if(current_type == FLOAT && V->ty == IRType::IRInt32){
                VALUE V1 = V;
                V = builder.createTempFReg();
                currentBlock->addInst(SyIRBuilder::createIItof(V1, V));
            }else if(current_type == INT && V->ty == IRType::IRFloat32){
                VALUE V1 = V;
                V = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
            }
            vecInitial.push_back(V);
            needInitial.push_back(1);
            count++;
        } else {//若是InitializedNode则更深一层
            deep++;
            int this_dim = 0;
            if (deep == 1) {
                this_dim = total;
            } else {
                this_dim = current_array_dims[deep - 2];
            }
            //在这一层需要填充到的数目
            int need = count + this_dim;
            for (auto init: node->inits) {
                visit(init);
            }
            while (count < need) {
                count++;
                if(current_type == INT){
                    vecInitial.push_back(getIntV(0));
                }else{
                    vecInitial.push_back(getFloatV(0.0));
                }
                needInitial.push_back(0);
            }
            deep--;
        }
    }

    void IRGenerate::visit(const std::shared_ptr<FuncDef>& node) {

        if(node->resultOnlyImpactByPara)
            impactByParrFuncSet.insert(node->func_name);

        entryScope();
        var_alloca_list.clear();
        builder.reset();
        varRecordMap.clear();
        // 清空record
        record.ifidx = 0;
        record.whileidx = 0;
        record.bbidx = 0;
        record.condidx = 0;

        // 参数处理
        vector<IRType> params{};

        para_names.clear();
        for(const auto& param: node->parameters){
            if(param->type() == COMMON_FORMAL_PARA){
                auto para = dynamic_pointer_cast<CommonFormalParaNode>(param);
                params.push_back(convertType(para->ty, false));
                visit(para);
            }else{
                auto para = dynamic_pointer_cast<ArrayFormalParaNode>(param);
                params.push_back(convertType(para->ty, true));
                // 将这个形参作为参数传递进入
                visit(para);
            }
        }

        FUNC_MSG_PTR p = make_shared<FUNC_MSG>();
        p->first = convertType(node->ret_type, false);
        p->second = params;

        currentFunc = SyIRBuilder::createFuncDef(node->func_name, p, para_names);

        func_record.insert({node->func_name, p});

        module->func_defs.push_back(currentFunc);
        currentBlock = currentFunc->entryBB;
        clearData();
        visit(dynamic_pointer_cast<CodeBlockNode>(node->block));
        leaveScope();

        // 将变量定义插入entryBB的最前面

        currentFunc->getEntryBB()->insertInstToTheFront(var_alloca_list);
    }

    void IRGenerate::visit(const std::shared_ptr<CommonFormalParaNode>& node) {
        unsigned t = getOccurTime(node->name);
        std::string id = node->name;
        if(t != 0)
            id.append(to_string(t));
        if(node->ty == FLOAT)
            V = SyIRBuilder::createFPE(id);
        else
            V = SyIRBuilder::createIPE(id);
        currentScope->addSymbol(node->name, V);
        currentNumberScope->addSymbol(node->name);
        para_names.push_back(id);
    }

    void IRGenerate::visit(const std::shared_ptr<ArrayFormalParaNode>& node) {
        unsigned t = getOccurTime(node->name);
        std::string id = node->name;
        if(t != 0)
            id.append(to_string(t));

        std::vector<int> dims;
        dims.push_back(4);
        auto it = node->arrayDims.rbegin();
        auto end = node->arrayDims.rend();
        for(;it != end;){
            dims.insert(dims.begin(), (dynamic_pointer_cast<IntValueNode>(*it))->v * dims.front());
            it++;
        }
        if(node->ty == FLOAT) {
            V = SyIRBuilder::createFPE(id);
            currentScope->addArraySymbol(new ArrayVarSymbol(node->name, IRType::IRFloat32, dims, V));
        }
        else{
            V = SyIRBuilder::createIPE(id);
            currentScope->addArraySymbol(new ArrayVarSymbol(node->name, IRType::IRInt32, dims, V));
        }
        currentNumberScope->addSymbol(node->name);
        para_names.push_back(id);
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<CodeBlockNode>& node) {
        for(auto& stmt: node->stmts){
            if(stmt->type() == VAR_DEF){
                visit(dynamic_pointer_cast<VarDef>(stmt));
            }
            else if(stmt->type() == ARRAY_VAR_DEF){
                visit(dynamic_pointer_cast<ArrayVarDef>(stmt));
            }
            else{
                visitStmt(stmt);
            }
        }
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<AssignStmtNode>& node) {

        VALUE V1;
        auto res1 = calcuExprStr(node->rhs);
        auto res2 = calcuExprStr(node->lhs);
        bool mustConv = hasMustConvert(node->rhs);

        if(res1 != "" && gvnMap.find(res1) != gvnMap.end() && !mustConv){
            V1 = gvnMap[res1];
            if(varValueMap[res2] == V1)
              return;
        }else{
            visit(node->rhs);
            V1 = V;
            if(!mustConv){
                gvnMap.insert({res1, V1});
            }
        }

        isLoad = false;
        isArrayLoad = false;
        visit(node->lhs);
        isLoad = true;
        isArrayLoad = true;

        if(V->ty == IRType::IRIntPtr32 && V1->ty == IRType::IRFloat32){
                VALUE V2 = V1;
                V1 = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIFtoi(V2, V1));
        }
        else if(V->ty == IRType::IRFloatPtr32 && V1->ty == IRType::IRInt32){
                VALUE V2 = V1;
                V1 = builder.createTempFReg();
                currentBlock->addInst(SyIRBuilder::createIItof(V2, V1));
        }
        currentBlock->addInst(SyIRBuilder::createIStore(V1, V));

        varValueMap[res2] = V1;
        removeSubExpr(res2);
    }

    // @fixme 暂时先不考虑 a||b 和 a&&b 中的特定情况，比如a||b, a为true就不判断b
    void IRGenerate::visit(const std::shared_ptr<IfStmtNode>& node) {

        if(node->correct_stmt == nullptr){
            if(node->else_stmt != nullptr)
                visitStmt(node->else_stmt);
            return;
        }

        // 解决 A || B， A && B 的问题
        isTemp = true;
        dealWithCondition(node->l_or_expr);

        BASIC_BLOCK trueBB = SyIRBuilder::createBB("if.body" + to_string(++record.ifidx));

        if(node->correct_stmt && node->else_stmt){


            BASIC_BLOCK falseBB = SyIRBuilder::createBB("else.body" + to_string(record.ifidx));
            BASIC_BLOCK nextBB = SyIRBuilder::createBB("next" + to_string(record.ifidx));

            auto label1 = SyIRBuilder::createLabel(trueBB->getBBName(), trueBB);
            auto label2 = SyIRBuilder::createLabel(falseBB->getBBName(), falseBB);

            for(auto& trueJump : trueMap[node->l_or_expr]){
                trueJump->replaceOp2(label1);
                trueBB->addPrevBB(inst2BbMap[trueJump]);
                inst2BbMap[trueJump]->addNextBB(trueBB);
            }

            for(auto& falseJump : falseMap[node->l_or_expr]){
                falseJump->replaceOp3(label2);
                falseBB->addPrevBB(inst2BbMap[falseJump]);
                inst2BbMap[falseJump]->addNextBB(falseBB);
            }

            trueMap.clear();
            falseMap.clear();
            inst2BbMap.clear();

            currentFunc->addBB(trueBB);
            currentBlock = trueBB;
            clearData();
            gvnMap = tempMapStack.back();
            
            visitStmt(node->correct_stmt);

            if(!currentBlock->isJump()){
                currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(nextBB->name, nextBB)));
            }

            currentBlock = falseBB;
            clearData();
            gvnMap = tempMapStack.back();
            currentFunc->addBB(falseBB);
            
            visitStmt(node->else_stmt);

            tempMapStack.pop_back();

            if(!currentBlock->isJump()){
                currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(nextBB->name, nextBB)));
            }

            currentFunc->addBB(nextBB);
            currentBlock = nextBB;
            clearData();
            if(!tempMapStack.empty()){
                gvnMap = tempMapStack.back();
            }
        }
        else{
            BASIC_BLOCK nextBB = SyIRBuilder::createBB("next" + to_string(record.ifidx));

            auto label1 = SyIRBuilder::createLabel(trueBB->getBBName(), trueBB);
            auto label2 = SyIRBuilder::createLabel(nextBB->getBBName(), nextBB);


            for(auto& trueJump : trueMap[node->l_or_expr]){
                trueJump->replaceOp2(label1);
                trueBB->addPrevBB(inst2BbMap[trueJump]);
                inst2BbMap[trueJump]->addNextBB(trueBB);
            }

            for(auto& falseJump : falseMap[node->l_or_expr]){
                falseJump->replaceOp3(label2);
                nextBB->addPrevBB(inst2BbMap[falseJump]);
                inst2BbMap[falseJump]->addNextBB(nextBB);
            }

            trueMap.clear();
            falseMap.clear();
            inst2BbMap.clear();

            currentFunc->addBB(trueBB);
            currentBlock = trueBB;
            clearData();
            gvnMap = tempMapStack.back();
            visitStmt(node->correct_stmt);

            tempMapStack.pop_back();

            if(!currentBlock->isJump()){
                currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(nextBB->name, nextBB)));
            }

            currentFunc->addBB(nextBB);
            currentBlock = nextBB;
            clearData();
            if(!tempMapStack.empty()){
                gvnMap = tempMapStack.back();
            }
        }


    }

    void IRGenerate::visit(const std::shared_ptr<WhileStmtNode>& node) {

        if(node->stmt == nullptr)
            return;

        BASIC_BLOCK beforeBB = SyIRBuilder::createBB("while.before" + to_string(++record.whileidx));
        BASIC_BLOCK bodyBB   = SyIRBuilder::createBB("while.body" + to_string(record.whileidx));
        BASIC_BLOCK afterBB  = SyIRBuilder::createBB("while.after" + to_string(record.whileidx));

        beforeBB_stack.push_back(beforeBB);
        nextBB_stack.push_back(afterBB);

        currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(beforeBB->name, beforeBB)));

        currentFunc->addBB(beforeBB);
        currentBlock = beforeBB;
        clearData();

        auto label1 = SyIRBuilder::createLabel(bodyBB->getBBName(), bodyBB);
        auto label2 = SyIRBuilder::createLabel(afterBB->getBBName(), afterBB);


        dealWithCondition(node->l_or_exp);

        for(auto& trueJump : trueMap[node->l_or_exp]){
            trueJump->replaceOp2(label1);
            bodyBB->addPrevBB(inst2BbMap[trueJump]);
            inst2BbMap[trueJump]->addNextBB(bodyBB);
        }

        for(auto& falseJump : falseMap[node->l_or_exp]){
            falseJump->replaceOp3(label2);
            afterBB->addPrevBB(inst2BbMap[falseJump]);
            inst2BbMap[falseJump]->addNextBB(afterBB);
        }

        trueMap.clear();
        falseMap.clear();
        inst2BbMap.clear();

        currentFunc->addBB(bodyBB);
        currentBlock = bodyBB;
        clearData();
        visitStmt(node->stmt);

        if(!currentBlock->isJump()){
            currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(beforeBB->name, beforeBB)));
        }

        currentFunc->addBB(afterBB);
        currentBlock = afterBB;
        clearData();

        nextBB_stack.pop_back();
        beforeBB_stack.pop_back();

    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<ExpressionStmtNode>& node) {
        if(node->expr)
            visit(node->expr);
    }

    void IRGenerate::visit(const std::shared_ptr<ContinueStmtNode>& node) {
        currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(beforeBB_stack.back()->name, beforeBB_stack.back())));
    }

    void IRGenerate::visit(const std::shared_ptr<BreakStmtNode>& node) {
        currentBlock->addInst(SyIRBuilder::createIJump(SyIRBuilder::createLabel(nextBB_stack.back()->name, nextBB_stack.back())));
    }

    void IRGenerate::visit(const std::shared_ptr<ReturnStmtNode>& node) {
        if(node->expr){
            visit(node->expr);
            if(V->ty == IRType::IRFloat32 && currentFunc->msgPtr->first == IRType::IRInt32){
                VALUE V1 = V;
                V = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
            }else if(V->ty == IRType::IRInt32 && currentFunc->msgPtr->first == IRType::IRFloat32){
                VALUE V1 = V;
                V = builder.createTempFReg();
                currentBlock->addInst(SyIRBuilder::createIItof(V1, V));
            }
            currentBlock->addInst(SyIRBuilder::createIRet(V));
        }else{
            currentBlock->addInst(SyIRBuilder::createIRet(nullptr));
        }
        currentBlock = SyIRBuilder::createBB("bb" + to_string(++record.bbidx));
        clearData();
        currentFunc->addBB(currentBlock);
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<ExpressionNode>& node) {
        switch (node->type()) {
            case COMMON_LVALUE:
                visit(dynamic_pointer_cast<CommonLValueNode>(node));
                break;
            case ARRAY_REF_LVALUE:
                visit(dynamic_pointer_cast<ArrayRefLValueNode>(node));
                break;
            case FUNC_CALL_NODE:
                visit(dynamic_pointer_cast<FuncCallNode>(node));
                break;
            case INT_ITER_NODE:
                visit(dynamic_pointer_cast<IntValueNode>(node));
                break;
            case FLOAT_ITER_NODE:
                visit(dynamic_pointer_cast<FloatValueNode>(node));
                break;
            case UNART_NODE:
                visit(dynamic_pointer_cast<UnaryExpNode>(node));
                break;
            case BINARY_OP_NODE:
                visit(dynamic_pointer_cast<BinaryOpExprNode>(node));
                break;
            default:
                break;
        }
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<UnaryExpNode>& node) {
        auto ty = node->node->type();
        if(ty == BINARY_OP_NODE || ty == ARRAY_REF_LVALUE || ty == UNART_NODE || ty == FUNC_CALL_NODE){
            auto res = calcuExprStr(node->node);
            if(res != ""){
                if(gvnMap.find(res) != gvnMap.end()){
                    V = gvnMap[res];
                }else{
                    visit(node->node);
                    gvnMap.insert({res, V});
                }
            }else{
                visit(node->node);
            }
        }else{
            visit(node->node);
        }
        if(node->unary_op == SUB_OP){
            VALUE V1 = V;
            if(node->expr_type == FLOAT){
                V = builder.createTempFReg();
            }else{
                V = builder.createTempIReg();
            }
            VALUE V2;
            if(V->ty == IRType::IRInt32){
                V2 = SyIRBuilder::createIConstant(0);
            }else{
                V2 = SyIRBuilder::createFConstant(0.0);
            }
            currentBlock->addInst(SyIRBuilder::createISub(V2, V1, V));
        }else if(node->unary_op == NOT){
            VALUE V1;
            if(V->ty == IRType::IRFloat32){
                V1 = V;
                V = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
            }
            V1 = V;
            V = builder.createTempIReg();
            currentBlock->addInst(SyIRBuilder::createIEqual(V1, SyIRBuilder::createIConstant(0), V));
        }
    }

    void IRGenerate::visit(const std::shared_ptr<FuncCallNode>& node) {

        bool reverse = true;
        for(auto& param : node->pparams){
            if(hasFuncCall(param)){
                reverse = false;
                break;
            }
        }

        int int_order = 0;
        int float_order = 0;
        int para_numer = node->pparams.size();

        if(reverse){
            auto it = node->pparams.crbegin();
            auto end = node->pparams.crend();
            auto ty_it = func_record[node->func_name]->second.crbegin();
            auto ty_end = func_record[node->func_name]->second.crend();
            int order = para_numer;
            int i = para_numer-1;
            queue<int> int_queue;
            queue<int> float_queue;
            stack<shared_ptr<InstParam>> floatParaInstStack;
            stack<shared_ptr<InstParam>> intParaInstStack;

            while(it != end){
                auto ty = (*it)->type();
                if(ty == FUNC_CALL_NODE || ty == BINARY_OP_NODE || ty == ARRAY_REF_LVALUE || ty == UNART_NODE){
                    auto res = calcuExprStr(*it);
                    if(res != ""){
                        if(gvnMap.find(res) != gvnMap.end()){
                            V = gvnMap[res];
                        }else{
                            if(func_record[node->func_name]->second[i] == IRType::IRFloatPtr32 || func_record[node->func_name]->second[i] == IRType::IRIntPtr32){
                                isLoad = false;
                                visit(*it);
                                isLoad = true;
                            }else{
                                visit(*it);
                            }
                            gvnMap.insert({res, V});
                        }
                    }else{
                        visit(*it);
                    }
                }else{
                    if(func_record[node->func_name]->second[i] == IRType::IRFloatPtr32 || func_record[node->func_name]->second[i] == IRType::IRIntPtr32){
                        isLoad = false;
                        visit(*it);
                        isLoad = true;
                    }else{
                        visit(*it);
                    }
                }
                // 数值转换
                if(V->ty == IRType::IRFloat32 && *ty_it == IRType::IRInt32){
                    VALUE V1 = V;
                    V = builder.createTempIReg();
                    currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
                }else if(V->ty == IRType::IRInt32 && *ty_it == IRType::IRFloat32){
                    VALUE V1 = V;
                    V = builder.createTempFReg();
                    currentBlock->addInst(SyIRBuilder::createIItof(V1, V));
                }

                auto paraInst = dynamic_pointer_cast<InstParam>(SyIRBuilder::createIParam(V, node->func_name, order, reverse));
                if(V->ty == IRType::IRFloat32){
                    float_order++;
                    float_queue.push(float_order);
                    floatParaInstStack.push(paraInst);
                }else{
                    int_order++;
                    int_queue.push(int_order);
                    intParaInstStack.push(paraInst);
                }
                currentBlock->addInst(dynamic_pointer_cast<IR::Instruction>(paraInst));
                i--;
                order--;
                it++;
                ty_it++;
            }

            while(!int_queue.empty() && !intParaInstStack.empty()){
                intParaInstStack.top()->setTypeOrder(int_queue.front());
                intParaInstStack.pop();
                int_queue.pop();
            }

            while(!float_queue.empty() && !floatParaInstStack.empty()){
                floatParaInstStack.top()->setTypeOrder(float_queue.front());
                floatParaInstStack.pop();
                float_queue.pop();
            }

        }
        else{
            int i = 0;
            int order = 1;
            auto ty_it = func_record[node->func_name]->second.begin();
            for(const auto& param: node->pparams){
                auto ty = param->type();
                if(ty == FUNC_CALL_NODE || ty == BINARY_OP_NODE || ty == ARRAY_REF_LVALUE || ty == UNART_NODE){
                    auto res = calcuExprStr(param);
                    if(res != ""){
                        if(gvnMap.find(res) != gvnMap.end()){
                            V = gvnMap[res];
                        }else{
                            if(func_record[node->func_name]->second[i] == IRType::IRFloatPtr32 || func_record[node->func_name]->second[i] == IRType::IRIntPtr32){
                                isLoad = false;
                                visit(param);
                                isLoad = true;
                            }else{
                                visit(param);
                            }
                            gvnMap.insert({res, V});
                        }
                    }else{
                        visit(param);
                    }
                }else{
                    if(func_record[node->func_name]->second[i] == IRType::IRFloatPtr32 || func_record[node->func_name]->second[i] == IRType::IRIntPtr32){
                        isLoad = false;
                        visit(param);
                        isLoad = true;
                    }else{
                        visit(param);
                    }
                }

                // 数值转换
                if(V->ty == IRType::IRFloat32 && *ty_it == IRType::IRInt32){
                    VALUE V1 = V;
                    V = builder.createTempIReg();
                    currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
                }else if(V->ty == IRType::IRInt32 && *ty_it == IRType::IRFloat32){
                    VALUE V1 = V;
                    V = builder.createTempFReg();
                    currentBlock->addInst(SyIRBuilder::createIItof(V1, V));
                }

                auto paramInst = dynamic_pointer_cast<InstParam>(SyIRBuilder::createIParam(V, node->func_name, order, reverse));
                if(V->ty == IRType::IRFloat32){
                    float_order++;
                    paramInst->setTypeOrder(float_order);
                }else{
                    int_order++;
                    paramInst->setTypeOrder(int_order);
                }
                currentBlock->addInst(dynamic_pointer_cast<IR::Instruction>(paramInst));
                i++;
                order++;
                ty_it++;
            }
        }

        if(func_record[node->func_name]->first == IRType::IRVoid32)
            currentBlock->addInst(SyIRBuilder::createIFuncCall(node->func_name, para_numer, int_order, float_order, reverse));
        else if(func_record[node->func_name]->first == IRType::IRInt32){
            V = builder.createTempIReg();
            currentBlock->addInst(SyIRBuilder::createIFuncCall(node->func_name, para_numer, int_order, float_order, reverse, V));
        }else{
            V = builder.createTempFReg();
            currentBlock->addInst(SyIRBuilder::createIFuncCall(node->func_name, para_numer, int_order, float_order, reverse, V));
        }
        if(node->mustConvert){
            VALUE V1 = V;
            V = builder.createTempIReg();
            currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
        }
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<ArrayRefLValueNode>& node) {
        // 数组的寻址 addr = base + i1 * w1 + i2 * w2 + i3 * w3 ...

        // 首先检查是不是作用域定义的数组
        ArrayVarSymbol* symbol = currentScope->getArraySymbol(node->id);

        bool temp = isLoad;
        bool temp1 = isArrayLoad;

        isLoad = true;
        isArrayLoad = true;

        calcuArrRef(node->arrayRef, symbol->dims);

        isLoad = temp;
        isArrayLoad = temp1;

        VALUE V1 = V;

        // 进行处理
        if(symbol->ty == IRType::IRFloat32)
            V = builder.createTempFPE();
        else
            V = builder.createTempIPE();

        currentBlock->addInst(SyIRBuilder::createIGetPtr(symbol->V, V1, V));

        if(node->arrayRef.size() < symbol->dims.size())
            return;

        if(isArrayLoad) {
            V1 = V;
            if(V->ty == IRType::IRFloatPtr32)
                V = builder.createTempFReg();
            else
                V = builder.createTempIReg();
            currentBlock->addInst(SyIRBuilder::createILoad(V1, V));

            if(node->mustConvert){
                V1 = V;
                V = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
            }
        }
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<IntValueNode>& node) {
        V = SyIRBuilder::createIConstant(node->v);
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<BinaryOpExprNode>& node) {

        VALUE V1;
        std::string res1 = calcuExprStr(node->lhs);
        std::string res2 = calcuExprStr(node->rhs);
        auto res = res1 + toOper(node->oper) + res2;

        bool mustConv1 = hasMustConvert(node->lhs);
        bool mustConv2 = hasMustConvert(node->lhs);
        bool mustConv = mustConv1 || mustConv2;

        if(res1 != "" && res2 != ""){
            if(gvnMap.find(res) != gvnMap.end() && !mustConv){
                V = gvnMap[res];
                return;
            }
        }

        auto ty = node->lhs->type();
        if(ty == BINARY_OP_NODE || ty == ARRAY_REF_LVALUE || ty == FUNC_CALL_NODE || ty == UNART_NODE){
          if(res1 != "" && gvnMap.find(res1) != gvnMap.end() && !mustConv1){
            V1 = gvnMap[res1];
          }else{
            visit(node->lhs);
            V1 = V;
            if(!mustConv1){
                gvnMap.insert({res1, V});
            }
          }
        }else{
          visit(node->lhs);
          V1 = V;
        }

        ty = node->rhs->type();
        if(ty == BINARY_OP_NODE || ty == ARRAY_REF_LVALUE || ty == FUNC_CALL_NODE || ty == UNART_NODE){
          if(res2 != "" && gvnMap.find(res2) != gvnMap.end() && !mustConv2){
            V = gvnMap[res2];
          }else{
            bool isOrOrAnd = false;
            visit(node->rhs);
            if(!mustConv2){
                gvnMap.insert({res2, V});
            }
          }
        }else{
          visit(node->rhs);
        }

        if(node->oper == AND || node->oper == OR)
            return;

        VALUE V2;

        bool isChange = true;
        if(V1->ty == IRType::IRFloat32 || V->ty == IRType::IRFloat32){
            if(V1->ty != IRType::IRFloat32){
                V2 = V1;
                V1 = builder.createTempFReg();
                currentBlock->addInst(SyIRBuilder::createIItof(V2, V1));
            }
            else if(V->ty != IRType::IRFloat32){
                V2 = V;
                V = builder.createTempFReg();
                currentBlock->addInst(SyIRBuilder::createIItof(V2, V));
            }
            isChange = false;
        }

        auto deal = [&](bool islogic = false) -> IR::VALUE {
            if(islogic)
                return builder.createTempIReg();
            else{
                if(isChange)
                    return builder.createTempIReg();
                return builder.createTempFReg();
            }
        };

        switch (node->oper) {
            case ADD_OP:
                V2 = deal();
                currentBlock->addInst(SyIRBuilder::createIAdd(V1, V, V2));
                break;
            case SUB_OP:
                V2 = deal();
                currentBlock->addInst(SyIRBuilder::createISub(V1, V, V2));
                break;
            case MUL_OP:
                V2 = deal();
                currentBlock->addInst(SyIRBuilder::createIMul(V1, V, V2));
                break;
            case DIV_OP:
                V2 = deal();
                currentBlock->addInst(SyIRBuilder::createIDiv(V1, V, V2));
                break;
            case MOD_OP:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createIMod(V1, V, V2));
                break;
            case GT:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createIGt(V1, V, V2));
                break;
            case LT:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createILt(V1, V, V2));
                break;
            case LE:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createILe(V1, V, V2));
                break;
            case GE:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createIGe(V1, V, V2));
                break;
            case EQUAL_OP:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createIEqual(V1, V, V2));
                break;
            case NEQUAL_OP:
                V2 = deal(true);
                currentBlock->addInst(SyIRBuilder::createINEqual(V1, V, V2));
                break;
            default:
                break;
        }
        V = V2;
//        if(res1 != "" && res2 != ""){
//            gvnMap.insert({res, V});
//        }
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<FloatValueNode>& node) {
        V = SyIRBuilder::createFConstant(node->v);
    }

    /* already write */
    void IRGenerate::visit(const std::shared_ptr<CommonLValueNode>& node) {
        if(!isLoad){
            V = currentScope->getSymbol(node->id);
            if(!V){
                V = currentScope->getArraySymbol(node->id)->V;
//                VALUE V1 = V;
//                if(V->ty == IRType::IRFloatPtr32)
//                    V = builder.createTempFPE();
//                else
//                    V = builder.createTempIPE();
//                currentBlock->addInst(SyIRBuilder::createIGetPtr(V1, SyIRBuilder::createIConstant(0), V));
            }
            return;
        }
        V = currentScope->getSymbol(node->id);
        auto reg = dynamic_pointer_cast<IR::Reg>(V);

        if(constVarMap.find(reg->getRegName()) != constVarMap.end()){
          V = constVarMap[reg->getRegName()];
          if(node->mustConvert){
                VALUE V1 = V;
                V = builder.createTempIReg();
                currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
          }
          return;
        }

        VALUE V1 = V;

        if(V1->ty == IRType::IRFloatPtr32)
            V = builder.createTempFReg();
        else
            V = builder.createTempIReg();
        currentBlock->addInst(SyIRBuilder::createILoad(V1, V));

        if(node->mustConvert){
            V1 = V;
            V = builder.createTempIReg();
            currentBlock->addInst(SyIRBuilder::createIFtoi(V1, V));
        }
    }
}
