//
// Created by 20580 on 2022/8/20.
//

#ifndef COMPILER_LOAD_ADDER_PASS_H
#define COMPILER_LOAD_ADDER_PASS_H

#include "pass.h"
#include <map>

namespace opt_pass{
    class LoadAdderPass : public Pass{
    public:
        LoadAdderPass() = default;
        void execute(IR::MODULE &m) override;
    private:
        /* 以栈的形式存储whilebefore基本块的指针, 方便展开 */
        std::vector<IR::BASIC_BLOCK> beforeBBStack;

        /* 用于load统计的数据结构 */
        typedef std::map<std::string, unsigned> VarLoadCountMap;
        typedef std::map<std::string, IR::VALUE> Var2ValueMap;
        typedef std::vector<VarLoadCountMap> VarLoadCountStack;
        typedef std::vector<Var2ValueMap> ValueMapStack;
        VarLoadCountStack countStack;
        ValueMapStack var2ValueStack;

        /* load出来的虚拟寄存器的计数 */
        int nameIndex{0};
    };
}


#endif //COMPILER_LOAD_ADDER_PASS_H
