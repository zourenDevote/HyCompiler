/**
 * Author:  Devotes
 * Date:    2022/06/22
 *
 * FixLog:
 *
 */

#include "initial_pass.h"
#include <map>


using namespace IR;

namespace opt_pass{

    /* *
     * InitPass: 模式匹配删除只有一条跳转指令的基本块, 以及删除不可达的基本块
     * */
    void InitPass::execute(IR::MODULE& m) {
        isChange = false;
        for(auto& func : m->getFuncDefs()){
            bbMap.clear();
            visitMap.clear();
            dfs(func->getCEntryBB());
            bbMap.insert({func->getCEntryBB()->getBBName(), func->getCEntryBB()});
            /* 1. 首先移除那些不可达的基本块 */
            for(const auto& bb : func->getOtherBB()){
                if(visitMap.find(bb->getBBName()) == visitMap.end()) {
                    func->removeBB(bb);
                    isChange = true;
                }
                else
                    bbMap.insert({bb->getBBName(), bb});
            }
            /* 2. 模式匹配*/
//            mergeBB(func);
        }
    }

    void InitPass::dfs(const IR::BASIC_BLOCK &bb) {
        visitMap.insert(bb->getBBName());
        for(const auto& succ : bb->getCNexts()){
            if(visitMap.find(succ->getBBName()) == visitMap.end())
                dfs(succ);
        }
    }

    void InitPass::mergeBB(const IR::FUNC_DEFINE &funcDefine) {

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
                            funcDefine->removeBB(bbMap[bbJumpMsg.second]);
                            bbJumpMsg.second = label;
                            has = true;
                        }
                    }
                    if(!has)
                        jumpBBMap.insert({bb->getBBName(), label});
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
