//
// Created by 20580 on 2022/8/15.
//

#ifndef COMPILER_INST_COMBINE_H
#define COMPILER_INST_COMBINE_H

#include "../ast_visitor.h"

class InstCombine : public AstVisitor{
public:
    InstCombine() = default;
    void visit(const std::shared_ptr<CompUnitNode> &node) override;
    void visit(const std::shared_ptr<FuncDef> &node) override;
    void visit(const std::shared_ptr<CodeBlockNode> &node) override;
private:
    bool optiStmt(const SmartNode& stmt);
    SmartNode firstCommStmt{nullptr};
    SmartNode lastStmt{nullptr};
    std::set<SmartNode> commStmts;
    int v = 0;
};


#endif //COMPILER_INST_COMBINE_H
