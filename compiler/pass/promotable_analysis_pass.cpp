//
// Created by devotes on 22-7-12.
//

#include "promotable_analyis_pass.h"
#include <cassert>


using namespace std;

namespace opt_pass{

    /// @brief 这个Pass是一个分析Pass，用于分析Func中那些promote的变量
    ///        和只有Alloca和Store了的变量
    void PromoteAnaPass::execute(IR::MODULE& m)  {
        isChange = false;

        unsigned instIndex = 0;

        /* 分析指令，得到变量的store次数，使用次数信息 */
        auto analysisInst = [&](const IR::INSTRUCTION& inst){
            instIndex++;
            switch (inst->getSpecInst()) {
                case IR::Inst::sy_alloca:{
                    auto instAlloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                    // @todo 针对数组变量的分析
                    if(instAlloca->isArrayAlloca())
                        break;
                    // @already do 目前只处理局部变量
                    string varName = dynamic_pointer_cast<IR::Reg>(instAlloca->getVarValue())->getRegName();
                    variableUseMap.insert({varName, 0});
                    break;
                }
                case IR::Inst::sy_store:{
                    /* 对一个变量的store次数进行判断， 数组引用除外 */
                    auto instStore = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    auto varName = dynamic_pointer_cast<IR::Reg>(instStore->getResultValue())->getRegName();
                    /* 目前暂不对getptr得到的临时指针对象的store的分析 */
                    if(varName[0] - '0' >= 0 && varName[0] - '9' <= 0){break;}
                    /* 1. 如果这个变量被store了不止一次 跳过 */
                    if(multiStoreSet.find(varName) != multiStoreSet.end()){break;}
                    /* 2. 如果在signalStoreMap中未发现这个Value, 添加到signalStoreMap, 如果发现了，移除这个varName，并添加到multiStoreSet */
                    if(signalStoreMap.find(varName) == signalStoreMap.end()){
                        signalStoreMap.insert({varName, {instIndex, instStore->getArg1Value()}});
                    }
                    else{
                        signalStoreMap.erase(varName);
                        multiStoreSet.insert(varName);
                    }
                    break;
                }
                case IR::Inst::sy_load:{
                    auto instLoad = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    auto varName = dynamic_pointer_cast<IR::Reg>(instLoad->getArg1Value())->getRegName();
                    if(variableUseMap.find(varName) != variableUseMap.end())
                        variableUseMap[varName]++;
                    break;
                }
                default:
                    break;
            }
        };

        /* 根据分析得到的结果处理指令信息 */
        auto dealWithInst = [&](const IR::INSTRUCTION& inst) -> bool{
            switch (inst->getSpecInst()) {
                case IR::Inst::sy_alloca : {
                    /* 对于这条alloca指令， 如果没有use， 删除这条alloca指令 */
                    auto instAlloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                    string varName = dynamic_pointer_cast<IR::Reg>(instAlloca->getVarValue())->getRegName();
                    if(variableUseMap.find(varName) != variableUseMap.end() && variableUseMap[varName] == 0)
                        return true;
                    break;
                }
                case IR::Inst::sy_store : {
                    /* 如果对于这个变量的store，但是没有use，删除这条store */
                    auto instStore = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    auto varName = dynamic_pointer_cast<IR::Reg>(instStore->getResultValue())->getRegName();
                    if(variableUseMap.find(varName) != variableUseMap.end() && variableUseMap[varName] == 0)
                        return true;
                    break;
                }
                case IR::Inst::sy_load : {
                    /* 如果这条load指令load的变量只在*/
                    auto instLoad = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    auto varName = dynamic_pointer_cast<IR::Reg>(instLoad->getArg1Value())->getRegName();
//                    if()
                }
                default : break;
        }
            return false;
        };

    }
}
