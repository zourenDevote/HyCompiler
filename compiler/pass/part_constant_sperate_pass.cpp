//
// Created by 20580 on 2022/8/8.
//

#include "part_constant_sperate_pass.h"

using namespace std;

namespace opt_pass{

    bool PartConstantSperatePass::dealInst(const IR::INSTRUCTION &inst, std::map<std::string, IR::VALUE> &v2rMap,
                                           std::map<IR::VALUE, IR::VALUE> &r2rMap,
                                           std::map<std::pair<std::string, unsigned int>, IR::VALUE> &arr2rMap,
                                           std::map<std::string, std::pair<std::string, unsigned int>> &p2arrMap,
                                           std::set<std::string> &removeSet) {
        if(inst->getSpecInst() == IR::Inst::sy_store){
            auto instStore = dynamic_pointer_cast<IR::InstTwoOp>(inst);
            if(r2rMap.find(instStore->getArg1Value()) != r2rMap.end())
                instStore->replaceArg1(r2rMap[instStore->getArg1Value()]);
            string varName = dynamic_pointer_cast<IR::Reg>(instStore->getResultValue())->getRegName();
            if(instStore->getArg1Value()->type() == IR::OpType::IRImmOp){
                if(varName[0] - '0' >= 0 && varName[0] - '9' <= 0){
                    if(p2arrMap.find(varName) != p2arrMap.end()){
                        arr2rMap[p2arrMap[varName]] = instStore->getArg1Value();
                    }
                }
                else{
                    if(v2rMap.find(varName) == v2rMap.end())
                        v2rMap.insert({varName, instStore->getArg1Value()});
                    else{
                        v2rMap[varName] = instStore->getArg1Value();
                    }
                }
            }else{
                if(varName[0] - '0' >= 0 && varName[0] - '9' <= 0){
                    arr2rMap.erase(p2arrMap[varName]);
                }
                else{
                    v2rMap.erase(varName);
                }
            }
            removeSet.erase(varName);
        }
        else if(inst->getSpecInst() == IR::Inst::sy_load){
            auto instLoad = dynamic_pointer_cast<IR::InstTwoOp>(inst);
            string varName = dynamic_pointer_cast<IR::Reg>(instLoad->getArg1Value())->getRegName();
            if(v2rMap.find(varName) != v2rMap.end()){
                r2rMap.insert({instLoad->getResultValue(), v2rMap[varName]});
                return true;
            }else{
                if(p2arrMap.find(varName) != p2arrMap.end()) {
                    if (arr2rMap.find(p2arrMap[varName]) != arr2rMap.end()) {
                        r2rMap.insert({instLoad->getResultValue(), arr2rMap[p2arrMap[varName]]});
                        return true;
                    } else {
                        removeSet.erase(varName);
                    }
                }
            }
        }else if(inst->getSpecInst() == IR::Inst::sy_getptr){
            auto instThree = dynamic_pointer_cast<IR::InstThreeOp>(inst);
            if(instThree->getOp2()->type() == IR::OpType::IRImmOp){
                int v = dynamic_pointer_cast<IR::ImmediateOp>(instThree->getOp2())->getIntValue();
                string pName = dynamic_pointer_cast<IR::Reg>(instThree->getOp3())->getRegName();
                string arrayName = dynamic_pointer_cast<IR::Reg>(instThree->getOp1())->getRegName();
                p2arrMap.insert({pName, {arrayName, v}});
                removeSet.insert(pName);
            }
        }else{
            switch (inst->getInstType()) {
                case IR::InstType::param:{
                    auto instParam = dynamic_pointer_cast<IR::InstParam>(inst);
                    if(r2rMap.find(instParam->getPara()) != r2rMap.end())
                        instParam->replacePara(r2rMap[instParam->getPara()]);
                    else{
                        if(dynamic_pointer_cast<IR::Reg>(instParam->getPara()))
                            removeSet.erase(dynamic_pointer_cast<IR::Reg>(instParam->getPara())->getRegName());
                        if(instParam->getPara()->type() == IR::OpType::IRReg){
                            auto reg = dynamic_pointer_cast<IR::Reg>(instParam->getPara());
                            string varName = reg->getRegName();
                            if(varName[0] - '0' >= 0 && varName[0] - '9' <= 0){
                                arr2rMap.erase(p2arrMap[varName]);
                            }else if(reg->ty == IR::IRType::IRIntPtr32 || reg->ty == IR::IRType::IRFloatPtr32){
                                for(const auto& value : p2arrMap){
                                    if(varName == value.second.first){
                                        arr2rMap.erase(value.second);
                                    }
                                }
                            }
                        }

                    }
                    break;
                }
                case IR::InstType::three:{
                    auto instThree = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    if(r2rMap.find(instThree->getOp1()) != r2rMap.end())
                        instThree->replaceOp1(r2rMap[instThree->getOp1()]);
                    if(r2rMap.find(instThree->getOp2()) != r2rMap.end())
                        instThree->replaceOp2(r2rMap[instThree->getOp2()]);
                    break;
                }
                case IR::InstType::two:{
                    auto instTwo = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end())
                        instTwo->replaceArg1(r2rMap[instTwo->getArg1Value()]);
                    break;
                }
                case IR::InstType::one:{
                    auto instOne = dynamic_pointer_cast<IR::InstOneOp>(inst);
                    if(r2rMap.find(instOne->getOp()) != r2rMap.end())
                        instOne->replaceOp(r2rMap[instOne->getOp()]);
                    break;
                }
                case IR::InstType::mov:{
                    auto instMov = dynamic_pointer_cast<IR::InstMov>(inst);
                    if(r2rMap.find(instMov->getArg1Value()) != r2rMap.end())
                        instMov->replaceArg1Value(r2rMap[instMov->getArg1Value()]);
                    break;
                }
                default:
                    break;
            }
        }
        return false;
    }

    bool PartConstantSperatePass::dealGetptr(const IR::INSTRUCTION &inst, const std::set<std::string> &removeSet) {
        if(inst->getSpecInst() == IR::Inst::sy_getptr){
            auto getPtr = dynamic_pointer_cast<IR::InstThreeOp>(inst);
            string name = dynamic_pointer_cast<IR::Reg>(getPtr->getOp3())->getRegName();
            if(removeSet.find(name) != removeSet.end())
                return true;
        }
        return false;
    }

    void PartConstantSperatePass::constSpread(const IR::BASIC_BLOCK &block, std::map<std::string, IR::VALUE> v2rMap,
                                              std::map<IR::VALUE, IR::VALUE> r2rMap,
                                              std::map<std::pair<std::string, unsigned int>, IR::VALUE> arr2rMap,
                                              std::map<std::string, std::pair<std::string, unsigned int>> p2arrMap,
                                              std::set<std::string> removeSet) {
        visitRecordSet.insert(block->getBBName());

        for(const auto& inst : block->getInsts()){
            if(dealInst(inst, v2rMap, r2rMap, arr2rMap, p2arrMap, removeSet)){
                isChange = true;
                block->removeInst(inst);
            }
        }

        for(const auto& inst : block->getInsts()){
            if(dealGetptr(inst, removeSet)){
                isChange = true;
                block->removeInst(inst);
            }
        }

        for(const auto& succ : block->getNexts()){
            if(visitRecordSet.find(succ->getBBName()) == visitRecordSet.end()){
                if(succ->getCPrevs().size() == 1){
                    constSpread(succ, v2rMap, r2rMap, arr2rMap, p2arrMap, removeSet);
                }else{
                    constSpread(succ, {}, {}, {}, {}, {});
                }
            }
        }

    }

    void PartConstantSperatePass::execute(IR::MODULE &m) {
        isChange = false;
        for(const auto& funcDef : m->getCFuncDefs()) {
            visitRecordSet.clear();
            constSpread(funcDef->getCEntryBB(), {}, {}, {}, {}, {});
        }
    }
}
