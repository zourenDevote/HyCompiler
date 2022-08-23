//
// Created by devotes on 22-7-25.
//

#ifndef COMPILER_INTERVAL_PASS_H
#define COMPILER_INTERVAL_PASS_H

#include "pass.h"

namespace opt_pass{

    class IntervalPass: public Pass{
    public:
        IntervalPass() = default;

        void execute(IR::MODULE &m) override;
    };

}

#endif //COMPILER_INTERVAL_PASS_H
