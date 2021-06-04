
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

struct Word{//单词属性
    explicit Word(string _type="",string _value="",bool _OneOrTwo=true):type(move(_type)),value(move(_value)),OneOrTwo(_OneOrTwo){}
    string type;
    string value;
    bool OneOrTwo;//one true;two false
};
//---------------------------词法分析器------------------------
class LexicalAnalyzer{
public:
    LexicalAnalyzer() = default;
    static int FindKeyWordsNum(const string& str)  {//常量map不能用[]取值，只能使用迭代器
        auto cit = KeyWords.find(str);
        if (cit != KeyWords.end()) return cit->second;
        else  return 0;
    }
    bool analyze(string str);
    void output();
    vector<Word> getResult(){
        return result;
    }
private:
    static const map<string,int> KeyWords;
    vector<Word> result;//单词类型，属性值
};
//-------------------------------------------------------------