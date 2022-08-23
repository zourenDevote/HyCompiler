/// @author: Devotes
/// @brief:

#ifndef COMPILER_PASS_H
#define COMPILER_PASS_H

#include "../ir/sy_ir.h"
#include <queue>
#include <functional>

namespace opt_pass{

    class Pass{
    public:
        friend class PassManager;

        virtual void execute(IR::MODULE& m) = 0;
    public:
        bool isChange{false};
    };

    class PassManager{

    public:
        PassManager() = default;

        /// registerPassWork - register pass
        void registerPassWork(const std::shared_ptr<Pass>& pass, int work_style = 1);

        /// seqExecutePass - sequential execute pass
        void seqExecutePass(IR::MODULE& m);
    private:
        std::vector<std::shared_ptr<Pass>> work_que;
        std::vector<std::shared_ptr<Pass>> before_que;
        std::vector<std::shared_ptr<Pass>> after_que;
    };

}

#endif //COMPILER_PASS_H
