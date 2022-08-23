//
// Created by devotes on 22-7-25.
//

#include "interval_pass.h"
#include <set>
#include <string>
#include <functional>

using namespace std;

namespace opt_pass{

    // 执行pass
    void IntervalPass::execute(IR::MODULE &m) {

        set<string> notIntervalVarSet;
        set<string> intervalVarSet;

        auto dealValue = [&](const IR::VALUE& value){
            if(value->type() == IR::OpType::IRReg){
                auto reg = dynamic_pointer_cast<IR::Reg>(value);
                if(reg->getRegName() == "arr"){
                    int a = 0;
                }
                if(intervalVarSet.find(reg->getRegName()) != intervalVarSet.end())
                    reg->setIsSinceUseIntervalFuncCall(true);
                else
                    notIntervalVarSet.insert(reg->getRegName());
            }
        };

        auto dealInst = [&](const IR::INSTRUCTION& inst){

            switch (inst->getInstType()) {
                case IR::InstType::param:{
                    auto instParam = dynamic_pointer_cast<IR::InstParam>(inst);
                    dealValue(instParam->getPara());
                    break;
                }
                case IR::InstType::func_call:{
                    auto instFuncCall = dynamic_pointer_cast<IR::InstFuncCall>(inst);
                    intervalVarSet.insert(notIntervalVarSet.begin(), notIntervalVarSet.end());
                    notIntervalVarSet.clear();
                    if(instFuncCall->getRetValue() != nullptr){
                        auto value = dynamic_pointer_cast<IR::Reg>(instFuncCall->getRetValue());
                        notIntervalVarSet.insert(value->getRegName());
                    }
                    break;
                }
                case IR::InstType::alloca:{
                    auto instAlloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                    auto value = dynamic_pointer_cast<IR::Reg>(instAlloca->getVarValue());
                    notIntervalVarSet.insert(value->getRegName());
                    break;
                }
                case IR::InstType::one:{
                    if(inst->getSpecInst() == IR::Inst::sy_ret){
                        auto instOne = dynamic_pointer_cast<IR::InstOneOp>(inst);
                        if(instOne->getOp()){
                            dealValue(instOne->getOp());
                        }
                    }
                    break;
                }
                case IR::InstType::two:{
                    auto instTwo = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    dealValue(instTwo->getArg1Value());
                    dealValue(instTwo->getResultValue());
                    break;
                }
                case IR::InstType::three:{
                    auto instThree = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    dealValue(instThree->getOp1());
                    dealValue(instThree->getOp2());
                    dealValue(instThree->getOp3());
                    break;
                }
                default:
                    break;
            }

        };

        set<string> visit;
        function<void (const IR::BASIC_BLOCK&)> dfs = [&](const IR::BASIC_BLOCK& bb){
            visit.insert(bb->getBBName());
            for(const auto& inst : bb->getCInsts())
                dealInst(inst);
            for(const auto& childBB : bb->getNexts()){
                if(visit.find(childBB->getBBName()) == visit.end())
                    dfs(childBB);
            }
        };

        for(const auto& funcDef : m->getFuncDefs()){
            visit.clear();
            dfs(funcDef->getCEntryBB());
        }
    }

}
