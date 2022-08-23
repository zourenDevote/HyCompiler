/// @author zouren
/// @brief： mem2reg的目的是为了将store/load类型的SSA IR类型转化为带有
/// Phi节点的类型，这样的IR尽可能少的使用load和shore指令，减少寄存器与内
/// 存交互的部分，尽量使得操作都在寄存器上进行，并且可以很好的进行优化操作。
///

#ifndef COMPILER_MEM2REG_PASS_H
#define COMPILER_MEM2REG_PASS_H


#include <map>
#include <set>
#include <list>
#include <stack>
#include <vector>
#include <iomanip>
#include "promotable_analyis_pass.h"


namespace opt_pass{

    class DominatorTreeNode;

    typedef std::shared_ptr<DominatorTreeNode> DTNode;

    /* 支配结点定义 */
    class DominatorTreeNode : public std::enable_shared_from_this<DominatorTreeNode>{
    public:
        explicit DominatorTreeNode(const IR::BASIC_BLOCK& bb);

        /// getPrevIndex - 获取当前DTNode的在CFG中的前驱节点的Label列表
        std::vector<std::string> getPrevIndex();
        /// getNextIndex - 获取当前DTNode在CFG中的后继节点的Label列表
        std::vector<std::string> getNextIndex();

        [[nodiscard]] std::string getDtName() const { return bb->getBBName(); }

        /// getDfn - 获取当前DTNode的后逆序编号
        unsigned int getDfn() const { return dfn; }

        /// setDfn - 设置当前DTNode的后逆序编号
        void setDfn(unsigned int order) { dfn = order; }

        /// addChild - 添加DTNode子节点
        void addChild(const DTNode& child);

        /// getChilds - 获取DTNode的子节点
        [[nodiscard]] std::vector<DTNode> getChilds() const;
        [[nodiscard]] const std::vector<DTNode>& getCChilds() const;

        /// setIdom - 设置Idom直接支配者
        void setIdom(const DTNode& node) { idom = node; }

        /// setSdom - 设置Sdom半支配者
        void setSdom(const DTNode& node) { sdom = node; }

        /// getIdom - 获取当前DTNode的直接支配者
        [[nodiscard]] DTNode getIdom() const { return idom; }

        /// getSdom - 获取当前DTNode的半支配者
        [[nodiscard]] DTNode getSdom() const { return sdom; }
        unsigned int getSdomDfn() const { return sdom->dfn; }

        /// initSdom - 初始化Sdom为自身
        void initSdom(){ sdom = shared_from_this(); }

        /// setParent - 设置父节点
        void setParent(const DTNode& father) { parent = father; }

        /// getParent - 获取DFS Tree的父节点
        [[nodiscard]] DTNode getParent() const { return parent; }

        /// dumpNode - 打印DT Tree
        void dumpNode(std::ostream& out);

        /// getBB - 获取DTNode对应的BASIC_BLOCK
        [[nodiscard]] IR::BASIC_BLOCK getBB() const { return bb; }
        [[nodiscard]] const IR::BASIC_BLOCK& getCBB() const { return bb; }

    private:
        DTNode idom;
        DTNode sdom;
        DTNode parent;
        std::vector<DTNode> childs;
        unsigned int dfn;
        IR::BASIC_BLOCK bb;
    };

    /* 支配者树 */
    class DominatorTree{
    public:
        DominatorTree(const IR::BASIC_BLOCK& entryBB);

        /// isDominate - 判断dom是否支配bdom
        bool isDominate(const std::string& dom, const std::string& bdom);

        /// isStrictDominate - 判断dom是否严格支配bdom
        bool isStrictDominate(const std::string& dom, const std::string& bdom);

        /// getDominateFrontier - 获取节点DTNode的支配边界
        /// 支配边界的定义为，我们约定 a sdom d, 表示a严格支配d，prev(d) 表示d在CFG中的前驱节点
        /// DominateFrontier(v) = DF(v) = { w | v dom prev(w) 且 !(v sdom w) }
        std::vector<IR::BASIC_BLOCK> getDominateFrontier(const std::string& bbName);

        IR::BASIC_BLOCK getBbByName(const std::string& name){
            return nodeMap[name]->getBB();
        }

        /// @brief 下面的两个方法适用于测试算法的准确性，提供一个直观的检查机制
        /// dumpDominatorTree - 输出基本快之间的直接支配关系
        void dumpDominatorTree(std::ostream& out){         setiosflags(std::ios::left); root->dumpNode(out); }

        /// dumpDominatorFrontier - 输出各个节点的支配边界
        void dumpDominatorFrontier(std::ostream& out);

        /// sedminator - 寻找半支配者的算法
        void sedminator();

        /// semi_nca - semi_nca支配树构造算法
        void semi_nca();

        /// execute - 暴露的外部的一个方法
        /// @todo 后续会考虑引入LLVM的PASS，采用优化遍的方式进行优化
        void execute(){
            semi_nca();
            calcuDFSetOfEachNode();
        }

        [[nodiscard]] const DTNode&  getRootNode() const { return root; }

    private:
        /// dfs - dfs构建CFG的DFS Tree和构建其逆后序
        void dfs(const IR::BASIC_BLOCK& vertex, std::set<std::string>& visit, int& n, const DTNode& parent);

        /// eval - 辅助查找节点半支配者算法的方法
        DTNode eval(DTNode& u, DTNode& v);

        /// calcuDFSetOfEachNode - 为CFG中的每一个节点求取其支配边界，并且存储起来方便快速查询，提升速度
        /// @brief: 算法伪代码
        ///     for each node in CFG:
        ///         DF[node] = empty
        ///     for each node in CFG:
        ///         if node's predecessors >= 2:
        ///             for pred in node's predecessors:
        ///                 runner = pred
        ///                 while(IDom(node) != runner)
        ///                     DF[runner] U= {node}
        ///                     runner = IDom(runner)
        void calcuDFSetOfEachNode();

        DTNode root;
        /// 帮助快速找到DTNode的前驱节点
        std::map<std::string, DTNode> nodeMap;
        /// 帮助快速找到DTNode的DmoinatorFrontier
        std::map<std::string, std::vector<IR::BASIC_BLOCK>> dfMap;
        /// CFG进行Deep First Search的逆后序组织列表
        std::vector<DTNode> reversePreOrderList;
        /// 辅助字段
        std::set<std::string> visit;
    };

    class Mem2RegPass: public Pass{
    public:
        Mem2RegPass() = default;

        void execute(IR::MODULE& m) override;
    private:
        // beforeInsertPhiNode - 在插入Phi节点的准备工作
        void beforeInsertPhiNode(const IR::FUNC_DEFINE& define);
        // insertPhiForPerCfg - 为控制流图插入Phi节点
        void insertPhiForPerCfg(DominatorTree& dtTree);
        // renameVar - 变量替换和重命名
        void renameVar(const DTNode& node);
        // Cfg中变量定义的集合
        std::map<std::string, IR::IRType> Globals;
        // Cfg中变量定义的位置
        std::map<std::string, std::vector<std::string>> Blocks;
        // alloca初始化的位置
        std::map<std::string, std::map<std::string, bool>> allocaMap;
        // 记录cfg中各个节点的映射，方便快速找到这个节点
        std::map<std::string, IR::BASIC_BLOCK> blockMap;
        // 记录变量使用的基本快的位置
        std::map<std::string, std::set<std::string>> varUseMap;
        bool isInsertPhi{false};
        std::string currFuncName;
        // 辅助定义
        std::map<IR::VALUE, IR::VALUE> r2rMap;
        std::map<std::string, std::shared_ptr<IR::Label>> labMap;

        struct NameStack{
            int counter{0};
            std::vector<IR::VALUE> valueStack;
            std::vector<std::shared_ptr<IR::Label>> labelStack;
        };

        /* 名称分配器 */
        std::map<std::string, NameStack> nameAllocator;
        /// 初始化名称分配器
        void initNameAllocator();
        /// 分配一个新的名字
        void newName(const std::string& name, const std::shared_ptr<IR::Label>& label, const IR::VALUE& v);
    };

}
#endif //COMPILER_MEM2REG_PASS_H
