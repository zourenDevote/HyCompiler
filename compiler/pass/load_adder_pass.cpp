//
// Created by 20580 on 2022/8/20.
//

#include "load_adder_pass.h"
#include <algorithm>
#include <cassert>

namespace opt_pass{

    void LoadAdderPass::execute(IR::MODULE &m) {

        std::set<std::string> varSet;
        std::set<std::string> loopVar;
        std::vector<std::set<std::string>> save;
        auto insertLoadInst = [&](const IR::BASIC_BLOCK& bb, std::string& varName, const IR::VALUE& value){
            bool isInsert = true;
            for(const auto& inst : bb->getCInsts()){
                if(inst->getSpecInst() == IR::Inst::sy_load){
                    auto instLoad = std::dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    if(std::dynamic_pointer_cast<IR::Reg>(instLoad->getArg1Value())->getRegName() == varName){
                        isInsert = false;
                        break;
                    }
                }
            }
            if(isInsert){
                std::string regName = "#" + varName + std::to_string(nameIndex++);
                IR::IRType ty = value->ty==IR::IRType::IRIntPtr32?IR::IRType::IRInt32:IR::IRType::IRFloat32;
                auto regV = std::make_shared<IR::Reg>(regName, ty);
                auto inst = std::make_shared<IR::InstTwoOp>(value, regV, IR::Inst::sy_load);
                bb->insertInstToTheFront(inst);
            }
        };

        auto compare = [](const std::pair<std::string, unsigned> p1, const std::pair<std::string, unsigned>& p2) -> bool{
            return p1.second > p2.second;
        };

        auto dealWithInst = [&](const IR::INSTRUCTION& inst){
            if(inst->getSpecInst() == IR::Inst::sy_load){
                auto loadInst = std::dynamic_pointer_cast<IR::InstTwoOp>(inst);
                std::string var = std::dynamic_pointer_cast<IR::Reg>(loadInst->getArg1Value())->getRegName();
                if(varSet.find(var) != varSet.end()&&loopVar.find(var)==loopVar.end()){
                    var2ValueStack.back()[var] = loadInst->getArg1Value();
                    if(countStack[countStack.size()-1].find(var) != countStack[countStack.size()-1].end()){
                        countStack[countStack.size()-1][var]++;
                    }else{
                        countStack[countStack.size()-1].insert({var, 1});
                    }
                }
            }
        };

        auto dealWithBasicBlock = [&](const IR::BASIC_BLOCK& bb){
            std::string name = bb->getBBName();
            if(name.find("entry") == 0){
                for(const auto& inst : bb->getCInsts()){
                    if(inst->getSpecInst() == IR::Inst::sy_alloca){
                        auto allocaInst = std::dynamic_pointer_cast<IR::InstAlloca>(inst);
                        if(!allocaInst->isArrayAlloca()){
                            varSet.insert(std::dynamic_pointer_cast<IR::Reg>(allocaInst->getVarValue())->getRegName());
                        }
                    }
                }
            }
            else if(name.find("while.before") == 0){
                beforeBBStack.push_back(bb);
                countStack.push_back({});
                var2ValueStack.push_back({});
                std::set<std::string> tmp;
                for(const auto& inst : bb->getCInsts()){
                    if(inst->getSpecInst() == IR::Inst::sy_load){
                        auto instLoad = std::dynamic_pointer_cast<IR::InstTwoOp>(inst);
                        std::string var=std::dynamic_pointer_cast<IR::Reg>(instLoad->getArg1Value())->getRegName();
                        if(varSet.find(var)!=varSet.end()&&loopVar.find(var)==loopVar.end()){
                            loopVar.insert(var);
                            tmp.insert(var);
                        }
                    }
                }
                save.push_back(tmp);
            }
            else if(name.find("while.after") == 0){
                if(!save.empty()){
                    std::set<std::string> tmp=save.back();
                    for(auto need_delete:tmp){
                        loopVar.erase(need_delete);
                    }
                    save.pop_back();
                }
                auto beforeBB = beforeBBStack.back();
                auto countVar = countStack.back();
                auto v2ValueMap = var2ValueStack.back();
                beforeBBStack.pop_back();
                countStack.pop_back();
                var2ValueStack.pop_back();
                /* 统计While中load出现最多的变量的次数，添加到while before中 */
                std::vector<std::pair<std::string, unsigned>> seqCountVec(countVar.begin(), countVar.end());
                std::sort(seqCountVec.begin(), seqCountVec.end(), compare);
                /* find load value */
                if(!seqCountVec.empty()){
                    insertLoadInst(beforeBB, seqCountVec[0].first, v2ValueMap[seqCountVec[0].first]);
                    if(seqCountVec.size() > 1){
                        insertLoadInst(beforeBB, seqCountVec[1].first, v2ValueMap[seqCountVec[1].first]);
                    }
                }
            }else{
                if(!beforeBBStack.empty()){
                    for(const auto& inst : bb->getCInsts()){
                        dealWithInst(inst);
                    }
                }
            }
        };

        /* 程序执行主体 */
        for(const auto& funcDef : m->getCFuncDefs()){
            nameIndex = 0;
            countStack.clear();
            beforeBBStack.clear();
            varSet.clear();
            auto params = funcDef->getParams();
            auto params_name = funcDef->getParamsName();
            int param_size = params.size();
            for (int i = 0; i < param_size; i++) {
                if (params[i] != IR::IRType::IRFloatPtr32&&params[i] != IR::IRType::IRIntPtr32) {
                    varSet.insert(params_name[i]);
                }
            }
            dealWithBasicBlock(funcDef->getCEntryBB());
            for(const auto& bb : funcDef->getCOtherBB()){
                dealWithBasicBlock(bb);
            }
            if(!beforeBBStack.empty()){
                assert(false && "load count pass has error!");
            }
        }

    }

}

