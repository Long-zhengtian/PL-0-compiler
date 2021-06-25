#include "TCG.h"

TargetCodeGenerator::TargetCodeGenerator() {
    code.clear();
}
void TargetCodeGenerator::init() {
    code.clear();
}
void TargetCodeGenerator::emit(string f, int l, int a) {
    code.emplace_back(f,l,a);
}
void TargetCodeGenerator::output() {
    for (auto &it:code){
        cout<<it.f<<' '<<it.l<<' '<<it.a<<endl;
    }
}

SymbolTable::SymbolTable(){
    DX=3;//初始DX为3
    table.clear();
}
void SymbolTable::init() {
    DX=3;
    table.clear();
}
vector<SymbolItem>::reverse_iterator SymbolTable::find(const string &name) {
    auto it=table.rbegin();
    for ( ; it!=table.rend(); it++){
        if((*it).name==name) return it;
    }
    return it;
}
bool SymbolTable::addItem(string name, string kind, int value, int nowlevel) {
    auto ItemIter=find(name);
    if(ItemIter!=table.rend() && (*ItemIter).level==nowlevel) {//同层重复定义
        cerr<<"SymbolTable Error! Variable definition!"<<endl;
        return false;
    }
    if(kind=="CONSTANT") table.emplace_back(name,kind,value,0,0);
    else if(kind=="VARIABLE") table.emplace_back(name,kind,0,nowlevel,DX++);
    else if(kind=="PROCEDURE") table.emplace_back(name,kind,0,nowlevel,0);
    else {
        cerr<<"SymbolTable Error! Unknown type appears!"<<endl;
        return false;
    }
    return true;
}
void SymbolTable::output() {
    for (auto &it:table){
        cout<<it.name<<' '<<it.kind<<' '<<it.value<<' '<<it.level<<' '<<it.addr<<endl;
    }
}