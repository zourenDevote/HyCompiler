//
// Created by devotes on 22-7-29.
//

#include "reduce_ana_pass.h"

namespace opt_pass{

bool startWith(const std::string& s1, const std::string& s2){
  if(s1.size() < s2.size())
    return false;
  int i = 0;
  int len = s2.size();
  while(i < len){
    if(s1[i] != s2[i])
      return false;
    i++;
  }
  return true;
}

void ReduceAnaPass::execute(IR::MODULE &m) {

  IR::FUNC_DEFINE def = nullptr;

  auto deal = [&](const IR::BASIC_BLOCK& bb){
      if(bb->getCInsts().empty())
        return;
      auto inst = bb->getCInsts().back();
      if(inst->getSpecInst() == IR::Inst::sy_if){
          auto instThree = std::dynamic_pointer_cast<IR::InstThreeOp>(inst);
          auto label1 = std::dynamic_pointer_cast<IR::Label>(instThree->getOp2());
          auto label2 = std::dynamic_pointer_cast<IR::Label>(instThree->getOp3());
          if(def->isBB2NextToTheBB1(bb, label1->getBB()))
            bb->setNextBBName(label1->getBB()->getBBName());
          if(def->isBB2NextToTheBB1(bb, label2->getBB()))
            bb->setNextBBName(label2->getBB()->getBBName());
      }
      else if(inst->getSpecInst() == IR::Inst::sy_goto){
          auto instOne = std::dynamic_pointer_cast<IR::InstOneOp>(inst);
          auto label = std::dynamic_pointer_cast<IR::Label>(instOne->getOp());
          if(def->isBB2NextToTheBB1(bb, label->getBB()))
            bb->setNextBBName(label->getBB()->getBBName());
      }
  };

  // 先移除空的BB

  for(const auto& func : m->getFuncDefs()){
    for(const auto& bb : func->getOtherBB()){
      if(startWith(bb->getBBName(), "cond") &&  bb->getCInsts().empty())
        func->removeBB(bb);
    }

    def = func;
    deal(func->getCEntryBB());
    for(const auto& bb : func->getCOtherBB())
      deal(bb);
  }

}

}


