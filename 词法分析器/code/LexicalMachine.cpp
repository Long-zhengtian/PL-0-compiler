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
    Word(string _type="",string _value="",bool _OneOrTwo=true):type(_type),value(_value),OneOrTwo(_OneOrTwo){}
    string type;
    string value;
    bool OneOrTwo;//one true;two false
};
class LexicalAnalyzer{
public:
    LexicalAnalyzer(){};
    int FindKeyWordsNum(const string& str)  {//常量map不能用[]取值，只能使用迭代器
        auto cit = KeyWords.find(str);
        if (cit != KeyWords.end()) return cit->second;
        else  return 0;
    }
    bool Analyze(string str);
    void Output();
private:
    static const map<string,int> KeyWords;
    vector<Word> LexicalAnalysisOutput;//单词类型，属性值
};
const map<string,int> LexicalAnalyzer::KeyWords={
        {"CONST",       1},
        {"VAR",         2},
        {"PROCEDURE",   3},
        {"BEGIN",       4},
        {"END",         5},
        {"ODD",         6},
        {"IF",          7},
        {"THEN",        8},
        {"CALL",        9},
        {"WHILE",       10},
        {"DO",          11},
        {"READ",        12},
        {"WRITE",       13},
        {"=",           14},
        {":=",          15},
        {"+",           16},
        {"-",           17},
        {"*",           18},
        {"/",           19},
        {"#",           20},
        {"<",           21},
        {"<=",          22},
        {">",           23},
        {">=",          24},
        {";",           25},
        {",",           26},
        {".",           27},
        {"(",           28},
        {")",           29}
};
bool LexicalAnalyzer::Analyze(string str){
    transform(str.begin(),str.end(),str.begin(),::toupper);
    string tempWord; tempWord.clear();
    for (int i=0; i<str.length(); i++){
        if(str[i]==' ' || str[i]==9){//空格或tab
            continue;
        } else if(str[i]>='A' && str[i]<='Z'){//识别标识符
            tempWord.clear();
            while((str[i]>='A' && str[i]<='Z') || (str[i]>='0' && str[i]<='9')){
                tempWord+=str[i]; i++;
            }
            i--;
            if(tempWord.length()>10){//词法错误
                return false;
            } else {
                int tempid=FindKeyWordsNum(tempWord);
                if(tempid){//位于map中
                    LexicalAnalysisOutput.emplace_back(tempWord,"TZL",true);//TZL为作者姓名首字母，无其他意义
                } else {//新的标识符
                    LexicalAnalysisOutput.emplace_back("IDENTIFIER",tempWord,false);
                }
            }
        } else if(str[i]>='0' && str[i]<='9'){//识别整数
            tempWord.clear();
            while(str[i]=='0'){//处理前导0
                i++;
            }
            while(str[i]>='0' && str[i]<='9'){
                tempWord+=str[i]; i++;
            }
            if(str[i]>='A' && str[i]<='Z'){
                return false;
            }
            if(tempWord.length()==0) {//防止单个的0
                tempWord += "0";
            }
            LexicalAnalysisOutput.emplace_back("NUMBER",tempWord,false);
            i--;
        } else {//其他
            tempWord.clear();
            tempWord+=str[i];
            int tempid=FindKeyWordsNum(tempWord);
            if(tempid){//位于map中
                tempWord+=str[i+1];
                int tempid2=FindKeyWordsNum(tempWord);
                if(tempid2){//二元的也在
                    LexicalAnalysisOutput.emplace_back(tempWord,"TZL",true);
                    i++;
                }
                else{//只存在一元的
                    tempWord.clear(); tempWord+=str[i];
                    LexicalAnalysisOutput.emplace_back(tempWord,"TZL",true);
                }
            } else {
                tempWord+=str[i+1];
                int tempid2=FindKeyWordsNum(tempWord);
                if(tempid2){//二元的在
                    LexicalAnalysisOutput.emplace_back(tempWord,"TZL",true);
                    i++;
                }
                else{//不存在
                    return false;
                }
            }
        }
    }
    return true;
}
void LexicalAnalyzer::Output(){
    for (auto i:LexicalAnalysisOutput){
        if(i.OneOrTwo) cout<<i.type<<endl;
        else cout<<i.type<<' '<<i.value<<endl;
    }
}
int main(){
    LexicalAnalyzer LexicalMachine;
    string LexicalAnalysisInput,tempstr;
    LexicalAnalysisInput.clear(); tempstr.clear();
    while(getline(cin,tempstr)){
        if(tempstr.length()!=0){
            LexicalAnalysisInput+=tempstr;
            LexicalAnalysisInput+=" ";
        }
    }

//    while(getline(cin,tempstr) && tempstr.length()){
//        LexicalAnalysisInput+=tempstr;
//        LexicalAnalysisInput+=" ";
//    }

    if(LexicalMachine.Analyze(LexicalAnalysisInput)){
        LexicalMachine.Output();
        //进行语法分析

    } else {//出现词法错误
        cout<<"Lexical Error"<<endl;
    }
    return 0;
}
