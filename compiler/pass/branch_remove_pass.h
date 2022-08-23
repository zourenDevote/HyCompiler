//
// Created by 20580 on 2022/8/10.
//

#ifndef COMPILER_BRANCH_REMOVE_PASS_H
#define COMPILER_BRANCH_REMOVE_PASS_H

#include "pass.h"

namespace opt_pass{

    /* *
     * IR层面分支预测Pass， 解决形如如此的问题:
     *      比如 if 1 <a> : <b> 这个指令会直接替换成一条goto指令
     *      if 1 <a> : <b> ==> goto <a>
     * */
    class BranchRemovePass : public Pass{
    public:
        BranchRemovePass() = default;

        void execute(IR::MODULE &m) override;
    private:

    };

}

#endif //COMPILER_BRANCH_REMOVE_PASS_H
