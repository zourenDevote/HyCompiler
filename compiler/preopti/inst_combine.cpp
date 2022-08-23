//
// Created by 20580 on 2022/8/15.
//

#include "inst_combine.h"
#include <algorithm>

/* 指令合并 */
void InstCombine::visit(const std::shared_ptr<CompUnitNode> &node) {
    for(const auto& elem : node->elems){
        if(elem->type() == FUNC_DEF) visit(std::dynamic_pointer_cast<FuncDef>(elem));
    }
}

/* 合并 */
void InstCombine::visit(const std::shared_ptr<FuncDef> &node) {
    if(node->func_name == "func"){
        visit(std::dynamic_pointer_cast<CodeBlockNode>(node->block));
    }
}

/* */
void InstCombine::visit(const std::shared_ptr<CodeBlockNode> &node) {
    firstCommStmt = nullptr;
    lastStmt = nullptr;
    commStmts.clear();
    for(const auto& stmt : node->stmts){
        lastStmt = stmt;
        if(!optiStmt(stmt))
            break;
    }

    if(commStmts.size() > 1){
        std::vector<SmartNode> newStmts;
        auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(std::dynamic_pointer_cast<AssignStmtNode>(firstCommStmt)->rhs);
        position pos{0, 0, ""};
        binary->setRhs(std::make_shared<IntValueNode>(pos, v));
        for(const auto& stmt : node->stmts){
            if(commStmts.find(stmt) == commStmts.end())
                newStmts.push_back(stmt);
        }
        node->stmts = newStmts;
    }
}

bool InstCombine::optiStmt(const SmartNode &stmt) {
    if(stmt->type() == ASSIGN_STATEMENT){
        auto assign = std::dynamic_pointer_cast<AssignStmtNode>(stmt);
        if(assign->lhs->type() == COMMON_LVALUE){
            if(assign->rhs->type() == BINARY_OP_NODE){
                auto lid = std::dynamic_pointer_cast<CommonLValueNode>(assign->lhs)->id;
                auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(assign->rhs);
                if(binary->oper == ADD_OP || binary->oper == SUB_OP){
                    auto l1id = std::dynamic_pointer_cast<CommonLValueNode>(binary->lhs);
                    auto ivalue = std::dynamic_pointer_cast<IntValueNode>(binary->rhs);
                    if(l1id && ivalue){
                        if(binary->oper == SUB_OP){
                            v -= ivalue->v;
                        }else{
                            v += ivalue->v;
                        }
                        if(l1id->id == lid){
                            if(firstCommStmt == nullptr){
                                firstCommStmt = stmt;
                                return true;
                            }
                            else{
                                auto lastAssign = std::dynamic_pointer_cast<AssignStmtNode>(lastStmt);
                                if(lastAssign){
                                    if(lastAssign->lhs->type() == COMMON_LVALUE){
                                        auto lastlid = std::dynamic_pointer_cast<CommonLValueNode>(lastAssign->lhs)->id;
                                        auto lastBinary = std::dynamic_pointer_cast<BinaryOpExprNode>(lastAssign->rhs);
                                        if(lastBinary){
                                            if(lastBinary->oper == ADD_OP || binary->oper == SUB_OP){
                                                auto lastl1id = std::dynamic_pointer_cast<CommonLValueNode>(lastBinary->lhs);
                                                auto lastivalue = std::dynamic_pointer_cast<IntValueNode>(lastBinary->rhs);
                                                if(lastl1id && lastivalue){
                                                    if(lastl1id->id == lastlid && lastlid == lid){
                                                        commStmts.insert(stmt);
                                                        return true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}





