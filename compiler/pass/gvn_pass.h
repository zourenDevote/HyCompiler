//
// Created by devotes on 22-7-28.
//

#ifndef COMPILER_GVN_PASS_H
#define COMPILER_GVN_PASS_H

#include "pass.h"
#include <map>

namespace opt_pass{


    class GvnPass : public Pass{
    public:
        GvnPass() = default;
        void execute(IR::MODULE &m) override;
        void localSubexprEmit(const IR::FUNC_DEFINE& funcDef);
    };

}


#endif //COMPILER_GVN_PASS_H
