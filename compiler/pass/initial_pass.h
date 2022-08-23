/**
 * Author:  Devotes
 * Date:    2022/06/22
 *
 * FixLog:
 *
 */
#ifndef COMPILER_INITIAL_PASS_H
#define COMPILER_INITIAL_PASS_H

#include "../ir/sy_ir.h"
#include "pass.h"
#include "mem2reg_pass.h"
#include <set>

namespace opt_pass{

    class InitPass: public Pass{

    public:
        InitPass() = default;
        // 执行优化
        void execute(IR::MODULE& m) override;
    private:
        void dfs(const IR::BASIC_BLOCK& bb);
        void mergeBB(const IR::FUNC_DEFINE& funcDefine);

        std::set<std::string> visitMap;
        std::map<std::string, IR::BASIC_BLOCK> bbMap;
    };
}

#endif //COMPILER_INITIAL_PASS_H
