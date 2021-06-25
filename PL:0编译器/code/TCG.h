//#pragma GCC optimize(2)
//#pragma G++ optimize(2)
//#include <bits/stdc++.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
using namespace std;

enum OperatorType {
    Plus, Minus, Mul, Div, Neg, Odd, Eq, Neq, Less, Le, Greater, Ge, Read, Write, Return
};

//-----------------目标代码格式--------------------------
struct TargetCodeItem{
    explicit TargetCodeItem(string _f="",int _l=0,int _a=0):f(move(_f)),l(_l),a(_a){}
    string f;//功能码
    int l;//层次差
    int a;//位移量
};
//-----------------目标代码产生器------------------------
class TargetCodeGenerator{
public:
    TargetCodeGenerator();
    void init();
    void emit(string f,int l,int a);
    void output();

    vector<TargetCodeItem> code;
};
//-----------------符号表项-----------------------------
struct SymbolItem{
    explicit SymbolItem(string _name="",string _kind="",int _value=0,int _level=0,int _addr=0):name(move(_name)),kind(move(_kind)),value(_value),level(_level),addr(_addr){}
    string name;//名
    string kind;//类型
    int value;//值
    int level;//层数
    int addr;//相对地址偏移量
};
//-----------------符号表-------------------------------
class SymbolTable{
public:
    SymbolTable();
    void init();
    bool addItem(string name,string kind,int value,int nowlevel);//向符号表中增加元素
    void output();
    vector<SymbolItem>::reverse_iterator find(const string &name);

    vector<SymbolItem> table;//符号表
    int DX;//当前正在处理的变量相对本层的偏移地址
};