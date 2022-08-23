//
// Created by 20580 on 2022/8/12.
//

#include "symbol_table.h"


void SymScope::addSymbol(const std::shared_ptr<Symbol> &symbol) {
    symbol->scodep = deep;
    if(curSymbolTable.find(symbol->id) == curSymbolTable.end()){
        curSymbolTable.insert({symbol->id, symbol});
    }
}

std::shared_ptr<Symbol> SymScope::getSymbol(const std::string &id) {
    auto pscope = parent;
    if(curSymbolTable.find(id) != curSymbolTable.end())
        return curSymbolTable[id];
    while(pscope != nullptr){
        if(pscope->curSymbolTable.find(id) != pscope->curSymbolTable.end())
            return pscope->curSymbolTable[id];
        pscope = pscope->parent;
    }
    return nullptr;
}