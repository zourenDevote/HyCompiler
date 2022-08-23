//
// Created by shadows on 22-6-27.
//
#include "codegen.h"
#include <queue>
#include <stack>
#include <functional>

using namespace std;

void CodeGen::emitModule(shared_ptr<IR::Module> m) {
    const auto &global_arrays = m->getGlobalArrayDef();
    const auto &global_vars = m->getGlobalVarDef();
    const auto &fun_defs = m->getFuncDefs();
    out << "\t.arch armv7ve\n";
    out << "\t.arm\n";
    if (!global_vars.empty() || !global_arrays.empty()) {
        out << "\t.data\n";
        for (auto var: global_vars) {
            emitGlobalVarDef(var);
        }
        for (auto array: global_arrays) {
            emitGlobalArrVarDef(array);
        }
    }
    out << "\t.text\n";
    for (auto fun: fun_defs) {
        emitFunctionDef(fun);
    }
}

void CodeGen::emitGlobalArrVarDef(shared_ptr<IR::GlobalArrayDef> array) {
    auto array_name = array->getArrayName();
    vector<int> need_initial = array->getNeed();

    if (!need_initial.empty()) {
        auto begin = need_initial.begin();
        out << "\t.global\t" << array_name << "\n";
        out << "\t.align\t2\n";
        out << array_name << ":\n";

        if (array->getArrayType() == IR::IRType::IRInt32) {
            int keep_space = 0;
            for (auto num: array->getIntList()) {
                if (num == 0 && *begin == 0) {
                    keep_space += 4;
                } else {
                    if (keep_space != 0) {
                        out << "\t.space " << keep_space << endl;
                        keep_space = 0;
                    }
                    out << "\t.word\t" << num << endl;
                }
                begin++;
            }
            if (keep_space != 0) {
                out << "\t.space " << keep_space << endl;
                keep_space = 0;
            }
        } else {
            int keep_space = 0;
            for (auto fp: array->getFloatList()) {
                if (fp == 0 && *begin == 0) {
                    keep_space += 4;
                } else {
                    if (keep_space != 0) {
                        out << "\t.space " << keep_space << endl;
                        keep_space = 0;
                    }
                    uint32_t *p = (uint32_t *) &fp;
                    out << "\t.word\t" << *p << endl;
                }
                begin++;
            }
            if (keep_space != 0) {
                out << "\t.space " << keep_space << endl;
                keep_space = 0;
            }
        }
    } else {
        out << "\t.comm\t" << array_name << ", " << array->getWidth() << ", 4" << endl;
    }

    ASM::Var v;
    v.name = array_name;
    v.is_global = true;
    if (array->getArrayType() == IR::IRType::IRFloat32) {
        v.is_int = false;
    }
    global_var_map[v.name] = v;

}

void CodeGen::emitGlobalVarDef(shared_ptr<IR::GlobalVarDef> var) {
    auto var_name = var->getVarName();
    out << "\t.global\t" << var_name << "\n";
    out << "\t.align\t2\n";
    out << var_name << ":\n";
    if (var->getVarType() == IR::IRType::IRInt32) {
        out << "\t.word\t" << var->getIntValue() << endl;
    } else {
        float fp = var->getFloatValue();
        uint32_t *p = (uint32_t *) &fp;
        out << "\t.word\t" << *p << endl;
    }
    ASM::Var v;
    v.name = var_name;
    v.is_global = true;
    if (var->getVarType() == IR::IRType::IRFloat32) {
        v.is_int = false;
    }
    global_var_map[v.name] = v;

}

void CodeGen::emitFunctionDef(shared_ptr<IR::FunctionDef> fun) {
    auto fun_name = fun->getFuncName();
    _fun = make_shared<ASM::Function>();
    fun_msg.name = fun_name;
    out << "\t.align\t4\n";
    out << "\t.global\t" << fun_name << "\n";
    out << "\t.syntax\tunified\n";
    out << "\t.thumb\n"
           "\t.thumb_func\n"
           "\t.fpu\tvfpv3-d16\n"
           "\t.type\t" << fun_name << ", %function\n";
    out << fun_name << ":\n";

    auto params = fun->getParams();
    auto params_name = fun->getParamsName();
    int param_size = params.size();
    int int_param = 0, float_param = 0;
    int stack_offset = 0;
    for (int i = 0; i < param_size; i++) {
        ASM::Var v;
        v.name = params_name[i];
        v.is_global = false;
        v.is_param = true;
        if (params[i] == IR::IRType::IRFloat32) {
            v.is_int = false;
            float_param++;
            if (float_param <= 16) {
                ss << "\tvstr.f32 s" << (float_param - 1) << ", [r7, #" + to_string(fun_msg.frame) + "]\n";
                v.offset = fun_msg.frame;
                fun_msg.frame += 4;
            } else {
                v.offset = stack_offset;
                stack_offset += 4;
                v.in_stack = true;
            }
        } else {
            int_param++;
            if (int_param <= 4) {
                ss << "\tstr r" << (int_param - 1) << ", [r7, #" + to_string(fun_msg.frame) + "]\n";
                v.offset = fun_msg.frame;
                fun_msg.frame += 4;
            } else {
                v.offset = stack_offset;
                stack_offset += 4;
                v.in_stack = true;
            }
        }
        _fun->addVar(v);
    }

    emitBlock(fun->getEntryBB());
    for (auto BB: fun->getOtherBB()) {
        emitBlock(BB);
    }

    //连接前继和后继
    linkBB(fun->getEntryBB());
    for (auto BB: fun->getOtherBB()) {
        linkBB(BB);
    }
    //消除不必要的str
    bool update = true;
    while (update) {
        update = false;
        for (auto &[name, BB]: _fun->getBBMap()) {
            set<string> tmp_live = BB->getEntryLiveVar();
            set<string> live;
            for (auto succ: BB->getSuccess()) {
                live.insert(succ->getEntryLiveVar().begin(), succ->getEntryLiveVar().end());
            }
            for (auto &[var, vec]: BB->getLdrAndStrVarInst()) {
                if (vec[0]->getOp() == ASM::Str || vec[0]->getOp() == ASM::Vstr) {
                    live.erase(var);
                } else {
                    live.insert(var);
                }
            }
            if (tmp_live != live) {
                update = true;
                BB->getEntryLiveVar() = live;
            }
        }
    }
    for (auto &[name, BB]: _fun->getBBMap()) {
        set<string> live;
        for (auto succ: BB->getSuccess()) {
            live.insert(succ->getEntryLiveVar().begin(), succ->getEntryLiveVar().end());
        }
        for (auto &[k, v]: BB->getLdrAndStrVarInst()) {
            auto begin = v.begin();
            auto end = v.end() - 1;
            while (begin < end) {
                auto this_inst = *begin;
                auto next_inst = *(begin + 1);
                if (this_inst->getOp() == ASM::Str && next_inst->getOp() == ASM::Str ||
                    this_inst->getOp() == ASM::Vstr && next_inst->getOp() == ASM::Vstr) {
                    _BB->getInsts().erase(this_inst);
                }
                begin++;
            }
            if (getVar(k).is_global) {
                continue;
            }
            if ((*end)->getOp() == ASM::Str || (*end)->getOp() == ASM::Vstr) {
                if (live.find(k) == live.end()) {
                    _BB->getInsts().erase(*end);
                }
            }
        }
    }

    while (true) {
        iterBuildInterfer();
        if (colorReg()) {
            break;
        } else {
//            printf("color failed, genspillcode and try again\n");
            genSpillCode();
            clearInterference();
        }
    }
    //合并mov指令，消除不必要的mov
    for (auto &[name, BB]: _fun->getBBMap()) {
        auto &inst = BB->getInsts();
        auto rbegin = inst.rbegin();
        auto rend = inst.rend();
        rend--;
        vector<decltype(inst.begin())> need_delete;
        while (rbegin != rend) {
            auto tmp = rbegin;
            rbegin++;
            if (tmp->getOp() == ASM::Mov && (rbegin->getOp() == ASM::Mov || rbegin->getOp() == ASM::Movwt)) {
                if (tmp->getArg1() == rbegin->getResult() && fun_msg.vir_reg_map[rbegin->getResult()].be_use == 1) {
                    rbegin->setResult(tmp->getResult());
//                    rbegin.base()->dumpInstruction(cout);
                    need_delete.push_back(rbegin.base());
                }
            }
        }
        for (auto &iter: need_delete) {
            inst.erase(iter);
        }
    }
    replaceReg();

    //为了栈对齐
    if ((fun_msg.memory_for_funcall + fun_msg.spill_offset) % 8) {
        fun_msg.spill_offset += 4;
    }
    fun_msg.stack_size = fun_msg.frame + fun_msg.memory_for_funcall + fun_msg.spill_offset;

    auto cmp = [](string s1, string s2) {
        return stoi(s1.substr(1, s1.length() - 1)) < stoi(s2.substr(1, s2.length() - 1));
    };
    fun_msg.need_push_regs.push_back("r7");
    std::sort(fun_msg.need_push_regs.begin(), fun_msg.need_push_regs.end(), cmp);
    std::sort(fun_msg.need_push_fregs.begin(), fun_msg.need_push_fregs.end(), cmp);
    //开辟栈帧
    if (fun_msg.has_call) {
        fun_msg.need_push_regs.push_back("lr");
    }
    string need_push;
    for (auto reg: fun_msg.need_push_regs) {
        need_push += reg + ",";
    }
    need_push.pop_back();
    out << "\tpush\t{" << need_push << "}" << endl;
    string need_vpush;
    if (!fun_msg.need_push_fregs.empty()) {
        for (auto reg: fun_msg.need_push_fregs) {
            need_vpush += reg + ",";
        }
        need_vpush.pop_back();
        out << "\tvpush\t{" << need_vpush << "}" << endl;
    }

    if ((fun_msg.need_push_regs.size() + fun_msg.need_push_fregs.size()) % 2) {
        if (fun_msg.stack_size % 8 == 0) {
            fun_msg.frame += 4;
            fun_msg.stack_size += 4;
        }
    } else {
        if (fun_msg.stack_size % 8) {
            fun_msg.frame += 4;
            fun_msg.stack_size += 4;
        }
    }
    int stack = fun_msg.stack_size;
    while (stack >= 4096) {
        out << "\tsub\tsp, sp, #" << 4096 << "\n";
        stack -= 4096;
    }
    if (stack > 0) {
        out << "\tsub\tsp, sp, #" << stack << "\n";
    }
    int memory = fun_msg.memory_for_funcall + fun_msg.spill_offset;
    while (memory >= 4096) {
        out << "\tadd\tr7, sp, #" << 4096 << "\n";
        memory -= 4096;
    }
    out << "\tadd\tr7, sp, #" << memory << "\n";

    int frame_and_reg_space = fun_msg.frame + (fun_msg.need_push_regs.size() + fun_msg.need_push_fregs.size()) * 4;
    for (auto inst: _fun->getBBMap()[fun->getEntryBB()->getBBName()]->getInsts()) {
        if (inst.getOp() == ASM::Movwt && (inst.isStackOffset() || inst.isSpillOffset())) {
            uint32_t lower_16 = stoi(inst.getArg1());
            uint32_t upper_16 = stoi(inst.getArg2());
            uint32_t total =
                    (lower_16 | (upper_16 << 16)) +
                    (inst.isStackOffset() ? frame_and_reg_space : fun_msg.memory_for_funcall);
            inst.setArg1(to_string(total & 0x0000FFFF));
            inst.setArg2(to_string((total >> 16) & 0x0000FFFF));
        }
        inst.dumpInstruction(ss);
    }
    for (auto BB: fun->getOtherBB()) {
        for (auto inst: _fun->getBBMap()[BB->getBBName()]->getInsts()) {
            if (inst.getOp() == ASM::Movwt && (inst.isStackOffset() || inst.isSpillOffset())) {
                uint32_t lower_16 = stoi(inst.getArg1());
                uint32_t upper_16 = stoi(inst.getArg2());
                uint32_t total =
                        (lower_16 | (upper_16 << 16)) +
                        (inst.isStackOffset() ? frame_and_reg_space : fun_msg.memory_for_funcall);
                inst.setArg1(to_string(total & 0x0000FFFF));
                inst.setArg2(to_string((total >> 16) & 0x0000FFFF));
            }
            inst.dumpInstruction(ss);
        }
    }
    //各个block中的语句
    out << ss.str();
    ss.str("");

    //恢复栈帧
    out << "." + fun_name + ".ret:" << endl;
    int need_add = fun_msg.frame;
    while (need_add >= 4096) {
        out << "\tadd\tr7, r7, #" << 4096 << "\n";
        need_add -= 4096;
    }
    out << "\tadd\tr7, r7, #" << need_add << "\n";
    out << "\tmov\tsp, r7\n";

    if (!fun_msg.need_push_fregs.empty()) {
        string need_vpop = need_vpush;
        out << "\tvpop\t{" << need_vpop << "}\n";
    }

    string need_pop;
    if (fun_msg.has_call) {
        for (auto begin = fun_msg.need_push_regs.begin(); begin < fun_msg.need_push_regs.end() - 1; begin++) {
            need_pop += *begin + ",";
        }
        need_pop += "pc";
        out << "\tpop\t{" << need_pop << "}\n";
    } else {
        need_pop = need_push;
        out << "\tpop\t{" << need_pop << "}\n";
        out << "\tbx\tlr\n";
    }
    fun_msg.reset();
}

void CodeGen::emitBlock(shared_ptr<IR::BasicBlock> BB) {
    _BB = make_shared<ASM::BasicBlock>(BB->getBBName(), BB->getNextBBName());
    _BB->addInst(ASM::Inst(ASM::Label, fun_msg.name + "." + BB->getBBName()));

    if (_open_loop_opt && BB->getBBName().find("while.after") == 0) {
        auto tmp = now_while_scope;
        for (auto &[var, reg]: tmp->loop_var2reg) {
            if (tmp->need_store.find(var) == tmp->need_store.end()) {
                continue;
            }
            ASM::Var v = getVar(var);
            if (v.is_global) {
                string des = getGlobalBase(var);
                if (v.is_int) {
                    _BB->addInst(ASM::Inst(ASM::Str, reg, des));
                } else {
                    _BB->addInst(ASM::Inst(ASM::Vstr, reg, des));
                }
                _BB->getInsts().rbegin()->setUse({reg, des});
            } else {
                if (v.in_stack || v.offset >= 4096) {
                    string imm_reg = getIntImmVirReg(v.offset);
                    if (v.in_stack) {
                        _BB->getInsts().rbegin()->setStackOffset(true);
                    }
                    if (v.is_int) {
                        _BB->addInst(ASM::Inst(ASM::Str, reg, "r7", imm_reg));
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Add, imm_reg, "r7", imm_reg));
                        _BB->getInsts().rbegin()->setUse({imm_reg});
                        _BB->getInsts().rbegin()->setDef(imm_reg);
                        _BB->addInst(ASM::Inst(ASM::Vstr, reg, imm_reg));
                    }
                    _BB->getInsts().rbegin()->setUse({reg, imm_reg});
                } else {
                    if (v.is_int) {
                        _BB->addInst(ASM::Inst(ASM::Str, reg, "r7", "#" + to_string(v.offset)));
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Vstr, reg, "r7", "#" + to_string(v.offset)));
                    }
                    _BB->getInsts().rbegin()->setUse({reg});
                }
            }
            _BB->getVar2Reg()[var] = {reg, ASM::Inst::inst_order};
            auto list_end = _BB->getInsts().rbegin();
            list_end++;
            _BB->getLdrAndStrVarInst()[var].push_back(list_end.base());
        }
        now_while_scope = tmp->parent;
    }

    for (auto inst: BB->getInsts()) {
        emitInstruct(inst);
        if (inst->ty == IR::Inst::sy_goto || inst->ty == IR::Inst::sy_ret) {
            break;
        }
    }

    _fun->addBB(_BB);

    if (_open_loop_opt && BB->getNextBBName().find("while.before") == 0) {
        auto tmp = make_shared<WhileVarScope>();
        tmp->parent = now_while_scope;
        now_while_scope = tmp;
        _preBB = _BB;
    }

}

void CodeGen::emitInstruct(shared_ptr<IR::Instruction> inst) {
    switch (inst->getInstType()) {
        case IR::InstType::alloca:
            emitAlloc(dynamic_pointer_cast<IR::InstAlloca>(inst));
            break;
        case IR::InstType::param:
            emitParam(dynamic_pointer_cast<IR::InstParam>(inst));
            break;
        case IR::InstType::func_call:
            emitFunCall(dynamic_pointer_cast<IR::InstFuncCall>(inst));
            break;
        case IR::InstType::one:
            emitInstOne(dynamic_pointer_cast<IR::InstOneOp>(inst));
            break;
        case IR::InstType::two:
            emitInstTwo(dynamic_pointer_cast<IR::InstTwoOp>(inst));
            break;
        case IR::InstType::three:
            emitInstThree(dynamic_pointer_cast<IR::InstThreeOp>(inst));
            break;
        default:
            emitMemset(dynamic_pointer_cast<IR::InstMemorySet>(inst));
            break;
    }
}

void CodeGen::emitAlloc(shared_ptr<IR::InstAlloca> inst) {
    ASM::Var v;
    v.name = dynamic_pointer_cast<IR::Reg>(inst->getVarValue())->getRegName();
    v.offset = fun_msg.frame;
    v.is_global = false;
    fun_msg.frame += inst->getWidth();
    if (inst->getVarType() == IR::IRType::IRFloat32) {
        v.is_int = false;
    }
    _fun->addVar(v);
}

void CodeGen::emitParam(std::shared_ptr<IR::InstParam> inst) {
    string reg_or_var;
    bool is_int;
    int imm_int;
    float imm_float;

    shared_ptr<IR::Value> value = inst->getPara();
    getValueAttri(value, &is_int, &reg_or_var, &imm_int, &imm_float);

    int order = inst->getOrder();
    int type_order = inst->getTypeOrder();
    string vir_reg;
    if (reg_or_var != "") {
        if (isVar(reg_or_var)) {
            vir_reg = getArrayBase(reg_or_var);
        } else {
            vir_reg = fun_msg.new_name_map[reg_or_var];
        }
    } else {
        if (is_int) {
            vir_reg = getIntImmVirReg(imm_int);
        } else {
            vir_reg = getFloatImmVirReg(imm_float);
        }
    }

    if (inst->getReverse()) {
        if (is_int) {
            if (type_order > 4) {
                int stack_order = type_order - 4 + (order - type_order > 16 ? order - type_order - 16 : 0);
                _BB->addInst(ASM::Inst(ASM::Str, vir_reg, "sp", "#" + to_string((stack_order - 1) * 4)));
                _BB->getInsts().rbegin()->setUse({vir_reg});
            } else {
                string tmp = vir_reg;
                vir_reg = createNewReg();
                _BB->addInst(ASM::Inst(ASM::Mov, vir_reg, tmp));
                _BB->getInsts().rbegin()->setUse({tmp});
                _BB->getInsts().rbegin()->setDef(vir_reg);
                fun_msg.vir_reg_map[vir_reg].is_fixed = true;
                fun_msg.vir_reg_map[vir_reg].phy_reg_name = 'r' + to_string(type_order - 1);
            }
        } else {
            if (type_order > 16) {
                int stack_order = type_order - 16 + (order - type_order > 4 ? order - type_order - 4 : 0);
                _BB->addInst(ASM::Inst(ASM::Vstr, vir_reg, "sp", "#" + to_string((stack_order - 1) * 4)));
                _BB->getInsts().rbegin()->setUse({vir_reg});
            } else {
                string tmp = vir_reg;
                vir_reg = createNewReg(false);
                _BB->addInst(ASM::Inst(ASM::Vmov, vir_reg, tmp));
                _BB->getInsts().rbegin()->setUse({tmp});
                _BB->getInsts().rbegin()->setDef(vir_reg);
                fun_msg.vir_reg_map[vir_reg].is_fixed = true;
                fun_msg.vir_reg_map[vir_reg].phy_reg_name = 's' + to_string(type_order - 1);
            }
        }
    }
    fun_msg.param_msg.push({is_int, order, type_order, vir_reg});
}

void CodeGen::emitFunCall(std::shared_ptr<IR::InstFuncCall> inst) {
    fun_msg.has_call = true;
    int param_num = inst->getParamNum();
    int int_param_num = inst->getIntParamNum();
    int float_param_num = inst->getFlaotParamNum();
    int stack_param =
            (int_param_num > 4 ? int_param_num - 4 : 0) + (float_param_num > 16 ? float_param_num - 16 : 0);
    fun_msg.memory_for_funcall = max(fun_msg.memory_for_funcall, stack_param * 4);

    set<int> int_no_used = {1, 2, 3, 4};
    set<int> float_no_used = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    set<string> use;

    for (int i = 0; i < param_num; i++) {
        auto param = fun_msg.param_msg.top();
        fun_msg.param_msg.pop();
        bool is_int = get<0>(param);
        int order = get<1>(param);
        int type_order = get<2>(param);
        string vir_reg = get<3>(param);
        if (inst->isReverse()) {
            if (is_int) {
                if (type_order <= 4) {
                    int_no_used.erase(type_order);
                    use.insert(vir_reg);
                }
            } else {
                if (type_order <= 16) {
                    float_no_used.erase(type_order);
                    use.insert(vir_reg);
                }
            }
        } else {
            if (is_int) {
                if (type_order <= 4) {
                    int_no_used.erase(type_order);
                    string tmp = createNewReg();
                    _BB->addInst(ASM::Inst(ASM::Mov, tmp, vir_reg));
                    _BB->getInsts().rbegin()->setUse({vir_reg});
                    _BB->getInsts().rbegin()->setDef(tmp);
                    fun_msg.vir_reg_map[tmp].is_fixed = true;
                    fun_msg.vir_reg_map[tmp].phy_reg_name = "r" + to_string(type_order - 1);
                    use.insert(tmp);
                } else {
                    int stack_order = type_order - 4 + (order - type_order > 16 ? order - type_order - 16 : 0);
                    _BB->addInst(ASM::Inst(ASM::Str, vir_reg, "sp", "#" + to_string((stack_order - 1) * 4)));
                    _BB->getInsts().rbegin()->setUse({vir_reg});
                }
            } else {
                if (type_order <= 16) {
                    float_no_used.erase(type_order);
                    string tmp = createNewReg(false);
                    _BB->addInst(ASM::Inst(ASM::Vmov, tmp, vir_reg));
                    _BB->getInsts().rbegin()->setUse({vir_reg});
                    _BB->getInsts().rbegin()->setDef(tmp);
                    fun_msg.vir_reg_map[tmp].is_fixed = true;
                    fun_msg.vir_reg_map[tmp].phy_reg_name = "s" + to_string(type_order - 1);
                    use.insert(tmp);
                } else {
                    int stack_order = type_order - 16 + (order - type_order > 4 ? order - type_order - 4 : 0);
                    _BB->addInst(ASM::Inst(ASM::Vstr, vir_reg, "sp", "#" + to_string((stack_order - 1) * 4)));
                    _BB->getInsts().rbegin()->setUse({vir_reg});
                }
            }
        }
    }
    for (auto no_used: int_no_used) {
        string tmp = createNewReg();
        fun_msg.vir_reg_map[tmp].is_fixed = true;
        fun_msg.vir_reg_map[tmp].phy_reg_name = "r" + to_string(no_used - 1);
        //设置为不可被spill(只用于构建冲突图，并不生成实际代码）
        fun_msg.vir_reg_map[tmp].spilled = true;
        use.insert(tmp);
        _BB->addInst(ASM::Inst(ASM::Nop, ""));
        _BB->getInsts().rbegin()->setDef({tmp});
    }

    for (auto no_used: float_no_used) {
        string tmp = createNewReg(false);
        fun_msg.vir_reg_map[tmp].is_fixed = true;
        fun_msg.vir_reg_map[tmp].phy_reg_name = "s" + to_string(no_used - 1);
        //设置为不可被spill(只用于构建冲突图，并不生成实际代码）
        fun_msg.vir_reg_map[tmp].spilled = true;
        use.insert(tmp);
        _BB->addInst(ASM::Inst(ASM::Nop, ""));
        _BB->getInsts().rbegin()->setDef({tmp});
    }

    _BB->addInst(ASM::Inst(ASM::Bl, inst->getFuncName()));
    _BB->getInsts().rbegin()->setUse(use);

    if (inst->getRetValue()) {
        auto value = dynamic_pointer_cast<IR::Reg>(inst->getRetValue());
        string ret;
        if (value->getType() == IR::IRType::IRInt32) {
            ret = createNewReg();
            _BB->addInst(ASM::Inst(ASM::Mov, ret, "r0"));
        } else {
            ret = createNewReg(false);
            _BB->addInst(ASM::Inst(ASM::Vmov, ret, "s0"));
        }
        fun_msg.new_name_map[value->getRegName()] = ret;
        _BB->getInsts().rbegin()->setDef(ret);
    }
}

void CodeGen::emitMemset(std::shared_ptr<IR::InstMemorySet> inst) {
    int size = inst->getSize();
    string name = inst->getName();
    string base = getArrayBase(name);
    string zero = getIntImmVirReg(0);
    while (size > 1024) {
        for (int i = 0; i < 1024; i++) {
            _BB->addInst(ASM::Inst(ASM::Str, zero, base, "#" + to_string(i * 4)));
            _BB->getInsts().rbegin()->setUse({zero, base});
        }
        string tmp = base;
        base = createNewReg();
        _BB->addInst(ASM::Inst(ASM::Add, base, tmp, "#4096"));
        _BB->getInsts().rbegin()->setUse({tmp});
        _BB->getInsts().rbegin()->setDef(base);
        size -= 1024;
    }
    for (int i = 0; i < size; i++) {
        _BB->addInst(ASM::Inst(ASM::Str, zero, base, "#" + to_string(i * 4)));
        _BB->getInsts().rbegin()->setUse({zero, base});
    }
}

void CodeGen::emitInstOne(std::shared_ptr<IR::InstOneOp> inst) {
    string reg_or_var;
    bool is_int;
    int imm_int;
    float imm_float;
    string label;

    shared_ptr<IR::Value> value = inst->getOp();
    if (value) {
        getValueAttri(value, &is_int, &reg_or_var, &imm_int, &imm_float, &label);
    }

    if (inst->getSpecInst() == IR::Inst::sy_goto && label != "") {
        label = fun_msg.name + "." + label;
        _BB->addInst(ASM::Inst(ASM::B, label));
    }

    if (inst->getSpecInst() == IR::Inst::sy_ret) {
        if (value) {
            string ret_reg;
            if (reg_or_var == "") {
                string imm_reg;
                if (is_int) {
                    ret_reg = createNewReg();
                    imm_reg = getIntImmVirReg(imm_int);
                    _BB->addInst(ASM::Inst(ASM::Mov, ret_reg, imm_reg));
                } else {
                    ret_reg = createNewReg(false);
                    imm_reg = getFloatImmVirReg(imm_float);
                    _BB->addInst(ASM::Inst(ASM::Vmov, ret_reg, imm_reg));
                }
                _BB->getInsts().rbegin()->setUse({imm_reg});
                _BB->getInsts().rbegin()->setDef(ret_reg);
            } else {
                if (is_int) {
                    ret_reg = createNewReg();
                    _BB->addInst(ASM::Inst(ASM::Mov, ret_reg, fun_msg.new_name_map[reg_or_var]));
                } else {
                    ret_reg = createNewReg(false);
                    _BB->addInst(ASM::Inst(ASM::Vmov, ret_reg, fun_msg.new_name_map[reg_or_var]));
                }
                _BB->getInsts().rbegin()->setUse({fun_msg.new_name_map[reg_or_var]});
                _BB->getInsts().rbegin()->setDef(ret_reg);
            }
            if (is_int) {
                fun_msg.vir_reg_map[ret_reg].phy_reg_name = "r0";
            } else {
                fun_msg.vir_reg_map[ret_reg].phy_reg_name = "s0";
            }
            fun_msg.vir_reg_map[ret_reg].is_fixed = true;
            //设置为不可被spill
            fun_msg.vir_reg_map[ret_reg].spilled = true;
            _BB->addInst(ASM::Inst(ASM::Nop, ""));
            _BB->getInsts().rbegin()->setUse({ret_reg});
        }
        _BB->addInst(ASM::Inst(ASM::B, fun_msg.name + ".ret"));
    }
}

void CodeGen::emitInstTwo(std::shared_ptr<IR::InstTwoOp> inst) {
    string reg_or_var1, reg_or_var2;
    bool is_int1, is_int2;
    int imm_int1, imm_int2;
    float imm_float1, imm_float2;

    shared_ptr<IR::Value> value = inst->getArg1Value();
    getValueAttri(value, &is_int1, &reg_or_var1, &imm_int1, &imm_float1);

    value = inst->getResultValue();
    getValueAttri(value, &is_int2, &reg_or_var2, &imm_int2, &imm_float2);

    if (inst->getSpecInst() == IR::Inst::sy_itof) {
        string vir_reg1;
        if (reg_or_var1 != "") {
            vir_reg1 = fun_msg.new_name_map[reg_or_var1];
        } else {
            vir_reg1 = getIntImmVirReg(imm_int1);
        }
        string vir_reg2 = createNewReg(false);
        fun_msg.new_name_map[reg_or_var2] = vir_reg2;
        _BB->addInst(ASM::Inst(ASM::Vmov, vir_reg2, vir_reg1));
        _BB->getInsts().rbegin()->setDef(vir_reg2);
        _BB->getInsts().rbegin()->setUse({vir_reg1});
        _BB->addInst(ASM::Inst(ASM::Itof, vir_reg2, vir_reg2));
        _BB->getInsts().rbegin()->setDef(vir_reg2);
        _BB->getInsts().rbegin()->setUse({vir_reg2});
    }

    if (inst->getSpecInst() == IR::Inst::sy_ftoi) {
        string vir_reg1;
        if (reg_or_var1 != "") {
            vir_reg1 = fun_msg.new_name_map[reg_or_var1];
        } else {
            vir_reg1 = getFloatImmVirReg(imm_float1);
        }
        string vir_reg2 = createNewReg();
        fun_msg.new_name_map[reg_or_var2] = vir_reg2;
        string tmp = createNewReg(false);
        _BB->addInst(ASM::Inst(ASM::Ftoi, tmp, vir_reg1));
        _BB->getInsts().rbegin()->setDef(tmp);
        _BB->getInsts().rbegin()->setUse({vir_reg1});
        _BB->addInst(ASM::Inst(ASM::Vmov, vir_reg2, tmp));
        _BB->getInsts().rbegin()->setDef(vir_reg2);
        _BB->getInsts().rbegin()->setUse({tmp});
    }

    if (inst->getSpecInst() == IR::Inst::sy_load) {
        if (_open_loop_opt && now_while_scope->getVar2Reg(reg_or_var1) != "") {
            fun_msg.new_name_map[reg_or_var2] = now_while_scope->getVar2Reg(reg_or_var1);
            return;
        }

        string vir_reg1;
        string vir_reg2;
        if (is_int2) {
            vir_reg2 = createNewReg();
        } else {
            vir_reg2 = createNewReg(false);
        }
        fun_msg.new_name_map[reg_or_var2] = vir_reg2;
        auto temp_BB = _BB;

        if (_open_loop_opt && _BB->getName().find("while.before") == 0) {
            if (isVar(reg_or_var1) && !getVar(reg_or_var1).is_global) {
                _BB = _preBB;
                now_while_scope->loop_var2reg[reg_or_var1] = vir_reg2;
            }
        }

        if (isVar(reg_or_var1)) {
            if (_BB->getVar2Reg().find(reg_or_var1) == _BB->getVar2Reg().end() ||
                _BB->getVar2Reg()[reg_or_var1].second < ASM::Inst::inst_order - 10) {
                ASM::Var v = getVar(reg_or_var1);
                if (v.is_global) {
                    vir_reg1 = getGlobalBase(reg_or_var1);
                    if (is_int2) {
                        _BB->addInst(ASM::Inst(ASM::Ldr, vir_reg2, vir_reg1));
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Vldr, vir_reg2, vir_reg1));
                    }
                    _BB->getInsts().rbegin()->setUse({vir_reg1});
                } else {
                    fun_msg.vir_reg_map[vir_reg2].base = "r7";
                    fun_msg.vir_reg_map[vir_reg2].offset = v.offset;
                    if (v.in_stack || v.offset >= 4096) {
                        string imm_reg = getIntImmVirReg(v.offset);
                        if (v.in_stack) {
                            fun_msg.vir_reg_map[vir_reg2].map2stack_var = true;
                            _BB->getInsts().rbegin()->setStackOffset(true);
                        }
                        if (is_int2) {
                            _BB->addInst(ASM::Inst(ASM::Ldr, vir_reg2, "r7", imm_reg));
                        } else {
                            _BB->addInst(ASM::Inst(ASM::Add, imm_reg, "r7", imm_reg));
                            _BB->getInsts().rbegin()->setUse({imm_reg});
                            _BB->getInsts().rbegin()->setDef(imm_reg);
                            _BB->addInst(ASM::Inst(ASM::Vldr, vir_reg2, imm_reg));
                        }
                        _BB->getInsts().rbegin()->setUse({imm_reg});
                    } else {
                        if (is_int2) {
                            _BB->addInst(ASM::Inst(ASM::Ldr, vir_reg2, "r7", "#" + to_string(v.offset)));
                        } else {
                            _BB->addInst(ASM::Inst(ASM::Vldr, vir_reg2, "r7", "#" + to_string(v.offset)));
                        }
                    }
                }
                auto list_end = _BB->getInsts().rbegin();
                list_end++;
                _BB->getLdrAndStrVarInst()[reg_or_var1].push_back(list_end.base());
            } else {
                string last_use = _BB->getVar2Reg()[reg_or_var1].first;
                if (is_int2)
                    _BB->addInst(ASM::Inst(ASM::Mov, vir_reg2, last_use));
                else
                    _BB->addInst(ASM::Inst(ASM::Vmov, vir_reg2, last_use));
                _BB->getInsts().rbegin()->setUse({last_use});
            }
            _BB->getVar2Reg()[reg_or_var1] = {vir_reg2, ASM::Inst::inst_order};
        } else {
            vir_reg1 = fun_msg.new_name_map[reg_or_var1];
            if (is_int2) {
                _BB->addInst(ASM::Inst(ASM::Ldr, vir_reg2, vir_reg1));
            } else {
                _BB->addInst(ASM::Inst(ASM::Vldr, vir_reg2, vir_reg1));
            }
            _BB->getInsts().rbegin()->setUse({vir_reg1});
        }
        _BB->getInsts().rbegin()->setDef(vir_reg2);
        if (_open_loop_opt && temp_BB->getName().find("while.before") == 0) {
            if (isVar(reg_or_var1) && !getVar(reg_or_var1).is_global) {
                _BB = temp_BB;
            }
        }

    }

    if (inst->getSpecInst() == IR::Inst::sy_store) {
        string vir_reg1;
        if (_open_loop_opt && now_while_scope->getVar2Reg(reg_or_var2) != "") {
            auto var2reg = now_while_scope->getVar2Reg(reg_or_var2);
            if (reg_or_var1 != "") {
                if (_BB->getInsts().rbegin()->getDef() == fun_msg.new_name_map[reg_or_var1]) {
                    _BB->getInsts().rbegin()->setResult(var2reg);
                    //为了上一条语句的def寄存器不会因未被使用而使这条语句被删除，同时保证循环变量对应的寄存器始终活跃
                    auto use = _BB->getInsts().rbegin()->getUse();
                    use.insert(var2reg);
                    _BB->getInsts().rbegin()->setUse(use);
                    _BB->getInsts().rbegin()->setDef(var2reg);
                } else {
                    if(is_int1){
                        _BB->addInst(ASM::Inst(ASM::Mov, var2reg, fun_msg.new_name_map[reg_or_var1]));
                    }else{
                        _BB->addInst(ASM::Inst(ASM::Vmov, var2reg, fun_msg.new_name_map[reg_or_var1]));
                    }
                    _BB->getInsts().rbegin()->setUse({fun_msg.new_name_map[reg_or_var1]});
                }
            } else {
                if (is_int1) {
                    uint32_t _int = imm_int1;
                    _BB->addInst(ASM::Inst(ASM::Movwt, var2reg, to_string(_int & 0x0000FFFF),
                                           to_string((_int >> 16) & 0x0000FFFF)));
                } else {
                    uint32_t *p = (uint32_t *) &imm_float1;
                    string tmp = getIntImmVirReg(*p);
                    _BB->addInst(ASM::Inst(ASM::Vmov, var2reg, tmp));
                    _BB->getInsts().rbegin()->setUse({tmp});
                }
            }
            now_while_scope->insertStore(reg_or_var2);
            return;
        }

        string vir_reg2;
        //不是立即数
        if (reg_or_var1 != "") {
            vir_reg1 = fun_msg.new_name_map[reg_or_var1];
        } else {
            if (is_int1) {
                vir_reg1 = getIntImmVirReg(imm_int1);
            } else {
                vir_reg1 = getFloatImmVirReg(imm_float1);
            }
        }

        if (isVar(reg_or_var2)) {
            ASM::Var v = getVar(reg_or_var2);
            if (v.is_global) {
                vir_reg2 = getGlobalBase(reg_or_var2);
                if (is_int1) {
                    _BB->addInst(ASM::Inst(ASM::Str, vir_reg1, vir_reg2));
                } else {
                    _BB->addInst(ASM::Inst(ASM::Vstr, vir_reg1, vir_reg2));
                }
                _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
            } else {
                if (v.in_stack || v.offset >= 4096) {
                    string imm_reg = getIntImmVirReg(v.offset);
                    if (v.in_stack) {
                        _BB->getInsts().rbegin()->setStackOffset(true);
                    }
                    if (is_int1) {
                        _BB->addInst(ASM::Inst(ASM::Str, vir_reg1, "r7", imm_reg));
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Add, imm_reg, "r7", imm_reg));
                        _BB->getInsts().rbegin()->setUse({imm_reg});
                        _BB->getInsts().rbegin()->setDef(imm_reg);
                        _BB->addInst(ASM::Inst(ASM::Vstr, vir_reg1, imm_reg));
                    }
                    _BB->getInsts().rbegin()->setUse({vir_reg1, imm_reg});
                } else {
                    if (is_int1) {
                        _BB->addInst(ASM::Inst(ASM::Str, vir_reg1, "r7", "#" + to_string(v.offset)));
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Vstr, vir_reg1, "r7", "#" + to_string(v.offset)));
                    }
                    _BB->getInsts().rbegin()->setUse({vir_reg1});
                }
            }
            _BB->getVar2Reg()[reg_or_var2] = {vir_reg1, ASM::Inst::inst_order};
            auto list_end = _BB->getInsts().rbegin();
            list_end++;
            _BB->getLdrAndStrVarInst()[reg_or_var2].push_back(list_end.base());
        } else {
            vir_reg2 = fun_msg.new_name_map[reg_or_var2];
            if (is_int1) {
                _BB->addInst(ASM::Inst(ASM::Str, vir_reg1, vir_reg2));
            } else {
                _BB->addInst(ASM::Inst(ASM::Vstr, vir_reg1, vir_reg2));
            }
            _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
        }
    }
}

void CodeGen::emitInstThree(std::shared_ptr<IR::InstThreeOp> inst) {
    string reg_or_var1, reg_or_var2, reg_or_var3;
    bool is_int1, is_int2, is_int3;
    int imm_int1, imm_int2, imm_int3;
    float imm_float1, imm_float2, imm_float3;
    string label1, label2;

    shared_ptr<IR::Value> value = inst->getOp1();
    getValueAttri(value, &is_int1, &reg_or_var1, &imm_int1, &imm_float1);

    value = dynamic_pointer_cast<IR::InstThreeOp>(inst)->getOp2();
    getValueAttri(value, &is_int2, &reg_or_var2, &imm_int2, &imm_float2, &label1);

    value = dynamic_pointer_cast<IR::InstThreeOp>(inst)->getOp3();
    getValueAttri(value, &is_int3, &reg_or_var3, &imm_int3, &imm_float3, &label2);

    if (inst->getSpecInst() == IR::Inst::sy_getptr) {
        string vir_reg1 = getArrayBase(reg_or_var1);
        string vir_reg2;

        string vir_reg3 = createNewReg();
        fun_msg.new_name_map[reg_or_var3] = vir_reg3;

        if (reg_or_var2 != "") {
            vir_reg2 = fun_msg.new_name_map[reg_or_var2];
            _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg1, vir_reg2));
            _BB->getInsts().rbegin()->setDef(vir_reg3);
            _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
        } else {
            if (imm_int2 <= 4096) {
                _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg1, "#" + to_string(imm_int2)));
                _BB->getInsts().rbegin()->setDef(vir_reg3);
                _BB->getInsts().rbegin()->setUse({vir_reg1});
            } else {
                vir_reg2 = getIntImmVirReg(imm_int2);
                _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg1, vir_reg2));
                _BB->getInsts().rbegin()->setDef(vir_reg3);
                _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
            }
        }
    }

    if (inst->getSpecInst() == IR::Inst::sy_add || inst->getSpecInst() == IR::Inst::sy_sub ||
        inst->getSpecInst() == IR::Inst::sy_mul || inst->getSpecInst() == IR::Inst::sy_div ||
        inst->getSpecInst() == IR::Inst::sy_mod) {
        string vir_reg1, vir_reg2, vir_reg3;
        if (reg_or_var1 != "") {
            vir_reg1 = fun_msg.new_name_map[reg_or_var1];
        }
        if (reg_or_var2 != "") {
            vir_reg2 = fun_msg.new_name_map[reg_or_var2];
        }
        if (is_int3) {
            vir_reg3 = createNewReg();
        } else {
            vir_reg3 = createNewReg(false);
        }
        fun_msg.new_name_map[reg_or_var3] = vir_reg3;

        switch (inst->getSpecInst()) {
            case IR::Inst::sy_add:
                //add指令最多一个立即数
                if (is_int3) {
                    if (reg_or_var1 != "" && reg_or_var2 != "") {
                        _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg1, vir_reg2));
                        _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                    } else if (reg_or_var1 != "") {
                        if (abs(imm_int2) <= 4096) {
                            _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg1, "#" + to_string(imm_int2)));
                            _BB->getInsts().rbegin()->setUse({vir_reg1});
                        } else {
                            string imm_reg = getIntImmVirReg(imm_int2);
                            _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg1, imm_reg));
                            _BB->getInsts().rbegin()->setUse({vir_reg1, imm_reg});
                        }
                    } else {
                        if (abs(imm_int1) <= 4096) {
                            _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg2, "#" + to_string(imm_int1)));
                            _BB->getInsts().rbegin()->setUse({vir_reg2});
                        } else {
                            string imm_reg = getIntImmVirReg(imm_int1);
                            _BB->addInst(ASM::Inst(ASM::Add, vir_reg3, vir_reg2, imm_reg));
                            _BB->getInsts().rbegin()->setUse({vir_reg2, imm_reg});
                        }
                    }
                } else {
                    if (reg_or_var1 == "") {
                        vir_reg1 = getFloatImmVirReg(imm_float1);
                    }
                    if (reg_or_var2 == "") {
                        vir_reg2 = getFloatImmVirReg(imm_float2);
                    }
                    _BB->addInst(ASM::Inst(ASM::Vadd, vir_reg3, vir_reg1, vir_reg2));
                    _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                }
                _BB->getInsts().rbegin()->setDef(vir_reg3);
                break;
            case IR::Inst::sy_sub:
                if (is_int3) {
                    if (reg_or_var1 != "" && reg_or_var2 != "") {
                        _BB->addInst(ASM::Inst(ASM::Sub, vir_reg3, vir_reg1, vir_reg2));
                        _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                    } else if (reg_or_var1 != "") {
                        if (abs(imm_int2) <= 4096) {
                            _BB->addInst(ASM::Inst(ASM::Sub, vir_reg3, vir_reg1, "#" + to_string(imm_int2)));
                            _BB->getInsts().rbegin()->setUse({vir_reg1});
                        } else {
                            string imm_reg = getIntImmVirReg(imm_int2);
                            _BB->addInst(ASM::Inst(ASM::Sub, vir_reg3, vir_reg1, imm_reg));
                            _BB->getInsts().rbegin()->setUse({vir_reg1, imm_reg});
                        }
                    } else if (reg_or_var2 != "") {
                        if (abs(imm_int2) <= 4096) {
                            _BB->addInst(ASM::Inst(ASM::Rsb, vir_reg3, vir_reg2, "#" + to_string(imm_int1)));
                            _BB->getInsts().rbegin()->setUse({vir_reg2});
                        } else {
                            string imm_reg = getIntImmVirReg(imm_int1);
                            _BB->addInst(ASM::Inst(ASM::Rsb, vir_reg3, vir_reg2, imm_reg));
                            _BB->getInsts().rbegin()->setUse({vir_reg2, imm_reg});
                        }
                    } else {
                        uint32_t _int = imm_int1 - imm_int2;
                        _BB->addInst(ASM::Inst(ASM::Movwt, vir_reg3, to_string(_int & 0x0000FFFF),
                                               to_string((_int >> 16) & 0x0000FFFF)));
                    }
                } else {
                    if (reg_or_var1 == "") {
                        vir_reg1 = getFloatImmVirReg(imm_float1);
                    }
                    if (reg_or_var2 == "") {
                        vir_reg2 = getFloatImmVirReg(imm_float2);
                    }
                    _BB->addInst(ASM::Inst(ASM::Vsub, vir_reg3, vir_reg1, vir_reg2));
                    _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                }
                _BB->getInsts().rbegin()->setDef(vir_reg3);
                break;
            case IR::Inst::sy_mul:
                if (reg_or_var1 == "") {
                    if (is_int1) {
                        vir_reg1 = getIntImmVirReg(imm_int1);
                    } else {
                        vir_reg1 = getFloatImmVirReg(imm_float1);
                    }
                }
                if (reg_or_var2 == "") {
                    if (is_int2) {
                        if (pow_map.find(imm_int2) == pow_map.end())
                            vir_reg2 = getIntImmVirReg(imm_int2);
                    } else {
                        vir_reg2 = getFloatImmVirReg(imm_float2);
                    }
                }
                if (is_int3) {
                    if (reg_or_var2 != "" || pow_map.find(imm_int2) == pow_map.end()) {
                        _BB->addInst(ASM::Inst(ASM::Mul, vir_reg3, vir_reg1, vir_reg2));
                        _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                        _BB->getInsts().rbegin()->setDef(vir_reg3);
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Lsl, vir_reg3, vir_reg1, to_string(pow_map[imm_int2])));
                        _BB->getInsts().rbegin()->setUse({vir_reg1});
                        _BB->getInsts().rbegin()->setDef(vir_reg3);
                    }
                } else {
                    _BB->addInst(ASM::Inst(ASM::Vmul, vir_reg3, vir_reg1, vir_reg2));
                    _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                }
                break;
            case IR::Inst::sy_div:
                if (reg_or_var1 == "") {
                    if (is_int1) {
                        vir_reg1 = getIntImmVirReg(imm_int1);
                    } else {
                        vir_reg1 = getFloatImmVirReg(imm_float1);
                    }
                }
                if (reg_or_var2 == "") {
                    if (is_int2) {
                        if (pow_map.find(imm_int2) == pow_map.end())
                            vir_reg2 = getIntImmVirReg(imm_int2);
                    } else {
                        vir_reg2 = getFloatImmVirReg(imm_float2);
                    }
                }
                if (is_int3) {
                    if (reg_or_var2 != "" || pow_map.find(imm_int2) == pow_map.end()) {
                        _BB->addInst(ASM::Inst(ASM::Div, vir_reg3, vir_reg1, vir_reg2));
                        _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                        _BB->getInsts().rbegin()->setDef(vir_reg3);
                    } else {
                        _BB->addInst(ASM::Inst(ASM::Asr, vir_reg3, vir_reg1, to_string(pow_map[imm_int2])));
                        _BB->getInsts().rbegin()->setUse({vir_reg1});
                        _BB->getInsts().rbegin()->setDef(vir_reg3);
                    }
                } else {
                    _BB->addInst(ASM::Inst(ASM::Vdiv, vir_reg3, vir_reg1, vir_reg2));
                    _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                }
                break;
            default:
                if (reg_or_var1 == "") {
                    vir_reg1 = getIntImmVirReg(imm_int1);
                }
                if (reg_or_var2 == "") {
                    vir_reg2 = getIntImmVirReg(imm_int2);
                }
                _BB->addInst(ASM::Inst(ASM::Div, vir_reg3, vir_reg1, vir_reg2));
                _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
                _BB->getInsts().rbegin()->setDef(vir_reg3);
                _BB->addInst(ASM::Inst(ASM::Mls, vir_reg3, vir_reg3, vir_reg2, vir_reg1));
                _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2, vir_reg3});
                _BB->getInsts().rbegin()->setDef(vir_reg3);
                break;
        }
    }

    if (inst->getSpecInst() == IR::Inst::sy_lt || inst->getSpecInst() == IR::Inst::sy_le ||
        inst->getSpecInst() == IR::Inst::sy_gt || inst->getSpecInst() == IR::Inst::sy_ge ||
        inst->getSpecInst() == IR::Inst::sy_equ || inst->getSpecInst() == IR::Inst::sy_nequ) {
        string vir_reg1, vir_reg2, vir_reg3;
        if (reg_or_var1 == "") {
            if (is_int1) {
                vir_reg1 = getIntImmVirReg(imm_int1);
            } else {
                vir_reg1 = getFloatImmVirReg(imm_float1);
            }
        } else {
            vir_reg1 = fun_msg.new_name_map[reg_or_var1];
        }
        if (reg_or_var2 == "") {
            if (is_int2) {
                vir_reg2 = getIntImmVirReg(imm_int2);
            } else {
                vir_reg2 = getFloatImmVirReg(imm_float2);
            }
        } else {
            vir_reg2 = fun_msg.new_name_map[reg_or_var2];
        }
        if (!is_int1 || !is_int2) {
            _BB->addInst(ASM::Inst(ASM::Vcmp, "", vir_reg1, vir_reg2));
        } else {
            _BB->addInst(ASM::Inst(ASM::Cmp, "", vir_reg1, vir_reg2));
        }
        _BB->getInsts().rbegin()->setUse({vir_reg1, vir_reg2});
        if (value->isCmpValue()) {
            switch (inst->getSpecInst()) {
                case IR::Inst::sy_lt:
                    fun_msg.jump_type = 1;
                    break;
                case IR::Inst::sy_le:
                    fun_msg.jump_type = 2;
                    break;
                case IR::Inst::sy_gt:
                    fun_msg.jump_type = 3;
                    break;
                case IR::Inst::sy_ge:
                    fun_msg.jump_type = 4;
                    break;
                case IR::Inst::sy_equ:
                    fun_msg.jump_type = 5;
                    break;
                case IR::Inst::sy_nequ:
                    fun_msg.jump_type = 6;
                    break;
                default:
                    break;
            }
        } else {
            vir_reg3 = createNewReg();
            fun_msg.new_name_map[reg_or_var3] = vir_reg3;
            switch (inst->getSpecInst()) {
                case IR::Inst::sy_lt:
                    _BB->addInst(ASM::Inst(ASM::Ite, "lt"));
                    _BB->addInst(ASM::Inst(ASM::Movlt, vir_reg3, "#1"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    _BB->addInst(ASM::Inst(ASM::Movge, vir_reg3, "#0"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    break;
                case IR::Inst::sy_le:
                    _BB->addInst(ASM::Inst(ASM::Ite, "le"));
                    _BB->addInst(ASM::Inst(ASM::Movle, vir_reg3, "#1"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    _BB->addInst(ASM::Inst(ASM::Movgt, vir_reg3, "#0"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    break;
                case IR::Inst::sy_gt:
                    _BB->addInst(ASM::Inst(ASM::Ite, "gt"));
                    _BB->addInst(ASM::Inst(ASM::Movgt, vir_reg3, "#1"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    _BB->addInst(ASM::Inst(ASM::Movle, vir_reg3, "#0"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    break;
                case IR::Inst::sy_ge:
                    _BB->addInst(ASM::Inst(ASM::Ite, "ge"));
                    _BB->addInst(ASM::Inst(ASM::Movge, vir_reg3, "#1"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    _BB->addInst(ASM::Inst(ASM::Movlt, vir_reg3, "#0"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    break;
                case IR::Inst::sy_equ:
                    _BB->addInst(ASM::Inst(ASM::Ite, "eq"));
                    _BB->addInst(ASM::Inst(ASM::Moveq, vir_reg3, "#1"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    _BB->addInst(ASM::Inst(ASM::Movne, vir_reg3, "#0"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    break;
                case IR::Inst::sy_nequ:
                    _BB->addInst(ASM::Inst(ASM::Ite, "ne"));
                    _BB->addInst(ASM::Inst(ASM::Movne, vir_reg3, "#1"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    _BB->addInst(ASM::Inst(ASM::Moveq, vir_reg3, "#0"));
                    _BB->getInsts().rbegin()->setDef(vir_reg3);
                    break;
                default:
                    break;
            }
        }
    }

    if (inst->getSpecInst() == IR::Inst::sy_if) {
        if (label1 != "")
            label1 = fun_msg.name + "." + label1;
        if (label2 != "")
            label2 = fun_msg.name + "." + label2;
        if (reg_or_var1 != "") {
            switch (fun_msg.jump_type) {
                case 1:
                    if (label1 != "")
                        _BB->addInst(ASM::Inst(ASM::Blt, label1));
                    if (label2 != "")
                        _BB->addInst(ASM::Inst(ASM::Bge, label2));
                    break;
                case 2:
                    if (label1 != "")
                        _BB->addInst(ASM::Inst(ASM::Ble, label1));
                    if (label2 != "")
                        _BB->addInst(ASM::Inst(ASM::Bgt, label2));
                    break;
                case 3:
                    if (label1 != "")
                        _BB->addInst(ASM::Inst(ASM::Bgt, label1));
                    if (label2 != "")
                        _BB->addInst(ASM::Inst(ASM::Ble, label2));
                    break;
                case 4:
                    if (label1 != "")
                        _BB->addInst(ASM::Inst(ASM::Bge, label1));
                    if (label2 != "")
                        _BB->addInst(ASM::Inst(ASM::Blt, label2));
                    break;
                case 5:
                    if (label1 != "")
                        _BB->addInst(ASM::Inst(ASM::Beq, label1));
                    if (label2 != "")
                        _BB->addInst(ASM::Inst(ASM::Bne, label2));
                    break;
                case 6:
                    if (label1 != "")
                        _BB->addInst(ASM::Inst(ASM::Bne, label1));
                    if (label2 != "")
                        _BB->addInst(ASM::Inst(ASM::Beq, label2));
                    break;
                default:
                    break;
            }
        } else {
            string vir_reg = getIntImmVirReg(imm_int1);
            _BB->addInst(ASM::Inst(ASM::Cmp, "", vir_reg, "#0"));
            _BB->getInsts().rbegin()->setUse({vir_reg});
            if (label1 != "")
                _BB->addInst(ASM::Inst(ASM::Bne, label1));
            if (label2 != "")
                _BB->addInst(ASM::Inst(ASM::Beq, label2));
        }

    }
}

string CodeGen::getGlobalBase(string var) {
    string result;
    if (_BB->getVarAddrReg().find(var) == _BB->getVarAddrReg().end()) {
        result = createNewReg();
        _BB->addInst(ASM::Inst(ASM::Movwt, result, ":lower16:" + var, "#:upper16:" + var));
        _BB->getInsts().rbegin()->setDef(result);
        if (_BB->getVarAddrReg().size() >= 5) {
            int min_use = INT32_MAX;
            string need_delete;
            for (auto &[key, value]: _BB->getVarAddrReg()) {
                if (value.second < min_use) {
                    min_use = value.second;
                    need_delete = key;
                }
            }
            _BB->getVarAddrReg().erase(need_delete);
        }
        _BB->getVarAddrReg()[var] = {result, ASM::Inst::inst_order};
    } else {
        result = _BB->getVarAddrReg()[var].first;
    }
    return result;
}

std::string CodeGen::createNewReg(bool is_int) {
    std::string reg_name = "reg_" + std::to_string(fun_msg.reg_order++);
    if (!is_int) {
        reg_name = "vreg_" + std::to_string(fun_msg.reg_order++);
        fun_msg.vir_reg_map[reg_name].is_int = false;
    }
    fun_msg.vir_reg_map[reg_name].name = reg_name;
    return reg_name;
}

std::string CodeGen::getFloatImmVirReg(float fp) {
    string result;
    uint32_t *p = (uint32_t *) &fp;
    string tmp = getIntImmVirReg(*p);
    result = createNewReg(false);
    _BB->addInst(ASM::Inst(ASM::Vmov, result, tmp));
    _BB->getInsts().rbegin()->setDef(result);
    _BB->getInsts().rbegin()->setUse({tmp});
    return result;
}

std::string CodeGen::getIntImmVirReg(uint32_t _int) {
    string result = createNewReg();
    _BB->addInst(ASM::Inst(ASM::Movwt, result, to_string(_int & 0x0000FFFF), to_string((_int >> 16) & 0x0000FFFF)));
    _BB->getInsts().rbegin()->setDef(result);
    return result;
}

std::string CodeGen::getArrayBase(std::string var) {
    string result;
    ASM::Var v = getVar(var);
    if (v.is_global) {
        result = getGlobalBase(var);
    } else {
        if (_BB->getVarAddrReg().find(var) == _BB->getVarAddrReg().end()) {
            result = createNewReg();
            if (v.is_param) {
                if (v.in_stack || v.offset >= 4096) {
                    string imm_reg = getIntImmVirReg(v.offset);
                    if (v.in_stack) {
                        _BB->getInsts().rbegin()->setStackOffset(true);
                    }
                    _BB->addInst(ASM::Inst(ASM::Ldr, result, "r7", imm_reg));
                    _BB->getInsts().rbegin()->setDef(result);
                    _BB->getInsts().rbegin()->setUse({imm_reg});
                } else {
                    _BB->addInst(ASM::Inst(ASM::Ldr, result, "r7", "#" + to_string(v.offset)));
                    _BB->getInsts().rbegin()->setDef(result);
                }
            } else {
                if (v.offset > 4096) {
                    string imm_reg = getIntImmVirReg(v.offset);
                    _BB->addInst(ASM::Inst(ASM::Add, result, "r7", imm_reg));
                    _BB->getInsts().rbegin()->setDef(result);
                    _BB->getInsts().rbegin()->setUse({imm_reg});
                } else {
                    _BB->addInst(ASM::Inst(ASM::Add, result, "r7", "#" + to_string(v.offset)));
                    _BB->getInsts().rbegin()->setDef(result);
                }
            }
            if (_BB->getVarAddrReg().size() >= 5) {
                int min_use = INT32_MAX;
                string need_delete;
                for (auto &[key, value]: _BB->getVarAddrReg()) {
                    if (value.second < min_use) {
                        min_use = value.second;
                        need_delete = key;
                    }
                }
                _BB->getVarAddrReg().erase(need_delete);
            }
            _BB->getVarAddrReg()[var] = {result, ASM::Inst::inst_order};
        } else {
            result = _BB->getVarAddrReg()[var].first;
        }
    }
    return result;
}

void
CodeGen::getValueAttri(shared_ptr<IR::Value> value, bool *is_int, string *reg_or_var, int *imm_int, float *imm_float,
                       string *label) {
    if (value->getType() == IR::IRType::IRInt32 || value->getType() == IR::IRType::IRIntPtr32 ||
        value->getType() == IR::IRType::IRFloatPtr32) {
        *is_int = true;
    } else {
        *is_int = false;
    }
    if (value->type() == IR::OpType::IRReg) {
        *reg_or_var = dynamic_pointer_cast<IR::Reg>(value)->getRegName();
    } else if (value->type() == IR::OpType::IRImmOp) {
        auto immediate = dynamic_pointer_cast<IR::ImmediateOp>(value);
        if (immediate->getType() == IR::IRType::IRInt32) {
            *imm_int = immediate->getIntValue();
        } else {
            *imm_float = immediate->getFloatValue();
        }
    } else {
        if (label)
            *label = dynamic_pointer_cast<IR::Label>(value)->getLabel();
        if (*label == _BB->getNextName())
            label->clear();
    }
}

void CodeGen::linkBB(shared_ptr<IR::BasicBlock> BB) {
    auto tmp = _fun->getBBMap()[BB->getBBName()];
    for (auto success: BB->getNexts()) {
        tmp->addSuccess(_fun->getBBMap()[success->getBBName()]);
    }
}

void CodeGen::genSpillCode() {
    set<string> spilled;
    for (auto &[name, BB]: _fun->getBBMap()) {
        auto &list = BB->getInsts();
        auto begin = list.begin();
        auto &vir_reg_map = fun_msg.vir_reg_map;
        while (begin != list.end()) {
            for (auto use: begin->getUse()) {
                auto &vir_reg = vir_reg_map[use];
                if (vir_reg.spill && !vir_reg.spilled) {
                    if (vir_reg.base == "sp" || vir_reg.map2stack_var || vir_reg.offset >= 4096) {
                        string imm_reg = createNewReg();
                        uint32_t offset = (uint32_t) vir_reg.offset;
                        ASM::Inst imm_inst(ASM::Movwt, imm_reg, to_string(offset & 0x0000FFFF),
                                           to_string((offset >> 16) & 0x0000FFFF));
                        imm_inst.setDef(imm_reg);
                        if (vir_reg.map2stack_var) {
                            imm_inst.setStackOffset(true);
                        }
                        if (vir_reg.base == "sp") {
                            imm_inst.setSpillOffset(true);
                        }
                        list.insert(begin, imm_inst);

                        if (vir_reg.is_int) {
                            ASM::Inst tmp_inst(ASM::Ldr, use, vir_reg.base, imm_reg);
                            tmp_inst.setDef(use);
                            tmp_inst.setUse({imm_reg});
                            list.insert(begin, tmp_inst);
                        } else {
                            ASM::Inst add_base(ASM::Add, imm_reg, vir_reg.base, imm_reg);
                            add_base.setUse({imm_reg});
                            add_base.setDef(imm_reg);
                            list.insert(begin, add_base);
                            ASM::Inst tmp_inst(ASM::Vldr, use, imm_reg);
                            tmp_inst.setDef(use);
                            tmp_inst.setUse({imm_reg});
                            list.insert(begin, tmp_inst);
                        }
                    } else {
                        if (vir_reg.is_int) {
                            ASM::Inst tmp_inst(ASM::Ldr, use, "r7", "#" + to_string(vir_reg.offset));
                            tmp_inst.setDef(use);
                            list.insert(begin, tmp_inst);
                        } else {
                            ASM::Inst tmp_inst(ASM::Vldr, use, "r7", "#" + to_string(vir_reg.offset));
                            tmp_inst.setDef(use);
                            list.insert(begin, tmp_inst);
                        }
                    }
                    spilled.insert(vir_reg.name);
                }
            }

            if (begin->getDef() != "") {
                string def = begin->getDef();
                auto &vir_reg = vir_reg_map[def];
                if (vir_reg.spill && !vir_reg.spilled) {
                    begin++;
                    string imm_reg = createNewReg();
                    uint32_t offset = (uint32_t) vir_reg.offset;
                    ASM::Inst imm_inst(ASM::Movwt, imm_reg, to_string(offset & 0x0000FFFF),
                                       to_string((offset >> 16) & 0x0000FFFF));
                    if (vir_reg.map2stack_var) {
                        imm_inst.setStackOffset(true);
                    }
                    if (vir_reg.base == "sp") {
                        imm_inst.setSpillOffset(true);
                    }
                    imm_inst.setDef(imm_reg);
                    list.insert(begin, imm_inst);
                    if (vir_reg.is_int) {
                        ASM::Inst tmp_inst(ASM::Str, def, vir_reg.base, imm_reg);
                        tmp_inst.setUse({def, imm_reg});
                        list.insert(begin, tmp_inst);
                    } else {
                        ASM::Inst add_sp(ASM::Add, imm_reg, vir_reg.base, imm_reg);
                        add_sp.setUse({imm_reg});
                        add_sp.setDef(imm_reg);
                        list.insert(begin, add_sp);
                        ASM::Inst tmp_inst(ASM::Vstr, def, imm_reg);
                        tmp_inst.setUse({def, imm_reg});
                        list.insert(begin, tmp_inst);
                    }
                    spilled.insert(vir_reg.name);
                    continue;
                }
            }
            begin++;
        }
    }
    for (auto is_spilled: spilled) {
        fun_msg.vir_reg_map[is_spilled].spilled = true;
    }
}

//迭代计算变量活跃性以及构建冲突图
void CodeGen::iterBuildInterfer() {
    while (true) {
        vector<set<string> > entry_live;
        vector<set<string> > entry_live2;
        for (auto &[name, BB]: _fun->getBBMap()) {
            entry_live.push_back(BB->getEntryLive());
            calcLiveAndBuildInterference(BB);
            entry_live2.push_back(BB->getEntryLive());
        }
        if (entry_live == entry_live2) {
            break;
        }
        clearInterference();
    }
    for (auto &[name, BB]: _fun->getBBMap()) {
        BB->setEntryLive(set<string>{});
    }
    deleteNoUse();
}

void CodeGen::calcLiveAndBuildInterference(std::shared_ptr<ASM::BasicBlock> BB) {
    set<string> live;
    for (auto bb: BB->getSuccess()) {
        for (auto live_reg: bb->getEntryLive()) {
            live.insert(live_reg);
        }
    }
    buildInterference(live);

    auto &vir_reg_map = fun_msg.vir_reg_map;
    auto rbegin = BB->getInsts().rbegin();
    auto rend = BB->getInsts().rend();
    while (rbegin != rend) {
        if (rbegin->getDef() != "") {
            live.erase(rbegin->getDef());
        }
        for (auto use: rbegin->getUse()) {
            live.insert(use);
            vir_reg_map[use].be_use++;
        }
        buildInterference(live);
        rbegin++;
    }
    BB->setEntryLive(live);
}

void CodeGen::buildInterference(std::set<std::string> live) {
    auto &vir_reg_map = fun_msg.vir_reg_map;
    for (auto reg: live) {
        auto &vir_reg = vir_reg_map[reg];
        for (auto reg1: live) {
            if (reg1 != reg && vir_reg.is_int == vir_reg_map[reg1].is_int &&
                vir_reg.conflict.find(reg1) == vir_reg.conflict.end()) {
                vir_reg.conflict.insert(reg1);
            }
        }
    }
}

void CodeGen::clearInterference() {
    for (auto &[name, vir]: fun_msg.vir_reg_map) {
        if (!vir.is_fixed)
            vir.phy_reg_name = "";
        vir.conflict.clear();
        vir.be_use = 0;
    }
}

void CodeGen::deleteNoUse() {
    bool ischange = false;
    for (auto &[name, BB]: _fun->getBBMap()) {
        auto begin = BB->getInsts().begin();
        while (begin != BB->getInsts().end()) {
            auto tmp = begin;
            begin++;
            string def = tmp->getDef();
            if (fun_msg.vir_reg_map.find(def) != fun_msg.vir_reg_map.end()) {
                if (fun_msg.vir_reg_map[def].be_use == 0) {
                    for (auto use: tmp->getUse()) {
                        if (fun_msg.vir_reg_map.find(use) != fun_msg.vir_reg_map.end()) {
                            fun_msg.vir_reg_map[use].be_use--;
                            ischange = true;
                        }
                    }
//                    cout << "delete" << endl;
//                    tmp->dumpInstruction(cout);
                    BB->getInsts().erase(tmp);
                    for (auto conflict: fun_msg.vir_reg_map[def].conflict) {
                        fun_msg.vir_reg_map[conflict].conflict.erase(def);
                    }
                    fun_msg.vir_reg_map.erase(def);
                }
            }
        }
    }
    if (ischange) {
        deleteNoUse();
    }
}

void CodeGen::replaceReg() {
    auto &vir_reg_map = fun_msg.vir_reg_map;
    for (auto &[name, BB]: _fun->getBBMap()) {
        for (auto &inst: BB->getInsts()) {
            if (vir_reg_map.find(inst.getResult()) != vir_reg_map.end()) {
                inst.setResult(vir_reg_map[inst.getResult()].phy_reg_name);
            }
            if (vir_reg_map.find(inst.getArg1()) != vir_reg_map.end()) {
                inst.setArg1(vir_reg_map[inst.getArg1()].phy_reg_name);
            }
            if (vir_reg_map.find(inst.getArg2()) != vir_reg_map.end()) {
                inst.setArg2(vir_reg_map[inst.getArg2()].phy_reg_name);
            }
            if (vir_reg_map.find(inst.getArg3()) != vir_reg_map.end()) {
                inst.setArg3(vir_reg_map[inst.getArg3()].phy_reg_name);
            }
        }
    }
}

bool CodeGen::colorReg() {
    bool int_success = false, float_success = false;
    auto vir_reg_map = fun_msg.vir_reg_map;

    auto f = [](pair<int, string> &p1, pair<int, string> &p2) { return p1.first < p2.first; };
    priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(f)> int_can_spill(f), float_can_spill(f);

    for (auto &[name, reg]: vir_reg_map) {
        if (!reg.spill && !reg.spilled) {
            if (reg.is_int)
                int_can_spill.push({reg.conflict.size(), reg.name});
            else
                float_can_spill.push({reg.conflict.size(), reg.name});
        }
    }

    struct Cmp {
        bool operator()(const string &s1, const string &s2) const {
            int first = stoi(s1.substr(1, s1.length() - 1));
            int second = stoi(s2.substr(1, s2.length() - 1));
            return first > second;
        }
    };


    set<string, Cmp> int_phy_reg = {"r0", "r1", "r2", "r3"};
    set<string, Cmp> float_phy_reg = {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "s12",
                                      "s13", "s14", "s15"};

    for (auto reg: fun_msg.need_push_regs) {
        int_phy_reg.insert(reg);
    }
    for (auto reg: fun_msg.need_push_fregs) {
        float_phy_reg.insert(reg);
    }
    while (!int_success) {
        for (auto reg: int_phy_reg) {
            for (auto &[name, vir_reg]: vir_reg_map) {
                if (vir_reg.is_int && vir_reg.phy_reg_name == "") {
                    bool can_color = true;
                    for (auto conflict: vir_reg.conflict) {
                        if (vir_reg_map[conflict].phy_reg_name == reg) {
                            can_color = false;
                            break;
                        }
                    }
                    if (can_color)
                        vir_reg.phy_reg_name = reg;
                }
            }
        }
        int_success = true;
        for (auto &[name, vir_reg]: vir_reg_map) {
            if (vir_reg.is_int && vir_reg.phy_reg_name == "") {
                int_success = false;
                break;
            }
        }
        if (int_success) {
            break;
        } else {
            if (!fun_msg.remain_regs.empty()) {
                int_phy_reg.insert(fun_msg.remain_regs.back());
                fun_msg.need_push_regs.push_back(fun_msg.remain_regs.back());
                fun_msg.remain_regs.pop_back();
            } else {
                break;
            }
        }
    }

    while (!float_success) {
        for (auto reg: float_phy_reg) {
            for (auto &[name, vir_reg]: vir_reg_map) {
                if (!vir_reg.is_int && vir_reg.phy_reg_name == "") {
                    bool can_color = true;
                    for (auto conflict: vir_reg.conflict) {
                        if (vir_reg_map[conflict].phy_reg_name == reg) {
                            can_color = false;
                            break;
                        }
                    }
                    if (can_color)
                        vir_reg.phy_reg_name = reg;
                }
            }
        }
        float_success = true;
        for (auto &[name, vir_reg]: vir_reg_map) {
            if (!vir_reg.is_int && vir_reg.phy_reg_name == "") {
                float_success = false;
                break;
            }
        }
        if (float_success) {
            break;
        } else {
            if (!fun_msg.remain_fregs.empty()) {
                float_phy_reg.insert(fun_msg.remain_fregs.back());
                fun_msg.need_push_fregs.push_back(fun_msg.remain_fregs.back());
                fun_msg.remain_fregs.pop_back();
            } else {
                break;
            }
        }
    }
    if (int_success && float_success) {
        fun_msg.vir_reg_map = vir_reg_map;
    } else {
        if (!int_success) {
            string spill_reg = int_can_spill.top().second;
            fun_msg.vir_reg_map[spill_reg].spill = true;
            if (fun_msg.vir_reg_map[spill_reg].base == "") {
                fun_msg.vir_reg_map[spill_reg].offset = fun_msg.spill_offset;
                fun_msg.vir_reg_map[spill_reg].base = "sp";
                fun_msg.spill_offset += 4;
            }
        }
        if (!float_success) {
            string spill_reg = float_can_spill.top().second;
            fun_msg.vir_reg_map[spill_reg].spill = true;
            if (fun_msg.vir_reg_map[spill_reg].base == "") {
                fun_msg.vir_reg_map[spill_reg].offset = fun_msg.spill_offset;
                fun_msg.vir_reg_map[spill_reg].base = "sp";
                fun_msg.spill_offset += 4;
            }
        }
    }
    return int_success && float_success;
}

