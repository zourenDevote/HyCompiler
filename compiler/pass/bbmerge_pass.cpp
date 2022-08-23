//
// Created by 20580 on 2022/8/10.
//
#include "bbmerge_pass.h"
#include <map>

using namespace IR;

namespace opt_pass{
    void BBMergePass::execute(IR::MODULE &m) {
        isChange = false;
        for(const auto& funcDef : m->getCFuncDefs()){
            bbMap.clear();
            bbMap.insert({funcDef->getCEntryBB()->getBBName(), funcDef->getCEntryBB()});
            for(const auto& bb : funcDef->getCOtherBB()){
                bbMap.insert({bb->getBBName(), bb});
            }
            mergeBB(funcDef);
        }
    }

    /* 3. */
    void BBMergePass::mergeBB(const IR::FUNC_DEFINE &funcDefine) {

        std::map<std::string, std::string> jumpBBMap;
        jumpBBMap.clear();

        auto analysisWithBB = [&](const BASIC_BLOCK& bb){
            size_t len = bb->instCount();
            if(len == 1){
                auto inst = bb->getCInsts().back();
                if(inst->getSpecInst() == IR::Inst::sy_goto){
                    auto instOneOp = std::dynamic_pointer_cast<IR::InstOneOp>(inst);
                    std::string label = std::dynamic_pointer_cast<IR::Label>(instOneOp->getOp())->getLabel();
                    bool has = false;
                    for(auto& bbJumpMsg : jumpBBMap){
                        if(bbJumpMsg.second == bb->getBBName()){
                            funcDefine->removeBB(bb);
                            bbJumpMsg.second = label;
                            has = true;
                        }
                    }
                    if(!has){
                        jumpBBMap.insert({bb->getBBName(), label});
                    }

                }
            }
        };

        /* 1. 分析出基本块之间的跳转关系 */
//        deal(funcDefine->getEntryBB());
        for(const auto& bb : funcDefine->getOtherBB())
            analysisWithBB(bb);

        /* 2. 移除那些只有一条goto指令的基本块 */
        for(auto& removeBB : jumpBBMap){
            auto bb = bbMap[removeBB.first];
            funcDefine->removeBB(bb);
            isChange = true;
        }

        auto executeMergeOperation = [&](const BASIC_BLOCK& bb){
            if(bb->instCount() != 0){
                auto inst = bb->getCInsts().back();
                switch (inst->getSpecInst()) {
                    case IR::Inst::sy_if:{
                        auto ifInst = std::dynamic_pointer_cast<InstThreeOp>(inst);
                        auto label1 = std::dynamic_pointer_cast<IR::Label>(ifInst->getOp2());
                        auto label2 = std::dynamic_pointer_cast<IR::Label>(ifInst->getOp3());
                        std::string lab1 = label1->getLabel();
                        std::string lab2 = label2->getLabel();
                        if(jumpBBMap.find(lab1) != jumpBBMap.end()){
                            label1->resetBB(bbMap[jumpBBMap[lab1]]);
                            bb->addNextBB(bbMap[jumpBBMap[lab1]]);
                            bbMap[jumpBBMap[lab1]]->addPrevBB(bb);
                        }
                        if(jumpBBMap.find(lab2) != jumpBBMap.end()){
                            label2->resetBB(bbMap[jumpBBMap[lab2]]);
                            bb->addNextBB(bbMap[jumpBBMap[lab2]]);
                            bbMap[jumpBBMap[lab2]]->addPrevBB(bb);
                        }
                        break;
                    }
                    case IR::Inst::sy_goto:{
                        auto gotoInst = std::dynamic_pointer_cast<InstOneOp>(inst);
                        auto label = std::dynamic_pointer_cast<IR::Label>(gotoInst->getOp());
                        std::string lab = label->getLabel();
                        if(jumpBBMap.find(lab) != jumpBBMap.end()){
                            label->resetBB(bbMap[jumpBBMap[lab]]);
                            bb->addNextBB(bbMap[jumpBBMap[lab]]);
                            bbMap[jumpBBMap[lab]]->addPrevBB(bb);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        /* 3. 执行合并操作 */
        executeMergeOperation(funcDefine->getCEntryBB());
        for(auto& bb : funcDefine->getCOtherBB())
            executeMergeOperation(bb);
    }
}