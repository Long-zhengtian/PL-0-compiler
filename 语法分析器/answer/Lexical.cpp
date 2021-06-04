#include "Lexical.h"

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
bool LexicalAnalyzer::analyze(string str){
    transform(str.begin(),str.end(),str.begin(),::toupper);
    string tempWord; tempWord.clear();
    for (int i=0; i<str.length(); i++){
        if(str[i]==' ' || str[i]==9){//空格或tab
            continue;
        } else if(isalpha(str[i])){//识别标识符
            tempWord.clear();
            while(isalnum(str[i])){//isalnum: 判断是否是a-z,A-Z,0-9
                tempWord+=str[i]; i++;
            }
            i--;
            if(tempWord.length()>10){//词法错误
                return false;
            } else {
                int tempid=FindKeyWordsNum(tempWord);
                if(tempid){//位于map中
                    result.emplace_back("TZL",tempWord,true);
                } else {//新的标识符
                    result.emplace_back("IDENTIFIER",tempWord,false);
                }
            }
        } else if(isdigit(str[i])){//识别整数
            tempWord.clear();
            while(str[i]=='0'){//处理前导0
                i++;
            }
            while(isdigit(str[i])){
                tempWord+=str[i]; i++;
            }
            if(isupper(str[i])){//isupper: 判断一个字符是否是大写字母
                return false;
            }
            if(tempWord.length()==0) {//防止单个的0
                tempWord += "0";
            }
            result.emplace_back("NUMBER",tempWord,false);
            i--;
        } else {//其他
            tempWord.clear();
            tempWord+=str[i];
            int tempid=FindKeyWordsNum(tempWord);
            if(tempid){//位于map中
                tempWord+=str[i+1];
                int tempid2=FindKeyWordsNum(tempWord);
                if(tempid2){//二元的也在
                    result.emplace_back("TZL",tempWord,true);
                    i++;
                } else{//只存在一元的
                    tempWord.clear(); tempWord+=str[i];
                    result.emplace_back("TZL",tempWord,true);
                }
            } else {
                tempWord+=str[i+1];
                int tempid2=FindKeyWordsNum(tempWord);
                if(tempid2){//二元的在
                    result.emplace_back("TZL",tempWord,true);
                    i++;
                } else{//不存在
                    return false;
                }
            }
        }
    }
    return true;
}
void LexicalAnalyzer::output(){
    for (const auto &i:result){
        if(i.OneOrTwo) cout<<i.value<<endl;
        else cout<<i.type<<' '<<i.value<<endl;
    }
}