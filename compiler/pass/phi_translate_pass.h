//
// Created by devotes on 22-7-17.
//
#include "pass.h"

#ifndef COMPILER_PHI_TRANSLATE_PASS_H
#define COMPILER_PHI_TRANSLATE_PASS_H

namespace opt_pass{

    /// @brief: 这个Pass翻译Phi指令。
    class PhiTranslatePass : public Pass{
    public:
        PhiTranslatePass() = default;

        /// execute - 执行编译优化
        void execute(IR::MODULE& m) override;
    };
}

#endif //COMPILER_PHI_TRANSLATE_PASS_H
