//
// Created by devotes on 22-7-26.
//
#include "dead_code_elimination_pass.h"
#include <set>

using namespace std;

namespace opt_pass{

    void DeadCodeElimination::execute(IR::MODULE &m) {

        auto analysis = [&](const IR::INSTRUCTION& inst){

            if(inst->getSpecInst() == IR::Inst::sy_alloca){
                auto alloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                if(!alloca->isArrayAlloca()){
                    auto reg = dynamic_pointer_cast<IR::Reg>(alloca->getVarValue());
                    deadVarSet.insert(reg->getRegName());
                }
            }
            else if(inst->getSpecInst() == IR::Inst::sy_load){
                auto load = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                auto reg = dynamic_pointer_cast<IR::Reg>(load->getArg1Value());
                deadVarSet.erase(reg->getRegName());
            }

        };

        auto remove = [&](const IR::BASIC_BLOCK& bb, const IR::INSTRUCTION& inst){

            if(inst->getSpecInst() == IR::Inst::sy_alloca){
                auto alloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                auto reg = dynamic_pointer_cast<IR::Reg>(alloca->getVarValue());
                if(deadVarSet.find(reg->getRegName()) != deadVarSet.end()){
                    isChange = true;
                    bb->removeInst(inst);
                }

            }
            else if(inst->getSpecInst() == IR::Inst::sy_store){
                auto load = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                auto reg = dynamic_pointer_cast<IR::Reg>(load->getResultValue());
                if(deadVarSet.find(reg->getRegName()) != deadVarSet.end()){
                    isChange = true;
                    bb->removeInst(inst);
                }
            }

        };

        auto deal = [&](const IR::FUNC_DEFINE& def){

            for(const auto& inst : def->getCEntryBB()->getCInsts())
                analysis(inst);

            for(const auto& bb : def->getCOtherBB()){
                for(const auto& inst : bb->getCInsts())
                    analysis(inst);
            }

            for(const auto& inst : def->getEntryBB()->getInsts())
                remove(def->getCEntryBB(), inst);

            for(const auto& bb : def->getCOtherBB()){
                for(const auto& inst : bb->getInsts())
                    remove(bb, inst);
            }
        };

        for(const auto& funcDef : m->getFuncDefs()){
            isChange = true;
            while(isChange){
                deadVarSet.clear();
                isChange = false;
                deal(funcDef);
                exprDeadCodeEmit(funcDef);
            }
        }
    }

    void DeadCodeElimination::exprDeadCodeEmit(const IR::FUNC_DEFINE &funcDefine) {

        map<IR::VALUE, unsigned> useCount;

        auto dealValue = [&](const IR::VALUE& value){
          if(value->type() == IR::OpType::IRReg){
            if(useCount.find(value) == useCount.end())
              useCount[value] = 1;
            else
              useCount[value]++;
          }
        };

        auto deal1Value = [&](const IR::VALUE& value) -> bool {
          if(value->type() == IR::OpType::IRReg){
            if(useCount.find(value) != useCount.end())
              if(useCount[value] > 0)
                return false;
            return true;
          }
          return false;
        };

        auto analysis = [&](const IR::INSTRUCTION& inst){

            switch (inst->getInstType()) {
                case IR::InstType::one:{
                    auto one = dynamic_pointer_cast<IR::InstOneOp>(inst);
                    if(one->getOp()!= nullptr){
                        dealValue(one->getOp());
                    }
                    break;
                }
                case IR::InstType::two:{
                    if(inst->getSpecInst() == IR::Inst::sy_load)
                        break;
                    auto two = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    dealValue(two->getArg1Value());
                    break;
                }
                case IR::InstType::three:{
                    auto three = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    dealValue(three->getOp1());
                    dealValue(three->getOp2());
                    break;
                }
                case IR::InstType::param:{
                    auto param = dynamic_pointer_cast<IR::InstParam>(inst);
                    dealValue(param->getPara());
                    break;
                }
                default:
                    break;
            }
        };

        auto deal = [&](const IR::INSTRUCTION& inst) -> bool{

            switch (inst->getInstType()) {
                case IR::InstType::three:{
                    if(inst->getSpecInst()==IR::Inst::sy_getptr)
                        return false;
                    auto three = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    return deal1Value(three->getOp3());
                }
                case IR::InstType::two:{
                    if(inst->getSpecInst()==IR::Inst::sy_store)
                        return false;
                    auto two = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    return deal1Value(two->getResultValue());
                }
                case IR::InstType::func_call:{
                    auto funcCall = dynamic_pointer_cast<IR::InstFuncCall>(inst);
                    if(funcCall->getRetValue() != nullptr){
                        if(useCount.find(funcCall->getRetValue()) != useCount.end() && useCount[funcCall->getRetValue()] == 0)
                            funcCall->replaceRetValue(nullptr);
                    }
                    return false;
                }
                default:
                    break;
            }
            return false;
        };

        for(const auto& inst : funcDefine->getCEntryBB()->getCInsts())
            analysis(inst);

        for(const auto& bb : funcDefine->getCOtherBB()){
            for(const auto& inst : bb->getCInsts())
                analysis(inst);
        }

        auto& centry = funcDefine->getCEntryBB();
        for(const auto& inst : centry->getInsts()){
            if(deal(inst)){
                isChange = true;
                centry->removeInst(inst);
            }
        }

        for(const auto& bb : funcDefine->getCOtherBB()){
            for(const auto& inst : bb->getInsts())
                if(deal(inst)){
                    isChange = true;
                    bb->removeInst(inst);
                }
        }

    }

}

