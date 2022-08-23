//
// Created by devotes on 22-7-26.
//

#ifndef COMPILER_DEAD_CODE_ELIMINATION_H
#define COMPILER_DEAD_CODE_ELIMINATION_H

#include "pass.h"
#include <map>

namespace opt_pass{

    class DeadCodeElimination : public Pass{
    public:
        DeadCodeElimination() = default;

        void execute(IR::MODULE &m) override;
        void exprDeadCodeEmit(const IR::FUNC_DEFINE& funcDefine);
    private:
        std::set<std::string> deadVarSet;
    };
}

#endif //COMPILER_DEAD_CODE_ELIMINATION_H
