//
// Created by 20580 on 2022/8/10.
//

#ifndef COMPILER_BBMERGE_PASS_H
#define COMPILER_BBMERGE_PASS_H

#include "pass.h"
#include <map>

/* *
 * BB Merge Pass 模式匹配合并多余的Pass
 * */

namespace opt_pass{
    class BBMergePass : public Pass{
    public:
        BBMergePass() = default;
        void execute(IR::MODULE &m) override;
    private:
        void mergeBB(const IR::FUNC_DEFINE &funcDefine);
        std::map<std::string, IR::BASIC_BLOCK> bbMap;
    };
}

#endif //COMPILER_BBMERGE_PASS_H
