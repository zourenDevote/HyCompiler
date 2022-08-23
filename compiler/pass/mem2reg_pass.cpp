/// @author devotes
///

#include "mem2reg_pass.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <functional>


using namespace std;

namespace opt_pass{

    DominatorTreeNode::DominatorTreeNode(const IR::BASIC_BLOCK &bb) : bb(bb),
        idom(nullptr), sdom(nullptr), parent(nullptr), dfn(0){}

    void DominatorTreeNode::addChild(const DTNode &child) { childs.push_back(child); }

    std::vector<DTNode> DominatorTreeNode::getChilds() const { return childs; }
    const std::vector<DTNode>& DominatorTreeNode::getCChilds() const { return childs; }

    std::vector<std::string> DominatorTreeNode::getPrevIndex() {
        vector<std::string> prevIndex;
        for(auto& prev : bb->getPrevs())
            prevIndex.push_back(prev->getBBName());
        return prevIndex;
    }

    std::vector<std::string> DominatorTreeNode::getNextIndex() {
        vector<string> nextIndex;
        for(auto& next : bb->getNexts())
            nextIndex.push_back(next->getBBName());
        return nextIndex;
    }

    void DominatorTreeNode::dumpNode(std::ostream &out) {
        for(auto& child : childs){
            std::string bb_name = "<" + bb->getBBName() + ">";
            out<<setw(13)<< bb_name <<" idom ";
            bb_name = "<" + child->getDtName() + ">\n";
            out<<" "<<bb_name;
        }

        for(auto& child : childs)
            child->dumpNode(out);
    }

    DominatorTree::DominatorTree(const IR::BASIC_BLOCK &entryBB) : root(nullptr) {
        int n = 1;
        set<string> visit{};
        dfs(entryBB, visit, n, nullptr);
    }

    bool DominatorTree::isDominate(const std::string &dom, const std::string &bdom) {
        if(nodeMap.find(dom) != nodeMap.end() && nodeMap.find(bdom) != nodeMap.end())
            return false;
        else{
            auto dt1 = nodeMap[dom];
            auto dt2 = nodeMap[bdom];
            while(dt1 != nullptr){
                if(dt1 == dt2)
                    return true;
                dt2 = dt2->getIdom();
            }
            return false;
        }
    }

    bool DominatorTree::isStrictDominate(const std::string &dom, const std::string &bdom) {
        if(isDominate(dom, bdom)){
            if(dom != bdom)
                return true;
        }
        return false;
    }

    void DominatorTree::dumpDominatorFrontier(std::ostream &out) {
        setiosflags(ios::left);
        for(auto& v : dfMap){
            out<<setw(13)<<v.first;
            out<<"' dominate frontier is {";
            int i = 0;
            int len = v.second.size() - 1;
            for(auto& dfBB : v.second){
                if(i < len)
                    out<<dfBB->getBBName()<<", ";
                else
                    out<<dfBB->getBBName();
                i++;
            }
            out<<"}\n";
        }
    }

    void DominatorTree::calcuDFSetOfEachNode() {
        for(auto& node : nodeMap)
            dfMap[node.first] = {};
        for(auto& node : nodeMap){
            auto predecessors = node.second->getPrevIndex();
            if(predecessors.size() > 1){
                for(auto& pred : predecessors){
                    auto runner = nodeMap[pred];
                    while(runner != nullptr && node.second->getIdom() != runner){
                        auto it = std::find(dfMap[runner->getDtName()].begin(), dfMap[runner->getDtName()].end(), node.second->getBB());
                        if(it == dfMap[runner->getDtName()].end())
                            dfMap[runner->getDtName()].push_back(node.second->getBB());
                        runner = runner->getIdom();
                    }
                }
            }
        }
    }

    std::vector<IR::BASIC_BLOCK> DominatorTree::getDominateFrontier(const std::string &bbName) {
        if(dfMap.find(bbName) == dfMap.end())
            assert(false && "Pase a error bb name to find the Dominator Frontier!");
        return dfMap[bbName];
    }

    /// @brief 对控制流图进行DFS遍历，构建DFS Tree和逆后序列表
    /// @param vertex 代表基本快，即控制流图中的节点
    /// @see sy_ir.h#BasicBlock
    void DominatorTree::dfs(const IR::BASIC_BLOCK &vertex, std::set<std::string>& visit, int& n, const DTNode& parent) {
        DTNode node = make_shared<DominatorTreeNode>(vertex);
        node->setParent(parent);
        node->setDfn(n);

        // CFG的逆后序序列
        reversePreOrderList.push_back(node);

        nodeMap.insert({vertex->getBBName(), node});

        if(n == 1){
            root = node;
        }
        n++;

        visit.insert(vertex->getBBName());

        for(auto& child : vertex->getCNexts()){
            if(visit.find(child->getBBName()) == visit.end())
                dfs(child, visit, n, node);
        }
    }

    /// sedominator算法
    /// sdom(v) = min{u|u0, u1, u2....uk = v }
    /// 需要满足条件， dfn(u) < dfn(v), 且dfn(ui) > dfn(v), 0 < i < k
    void DominatorTree::sedminator() {

        visit.clear();

        for(auto& vertex : reversePreOrderList)
            vertex->initSdom();

        vector<string> prevs;

        for(auto& vertex : reversePreOrderList){
            if(vertex != root){
                prevs = vertex->getPrevIndex();
                for(auto& prev : prevs){
                    auto dtNode = nodeMap[prev];
                    auto res = eval(dtNode, vertex);
                    if(res->getDfn() < vertex->getSdomDfn())
                        vertex->setSdom(res);
                }
            }
        }
    }

    DTNode DominatorTree::eval(DTNode& u, DTNode& v) {

        visit.insert(u->getDtName());
        if(u->getDfn() < v->getDfn())
            return u;
        else{
            DTNode min = v;
            auto prevs = u->getPrevIndex();
            for(auto& prev : prevs){
                if(visit.find(prev) == visit.end()){
                    auto dtNode = nodeMap[prev];
                    auto res = eval(dtNode, v);
                    if(res->getDfn() < min->getDfn())
                        min = res;
                }
            }
            return min;
        }
    }

    /// semi_nca - 支配者树构建算法 semi_nca O(n^2)
    void DominatorTree::semi_nca() {
        sedminator();
        for(auto& vertex : reversePreOrderList){
            if(vertex != root){
                auto p = vertex->getParent();
                while(p != nullptr){
                    if(p->getDfn() <= vertex->getSdom()->getDfn()){
                        vertex->setIdom(p);
                        p->addChild(vertex);
                        break;
                    }
                    p = p->getParent();
                }
            }
        }
    }

    void Mem2RegPass::execute(IR::MODULE &m) {
        for(const auto& funcDef : m->getFuncDefs()){
            Globals.clear();
            Blocks.clear();
            varUseMap.clear();
            blockMap.clear();
            r2rMap.clear();
            labMap.clear();
            // Phi节点插入之前的准备
            beforeInsertPhiNode(funcDef);
            // 计算支配者树和各个节点的支配边界
            DominatorTree dtTree(funcDef->getEntryBB());
            dtTree.execute();
//            dtTree.dumpDominatorTree(cout);
//            cout<<"==========================\n";
//            dtTree.dumpDominatorFrontier(cout);
//            cout<<"==========================\n";
            // 插入Phi节点
            insertPhiForPerCfg(dtTree);
            // 重命名
            initNameAllocator();
            renameVar(dtTree.getRootNode());

        }
        int a = 0;
    }

    /*
     * beforeInsertPhiNode Function
     * @brief: 在插入Phi节点之前，需要先分析CFG，一共定义了多少个可以Promote的变量放入Globals，然后对于哪些变量在那些
     *         基本快中有定义，哪些变量在哪些基本快中有使用
     */
    void Mem2RegPass::beforeInsertPhiNode(const IR::FUNC_DEFINE &define) {


        auto otherAnalysis = [&](const IR::BASIC_BLOCK& bb, const IR::INSTRUCTION& inst){

            switch (inst->getSpecInst()) {
                case IR::Inst::sy_store:{
                    auto instStore = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    auto varName = dynamic_pointer_cast<IR::Reg>(instStore->getResultValue())->getRegName();
                    /* 如果这个变量存在于Global中， 记录起来他的定义 */
                    if(Globals.find(varName) != Globals.end()){
                        if(Blocks.find(varName) == Blocks.end()){
                            Blocks.insert({varName, {bb->getBBName()}});
                        }else{
                            /* 如果变量已经在这个基本块定义，替换掉Value，否则就在定义之后追加一个定义信息 */
                            bool notHaveThisDef = true;
                            for(auto& def : Blocks[varName]){
                                if(def == bb->getBBName()){
                                    notHaveThisDef = false;
                                    break;
                                }
                            }
                            if(notHaveThisDef){
                                Blocks[varName].push_back(bb->getBBName());
                            }
                        }
                    }
                    break;
                }
                case IR::Inst::sy_load:{
                    /* 记录这个变量在基本块里面的使用情况 */
                    auto instLoad = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    auto varName = dynamic_pointer_cast<IR::Reg>(instLoad->getArg1Value())->getRegName();
                    if(Globals.find(varName) != Globals.end()){
                        varUseMap[varName].insert(bb->getBBName());
                    }
                    break;
                }
                default: break;
            }
        };

        /* 针对于入口块的分析 */
        auto entryAnalysis = [&](const IR::BASIC_BLOCK& bb, const IR::INSTRUCTION& inst){
            if(inst->ty == IR::Inst::sy_alloca){
                auto instAlloca = dynamic_pointer_cast<IR::InstAlloca>(inst);
                if(instAlloca->isArrayAlloca())
                    return;
                auto var = dynamic_pointer_cast<IR::Reg>(instAlloca->getVarValue());
                Globals.insert({var->getRegName(), var->getType()});
                Blocks.insert({var->getRegName(), {instAlloca->getDefBB()->getBBName()}});
                allocaMap[instAlloca->getDefBB()->getBBName()].insert({var->getRegName(), var->getType()==IR::IRType::IRFloatPtr32});
                return;
            }
            otherAnalysis(std::forward<const IR::BASIC_BLOCK&>(bb), std::forward<const IR::INSTRUCTION&>(inst));
        };

        auto entry = define->getCEntryBB();
        for(const auto& inst : entry->getCInsts()){
            entryAnalysis(entry, inst);
        }

        for(const auto& othBB : define->getCOtherBB()){
            for(const auto& inst : othBB->getCInsts()){
                otherAnalysis(othBB, inst);
            }
        }

    }

    /*
     * insertPhiForPerCfg
     * @brief 插入Phi节点
     */
    void Mem2RegPass::insertPhiForPerCfg(DominatorTree &dtTree) {
        map<string, set<string>> PHIMap;
        /* 1.遍历Globals */
        for(const auto& v : Globals){
            /* 2.对于这些变量，拿到其在基本块中定义的WorkList */
            std::vector<std::string> WorkLists;
            for(const auto& def : Blocks[v.first]){
                WorkLists.push_back(def);
            }
            /* 3.开始插入Phi结点 */
            while(!WorkLists.empty()){
                auto node = WorkLists.back();
                WorkLists.pop_back();
                /* 4.拿到支配边界 */
                auto Df = dtTree.getDominateFrontier(node);
                for(const auto& bb : Df){
                    /* 5.如果这个基本块中存在对变量的使用，并且没有插入Phi结点，那么就插入一条Phi指令 */
                    if(PHIMap[v.first].find(bb->getBBName()) == PHIMap[v.first].end()){
                        PHIMap[v.first].insert(bb->getBBName());
                        auto value = make_shared<IR::Reg>(v.first, v.second==IR::IRType::IRFloatPtr32?IR::IRType::IRFloat32:IR::IRType::IRInt32);
                        auto phi = make_shared<IR::InstPhi>(v.first, value);
                        bb->insertInstToTheFront(phi);
                        WorkLists.push_back(bb->getBBName());
                    }

                }
            }
        }
    }

    void Mem2RegPass::initNameAllocator() {
        nameAllocator.clear();
        for(const auto& var : Globals){
            nameAllocator.insert({var.first, {}});
        }
    }

    void Mem2RegPass::newName(const std::string& name, const std::shared_ptr<IR::Label>& label, const IR::VALUE& v) {
        auto reg = dynamic_pointer_cast<IR::Reg>(v);
        assert(reg != nullptr && "there has error in func newName in Mem2RegPass!");
        nameAllocator[name].counter++;
        string newName = "$" + name + to_string(nameAllocator[name].counter);
        reg->resetName(newName);
        nameAllocator[name].valueStack.push_back(v);
        nameAllocator[name].labelStack.push_back(label);
    }

    void Mem2RegPass::renameVar(const DTNode& node) {

        std::map<std::string, unsigned> varAllocateSet;

        auto bb = node->getBB();

        for(const auto& def : allocaMap[bb->getBBName()]){
            varAllocateSet.insert({def.first, 1});
            if(def.second){
                nameAllocator[def.first].valueStack.push_back(make_shared<IR::ImmediateOp>(0.0f));
            }else{
                nameAllocator[def.first].valueStack.push_back(make_shared<IR::ImmediateOp>(0));
            }
            if(labMap.find(bb->getBBName()) == labMap.end())
                labMap.insert({bb->getBBName(), make_shared<IR::Label>(bb->getBBName(), bb)});
            nameAllocator[def.first].labelStack.push_back(labMap[bb->getBBName()]);
        }

        for(const auto& inst : bb->getInsts()){
            switch (inst->getInstType()) {
                case IR::InstType::phi : {
                    /* 分配一个新的name， 并将var与value对应起来 */
                    auto instPhi = dynamic_pointer_cast<IR::InstPhi>(inst);
                    if(labMap.find(bb->getBBName()) == labMap.end())
                        labMap.insert({bb->getBBName(), make_shared<IR::Label>(bb->getBBName(), bb)});
                    newName(instPhi->getVarName(), labMap[bb->getBBName()], instPhi->getResult());
                    varAllocateSet.insert({instPhi->getVarName(), 1});
                    break;
                }
                case IR::InstType::param : {
                    auto instParam = dynamic_pointer_cast<IR::InstParam>(inst);
                    if(r2rMap.find(instParam->getPara()) != r2rMap.end())
                        instParam->replacePara(r2rMap[instParam->getPara()]);
                    break;
                }
                case IR::InstType::one : {
                    auto instOne = dynamic_pointer_cast<IR::InstOneOp>(inst);
                    if(instOne->getOp()){
                        if(r2rMap.find(instOne->getOp()) != r2rMap.end()){
                            instOne->replaceOp(r2rMap[instOne->getOp()]);
                        }
                    }
                    break;
                }
                case IR::InstType::two : {
                    auto instTwo = dynamic_pointer_cast<IR::InstTwoOp>(inst);
                    /* 1. 如果是load, 判断是否可以删除这条load */
                    if(instTwo->ty == IR::Inst::sy_load){
                        string varName = dynamic_pointer_cast<IR::Reg>(instTwo->getArg1Value())->getRegName();
                        if(!nameAllocator[varName].valueStack.empty()){
                            r2rMap.insert({instTwo->getResultValue(), nameAllocator[varName].valueStack.back()});
                            bb->removeInst(inst);
                        }
                    }
                    /* 2. 如果是store, 判断是否可以将这条store指令移除掉 */
                    else if(instTwo->ty == IR::Inst::sy_store){
                        if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end()){
                            instTwo->replaceArg1(r2rMap[instTwo->getArg1Value()]);
                        }
                        string varName = dynamic_pointer_cast<IR::Reg>(instTwo->getResultValue())->getRegName();
                        if(Globals.find(varName) != Globals.end()){
                            if(labMap.find(bb->getBBName()) == labMap.end())
                                labMap.insert({bb->getBBName(), make_shared<IR::Label>(bb->getBBName(), bb)});
                            nameAllocator[varName].valueStack.push_back(instTwo->getArg1Value());
                            nameAllocator[varName].labelStack.push_back(labMap[bb->getBBName()]);
                            if(varAllocateSet.find(varName) != varAllocateSet.end()){
                                varAllocateSet[varName]++;
                            }else{
                                varAllocateSet.insert({varName, 1});
                            }
                            bb->removeInst(inst);
                        }
                    }
                    /* 3. 如果是 */
                    else{
                        if(r2rMap.find(instTwo->getArg1Value()) != r2rMap.end())
                            instTwo->replaceArg1(r2rMap[instTwo->getArg1Value()]);
                    }
                    break;
                }
                case IR::InstType::three : {
                    auto instThree = dynamic_pointer_cast<IR::InstThreeOp>(inst);
                    if(r2rMap.find(instThree->getOp1()) != r2rMap.end())
                        instThree->replaceOp1(r2rMap[instThree->getOp1()]);
                    if(r2rMap.find(instThree->getOp2()) != r2rMap.end())
                        instThree->replaceOp2(r2rMap[instThree->getOp2()]);
                    break;
                }
                default:
                    break;
            }
        }

        for(const auto& succ : bb->getNexts()){
            for(const auto& inst : succ->getCInsts()){
                if(inst->getInstType() != IR::InstType::phi)
                    break;
                auto instPhi = dynamic_pointer_cast<IR::InstPhi>(inst);
                if(!nameAllocator[instPhi->getVarName()].valueStack.empty()){
                    if(nameAllocator[instPhi->getVarName()].labelStack.back()->getLabel() != succ->getBBName()){
                        instPhi->addPhiParameter({nameAllocator[instPhi->getVarName()].valueStack.back(), nameAllocator[instPhi->getVarName()].labelStack.back()});
                    }
                }
            }
        }

        for(const auto& child : node->getCChilds())
            renameVar(child);

        for(const auto& var : varAllocateSet){
            for(int i = 0 ; i < var.second ; ++i){
                nameAllocator[var.first].valueStack.pop_back();
                nameAllocator[var.first].labelStack.pop_back();
            }
        }

    }

}
