//
// Created by shadows on 22-7-6.
//

#ifndef COMPILER_BASIC_H
#define COMPILER_BASIC_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <set>
#include <memory>

namespace ASM {

    enum Op {
        Add = 1,
        Sub,
        Subs,
        Rsb,
        Mul,
        Div,
        Asr,
        Lsl,
        Vadd,
        Vsub,
        Vcmp,
        Vrsb,
        Vmul,
        Vdiv,
        Itof,
        Ftoi,
        Ldr,
        Str,
        Strd,
        Vldr,
        Vstr,
        Mov,
        Movlt,
        Movle,
        Movgt,
        Movge,
        Moveq,
        Movne,
        Movwt,
        Vmov,
        Push,
        Pop,
        Cmp,
        Blt,
        Ble,
        Bgt,
        Bge,
        Beq,
        Bne,
        B,
        Bl,
        Bx,
        Label,
        Nop,
        Mls,
        Ite
    };

    struct VirtualReg {
        std::string name;
        bool is_fixed = false;
        std::string phy_reg_name;

        bool is_int = true;
        bool spill = false;
        bool spilled = false;

        //内存偏移地址(若该寄存器需要spill)
        std::string base;
        int offset = 0;
        std::set<std::string> conflict;
        //使用在栈中的变量需在寄存器分配后重新计算偏移量
        bool map2stack_var = false;
        int be_use = 0;
    };

    struct Var {
        std::string name;
        bool is_global;
        bool is_param = false;
        bool is_int = true;
        int offset = 0;
        //使用在栈中的变量需在寄存器分配后重新计算偏移量
        bool in_stack = false;
    };

    class Inst {
    public:
        Inst(Op op, std::string result, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "") :
                _op(op), _result(result), _arg1(arg1), _arg2(arg2), _arg3(arg3) {}

        void dumpInstruction(std::ostream &out);

        Op getOp() { return _op; }

        void setResult(std::string result) { _result = result; }

        std::string getResult() { return _result; }

        void setArg1(std::string arg1) { _arg1 = arg1; }

        std::string getArg1() { return _arg1; }

        void setArg2(std::string arg2) { _arg2 = arg2; }

        std::string getArg2() { return _arg2; }

        void setArg3(std::string arg3) { _arg3 = arg3; }

        std::string getArg3() { return _arg3; }

        void setDef(std::string def) { _def = def; }

        std::string getDef() { return _def; }

        void setUse(std::set<std::string> use) { _use = use; }

        std::set<std::string> getUse() { return _use; }

        void setStackOffset(bool is) { is_stack_offset = is; }

        bool isStackOffset() { return is_stack_offset; }

        void setSpillOffset(bool is) { if_spilled_offset = is; }

        bool isSpillOffset() { return if_spilled_offset; }

        static int inst_order;
    private:
        Op _op;
        std::string _result;
        std::string _arg1;
        std::string _arg2;
        std::string _arg3;
        std::string _def;
        std::set<std::string> _use;

        //使用在栈中的变量需在寄存器分配后重新计算偏移量
        bool is_stack_offset = false;
        //溢出的偏移量需加上为传递函数预留的栈帧
        bool if_spilled_offset = false;
    };

    class BasicBlock {
    public:
        BasicBlock(std::string name, std::string next_name) : _name(name), _next_name(next_name) {}

        std::string getName() { return _name; }

        std::string getNextName() { return _next_name; }

        std::list<Inst> &getInsts() { return _inst; }

        void addInst(Inst inst) {
            Inst::inst_order++;
            _inst.push_back(inst);
        }

        std::set<std::string> &getEntryLive() { return _entry_live; }

        std::set<std::string> &getEntryLiveVar() { return _entry_live_var; }

        void setEntryLive(std::set<std::string> entry_live) { _entry_live = entry_live; }

//        std::vector<std::shared_ptr<BasicBlock>> getPre() { return _pre; }
//
//        void addPre(std::shared_ptr<BasicBlock> pre) { _pre.push_back(pre); }

        std::vector<std::shared_ptr<BasicBlock>> getSuccess() { return _success; }

        void addSuccess(std::shared_ptr<BasicBlock> success) { _success.push_back(success); }

        std::unordered_map<std::string, std::pair<std::string, int>> &getVarAddrReg() { return _var_addr_to_reg; }

        std::unordered_map<std::string, std::pair<std::string, int>> &getVar2Reg() { return _var2reg; }

        std::unordered_map<std::string, std::vector<std::list<Inst>::iterator>> &
        getLdrAndStrVarInst() { return _ldr_and_str_var_inst; }

    private:
        std::string _name, _next_name;
        std::set<std::string> _entry_live;
        std::list<Inst> _inst;
//        std::vector<std::shared_ptr<BasicBlock>> _pre;
        std::vector<std::shared_ptr<BasicBlock>> _success;
        //变量的地址对应的虚拟寄存器名称(全局变量的地址和数组基址)
        std::unordered_map<std::string, std::pair<std::string, int>> _var_addr_to_reg;
        //变量的最新值所在的寄存器
        std::unordered_map<std::string, std::pair<std::string, int>> _var2reg;
        //变量的load和store序列
        std::unordered_map<std::string, std::vector<std::list<Inst>::iterator>> _ldr_and_str_var_inst;

        std::set<std::string> _entry_live_var;
    };

    class Function {
    public:
        Function() {}

        void addBB(std::shared_ptr<BasicBlock> BB) { _BB_map.insert({BB->getName(), BB}); }

        std::unordered_map<std::string, std::shared_ptr<BasicBlock>> &getBBMap() { return _BB_map; }

        void addVar(Var v) { _var_map.insert({v.name, v}); }

        std::unordered_map<std::string, Var> &getVarMap() { return _var_map; }

    private:
        std::unordered_map<std::string, std::shared_ptr<BasicBlock>> _BB_map;
        std::unordered_map<std::string, Var> _var_map;
    };
};
#endif //COMPILER_BASIC_H
