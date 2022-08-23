//
// Created by 20580 on 2022/8/8.
//

#ifndef COMPILER_PART_CONSTANT_SPERATE_PASS_H
#define COMPILER_PART_CONSTANT_SPERATE_PASS_H
#include "pass.h"
#include <map>

namespace opt_pass{

    class PartConstantSperatePass : public Pass{
    public:
        PartConstantSperatePass() = default;
        void execute(IR::MODULE &m) override;
    private:
        /* 处理常量传播的辅助函数 */
        bool dealInst(const IR::INSTRUCTION& inst,
                      std::map<std::string, IR::VALUE>& v2rMap,
                      std::map<IR::VALUE, IR::VALUE>& r2rMap,
                      std::map<std::pair<std::string, unsigned>, IR::VALUE>& arr2rMap,
                      std::map<std::string, std::pair<std::string, unsigned>>& p2arrMap,
                      std::set<std::string>& removeSet);

        bool dealGetptr(const IR::INSTRUCTION& inst,
                        const std::set<std::string>& removeSet);

        void constSpread(const IR::BASIC_BLOCK& block,
                         std::map<std::string, IR::VALUE> v2rMap,
                         std::map<IR::VALUE, IR::VALUE> r2rMap,
                         std::map<std::pair<std::string, unsigned>, IR::VALUE> arr2rMap,
                         std::map<std::string, std::pair<std::string, unsigned>> p2arrMap,
                         std::set<std::string> removeSet);

        std::set<std::string> visitRecordSet;
    };
}

#endif //COMPILER_PART_CONSTANT_SPERATE_PASS_H
