//
// Created by devotes on 22-7-28.
//

#include "gvn_pass.h"
#include <set>
#include <map>

using namespace std;

namespace opt_pass{

// @brief 这个宏用于决定是否删除掉这条load指令，如果当前的load与上一条load的指令间隔超过INTERVAL_DISTANCE
//        则不删除，否则删除
//#define INTERVAL_DISTANCE 10

    static string getStrOp(IR::Inst inst){
        switch (inst) {
            case IR::Inst::sy_add:
                return "+";
            case IR::Inst::sy_sub:
                return "-";
            case IR::Inst::sy_mul:
                return "*";
            case IR::Inst::sy_div:
                return "/";
            case IR::Inst::sy_mod:
                return "%";
            case IR::Inst::sy_and:
                return "&&";
            case IR::Inst::sy_or:
                return "||";
            case IR::Inst::sy_equ:
                return "==";
            case IR::Inst::sy_nequ:
                return "!=";
            case IR::Inst::sy_ge:
                return "<=";
            case IR::Inst::sy_gt:
                return "<";
            case IR::Inst::sy_le:
                return ">=";
            case IR::Inst::sy_lt:
                return ">";
            default:
                break;
        }
        return "";
    }

    /// @brief 局部公共子表达式消除
    ///        对于每一个基本快进行
    ///        全局值编号
    void GvnPass::execute(IR::MODULE &m) {

        // 记录 VALUE to VALUE 的映射表
        // map<IR::VALUE, IR::VALUE> r2rMap;
        // 记录 VAR to VALUE 的映射表
        map<std::string, IR::VALUE> v2rMap;
        // 记录 VALUE 与 出现位置的映射表
        // map<IR::VALUE, unsigned> v2StepNumver;
        // 记录当前的指令位置
        // unsigned instIndex = 0;
        // 多余的局部load删除
        auto dealInst = [&](const IR::BASIC_BLOCK&bb, const IR::INSTRUCTION& inst){
            if(inst->getSpecInst() == IR::Inst::sy_load){
                auto instTwo = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                auto reg = dynamic_pointer_cast<IR::Reg>(instTwo->getArg1Value());
                if(v2rMap.find(reg->getRegName()) != v2rMap.end()){
                    auto movInst = make_shared<IR::InstMov>(v2rMap[reg->getRegName()], instTwo->getResultValue());
                    v2rMap[reg->getRegName()] = instTwo->getResultValue();
                    bb->replaceTo(movInst, inst);
                }else{
                    v2rMap.insert({reg->getRegName(), instTwo->getResultValue()});
                }
            }
            else if(inst->getSpecInst() == IR::Inst::sy_store){
                auto instTwo = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                auto reg = dynamic_pointer_cast<IR::Reg>(instTwo->getResultValue());
                v2rMap.erase(reg->getRegName());
            }
        };

        auto dealBB = [&](const IR::BASIC_BLOCK& bb){
            // r2rMap.clear();
            v2rMap.clear();
            // v2StepNumver.clear();
            // instIndex = 0;
            for(const auto& inst : bb->getInsts()){
                dealInst(bb, inst);
            }
        };

        for(const auto& func : m->getFuncDefs()){
            dealBB(func->getCEntryBB());
            for(const auto& bb : func->getCOtherBB())
                dealBB(bb);
            localSubexprEmit(func);
        }

    }

    void GvnPass::localSubexprEmit(const IR::FUNC_DEFINE &funcDef) {

        map<std::string, IR::VALUE> gvnMap;
        map<IR::VALUE, IR::VALUE> r2rMap;

        auto dealInst = [&](const IR::INSTRUCTION& inst) -> bool{

            switch(inst->getInstType()){
                case IR::InstType::one:{
                    auto one = dynamic_pointer_cast<IR::InstOneOp>(inst);
                    if(r2rMap.find(one->getOp()) != r2rMap.end()){
                        isChange = true;
                        one->replaceOp(r2rMap[one->getOp()]);
                    }
                    return false;
                }
                case IR::InstType::two:{
                    auto two = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    if(r2rMap.find(two->getArg1Value()) != r2rMap.end()) {
                        isChange = true;
                        two->replaceArg1(r2rMap[two->getArg1Value()]);
                    }
                    if(r2rMap.find(two->getResultValue()) != r2rMap.end()){
                        isChange = true;
                        two->replaceRes(r2rMap[two->getResultValue()]);
                    }
                    return false;
                }
                case IR::InstType::three:{
                    auto three = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    if(r2rMap.find(three->getOp1()) != r2rMap.end()){
                        isChange = true;
                        three->replaceOp1(r2rMap[three->getOp1()]);
                    }
                    if(r2rMap.find(three->getOp2()) != r2rMap.end()){
                        isChange = true;
                        three->replaceOp2(r2rMap[three->getOp2()]);
                    }
                    string expr = three->getOp1()->toIrStr() + getStrOp(three->ty) + three->getOp2()->toIrStr();
                    if(gvnMap.find(expr) != gvnMap.end()){
                        r2rMap.insert({three->getOp3(), gvnMap[expr]});
                        return true;
                    }else
                        gvnMap.insert({expr, three->getOp3()});
                    return false;
                }
                case IR::InstType::param:{
                    auto param = dynamic_pointer_cast<IR::InstParam>(inst);
                    if(r2rMap.find(param->getPara()) != r2rMap.end())
                        param->replacePara(r2rMap[param->getPara()]);
                    return false;
                }
                default:
                    return false;
            }

        };

        gvnMap.clear();
        r2rMap.clear();
        auto& centry = funcDef->getCEntryBB();
        for(const auto& inst : centry->getInsts()){
            if(dealInst(inst)) {
                isChange = true;
                centry->removeInst(inst);
            }
        }

        for(const auto& bb : funcDef->getCOtherBB()){
            gvnMap.clear();
            r2rMap.clear();
            for(const auto& inst : bb->getInsts()){
                if(dealInst(inst)){
                    isChange = true;
                    bb->removeInst(inst);
                }
            }
        }
    }

}

