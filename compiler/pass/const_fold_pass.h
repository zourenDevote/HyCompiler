//
// Created by devotes on 22-8-7.
//

#ifndef COMPILER_COMPILER_PASS_CONST_FOLD_H_
#define COMPILER_COMPILER_PASS_CONST_FOLD_H_

#include "pass.h"
#include <map>

namespace opt_pass{
    class PartConstFoldPass : public Pass{
    public:
        PartConstFoldPass() = default;
        void execute(IR::MODULE &m) override;
    private:
        std::map<IR::VALUE, IR::VALUE> r2rMap;
    };
}

#endif //COMPILER_COMPILER_PASS_CONST_FOLD_H_
