/**
 * Author:  Devotes
 * Date:    2022/5/31
 *
 * FixLog:
 *
 */

#include "symbol.h"
#include <algorithm>

using namespace std;

string inline convertStrType(Type type){

    switch (type) {
        case INT:   return "int";
        case VOID:  return "void";
        case FLOAT: return "float";
    }
    return "";
}


/**********************************************
 * 形参符号
 **********************************************/
bool FormalParaSymbol::operator==(const FormalParaSymbol &symbol) {
    if(type != symbol.type) return false;
    return array_dim == symbol.array_dim;
}

/**********************************************
 * 函数符号
 **********************************************/
bool FuncSymbol::operator==(const FuncSymbol &symbol) {
    if(id != symbol.id) return false;
    if(ret_type != symbol.ret_type) return false;
    if(args.size() != symbol.args.size()) return false;

    auto it1 = args.begin();
    auto it2 = symbol.args.cbegin();
    auto end1 = args.end();
    auto end2 = symbol.args.cend();

    while(it1 != end1 && it2 != end2){
        if(*it1 == *it2)
            continue;
        return false;
    }
    return true;
}

/**********************************************
 * 普通符号
 **********************************************/
bool CommonVarSymbol::operator==(const CommonVarSymbol &symbol) {
    if(id != symbol.id) return false;
    if(type != symbol.type) return false;
    return is_const == symbol.is_const;
}

/**********************************************
 * 数组符号
 **********************************************/
bool ArrayVarSymbol::operator==(const ArrayVarSymbol &symbol) {
    if(id != symbol.id) return false;
    if(type != symbol.type) return false;
    if(is_const != symbol.is_const) return false;
    return arrDims == symbol.arrDims;
}

/**********************************************
 * 符号表管理器
 **********************************************/
SymbolTableManager::SymbolTableManager() {
    global_scope = new SyScope("GlobalScope");
    current_scope = global_scope;
}

SymbolTableManager::~SymbolTableManager() {
    /* 递归地删除符号表 */
    destroyScope(global_scope);
    current_scope = nullptr;
    global_scope = nullptr;
}

void SymbolTableManager::destroyScope(SyScope *&scope) {

    for(auto& child: scope->childs){
        destroyScope(child);
        child = nullptr;
    }
    scope->var_symbol_table.clear();
    scope->array_symbol_table.clear();
    delete scope;
}

bool SymbolTableManager::isInGlobalScope() {
    return current_scope->parent == nullptr;
}

void SymbolTableManager::entryCurrentSymbol(const std::string& name) {
    string sc_name;
    if(name == ""){
       sc_name = current_scope->scope_name + "." + "scope" + to_string(index);
       index++;
    }else{
       sc_name = current_scope->scope_name+ "." + name;
    }
    auto new_scope = new SyScope(sc_name);
    current_scope->childs.push_back(new_scope);
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void SymbolTableManager::leaveCurrentSymbol() {
    if(current_scope->parent != nullptr)
        current_scope = current_scope->parent;
}

bool SymbolTableManager::isInCurrentVarST(const std::string &id) {
    if(current_scope->var_symbol_table.find(id) != current_scope->var_symbol_table.end())
        return true;
    return false;
}

bool SymbolTableManager::isInCurrentArrayST(const std::string &id) {
    if(current_scope->array_symbol_table.find(id) != current_scope->array_symbol_table.end())
        return true;
    return false;
}

bool SymbolTableManager::isInArrayST(const std::string &id) {
    SyScope* temp = current_scope;
    while(temp != nullptr){
        if(temp->array_symbol_table.find(id) != temp->array_symbol_table.end())
            return true;
        temp = temp->parent;
    }
    return false;
}

bool SymbolTableManager::isInVarST(const std::string &id) {
    SyScope* temp = current_scope;
    while(temp != nullptr){
        if(temp->var_symbol_table.find(id) != temp->var_symbol_table.end())
            return true;
        temp = temp->parent;
    }
    return false;
}

bool SymbolTableManager::isInFuncCallST(const std::string &id) {
    if(current_scope->funcCallSymbol.find(id) != current_scope->funcCallSymbol.end())
        return true;
    return false;
}

void SymbolTableManager::addFuncCallSymbol(const std::string id) {
    current_scope->funcCallSymbol.insert(id);
}

bool SymbolTableManager::isInFuncST(const std::string &id) {
    for(auto& func: func_symbol_pool){
        if(id == func.id)
            return true;
    }
    return false;
}

CommonVarSymbol SymbolTableManager::getCommonVarSymbol(const std::string &id) {
    auto scope = current_scope;
    while(scope != nullptr){
        if(scope->var_symbol_table.find(id) != scope->var_symbol_table.end())
            return scope->var_symbol_table[id];
        scope = scope->parent;
    }
    throw "No Symbol Found";
}

ArrayVarSymbol SymbolTableManager::getArrayVarSymbol(const std::string &id) {
    auto scope = current_scope;
    while(scope != nullptr){
        if(scope->array_symbol_table.find(id) != scope->array_symbol_table.end())
            return scope->array_symbol_table[id];
        scope = scope->parent;
    }
    throw "No Symbol Found";
}

FuncSymbol SymbolTableManager::getFuncVarSymbol(const std::string &id) {

    for(const auto& func: func_symbol_pool){
        if(func.id == id) return func;
    }
    throw "No Func Found";
}

void SymbolTableManager::addVarSymbol(const CommonVarSymbol &symbol) {
    current_scope->var_symbol_table.insert({symbol.id, symbol});
}

void SymbolTableManager::addArrayVarSymbol(const ArrayVarSymbol &symbol) {
    current_scope->array_symbol_table.insert({symbol.id, symbol});
}

void SymbolTableManager::addFuncSymbol(const FuncSymbol &symbol) {
    func_symbol_pool.push_back(symbol);
}

void SymbolTableManager::printSymbolTable(std::ostream &out) {
    out<<"SymbolTable:"<<endl;
    std::vector<SyScope*> scopes;
    scopes.push_back(current_scope);
    int s_index;
    while(!scopes.empty()){
        std::vector<SyScope*> temp;
        for(const auto& scope: scopes){
            temp.insert(temp.end(), scope->childs.begin(), scope->childs.end());
            s_index = 1;
            out<<"\tScopeName: "<<scope->scope_name<<endl;
            for(const auto& symbol: scope->var_symbol_table){
                out<<"\t\tSymbol"<<to_string(s_index)<<": id = "<<symbol.second.id<<" is_const = "<<(symbol.second.is_const?"true":"false");
                out<<" type = "<<convertStrType(symbol.second.type)<<endl;
                index++;
            }
            s_index = 1;
            for(const auto& symbol: scope->array_symbol_table){
                out<<"\t\tSymbol"<<to_string(s_index)<<": id = "<<symbol.second.id<<" is_const = "<<(symbol.second.is_const?"true":"false");
                out<<" type = "<<convertStrType(symbol.second.type)<<" Dim = "<<symbol.second.arrDims.size()<<endl;
            }
        }
        scopes.assign(temp.begin(), temp.end());
    }
    out<<"FuncSymbol:"<<endl;
    for(const auto& func: func_symbol_pool){
        out<<"\tFuncName:"<<func.id<<" Return Type = "<<convertStrType(func.ret_type)<<endl;
        for(const auto&para: func.args){
            if(para.array_dim.size() > 0){
                out<<"\t\tFuncParamNode:"<<" Type = "<<convertStrType(para.type)<<" Dim = "<<(para.array_dim.size()+1)<<endl;
            }else{
                out<<"\t\tFuncParamNode:"<<" Type = "<<convertStrType(para.type)<<endl;
            }
        }
    }
}
