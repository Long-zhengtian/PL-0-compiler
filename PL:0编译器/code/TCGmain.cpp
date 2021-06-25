#include "GAandSA.h"

int main(){
    freopen("program.code", "w", stdout);

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

    LexicalAnalyzer LexicalMachine;//建立词法分析器
    if(LexicalMachine.analyze(LexicalAnalysisInput)){//词法分析成功
        //进行语法分析
//        for (auto it:LexicalMachine.getResult()){
//            cout<<it.type<<' '<<it.value<<endl;
//        }
        GandSAnalyzer GandSMachine(LexicalMachine.getResult());//建立语法分析器
        if(GandSMachine.analyze()){//语法分析、语义分析、中间代码生成成功
            GandSMachine.outputCode();
        } else {//出现语法错误
            cout<<"Syntax Error"<<endl;
            return -1;
        }

    } else {//出现词法错误
        cout<<"Lexical Error"<<endl;
        return -1;
    }
    return 0;
}
