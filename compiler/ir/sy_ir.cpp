/**
 * Author:  Devotes
 * Date:    20220615
 *
 *
 */

#include "sy_ir.h"
#include <algorithm>
#include <set>
#include <cassert>

using namespace std;

namespace IR{

    string toType(IRType ty){
        switch (ty) {
            case IRType::IRFloatPtr32:  return "float*";
            case IRType::IRIntPtr32:    return "int*";
            case IRType::IRFloat32:     return "float";
            case IRType::IRVoid32:      return "void";
            case IRType::IRInt32:       return "int";
        }
    }

    /*=-----------------------------=*/
    // 寄存器类型
    /*=-----------------------------=*/
    Reg::Reg(const std::string &name, IRType ty) : Value(ty), reg_name(name){}
    std::string Reg::toIrStr() { return toType(ty)+ " %" + reg_name + (isSinceUseIntervalFuncCall?" <inv> ":""); }

    /*=-----------------------------=*/
    // 立即数类型
    /*=-----------------------------=*/
    ImmediateOp::ImmediateOp(int v) : Value(IRType::IRInt32), v(v), v1(0){}
    ImmediateOp::ImmediateOp(float v) : Value(IRType::IRFloat32), v(0), v1(v){}
    std::string ImmediateOp::toIrStr() { return ty == IRType::IRInt32? to_string(v): to_string(v1); }

    /*=-----------------------------=*/
    // 标签类型
    /*=-----------------------------=*/
    Label::Label(const std::string &name, BASIC_BLOCK bb) : Value(IRType::IRVoid32), bb(std::move(bb)), label(name){}
    std::string Label::toIrStr() { return "<" + label + ">" + (isNext?" IsNext ":""); }

    void Label::resetBB(const BASIC_BLOCK &rbb) {
        bb = rbb;
        label = rbb->getBBName();
    }

    /*=-----------------------------=*/
    // 指令基类
    // getInstType 根据具体的指令操作返回指令类型
    // InstType::phi        --->    sy_phi
    // InstType::func_call  --->    sy_call
    // InstType::alloca     --->    sy_alloca
    // InstType::one        --->    sy_param, sy_goto, sy_ret
    // InstType::two        --->    sy_not, sy_itof, sy_ftoi, sy_store, sy_load
    // InstType::three      --->    other
    /*=-----------------------------=*/
    InstType Instruction::getInstType() {
        const static set<Inst> one_sy_set = {Inst::sy_param, Inst::sy_goto, Inst::sy_ret};
        const static set<Inst> two_inst_set = {Inst::sy_not, Inst::sy_itof, Inst::sy_ftoi, Inst::sy_store, Inst::sy_load};

        switch(ty){
            case Inst::sy_call:
                return InstType::func_call;
            case Inst::sy_alloca:
                return InstType::alloca;
            case Inst::sy_phi:
                return InstType::phi;
            case Inst::sy_param:
                return InstType::param;
            case Inst::sy_memset:
                return InstType::memset;
            case Inst::sy_mov:
                return InstType::mov;
            default:
                break;
        }

        if(one_sy_set.find(ty) != one_sy_set.end())
            return InstType::one;

        if(two_inst_set.find(ty) != two_inst_set.end())
            return InstType::two;

        return InstType::three;
    }

    /*=-----------------------------=*/
    // param指令
    /*=-----------------------------=*/
    InstParam::InstParam(VALUE para, const std::string &name, int ord, bool reverse) : Instruction(Inst::sy_param), para(para), whichFunc(name), order(ord), isReverse(reverse){}

    void InstParam::dump(std::ostream &dump, int number) {
            dump<<"\t\t"<<number<<". ";
            dump<<"param "<<para->toIrStr()<<" to "<<whichFunc<<"\n";
    }

    /*=-----------------------------=*/
    // Phi指令
    /*=-----------------------------=*/
    void InstPhi::dump(std::ostream &dump, int number) {
        dump<<"\t\t"<<number<<". "<<result->toIrStr()<<" = Phi(";
        int i = 0;
        int len = phiList.size() - 1;
        for(auto& phi : phiList){
            dump<<"<"<<phi.first->toIrStr()<<" : "<<phi.second->toIrStr()<<">";
            if(i < len)
                dump<<", ";
            i++;
        }
        dump<<")"<<endl;
    }

    /*=-----------------------------=*/
    // 函数调用指令
    /*=-----------------------------=*/
    InstFuncCall::InstFuncCall(const std::string &name, VALUE ret, int paraNum, int intParaNum, int floatParaNum, bool reverse) : Instruction(Inst::sy_call), name(name), ret(std::move(ret))
                                ,int_param_num(intParaNum), param_number(paraNum), float_param_num(floatParaNum), is_reverse(reverse){}

    void InstFuncCall::dump(std::ostream &dump, int number) {
        if(ret)
            dump<<"\t\t"<<number<<". call "<<name<<" "<<ret->toIrStr()<<"\n";
        else
            dump<<"\t\t"<<number<<". call "<<name<<"\n";
    }

    /*=-----------------------------=*/
    // 变量空间申请操作数的指令
    /*=-----------------------------=*/
    InstAlloca::InstAlloca(VALUE result, IRType ty, unsigned int w, const IR::BASIC_BLOCK& def, bool isArray) : Instruction(Inst::sy_alloca), ty(ty), result(std::move(result)), w(w), isArray(isArray), defBB(def){}

    void InstAlloca::dump(std::ostream& dump, int number) {
        dump<<"\t\t"<<number<<". ";
        dump<<result->toIrStr()<<" = alloca ";
        if(isArray){
            dump<<"["<<toType(ty)<<" x "<<to_string(w/4)<<"] align 4"<<"\n";
        }else{
            dump<<toType(ty)<<" align 4 define in "<<defBB->getBBName()<<"\n";
        }
    }

    /*=-----------------------------=*/
    // memset的指令
    /*=-----------------------------=*/
    void InstMemorySet::dump(std::ostream &dump, int number) {
       dump<<"\t\t"<<number<<". ";
       dump<<"memset array "<<arrayName<<" type: "<<(value->ty==IRType::IRFloatPtr32?"float":"int")<<" elem number: "<<this->number<<endl;
    }

    /*=-----------------------------=*/
    // mov的指令
    /*=-----------------------------=*/
    void InstMov::dump(std::ostream &dump, int number) {
      dump<<"\t\t"<<number<<". ";
      dump<<"mov "<<arg1->toIrStr()<<" to "<<result->toIrStr()<<endl;
    }

    /*=-----------------------------=*/
    // 单个操作数的指令
    /*=-----------------------------=*/
    InstOneOp::InstOneOp(VALUE result, Inst ty) : result(std::move(result)), Instruction(ty){}

    void InstOneOp::dump(std::ostream& dump, int number) {
        dump<<"\t\t"<<number<<". ";
        switch (ty) {
            case Inst::sy_ret:
                if(result)
                    dump<<"ret "<<result->toIrStr()<<"\n";
                else
                    dump<<"ret\n";
                break;
            case Inst::sy_goto:
                dump<<"goto "<<result->toIrStr()<<"\n";
                break;
            default:
                break;
        }
    }

    /*=-----------------------------=*/
    // 两个操作数的指令
    /*=-----------------------------=*/
    InstTwoOp::InstTwoOp(VALUE arg1, VALUE result, Inst ty) : arg1(std::move(arg1)), result(std::move(result)), Instruction(ty){}

    void InstTwoOp::dump(std::ostream& dump, int number) {
        dump<<"\t\t"<<number<<". ";
        switch (ty) {
            case Inst::sy_not:
                dump<<result->toIrStr()<<"="<<" not "<<arg1->toIrStr()<<"\n";
                break;
            case Inst::sy_itof:
                dump<<result->toIrStr()<<"= itof "<<arg1->toIrStr()<<"\n";
                break;
            case Inst::sy_ftoi:
                dump<<result->toIrStr()<<"= ftoi "<<arg1->toIrStr()<<"\n";
                break;
            case Inst::sy_load:
                dump<<"load "<<arg1->toIrStr()<<" to "<<result->toIrStr()<<"\n";
                break;
            case Inst::sy_store:
                dump<<"store "<<arg1->toIrStr()<<" to "<<result->toIrStr()<<"\n";
                break;
            default:
                break;
        }
    }

    /*=-----------------------------=*/
    // 三个操作数的指令
    /*=-----------------------------=*/
    InstThreeOp::InstThreeOp(VALUE arg1, Inst ty) : arg1(std::move(arg1)), arg2(nullptr), result(nullptr), Instruction(ty){}

    InstThreeOp::InstThreeOp(VALUE arg1, VALUE arg2, VALUE result, Inst ty) : arg1(std::move(arg1)), arg2(std::move(arg2)), result(std::move(result)), Instruction(ty){}

    void InstThreeOp::dump(std::ostream& dump, int number) {
        dump<<"\t\t"<<number<<". ";
        switch(ty){
            case Inst::sy_add:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" + "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_sub:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" - "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_mul:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" * "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_div:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" / "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_mod:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" % "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_le:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" <= "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_lt:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" < "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_ge:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" >= "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_gt:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" > "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_equ:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" == "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_nequ:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" != "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_and:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" && "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_or:
                dump<<result->toIrStr()<<" = "<<arg1->toIrStr()<<" || "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_getptr:
                dump<<result->toIrStr()<<" = getptr "<<arg1->toIrStr()<<", "<<arg2->toIrStr()<<"\n";
                break;
            case Inst::sy_if:
                dump<<"if "<<arg1->toIrStr()<<" "<<arg2->toIrStr()<<" : "<<result->toIrStr()<<"\n";
                break;
            default:
                break;
        }
    }

    /*=-----------------------------=*/
    // 基本块定义
    /*=-----------------------------=*/
    BasicBlock::BasicBlock(const std::string &name) : name(name){}

    // @fixme 如果后期需要知道从当前基本块能到达哪个基本块，可以将next的类型从BasicBlock* 变成 std::vector<BasicBlock*>
    void BasicBlock::addInst(const INSTRUCTION& inst) {
        insts.push_back(inst);
        if(inst->getSpecInst() == Inst::sy_goto){
            auto nextBB = dynamic_pointer_cast<Label>(dynamic_pointer_cast<InstOneOp>(inst)->getOp())->getBB();
            hasJump = true;
            nextBB->addPrevBB(shared_from_this());
            addNextBB(nextBB);
        }else if(inst->getSpecInst() == Inst::sy_if){
            auto instThree = dynamic_pointer_cast<InstThreeOp>(inst);
            hasJump = true;
            if(instThree->getOp2() && instThree->getOp3()){
                auto nextBB1 = dynamic_pointer_cast<Label>(instThree->getOp2())->getBB();
                auto nextBB2 = dynamic_pointer_cast<Label>(instThree->getOp3())->getBB();
                nextBB1->addPrevBB(shared_from_this());
                nextBB2->addPrevBB(shared_from_this());
                addNextBB(nextBB1);
                addNextBB(nextBB2);
            }
        }
    }

    void BasicBlock::addPrevBB(const BASIC_BLOCK &prev) {
        auto it = find(prevs.begin(), prevs.end(), prev);
        if(it == prevs.end())
            prevs.push_back(prev);
    }

    void BasicBlock::addNextBB(const BASIC_BLOCK &next) {
        auto it = find(nexts.begin(), nexts.end(), next);
        if(it == nexts.end())
            nexts.push_back(next);
    }

    const std::vector<BASIC_BLOCK> &BasicBlock::getCPrevs() const {
        return prevs;
    }

    const std::vector<BASIC_BLOCK> &BasicBlock::getCNexts() const {
        return nexts;
    }

    std::vector<BASIC_BLOCK> BasicBlock::getPrevs() const {
        return prevs;
    }

    std::vector<BASIC_BLOCK> BasicBlock::getNexts() const {
        return nexts;
    }

    bool BasicBlock::removePrev(const BASIC_BLOCK &prev) {
        auto it = find(prevs.begin(), prevs.end(), prev);
        if(it != prevs.end()){
            auto bb = static_pointer_cast<BasicBlock>(*it);
            prevs.erase(it);
            return true;
        }
        return false;
    }

    bool BasicBlock::removeNext(const BASIC_BLOCK &next) {
        auto it = find(nexts.begin(), nexts.end(), next);
        if(it != nexts.end()){
            nexts.erase(it);
            return true;
        }
        return false;
    }

    void BasicBlock::insertInstToTheFront(const INSTRUCTION &inst) {
        insts.insert(insts.begin(), inst);
    }

    void BasicBlock::insertInstToTheFront(const std::vector<INSTRUCTION> &insts) {
        this->insts.insert(this->insts.begin(), insts.begin(), insts.end());
    }

    void BasicBlock::insertInstTo(const INSTRUCTION &inst) {
        if(insts.empty())
            insts.push_back(inst);
        else{
            auto jump = insts.back();
            insts.pop_back();
            insts.push_back(inst);
            insts.push_back(jump);
        }
    }

    void BasicBlock::replaceTo(const INSTRUCTION &new_inst, const INSTRUCTION &old_inst) {
        std::replace(this->insts.begin(), this->insts.end(), old_inst, new_inst);
    }

    void BasicBlock::dump(std::ostream &out, int& number) {
        out<<"\t"<<name<<":\n";
        for(auto& inst : insts){
            inst->dump(out, number++);
        }
        out<<"\n";
    }

    void BasicBlock::removeInst(const std::shared_ptr<Instruction> &inst) {
        auto it = find(insts.begin(), insts.end(), inst);
        if(it != insts.end()){
            insts.erase(it);
        }
    }

    /*=-----------------------------=*/
    // 函数定义
    /*=-----------------------------=*/
    FunctionDef::FunctionDef(const std::string& name, const std::vector<std::string>& param_names, const FUNC_MSG_PTR& ptr) : fun_name(name),
        msgPtr(ptr), pparam_names(param_names){
        entryBB = shared_ptr<BasicBlock>(new BasicBlock("entry"));
    }

    void FunctionDef::addBB(const BASIC_BLOCK& bb) {
        otherBB.push_back(bb);
    }

    void FunctionDef::dump(std::ostream &out, int& number) {
        out<<"define @"<<toType(msgPtr->first)<<" "<<fun_name<<"(";
        unsigned int i = 0;
        unsigned int len = msgPtr->second.size()-1;
        auto it = pparam_names.begin();
        for(const auto&p: msgPtr->second){
            if(i < len){
                out<<toType(p)<<" %"<<*it<<", ";
            }else{
                out<<toType(p)<<" %"<<*it;
            }
            i++;
            it++;
        }
        out<<"):\n";
        number++;
        entryBB->dump(out, number);
        for(auto& bb: otherBB){
            number++;
            bb->dump(out, number);
        }

        out<<"\n";
    }

    void FunctionDef::removeBB(const BASIC_BLOCK& bb) {
        auto it = find(otherBB.begin(), otherBB.end(), bb);
        for(auto& node : bb->getPrevs())
            node->removeNext(bb);
        for(auto& node : bb->getNexts())
            node->removePrev(bb);
//        assert(it != otherBB.end());
        otherBB.erase(it);
    }

    bool FunctionDef::isBB2NextToTheBB1(const IR::BASIC_BLOCK &bb1, const IR::BASIC_BLOCK &bb2) {
        if(bb1->getBBName() == entryBB->getBBName()){
            if(otherBB.empty())
              return false;
            if(bb2->getBBName() == otherBB[0]->getBBName())
              return true;
            return false;
        }

        int i = 0;
        for(; i < otherBB.size() - 1 ; ++i){
          if(bb1->getBBName() == otherBB[i]->getBBName()){
            if(bb2->getBBName() == otherBB[i+1]->getBBName())
              return true;
            else
              break;
          }
        }
        return false;
    }


    /*=-----------------------------=*/
    // 函数声明
    /*=-----------------------------=*/
    FunctionDecl::FunctionDecl(const std::string& name, const FUNC_MSG_PTR& ptr) : fun_name(name),
                                                                                    msgPtr(ptr){}

    void FunctionDecl::dump(std::ostream &out) {
        out<<"declare @"<<toType(msgPtr->first)<<" "<<fun_name<<"(";
        unsigned int i = 0;
        unsigned int len = msgPtr->second.size()-1;
        for(const auto&p: msgPtr->second){
            if(i < len){
                out<<toType(p)<<", ";
            }else{
                out<<toType(p);
            }
            i++;
        }
        out<<")\n\n";
    }

    /*=-----------------------------=*/
    // 全局变量定义
    /*=-----------------------------=*/
    GlobalVarDef::GlobalVarDef(const std::string &name, int v) : name(name), iv(v), fv(0){
        ty = IRType::IRInt32;
    }

    GlobalVarDef::GlobalVarDef(const std::string &name, float v) : name(name), iv(0), fv(v){
        ty = IRType::IRFloat32;
    }

    void GlobalVarDef::dump(std::ostream &out) {
        out<<"global "<<toType(ty)<<" @"<<name<<" = ";
        if(ty == IRType::IRInt32){
            out<<to_string(iv)<<"\n";
        }else{
            out<<to_string(fv)<<"\n";
        }
    }

    /*=-----------------------------=*/
    // 全局数组定义
    /*=-----------------------------=*/
    GlobalArrayDef::GlobalArrayDef(const std::string &name, const std::vector<int> &inits,const std::vector<int>& needs,
                                   unsigned int w) : name(name), f_initial_list(), i_initial_list(inits), need_initail(needs),width(w){
        ty = IRType::IRInt32;
    }

    GlobalArrayDef::GlobalArrayDef(const std::string &name, const std::vector<float> &inits,const std::vector<int>& needs, unsigned int w) :
    name(name), f_initial_list(inits), i_initial_list(),need_initail(needs), width(w){
        ty = IRType::IRFloat32;
    }

#define MAX_SHOW 10
    void GlobalArrayDef::dump(std::ostream &out) {
        out<<"global ["<<toType(ty)<<" x "<<to_string(width/4)<<"] "<<name<<" = ";
        // 初始化数组处理
        if(ty == IRType::IRInt32){
            if(i_initial_list.empty()){
                out<<"ZeroInitialized\n";
            }
            else{
                if(i_initial_list.size() <= MAX_SHOW){
                    unsigned int i = 0;
                    unsigned int len = i_initial_list.size()-1;
                    out<<"[";
                    for(auto& v: i_initial_list){
                        if(i < len)
                            out<<to_string(v)<<", ";
                        else
                            out<<to_string(v)<<"]\n";
                        i++;
                    }
                }else{
                    out<<"["<<to_string(i_initial_list[0])<<", "<<to_string(i_initial_list[1])<<", ";
                    out<<to_string(i_initial_list[2])<<", ..."<<to_string(i_initial_list[i_initial_list.size()-1])<<"]\n";
                }
            }
        }else{
            if(f_initial_list.empty()){
                out<<"ZeroInitialized\n";
            }
            else{
                if(f_initial_list.size() <= MAX_SHOW){
                    unsigned int i = 0;
                    unsigned int len = f_initial_list.size()-1;
                    out<<"[";
                    for(auto& v: f_initial_list){
                        if(i < len)
                            out<<to_string(v)<<", ";
                        else
                            out<<to_string(v)<<"]\n";
                        i++;
                    }
                }else{
                    out<<"["<<to_string(f_initial_list[0])<<", "<<to_string(f_initial_list[1])<<", ";
                    out<<to_string(f_initial_list[2])<<", ..."<<to_string(f_initial_list[f_initial_list.size()-1])<<"]\n";
                }
            }
        }
    }

    /*=-----------------------------=*/
    // 模块定义
    /*=-----------------------------=*/
    Module::Module(const std::string &name) : name(name), global_vars(), global_arrays(), func_defs(), func_decls(){}

    void Module::addGlobalArrayDef(const GLOBAL_ARRAY_DEF& def) {
        global_arrays.push_back(def);
    }

    void Module::addGlobalVarDef(const GLOBAL_VAR_DEF& def) {
        global_vars.push_back(def);
    }

    void Module::addFuncDef(const FUNC_DEFINE& def) {
        func_defs.push_back(def);
    }

    void Module::addFuncDecl(const FUNC_DECL& decl) {
        func_decls.push_back(decl);
    }

    void Module::dump(std::ostream &out) {
        out<<"Module: "<<name<<"\n";
        out<<"// GlobalVariableDefine\n";
        for(auto& global_var: global_vars)
            global_var->dump(out);
        out<<"// GlobalArrayDefine\n";
        for(auto& global_array: global_arrays)
            global_array->dump(out);
        out<<"// FunctionDeclare\n";
        for(auto& decl: func_decls)
            decl->dump(out);
        out<<"// FunctionDef\n";
        int i = 0;
        for(auto& def: func_defs)
            def->dump(out, i);
    }

}
