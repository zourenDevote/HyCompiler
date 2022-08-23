/**
 * Author:  Devotes
 * Date:    20220615
 *
 *FixLog:
 *  @fixme 后期可以将指针类型替换成shared_ptr来管理内存空间
 */


#ifndef COMPILER_SY_IR_H
#define COMPILER_SY_IR_H
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <memory>

#define DEF_FRIEND_CLASS \
                friend class SyIRBuilder;   \
                friend class IRGenerate;    \
                friend class SyScope;       \
                friend class BasicBlock;


namespace IR{

    class BasicBlock;
    class Value;
    class Instruction;
    class GlobalVarDef;
    class GlobalArrayDef;
    class FunctionDef;
    class FunctionDecl;
    class Module;

    typedef  std::shared_ptr<BasicBlock> BASIC_BLOCK;
    typedef  std::shared_ptr<Value>  VALUE;
    typedef  std::shared_ptr<Instruction> INSTRUCTION;
    typedef  std::shared_ptr<GlobalVarDef> GLOBAL_VAR_DEF;
    typedef  std::shared_ptr<GlobalArrayDef> GLOBAL_ARRAY_DEF;
    typedef  std::shared_ptr<FunctionDef> FUNC_DEFINE;
    typedef  std::shared_ptr<FunctionDecl> FUNC_DECL;
    typedef  std::shared_ptr<Module> MODULE;

    enum class IRType{
        IRInt32,
        IRVoid32,
        IRFloat32,
        IRIntPtr32,
        IRFloatPtr32
    };

    // 函数记录表
    typedef std::pair<IRType, std::vector<IRType>> FUNC_MSG;
    typedef std::shared_ptr<FUNC_MSG> FUNC_MSG_PTR;

    /*=-----------------------------=*/
    // 操作数类型的定义
    // 1.寄存器
    //      1. 寄存器内部存放的是指针
    //      2. 寄存器内部存放的是数值
    // 2.立即数
    //      1. 整数型立即数
    //      2. 浮点型立即数
    /*=-----------------------------=*/
    enum class OpType{
        IRReg,
        IRImmOp,
        IRLabel,
    };

    /*=-----------------------------=*/
    // 指令类型的枚举类型：
    // 1.
    /*=-----------------------------=*/
    enum class Inst{
        sy_not,     sy_add,     sy_sub,
        sy_mul,     sy_div,     sy_mod,
        sy_le,      sy_lt,      sy_ge,
        sy_gt,      sy_equ,     sy_nequ,
        sy_itof,    sy_ftoi,    sy_load,
        sy_store,   sy_getptr,  sy_goto,
        sy_if,      sy_alloca,  sy_call,
        sy_ret,     sy_param,   sy_and,
        sy_or,      sy_phi,     sy_memset,
        sy_mov
    };

    enum class InstType{
        one,
        two,
        three,
        alloca,
        func_call,
        param,
        phi,
        memset,
        mov
    };

    class Value{
    public:
        DEF_FRIEND_CLASS

        explicit Value(IRType ty) : ty(ty){}

        IRType ty;
        virtual OpType type() = 0;
        virtual std::string toIrStr() = 0;
        [[nodiscard]] inline IRType getType() const {return ty;}

        void setCmpValue(){ iscmpValue = true; }
        bool isCmpValue() const { return iscmpValue; }

     private:
        bool iscmpValue{false};
    };

    class Reg: public Value{
    public:
        DEF_FRIEND_CLASS

        Reg(const std::string& name, IRType ty);
        OpType type() override { return OpType::IRReg; }
        std::string toIrStr() override;

        [[nodiscard]] std::string getRegName() const { return reg_name; }
        [[nodiscard]] bool getUseInterval() const { return isSinceUseIntervalFuncCall; }
        void resetName(const std::string& name) { reg_name = name; }
        void setIsSinceUseIntervalFuncCall(bool isInterval) { isSinceUseIntervalFuncCall = isInterval; }

    private:
        std::string reg_name;
        bool isSinceUseIntervalFuncCall{false};
    };

    class ImmediateOp: public Value{
    public:
        DEF_FRIEND_CLASS

        explicit ImmediateOp(int v);
        explicit ImmediateOp(float v);
        OpType type() override{ return OpType::IRImmOp; }
        std::string toIrStr() override;

        [[nodiscard]] int getIntValue() const { return v; }
        [[nodiscard]] float getFloatValue() const { return v1; }
    private:
        int v;
        float v1;
    };

    class Label: public Value{
    public:
        DEF_FRIEND_CLASS

        Label(const std::string& name, BASIC_BLOCK bb);
        OpType type() override { return OpType::IRLabel; }
        std::string toIrStr() override;

        void setIsNext() { isNext = true; }
        bool getIsNext() const { return isNext; }

        [[nodiscard]] std::string getLabel() const { return label; }
        [[nodiscard]] BASIC_BLOCK getBB() const { return bb; }
        void resetBB(const BASIC_BLOCK& rbb);
    private:
        bool isNext{false};
        std::string label;
        BASIC_BLOCK bb;
    };

    /*=-----------------------------=*/
    // 指令一个基本指令
    // 指令以双向链表的形式组织
    /*=-----------------------------=*/
    class Instruction{
    public:

        Instruction(Inst ty): ty(ty){}

        // 获取指令的类的类型
        InstType getInstType();

        // 获取具体的指令
        [[nodiscard]] Inst getSpecInst() const { return ty; }
        virtual void dump(std::ostream& dump, int number) = 0;

        Inst ty;
    };

#define EINST :public Instruction

    class InstPhi EINST{
    public:
        DEF_FRIEND_CLASS

        InstPhi(const std::string& var, const VALUE& res) : Instruction(Inst::sy_phi), result(res), var(var){}

        void addPhiParameter(const std::pair<VALUE, std::shared_ptr<Label>>& para){
            for(const auto& value : phiList){
                if(value.second->getLabel() == para.second->getLabel())
                    return;
            }
            phiList.push_back(para);
        }
        [[nodiscard]] const std::vector<std::pair<VALUE, std::shared_ptr<Label>>>& getPhiList() const{ return phiList; }
        [[nodiscard]] VALUE getResult() const{ return result; }
        [[nodiscard]] std::string getVarName() const { return var; }

        void dump(std::ostream &dump, int number) override;
    private:
        std::vector<std::pair<VALUE, std::shared_ptr<Label>>> phiList;
        std::string var;
        VALUE result;
    };

    class InstFuncCall EINST{
    public:
        DEF_FRIEND_CLASS
        InstFuncCall(const std::string& name, VALUE ret, int paraNum, int intParaNum, int floatParaNum, bool reverse);
        void dump(std::ostream &dump, int number) override;

        [[nodiscard]] VALUE getRetValue() const { return ret; }
        [[nodiscard]] std::string getFuncName() const { return name; }
        void replaceRetValue(const VALUE& v){ ret = v; }

        //需添加的功能
        void setParamNum(int num){ param_number=num; }
        void setIntParamNum(int num) { int_param_num=num; }
        void setFloatParamNum(int num) { float_param_num=num; }
        void setReverse(bool rev) { is_reverse=rev; }

        int getParamNum(){return param_number;}
        int getIntParamNum(){return int_param_num;}
        int getFlaotParamNum(){return float_param_num;}
        bool isReverse() { return is_reverse; }
    private:
        VALUE ret;
        std::string name;

        int int_param_num;
        int float_param_num;
        int param_number;
        bool is_reverse;
    };

    // 空间申请指令
    class InstAlloca EINST{
    public:
        DEF_FRIEND_CLASS

        InstAlloca(VALUE result, IRType ty, unsigned int w, const IR::BASIC_BLOCK& def, bool isArray = false);
        void dump(std::ostream& dump, int number) override;

        [[nodiscard]] unsigned int getWidth() const { return w; }
        [[nodiscard]] bool isArrayAlloca() const { return isArray; }
        [[nodiscard]] IRType getVarType() const { return ty; }
        [[nodiscard]] VALUE  getVarValue() const { return result; }
        [[nodiscard]] BASIC_BLOCK getDefBB() const { return defBB; }


    private:
        unsigned int w;              // 申请的空间长度
        IRType ty;                  // 类型
        VALUE result;               // 地址存放的目标寄存器
        bool isArray;
        BASIC_BLOCK defBB;          // 定义的基本块的位置
    };

    class InstParam EINST{
    public:
        DEF_FRIEND_CLASS

        InstParam(VALUE para, const std::string& name, int ord, bool reverse = false);
        void dump(std::ostream &dump, int number) override;

        [[nodiscard]] VALUE getPara() const { return para; }
        void replacePara(const VALUE& p){ para = p; }
        [[nodiscard]] std::string getFuncToParam() const { return whichFunc; }

        void setOrder(int ord){ order = ord; }
        void setTypeOrder(int ord){ type_order = ord; }
        void setReverse(bool reverse){ isReverse = reverse; }


        [[nodiscard]] int getTypeOrder() const { return type_order; }
        [[nodiscard]] int getOrder() const { return order; }
        [[nodiscard]] bool getReverse() const { return isReverse; }


    private:
        VALUE para;
        std::string whichFunc;
        int type_order;
        int order;
        bool isReverse;

    };

    // memery set 指令
    class InstMemorySet EINST{
     public:
        DEF_FRIEND_CLASS

        InstMemorySet(const std::string& name, const IR::VALUE& value, int number): arrayName(name), number(number), value(value), Instruction(Inst::sy_memset) {}

        void dump(std::ostream &dump, int number) override;

        std::string getName(){ return arrayName; }

        int getSize() { return number; }
     private:
        int number;
        std::string arrayName;
        VALUE value;
    };

    // mov 指令
    class InstMov EINST{
     public:
      DEF_FRIEND_CLASS

      InstMov(const VALUE& arg1, const VALUE& result) : arg1(arg1), result(result), Instruction(Inst::sy_mov){}

      void dump(std::ostream &dump, int number) override;

      [[nodiscard]] VALUE getArg1Value() const { return arg1; }
      [[nodiscard]] VALUE getResultValue() const { return result; }
      void replaceArg1Value(const VALUE& v) { arg1 = v; }
      void replaceResultValue(const VALUE& v) { result = v; }

     private:
        VALUE arg1;
        VALUE result;
    };


    // 单操作数指令
    class InstOneOp EINST{
    public:
        DEF_FRIEND_CLASS

        InstOneOp(VALUE result, Inst ty);
        void dump(std::ostream& dump, int number) override;

        [[nodiscard]] VALUE getOp() const {return result;}
        void replaceOp(const VALUE& res) { result = res; }
    private:
        VALUE result;
    };

    // 双操作数指令
    class InstTwoOp EINST{
    public:
        DEF_FRIEND_CLASS

        InstTwoOp(VALUE arg1, VALUE result, Inst ty);
        void dump(std::ostream& dump, int number) override;

        [[nodiscard]] VALUE getArg1Value() const { return arg1; }
        [[nodiscard]] VALUE getResultValue() const { return result; }
        void replaceArg1(const VALUE& op1) { arg1 = op1; }
        void replaceRes(const VALUE& res) { result = res; }
    private:
        VALUE arg1;
        VALUE result;
    };

    // 三操作数指令
    class InstThreeOp EINST{
    public:
        DEF_FRIEND_CLASS

        InstThreeOp(VALUE arg1, Inst ty);
        InstThreeOp(VALUE arg1, VALUE arg2, VALUE result, Inst ty);
        void dump(std::ostream& dump, int number) override;

        [[nodiscard]] VALUE getOp1() const { return arg1; }
        [[nodiscard]] VALUE getOp2() const { return arg2; }
        [[nodiscard]] VALUE getOp3() const { return result; }
        void replaceOp1(const VALUE& op1) { arg1 = op1; }
        void replaceOp2(const VALUE& op2) { arg2 = op2; }
        void replaceOp3(const VALUE& res) { result = res; }
    private:
        VALUE arg1;
        VALUE arg2;
        VALUE result;
    };


    /*=-----------------------------=*/
    // 基本块定义
    /*=-----------------------------=*/
    class BasicBlock: public std::enable_shared_from_this<BasicBlock>{
    public:
        DEF_FRIEND_CLASS

        explicit BasicBlock(const std::string& name);

        void dump(std::ostream& out, int& number);

        void addInst(const INSTRUCTION& inst);
        void insertInstToTheFront(const INSTRUCTION& inst);
        void insertInstToTheFront(const std::vector<INSTRUCTION>& insts);
        void insertInstTo(const INSTRUCTION& inst);
        void replaceTo(const INSTRUCTION& new_inst, const INSTRUCTION& old_inst);
        void removeInst(const INSTRUCTION& inst);
        void removeInst(const std::list<INSTRUCTION>::iterator & it){ insts.erase(it); }

        void addPrevBB(const BASIC_BLOCK& prev);
        void addNextBB(const BASIC_BLOCK& next);

        /// @brief 这两个函数的定义是为了快速高效的获取此基本快的前驱和后继节点，方便遍历，对调用这个成员函数返回的对象仅限读取操作
        [[nodiscard]] const std::vector<BASIC_BLOCK>& getCPrevs() const;
        [[nodiscard]] const std::vector<BASIC_BLOCK>& getCNexts() const;
        [[nodiscard]] std::vector<BASIC_BLOCK> getPrevs() const;
        [[nodiscard]] std::vector<BASIC_BLOCK> getNexts() const;

        /// 获取指令的条数
        [[nodiscard]] size_t instCount() const { return insts.size(); }

        /// setNext 设置当前基本块在逻辑顺序下的基本块的bbName
        void setNextBBName(const std::string& bn ) { nextName = bn; }
        std::string getNextBBName() { return nextName; }

        /// @brief 移除当前基本快的某个前驱节点或后继节点
        /// @return 成功 -- true, 失败 -- false
        bool removePrev(const BASIC_BLOCK& prev);
        bool removeNext(const BASIC_BLOCK& next);

        [[nodiscard]] std::list<INSTRUCTION> getInsts() const { return insts; }
        [[nodiscard]] const std::list<INSTRUCTION>& getCInsts() const { return insts; }

        bool isJump() const { return hasJump; }
        std::string getBBName() const { return name; }

    private:
        bool hasJump{false};
        std::string name;
        std::string nextName;
        std::list<INSTRUCTION> insts{};
        std::vector<BASIC_BLOCK> prevs{};
        std::vector<BASIC_BLOCK> nexts{};

    };

    /*=-----------------------------=*/
    // 函数定义
    // makelist(i): 创建一个只包含i的列表，标号就是i，makelist返回值个指向新创建列表的指针
    // merge(p1, p2): 将p1和p2指向的列表进行合并，它返回的指针指向合并后的列表
    // backpatch(p, i): 将i作为目标标号插入到p指的列表中的个指令中
    /*=-----------------------------=*/
    class FunctionDef{
    public:
        DEF_FRIEND_CLASS

        FunctionDef(const std::string& name, const std::vector<std::string>& param_names, const FUNC_MSG_PTR& ptr);
        void dump(std::ostream& out, int& number);

        void addBB(const BASIC_BLOCK& bb);
        void removeBB(const BASIC_BLOCK& bb);

        [[nodiscard]] BASIC_BLOCK getEntryBB() const { return entryBB; }
        [[nodiscard]] const BASIC_BLOCK& getCEntryBB() const { return entryBB; }
        [[nodiscard]] std::vector<BASIC_BLOCK> getOtherBB() const { return otherBB; }
        [[nodiscard]] const std::vector<BASIC_BLOCK>& getCOtherBB() const { return otherBB; }
        [[nodiscard]] std::string getFuncName() const { return fun_name; }
        [[nodiscard]] std::vector<IRType> getParams() const { return msgPtr->second; }
        [[nodiscard]] std::vector<std::string> getParamsName() const { return pparam_names; }
        [[nodiscard]] IRType getRetType() const { return msgPtr->first; }
        [[nodiscard]] bool isBB2NextToTheBB1(const IR::BASIC_BLOCK& bb1,  const IR::BASIC_BLOCK& bb2);


    private:
        BASIC_BLOCK entryBB;
        std::vector<BASIC_BLOCK> otherBB;
        std::vector<std::string> pparam_names;
        std::string fun_name;

        FUNC_MSG_PTR msgPtr;
    };

    /*=-----------------------------=*/
    // 函数声明
    /*=-----------------------------=*/
    class FunctionDecl{
    public:
        DEF_FRIEND_CLASS

        FunctionDecl(const std::string& name, const FUNC_MSG_PTR& ptr);
        void dump(std::ostream& out);

        [[nodiscard]] IRType getRetType() const { return msgPtr->first; }
        [[nodiscard]] std::vector<IRType> getParams(){ return msgPtr->second; }
        [[nodiscard]] std::string getFuncName() const { return fun_name; }

    private:
        std::string fun_name;
        FUNC_MSG_PTR msgPtr;
    };

    /*=-----------------------------=*/
    // 全局变量
    /*=-----------------------------=*/
    class GlobalVarDef{
    public:
        DEF_FRIEND_CLASS

        GlobalVarDef(const std::string& name, int v);
        GlobalVarDef(const std::string& name, float v);
        void dump(std::ostream& out);

        [[nodiscard]] std::string getVarName() const { return name; }
        [[nodiscard]] int getIntValue() const { return iv; }
        [[nodiscard]] float getFloatValue() const { return fv; }
        [[nodiscard]] IRType getVarType() const { return ty; }

    private:
        std::string name;
        int iv;
        float fv;
        IRType ty;
    };

    class GlobalArrayDef{
    public:
        DEF_FRIEND_CLASS

        GlobalArrayDef(const std::string& name, const std::vector<int>& inits, const std::vector<int>& needs, unsigned int w);
        GlobalArrayDef(const std::string& name, const std::vector<float>& inits,  const std::vector<int>& needs,unsigned int w);

        void dump(std::ostream& out);

        [[nodiscard]] std::string getArrayName() const { return name; }
        [[nodiscard]] unsigned int getWidth() const { return width; }
        [[nodiscard]] std::vector<int> getIntList() const { return i_initial_list; }
        [[nodiscard]] std::vector<float> getFloatList() const { return f_initial_list; }
        [[nodiscard]] std::vector<int> getNeed() const { return need_initail; }
        [[nodiscard]] IRType getArrayType() const { return ty; }

    private:
        std::string name;
        // dims定义用于方便后续数组寻址
        IRType ty;
        unsigned int width;
        std::vector<int> i_initial_list{};
        std::vector<float> f_initial_list{};
        std::vector<int> need_initail{};
    };

    /*=-----------------------------=*/
    // 模块定义
    /*=-----------------------------=*/
    class Module{
    public:
        DEF_FRIEND_CLASS;

        explicit Module(const std::string& name);

        void addGlobalVarDef(const GLOBAL_VAR_DEF& def);
        void addGlobalArrayDef(const GLOBAL_ARRAY_DEF& def);
        void addFuncDef(const FUNC_DEFINE& def);
        void addFuncDecl(const FUNC_DECL& decl);
        void dump(std::ostream& out);

        // 函数定义，全局变量，数组变量获取
        [[nodiscard]] std::vector<FUNC_DEFINE>              getFuncDefs()           const   { return func_defs; }
        [[nodiscard]] const std::vector<FUNC_DEFINE>&       getCFuncDefs()          const   { return func_defs; }
        [[nodiscard]] std::vector<GLOBAL_VAR_DEF>           getGlobalVarDef()       const   { return global_vars; }
        [[nodiscard]] const std::vector<GLOBAL_VAR_DEF>&    getCGlobalVarDef()      const   { return global_vars; }
        [[nodiscard]] std::vector<GLOBAL_ARRAY_DEF>         getGlobalArrayDef()     const   { return global_arrays; }
        [[nodiscard]] const std::vector<GLOBAL_ARRAY_DEF>&  getCGlobalArrayDef()    const   { return global_arrays; }
        [[nodiscard]] std::string                           getModuleName()         const   { return name; }

    private:
        std::string name;
        std::vector<GLOBAL_VAR_DEF>     global_vars;
        std::vector<GLOBAL_ARRAY_DEF>   global_arrays;
        std::vector<FUNC_DEFINE>        func_defs;
        std::vector<FUNC_DECL>          func_decls;
    };
}

#endif //COMPILER_SY_IR_H
