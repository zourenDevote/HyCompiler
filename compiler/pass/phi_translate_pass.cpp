//
// Created by devotes on 22-7-17.
//

#include "phi_translate_pass.h"
#include <map>

using namespace std;

namespace opt_pass{

    void PhiTranslatePass::execute(IR::MODULE& m) {
        isChange = false;
        map<std::string, IR::VALUE> v2rMap;

        /// 翻译Phi指令
        for(const auto& funcDef : m->getFuncDefs()){
            v2rMap.clear();
            for(const auto& inst : funcDef->getCEntryBB()->getCInsts()){
                if(inst->getSpecInst() != IR::Inst::sy_alloca)
                    break;
                auto instAlloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                string var = dynamic_pointer_cast<IR::Reg>(instAlloca->getVarValue())->getRegName();
                v2rMap.insert({var, instAlloca->getVarValue()});
            }

            for(const auto& bb : funcDef->getCOtherBB()){
                for(const auto& inst : bb->getCInsts()){
                    if(inst->getSpecInst() != IR::Inst::sy_phi)
                        break;
                    auto instPhi = dynamic_pointer_cast<IR::InstPhi>(inst);
                    /// 首先找到对应的BB，插入一条store指令
                    string varName = instPhi->getVarName();
                    for(const auto& value : instPhi->getPhiList()){
                        auto prevBB = dynamic_pointer_cast<IR::Label>(value.second)->getBB();
                        auto storeInst = make_shared<IR::InstTwoOp>(value.first, v2rMap[varName], IR::Inst::sy_store);
                        prevBB->insertInstTo(storeInst);
                    }
                    /// 将这条指令替换为load
                    auto loadInst = make_shared<IR::InstTwoOp>(v2rMap[varName], instPhi->getResult(), IR::Inst::sy_load);
                    bb->replaceTo(loadInst, inst);
                }
            }
        }
    }

}