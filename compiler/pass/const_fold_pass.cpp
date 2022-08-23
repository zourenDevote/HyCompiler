//
// Created by devotes on 22-8-7.
//

#include "const_fold_pass.h"
#include <cassert>

using namespace std;

namespace opt_pass{

#define OP(V1, V2, op, isFloat) if(isFloat){                \
    float v = V1->getFloatValue() op V2->getFloatValue();   \
    return make_shared<IR::ImmediateOp>(v);                 \
}else{                                                      \
    int v = V1->getIntValue() op V2->getIntValue();         \
    return make_shared<IR::ImmediateOp>(v);                 \
}

#define LGOP(V1, V2, op, isFloat) if(isFloat){                \
    int v = V1->getFloatValue() op V2->getFloatValue();   \
    return make_shared<IR::ImmediateOp>(v);                 \
}else{                                                      \
    int v = V1->getIntValue() op V2->getIntValue();         \
    return make_shared<IR::ImmediateOp>(v);                 \
}

    IR::VALUE constFold(const IR::VALUE& v1, const IR::VALUE& v2, IR::Inst inst){
        auto imm1 = dynamic_pointer_cast<IR::ImmediateOp>(v1);
        auto imm2 = dynamic_pointer_cast<IR::ImmediateOp>(v2);
        bool isFloat = imm1->ty == IR::IRType::IRFloat32?true:false;
        switch (inst) {
            case IR::Inst::sy_add:
                OP(imm1, imm2, +, isFloat)
            case IR::Inst::sy_sub:
                OP(imm1, imm2, -, isFloat)
            case IR::Inst::sy_mul:
                OP(imm1, imm2, *, isFloat)
            case IR::Inst::sy_div:
                OP(imm1, imm2, /, isFloat)
            case IR::Inst::sy_mod: {
                int v = imm1->getIntValue() % imm2->getIntValue();
                return make_shared<IR::ImmediateOp>(v);
            }
            case IR::Inst::sy_le:
                LGOP(imm1, imm2, <=, isFloat)
            case IR::Inst::sy_lt:
                LGOP(imm1, imm2, <, isFloat)
            case IR::Inst::sy_ge:
                LGOP(imm1, imm2, >=, isFloat)
            case IR::Inst::sy_gt:
                LGOP(imm1, imm2, >, isFloat)
            case IR::Inst::sy_and:
                LGOP(imm1, imm2, &&, isFloat)
            case IR::Inst::sy_or:
                LGOP(imm1, imm2, ||, isFloat)
            case IR::Inst::sy_equ:
                LGOP(imm1, imm2, ==, isFloat)
            case IR::Inst::sy_nequ:
                LGOP(imm1, imm2, !=, isFloat)
            default:
                break;
        }
        assert(false && "This has unknown operation in func constFold!");
    }
#undef OP

    void PartConstFoldPass::execute(IR::MODULE &m) {
        isChange = false;
        auto dealInst = [&](const IR::INSTRUCTION& inst)-> bool{
            switch (inst->getInstType()) {
                case IR::InstType::param:{
                    auto param = dynamic_pointer_cast<IR::InstParam>(inst);
                    if(r2rMap.find(param->getPara()) != r2rMap.end())
                        param->replacePara(r2rMap[param->getPara()]);
                    break;
                }
                case IR::InstType::mov:{
                    auto movInst = dynamic_pointer_cast<IR::InstMov>(inst);
                    if(r2rMap.find(movInst->getArg1Value()) != r2rMap.end()){
                        r2rMap.insert({movInst->getResultValue(), r2rMap[movInst->getArg1Value()]});
                        return true;
                    }else if(movInst->getArg1Value()->type() == IR::OpType::IRImmOp){
                        r2rMap.insert({movInst->getResultValue(), movInst->getArg1Value()});
                        return true;
                    }
                    break;
                }
                case IR::InstType::two:{
                    auto instTwo = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    switch (inst->getSpecInst()) {
                        case IR::Inst::sy_ftoi:{
                            if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end()){
                                int v = (int)dynamic_pointer_cast<IR::ImmediateOp>(r2rMap[instTwo->getArg1Value()])->getFloatValue();
                                auto intImm = make_shared<IR::ImmediateOp>(v);
                                r2rMap.insert({instTwo->getResultValue(), intImm});
                                return true;
                            }
                            break;
                        }
                        case IR::Inst::sy_itof:{
                            if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end()){
                                float v = (float)dynamic_pointer_cast<IR::ImmediateOp>(r2rMap[instTwo->getArg1Value()])->getIntValue();
                                auto intImm = make_shared<IR::ImmediateOp>(v);
                                r2rMap.insert({instTwo->getResultValue(), intImm});
                                return true;
                            }
                            break;
                        }
                        case IR::Inst::sy_store:{
                            if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end()) {
//                                r2rMap.insert({instTwo->getResultValue(),r2rMap[instTwo->getArg1Value()]});
                                instTwo->replaceArg1(r2rMap[instTwo->getArg1Value()]);
                            }
                            break;
                        }
                        case IR::Inst::sy_load:{
                            if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end()) {
                                r2rMap.insert({instTwo->getResultValue(),r2rMap[instTwo->getArg1Value()]});
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case IR::InstType::three:{
                    auto instThree = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    if(r2rMap.find(instThree->getOp1()) != r2rMap.end())
                        instThree->replaceOp1(r2rMap[instThree->getOp1()]);
                    if(r2rMap.find(instThree->getOp2()) != r2rMap.end())
                        instThree->replaceOp2(r2rMap[instThree->getOp2()]);
                    if(instThree->getOp1()->type() == IR::OpType::IRImmOp && instThree->getOp2()->type() == IR::OpType::IRImmOp){
                        r2rMap.insert({instThree->getOp3(), constFold(instThree->getOp1(), instThree->getOp2(), instThree->getSpecInst())});
                        return true;
                    }
                    break;
                }
                case IR::InstType::one:{
                    auto instOne = dynamic_pointer_cast<IR::InstOneOp>(inst);
                    if(r2rMap.find(instOne->getOp()) != r2rMap.end())
                        instOne->replaceOp(r2rMap[instOne->getOp()]);
                    break;
                }
                default:
                    break;
            }
            return false;
        };

        for(const auto& funcDef : m->getCFuncDefs()){
            r2rMap.clear();
            auto entry = funcDef->getEntryBB();
            for(const auto& inst : entry->getInsts()){
                if(dealInst(inst)){
                    isChange = true;
                    entry->removeInst(inst);
                }
            }
            for(const auto& bb : funcDef->getCOtherBB()){
                for(const auto& inst : bb->getInsts()){
                    if(dealInst(inst)){
                        isChange = true;
                        bb->removeInst(inst);
                    }
                }
            }
        }
    }

}

