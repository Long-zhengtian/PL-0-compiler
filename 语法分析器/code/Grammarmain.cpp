#include "Grammar.h"

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


    if(LexicalMachine.analyze(LexicalAnalysisInput)){
        //进行语法分析
        GrammarAnalyzer GrammarMachine(LexicalMachine.getResult());//语法分析器
        if(GrammarMachine.analyze()){
            cout<<GrammarMachine.getResult()<<endl;
            //GrammarMachine.output();
        } else {//出现语法错误
            cout<<"Syntax Error"<<endl;
        }

    } else {//出现词法错误
        cout<<"Lexical Error"<<endl;
    }
    return 0;
}
