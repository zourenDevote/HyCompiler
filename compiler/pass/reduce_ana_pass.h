//
// Created by devotes on 22-7-29.
//

#ifndef COMPILER_COMPILER_PASS_REDUCE_ANA_PASS_H_
#define COMPILER_COMPILER_PASS_REDUCE_ANA_PASS_H_

#include "pass.h"

namespace opt_pass{

class ReduceAnaPass : public Pass{
 public:
    ReduceAnaPass() = default;
    void execute(IR::MODULE &m) override;
 private:
};

}


#endif //COMPILER_COMPILER_PASS_REDUCE_ANA_PASS_H_
