//
// Created by shadows on 22-6-27.
//

#ifndef COMPILER_CODEGEN_H
#define COMPILER_CODEGEN_H

#include "../ir/sy_ir.h"
#include "basic.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <set>
#include <tuple>

struct WhileVarScope : public std::enable_shared_from_this<WhileVarScope> {
    std::unordered_map<std::string, std::string> loop_var2reg;
    std::set<std::string> need_store;
    std::shared_ptr<WhileVarScope> parent;

    std::string getVar2Reg(std::string var) {
        for (auto tmp = shared_from_this(); tmp != nullptr; tmp = tmp->parent) {
            if (tmp->loop_var2reg.find(var) != tmp->loop_var2reg.end()) {
                return tmp->loop_var2reg[var];
            }
        }
        return "";
    }

    void insertStore(std::string var) {
        for (auto tmp = shared_from_this(); tmp != nullptr; tmp = tmp->parent) {
            if (tmp->loop_var2reg.find(var) != tmp->loop_var2reg.end()) {
                tmp->need_store.insert(var);
                break;
            }
        }
    }
};

struct FunctionMsg {
    std::string name;
    int frame = 0;
    int stack_size = 0;
    bool has_call = false;
    int memory_for_funcall = 0;
    int reg_order = 0;
    //为spill变量分配的内存偏移量(在memory_for_funcall的基础上)
    int spill_offset = 0;
    //将SSA的虚拟寄存器映射到翻译后的汇编虚拟寄存器
    std::unordered_map<std::string, std::string> new_name_map;
    //翻译后的汇编虚拟寄存器
    std::unordered_map<std::string, ASM::VirtualReg> vir_reg_map;
    //需要压栈的寄存器
    std::vector<std::string> need_push_regs;
    std::vector<std::string> remain_regs = {"r11", "r10", "r9", "r8", "r6", "r5", "r4"};
    std::stack<std::tuple<bool, int, int, std::string>> param_msg;

    std::vector<std::string> need_push_fregs;
    std::vector<std::string> remain_fregs = {"s31", "s30", "s29", "s28", "s27", "s26", "s25", "s24", "s23", "s22",
                                             "s21", "s20", "s19", "s18", "s17", "s16"};
    int jump_type = 0;

    void reset() {
        name = "";
        frame = 0;
        stack_size = 0;
        has_call = false;
        memory_for_funcall = 0;
        reg_order = 0;
        spill_offset = 0;
        new_name_map.clear();
        vir_reg_map.clear();
        need_push_regs.clear();
        need_push_fregs.clear();
        remain_regs = {"r11", "r10", "r9", "r8", "r6", "r5", "r4"};
        remain_fregs = {"s31", "s30", "s29", "s28", "s27", "s26", "s25", "s24", "s23", "s22", "s21", "s20", "s19",
                        "s18", "s17", "s16"};
        while (!param_msg.empty()) {
            param_msg.pop();
        }
        jump_type = 0;
        ASM::Inst::inst_order = 0;
    }
};

class CodeGen {
public:
    CodeGen(std::string out_file = "testcase.s") {
        out.open(out_file, std::ios::trunc);
        for (int i = 1; i <= 12; i++) {
            pow_map.insert({1 << i, i});
        }
    }

    void emitModule(std::shared_ptr<IR::Module>);

    void emitGlobalArrVarDef(std::shared_ptr<IR::GlobalArrayDef>);

    void emitGlobalVarDef(std::shared_ptr<IR::GlobalVarDef>);

    void emitFunctionDef(std::shared_ptr<IR::FunctionDef>);

    void emitBlock(std::shared_ptr<IR::BasicBlock>);

    void emitInstruct(std::shared_ptr<IR::Instruction>);

    void emitMemset(std::shared_ptr<IR::InstMemorySet>);

    void emitAlloc(std::shared_ptr<IR::InstAlloca>);

    void emitParam(std::shared_ptr<IR::InstParam>);

    void emitFunCall(std::shared_ptr<IR::InstFuncCall>);

    void emitInstOne(std::shared_ptr<IR::InstOneOp>);

    void emitInstTwo(std::shared_ptr<IR::InstTwoOp>);

    void emitInstThree(std::shared_ptr<IR::InstThreeOp>);

    void openOpt() { _open_loop_opt = true; }

    ~CodeGen() { out.close(); }

private:
    std::string getGlobalBase(std::string var);

    std::string createNewReg(bool is_int = true);

    std::string getFloatImmVirReg(float);

    std::string getIntImmVirReg(uint32_t);

    std::string getArrayBase(std::string var);

    void getValueAttri(std::shared_ptr<IR::Value> value, bool *is_int, std::string *reg_or_var, int *imm_int,
                       float *imm_float, std::string *label = nullptr);

    void linkBB(std::shared_ptr<IR::BasicBlock> BB);

    void iterBuildInterfer();

    void calcLiveAndBuildInterference(std::shared_ptr<ASM::BasicBlock> BB);

    void buildInterference(std::set<std::string> live);

    void clearInterference();

    void deleteNoUse();

    bool colorReg();

    void genSpillCode();

    void replaceReg();

    ASM::Var getVar(std::string var) {
        if (_fun->getVarMap().find(var) != _fun->getVarMap().end()) {
            return _fun->getVarMap()[var];
        }
        return global_var_map[var];
    }

    bool isVar(std::string var) {
        if (_fun->getVarMap().find(var) != _fun->getVarMap().end() ||
            global_var_map.find(var) != global_var_map.end()) {
            return true;
        }
        return false;
    }

    std::ofstream out;
    std::stringstream ss;

    std::unordered_map<std::string, ASM::Var> global_var_map;
    FunctionMsg fun_msg;
    std::shared_ptr<ASM::Function> _fun;
    std::shared_ptr<ASM::BasicBlock> _BB, _preBB;
    std::unordered_map<int, int> pow_map;
    std::shared_ptr<WhileVarScope> now_while_scope = std::make_shared<WhileVarScope>();
    bool _open_loop_opt = false;
};

#endif //COMPILER_CODEGEN_H
