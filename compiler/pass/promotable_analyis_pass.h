//
// Created by devotes on 22-7-12.
//

#ifndef COMPILER_PROMOTABLE_ANALYIS_PASS_H
#define COMPILER_PROMOTABLE_ANALYIS_PASS_H

#include "pass.h"
#include <set>
#include <map>

namespace opt_pass{

    /* *
     * PromoteAnaPass : 前导分析删除一些可以可删除的Pass
     *  1. 删除那些只有alloca和store但是没有load的变量
     *  2. 对于那些只store了一次的变量，用store的右值替代任意地方的Use
     *  3. 存储变量的promote信息。给出promote的提示。
     * */
    class PromoteAnaPass : public Pass{
    public:
        PromoteAnaPass() = default;
        void execute(IR::MODULE& m) override;
    private:
        // 存储变量的使用次数
        std::map<std::string, unsigned> variableUseMap;
        // 只store了一次的变量对应的ValueMap
        std::map<std::string, std::pair<unsigned, IR::VALUE>> signalStoreMap;
        // 那些有多次store的valueSet
        std::set<std::string> multiStoreSet;
        //
        std::map<IR::VALUE, IR::VALUE> r2rMap;
    };
}

#endif //COMPILER_PROMOTABLE_ANALYIS_PASS_H

