/**
 * Author:  Devotes
 * Date:    20220616
 *
 */

#include "sy_ir_builder.h"
using namespace std;
namespace IR{

    /*=-----------------------------=*/
    // IRBuilder
    /*=-----------------------------=*/

    VALUE SyIRBuilder::createFConstant(float v) {
        return VALUE(new ImmediateOp(v));
    }

    VALUE SyIRBuilder::createIConstant(int v) {
        return VALUE(new ImmediateOp(v));
    }

    VALUE SyIRBuilder::createFReg(const std::string &name) {
        return VALUE(new Reg(name, IRType::IRFloat32));
    }

    VALUE SyIRBuilder::createIReg(const std::string &name) {
        return VALUE(new Reg(name, IRType::IRInt32));
    }

    VALUE SyIRBuilder::createTempFReg() {
        ssa_use++;
        return VALUE(new Reg(to_string(ssa_use), IRType::IRFloat32));
    }

    VALUE SyIRBuilder::createTempIReg() {
        ssa_use++;
        return VALUE(new Reg(to_string(ssa_use), IRType::IRInt32));
    }

    VALUE SyIRBuilder::createFPE(const std::string &name) {
        return VALUE(new Reg(name, IRType::IRFloatPtr32));
    }

    VALUE SyIRBuilder::createIPE(const std::string &name) {
        return VALUE(new Reg(name, IRType::IRIntPtr32));
    }

    VALUE SyIRBuilder::createTempFPE() {
        ssa_use++;
        return VALUE(new Reg(to_string(ssa_use), IRType::IRFloatPtr32));
    }

    VALUE SyIRBuilder::createTempIPE() {
        ssa_use++;
        return VALUE(new Reg(to_string(ssa_use), IRType::IRIntPtr32));
    }

    VALUE SyIRBuilder::createLabel(const std::string &name, BASIC_BLOCK bb) {
        // @fixme 需不需要Label维护一个指向基本块的指针
        return VALUE(new Label(name, std::move(bb)));
    }

    GLOBAL_VAR_DEF SyIRBuilder::createFGlobalVariable(const std::string &name, float v) {
        return make_shared< GlobalVarDef>(name, v);
    }

    GLOBAL_VAR_DEF SyIRBuilder::createIGlobalVariable(const std::string &name, int v) {
        return make_shared< GlobalVarDef>(name, v);
    }

    GLOBAL_ARRAY_DEF SyIRBuilder::createIGlobalArray(const std::string &name, int width, const std::vector<int>& inits,const std::vector<int>& needs) {
        return make_shared<GlobalArrayDef>(name, inits, needs, width);
    }

    GLOBAL_ARRAY_DEF SyIRBuilder::createFGlobalArray(const std::string &name, int width, const std::vector<float>& inits,const std::vector<int>& needs) {
        return make_shared<GlobalArrayDef>(name, inits, needs,width);
    }

    FUNC_DEFINE SyIRBuilder::createFuncDef(const std::string &name, const FUNC_MSG_PTR& ptr, const std::vector<std::string>& param_names) {
        return make_shared<FunctionDef>(name, param_names, ptr);
    }

    FUNC_DECL SyIRBuilder::createFuncDecl(const std::string& name, const FUNC_MSG_PTR& ptr){
        return make_shared<FunctionDecl>(name, ptr);
    }

    BASIC_BLOCK SyIRBuilder::createBB(const std::string &name) {
        return make_shared<BasicBlock>(name);
    }

#define EMPTY nullptr

    INSTRUCTION SyIRBuilder::createFAlloca(VALUE result, const IR::BASIC_BLOCK& def, int w, bool isArray) {
        return INSTRUCTION(new InstAlloca(std::move(result), IRType::IRFloat32, w, def, isArray));
    }

    INSTRUCTION SyIRBuilder::createIAlloca(VALUE result, const IR::BASIC_BLOCK& def, int w, bool isArray) {
        return INSTRUCTION(new InstAlloca(std::move(result), IRType::IRInt32, w, def, isArray));
    }

    INSTRUCTION SyIRBuilder::createINot(VALUE arg1, VALUE result) {
        return INSTRUCTION(new InstTwoOp(std::move(arg1), std::move(result), Inst::sy_not));
    }

    INSTRUCTION SyIRBuilder::createIAdd(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_add));
    }

    INSTRUCTION SyIRBuilder::createISub(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_sub));
    }

    INSTRUCTION SyIRBuilder::createIDiv(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_div));

    }

    INSTRUCTION SyIRBuilder::createIMul(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_mul));

    }

    INSTRUCTION SyIRBuilder::createIMod(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_mod));
    }

    INSTRUCTION SyIRBuilder::createILe(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_le));
    }

    INSTRUCTION SyIRBuilder::createILt(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_lt));
    }

    INSTRUCTION SyIRBuilder::createIGe(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_ge));
    }

    INSTRUCTION SyIRBuilder::createIGt(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_gt));
    }

    INSTRUCTION SyIRBuilder::createIEqual(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_equ));
    }

    INSTRUCTION SyIRBuilder::createINEqual(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_nequ));
    }

    INSTRUCTION SyIRBuilder::createIOr(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_or));
    }

    INSTRUCTION SyIRBuilder::createIAnd(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_and));
    }

    INSTRUCTION SyIRBuilder::createIGetPtr(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION(new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_getptr));
    }

    INSTRUCTION SyIRBuilder::createIJump(VALUE arg1) {
        return INSTRUCTION(new InstOneOp(std::move(arg1), Inst::sy_goto));
    }

    INSTRUCTION SyIRBuilder::createICmp(VALUE arg1, VALUE arg2, VALUE result) {
        return INSTRUCTION( new InstThreeOp(std::move(arg1), std::move(arg2), std::move(result), Inst::sy_if));
    }

    INSTRUCTION SyIRBuilder::createIParam(VALUE arg1, const std::string& func, int ord, bool is_reverse) {
        return INSTRUCTION(new InstParam(std::move(arg1), func, ord, is_reverse));
    }

    INSTRUCTION SyIRBuilder::createIFuncCall(const std::string& name, int paraNum, int intParaNum, int floatParaNum, bool reverse, VALUE ret) {
        return INSTRUCTION(new InstFuncCall(name, std::move(ret), paraNum, intParaNum, floatParaNum, reverse));
    }

    INSTRUCTION SyIRBuilder::createIRet(VALUE arg1) {
        return INSTRUCTION(new InstOneOp(std::move(arg1), Inst::sy_ret));
    }

    INSTRUCTION SyIRBuilder::createILoad(VALUE arg1, VALUE result) {
        return INSTRUCTION(new InstTwoOp(std::move(arg1), std::move(result), Inst::sy_load));
    }

    INSTRUCTION SyIRBuilder::createIStore(VALUE arg1, VALUE result) {
        return INSTRUCTION(new InstTwoOp(std::move(arg1), std::move(result), Inst::sy_store));
    }

    INSTRUCTION SyIRBuilder::createIItof(VALUE arg1, VALUE result) {
        return INSTRUCTION(new InstTwoOp(std::move(arg1), std::move(result), Inst::sy_itof));
    }

    INSTRUCTION SyIRBuilder::createIFtoi(VALUE arg1, VALUE result) {
        return INSTRUCTION(new InstTwoOp(std::move(arg1), std::move(result), Inst::sy_ftoi));
    }

    INSTRUCTION SyIRBuilder::createIMemSet(VALUE arg1, const std::string &name, int number) {
        return INSTRUCTION(new InstMemorySet(name, std::move(arg1), number));
    }

    std::shared_ptr<IR::InstThreeOp> SyIRBuilder::createIEmptyCmp(VALUE arg1) {
        return make_shared<IR::InstThreeOp>(std::move(arg1), Inst::sy_if);
    }


}

