//
// Created by 20580 on 2022/8/10.
//

#include "branch_remove_pass.h"

using namespace std;

namespace opt_pass{

    void BranchRemovePass::execute(IR::MODULE &m) {

        auto dealCondJump = [&](const IR::BASIC_BLOCK& bb){
            if(bb->instCount() != 0){
                auto inst = bb->getCInsts().back();
                if(inst->getSpecInst() == IR::Inst::sy_if){
                    auto instIf = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    if(instIf->getOp1()->type() == IR::OpType::IRImmOp){
                        auto corrLabel = dynamic_pointer_cast<IR::Label>(instIf->getOp2());
                        auto errLabel = dynamic_pointer_cast<IR::Label>(instIf->getOp3());
                        int v = dynamic_pointer_cast<IR::ImmediateOp>(instIf->getOp1())->getIntValue();
                        if(v == 0){
                            auto gotoInst = make_shared<IR::InstOneOp>(errLabel, IR::Inst::sy_goto);
                            /* 替换成goto指令 */
                            bb->replaceTo(gotoInst, inst);
                            /* 移除前后继关系 */
                            corrLabel->getBB()->removePrev(bb);
                            bb->removeNext(corrLabel->getBB());
                            isChange = true;
                        }else{
                            auto gotoInst = make_shared<IR::InstOneOp>(corrLabel, IR::Inst::sy_goto);
                            /* 替换成goto指令 */
                            bb->replaceTo(gotoInst, inst);
                            /* 移除前后继关系 */
                            errLabel->getBB()->removePrev(bb);
                            bb->removeNext(errLabel->getBB());
                            isChange = true;
                        }
                    }
                }
            }
        };

        /* 不动点标志置位 */
        isChange = false;
        for(const auto& funcDef : m->getCFuncDefs()){
            auto entryBb = funcDef->getCEntryBB();
            dealCondJump(entryBb);
            for(const auto& bb : funcDef->getCOtherBB())
                dealCondJump(bb);
        }

    }
}
