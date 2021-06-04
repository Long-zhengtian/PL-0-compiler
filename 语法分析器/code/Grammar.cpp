#include "Grammar.h"

void GrammarAnalyzer::prettydfs(int &index, int depth) {
    again:
    prettyAns += string(depth, '\t');
    prettyAns += tokens[index];
    index++;
    if (index < tokens.size() && tokens[index] == "("){
        prettyAns += "(\n";
        index++;
        prettydfs(index, depth + 1);
        if (tokens[index] != ")")throw 233;
        prettyAns += string(depth,'\t') +")";
        index++;
    }
    if (index < tokens.size() && tokens[index] == ","){
        prettyAns += ",\n";
        index++;
        goto again;
    }
    prettyAns += "\n";
}
bool GrammarAnalyzer::analyze() {
    if(input.empty()) {
        cerr<<"Syntax Error: input empty!"<<endl;
        return false;
    }
    bool flag=PROGRAM();
    if(now<input.size())
        return false;
    return flag;
}
bool GrammarAnalyzer::PROGRAM() {//<程序>→<分程序>.
    TreeRoot=new TreeNode("PROGRAM");
    if(!SUBPROG(TreeRoot)) {
        cerr<<"Syntax Error: PROGRAM-SUBPROG Error!"<<endl;
        return false;
    }
    if(input[now].value=="."){
        TreeRoot->newChild(".");
        now++;
        return true;
    } else {
        cerr<<"Syntax Error: PROGRAM must end with \".\""<<endl;
        return false;
    }
}
bool GrammarAnalyzer::SUBPROG(TreeNode* Node) {//<分程序>→ [<常量说明部分>][<变量说明部分>][<过程说明部分>]<语句>
    TreeNode* nowNode=Node->newChild("SUBPROG");
    if(input[now].value=="CONST" && !CONST(nowNode)) {return false;}//常量说明部分
    if(input[now].value=="VAR" && !VAR(nowNode)) {return false;}//变量说明部分
    if(input[now].value=="PROCEDURE" && !PROCEDURE(nowNode)) {return false;}//过程说明部分，会有多层过程说明部分
    return SENTENCE(nowNode);//语句
}
bool GrammarAnalyzer::CONST(TreeNode* Node) {//<常量说明部分> → CONST<常量定义>{ ,<常量定义>};
    TreeNode* nowNode=Node->newChild("CONSTANTDECLARE");
    nowNode->newChild("CONST");
    now++;//CONST
    if(input[now].type=="IDENTIFIER" && input[now+1].value=="=" && input[now+2].type=="NUMBER"){//<常量定义> → <标识符>=<无符号整数>
        TreeNode* sonNode=nowNode->newChild("CONSTANTDEFINE");
        sonNode->newChild(input[now].value);
        sonNode->newChild(input[now+1].value);
        sonNode->newChild(input[now+2].value);
        now+=3;
    } else {
        cerr<<"Syntax Error: CONST first CONSTANTDEFINE Error!"<<endl;
        return false;
    }
    while(input[now].value==",") {
        nowNode->newChild("COMMA");
        now++;
        if(input[now].type=="IDENTIFIER" && input[now+1].value=="=" && input[now+2].type=="NUMBER"){
            TreeNode* sonNode=nowNode->newChild("CONSTANTDEFINE");
            sonNode->newChild(input[now].value);
            sonNode->newChild(input[now+1].value);
            sonNode->newChild(input[now+2].value);
            now+=3;
        } else {
            cerr<<"Syntax Error: CONST second CONSTANTDEFINE Error!"<<endl;
            return false;
        }
    }
    if(input[now].value==";"){
        nowNode->newChild(";"); now++;
        return true;
    } else {
        cerr<<"Syntax Error: CONST has an undefined symbol!"<<endl;
        return false;
    }
}
bool GrammarAnalyzer::VAR(TreeNode* Node) {//<变量说明部分> → VAR<标识符>{ ,<标识符>};
    TreeNode* nowNode=Node->newChild("VARIABLEDECLARE");
    nowNode->newChild("VAR");
    now++;//VAR
    if(input[now].type=="IDENTIFIER"){
        nowNode->newChild(input[now].value); now++;
    } else {
        cerr<<"Syntax Error: VAR first IDENTIFIER Error"<<endl;
        return false;
    }
    while(input[now].value==",") {
        nowNode->newChild("COMMA"); now++;
        if(input[now].type=="IDENTIFIER"){
            nowNode->newChild(input[now].value); now++;
        } else {
            cerr<<"Syntax Error: VAR second IDENTIFIER Error"<<endl;
            return false;
        }
    }
    if(input[now].value==";"){
        nowNode->newChild(";"); now++;
        return true;
    } else {
        cerr<<"Syntax Error: VAR has an undefined symbol!"<<endl;
        return false;
    }
}
bool GrammarAnalyzer::PROCEDURE(TreeNode* Node) {//<过程说明部分> → <过程首部><分程序>;{<过程说明部分>}
    if(++level>3){//判断当前过程说明部分是第几层
        level=0;
        cerr<<"Syntax Error: PROCEDURE level must <= 3"<<endl;
        return false;
    }

    TreeNode* nowNode=Node->newChild("PROCEDUREDECLARE");
    TreeNode* sonNode=nowNode->newChild("PROCEDUREHEAD");
    sonNode->newChild("PROCEDURE");
    now++;//procedure
    if(input[now].type=="IDENTIFIER" && input[now+1].value==";"){//过程首部
        sonNode->newChild(input[now].value);
        sonNode->newChild(";");
        now+=2;//f1;
    } else {
        cerr<<"Syntax Error: PROCEDURE first PROCEDUREHEAD Error!"<<endl;
        return false;
    }
    if(!SUBPROG(nowNode)) {
        cerr<<"Syntax Error: PROCEDURE SUBPROG Error!"<<endl;
        return false;
    }
    level--;
    if(input[now].value==";"){
        nowNode->newChild(";");
        now++;
    } else {
        cerr<<"Syntax Error: PROCEDURE without \";\""<<endl;
        return false;
    }
    while(input[now].value=="PROCEDURE"){
        if(!PROCEDURE(nowNode)){
            cerr<<"Syntax Error: PROCEDURE second PROCEDUREHEAD Error!"<<endl;
            return false;
        }
    }
    return true;
}
bool GrammarAnalyzer::SENTENCE(TreeNode* Node){//<语句> → <赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|<空语句>
    TreeNode* nowNode=Node->newChild("SENTENCE");
    if(now+1<input.size() && input[now+1].value==":="){ return ASSIGNMENT(nowNode); }//赋值语句
    else if(input[now].value=="IF") { return IF(nowNode); }//条件语句
    else if(input[now].value=="WHILE"){ return WHILE(nowNode); }//当型循环语句
    else if(input[now].value=="CALL") { return CALL(nowNode); }//过程调用语句
    else if(input[now].value=="READ") { return READ(nowNode); }//读语句
    else if(input[now].value=="WRITE") { return WRITE(nowNode); }//写语句
    else if(input[now].value=="BEGIN") { return BEGIN(nowNode); }//复合语句
    else EPSILON(nowNode);//空语句
    return true;

}
bool GrammarAnalyzer::ASSIGNMENT(TreeNode* Node) {//<赋值语句> → <标识符>:=<表达式>
    TreeNode* nowNode=Node->newChild("ASSIGNMENT");
    if(input[now].type=="IDENTIFIER") nowNode->newChild(input[now].value);//标识符
    else {
        cerr<<"Syntax Error: ASSIGNMENT IDENTIFIER Error!"<<endl;
        return false;
    }
    now++;
    nowNode->newChild(":=");
    now++;
    if(!EXPRESSION(nowNode)){
        cerr<<"Syntax Error: ASSIGMENT EXPRESSION Error!"<<endl;
        return false;
    } else return true;
}
bool GrammarAnalyzer::EXPRESSION(TreeNode* Node) {//<表达式> → [+|-]<项>{<加减运算符><项>}
    TreeNode* nowNode=Node->newChild("EXPRESSION");
    if(input[now].value=="+" || input[now].value=="-"){
        nowNode->newChild(input[now].value);
        now++;
    }
    if(!ITEM(nowNode)) {
        cerr<<"Syntax Error: EXPRESSION first ITEM Error!"<<endl;
        return false;
    }
    while(input[now].value=="+" || input[now].value=="-"){
        nowNode->newChild(input[now].value);
        now++;
        if(!ITEM(nowNode)) {
            cerr<<"Syntax Error: EXPRESSION second ITEM Error!"<<endl;
            return false;
        }
    }
    return true;
}
bool GrammarAnalyzer::ITEM(TreeNode* Node) {//<项> → <因子>{<乘除运算符><因子>}
    TreeNode* nowNode=Node->newChild("ITEM");
    if(!FACTOR(nowNode)) return false;
    while(input[now].value=="*" || input[now].value=="/"){
        nowNode->newChild(input[now].value);
        now++;
        if(!FACTOR(nowNode)) {
            cerr<<"Syntax Error: ITEM FACTOR Error!"<<endl;
            return false;
        }
    }
    return true;
}
bool GrammarAnalyzer::FACTOR(TreeNode *Node) {//<因子> → <标识符>|<无符号整数>|(<表达式>)
    TreeNode* nowNode=Node->newChild("FACTOR");
    if(input[now].type=="IDENTIFIER" || input[now].type=="NUMBER"){
        nowNode->newChild(input[now].value);
        now++;
    } else if(input[now].value=="("){
        nowNode->newChild("LP");
        now++;
        EXPRESSION(nowNode);
        if(input[now].value==")"){
            nowNode->newChild("RP");
            now++;
        } else {
            cerr<<"Syntax Error: FACTOR EXPRESSION must end of \")\""<<endl;
            return false;
        }
    } else {
        cerr<<"Syntax Error: FACTOR EXPRESSION must begin with \"(\""<<endl;
        return false;
    }
    return true;
}
bool GrammarAnalyzer::IF(TreeNode* Node) {//<条件语句> → IF<条件>THEN<语句>
    TreeNode* nowNode=Node->newChild("IFSENTENCE");
    nowNode->newChild("IF");
    now++;//if
    if(!CONDITION(nowNode)) {
        cerr<<"Syntax Error: The conditional statement of the IF statement is Error!"<<endl;
        return false;
    }
    if(input[now].value=="THEN"){
        nowNode->newChild("THEN");
        now++;
    } else {
        cerr<<"Syntax Error: IF sentence must have THEN"<<endl;
        return false;
    }
    if(!SENTENCE(nowNode)) {
        cerr<<"Syntax Error: IF-THEN-SENTENCE Error!"<<endl;
        return false;
    }
    return true;
}
bool GrammarAnalyzer::CONDITION(TreeNode *Node) {//<条件> → <表达式><关系运算符><表达式>|ODD<表达式>
    TreeNode* nowNode=Node->newChild("CONDITION");
    if(input[now].value=="ODD"){
        nowNode->newChild("ODD");
        now++;
        if(!EXPRESSION(nowNode)) {
            cerr<<"Syntax Error: CONDITION ODD->EXPRESSION Error!"<<endl;
            return false;
        }
    } else {
        if(!EXPRESSION(nowNode)) {
            cerr<<"Syntax Error: CONDITION EXPRESSION->operator Error!"<<endl;
            return false;
        }
        if(input[now].value=="=" || input[now].value=="#" || input[now].value=="<=" || input[now].value==">=" || input[now].value=="<" || input[now].value==">"){
            nowNode->newChild(input[now].value);
            now++;
        } else {
            cerr<<"Syntax Error: CONDITION relational operator must be \"=\",\"#\",\"<=\",\">=\",\"<\",\">\""<<endl;
            return false;
        }
        if(!EXPRESSION(nowNode)) {
            cerr<<"Syntax Error: CONDITION operator->EXPRESSION Error!"<<endl;
            return false;
        }
    }
    return true;
}
bool GrammarAnalyzer::WHILE(TreeNode* Node) {//<当型循环语句> → WHILE<条件>DO<语句>
    TreeNode* nowNode=Node->newChild("WHILESENTENCE");
    nowNode->newChild("WHILE");
    now++;
    if(!CONDITION(nowNode)) {
        cerr<<"Syntax Error: WHILE CONDITION Error!"<<endl;
        return false;
    }
    if(input[now].value=="DO"){
        nowNode->newChild("DO");
        now++;
        return SENTENCE(nowNode);
    } else {
        cerr<<"Syntax Error: WHILE without DO!"<<endl;
        return false;
    }
}
bool GrammarAnalyzer::CALL(TreeNode* Node) {//<过程调用语句> → CALL<标识符>
    TreeNode* nowNode=Node->newChild("CALLSENTENCE");
    nowNode->newChild("CALL");
    now++;
    if(input[now].type=="IDENTIFIER") {
        nowNode->newChild(input[now].value);
        now++;
        return true;
    } else {
        cerr<<"Syntax Error: CALL IDENTIFIER Error!"<<endl;
        return false;
    }
}
bool GrammarAnalyzer::READ(TreeNode* Node){//<读语句> → READ(<标识符>{ ,<标识符>})
    TreeNode* nowNode=Node->newChild("READSENTENCE");
    nowNode->newChild("READ");
    now++;
    if(input[now].value=="("){
        nowNode->newChild("LP"); now++;
        if(input[now].type=="IDENTIFIER") nowNode->newChild(input[now].value);
        else {
            cerr<<"Syntax Error: READ IDENTIFIER Error!"<<endl;
            return false;
        }
        now++;
        while(input[now].value==","){
            nowNode->newChild("COMMA");
            now++;
            if(input[now].type=="IDENTIFIER") nowNode->newChild(input[now].value);
            else {
                cerr<<"Syntax Error: READ IDENTIFIER Error!"<<endl;
                return false;
            }
            now++;
        }
        if(input[now].value==")"){
            nowNode->newChild("RP"); now++;
            return true;
        } else {
            cerr<<"Syntax Error: READ must end of \")\""<<endl;
            return false;
        }
    } else {
        cerr<<"Syntax Error: READ must begin with \"(\""<<endl;
        return false;
    }
}
bool GrammarAnalyzer::WRITE(TreeNode* Node) {//<写语句> → WRITE(<标识符>{,<标识符>})
    TreeNode* nowNode=Node->newChild("WRITESENTENCE");
    nowNode->newChild("WRITE");
    now++;
    if(input[now].value=="("){
        nowNode->newChild("LP"); now++;
        if(input[now].type=="IDENTIFIER") nowNode->newChild(input[now].value);
        else {
            cerr<<"Syntax Error: WRITE IDENTIFIER Error!"<<endl;
            return false;
        }
        now++;
        while(input[now].value==","){
            nowNode->newChild("COMMA");
            now++;
            if(input[now].type=="IDENTIFIER") nowNode->newChild(input[now].value);
            else {
                cerr<<"Syntax Error: WRITE IDENTIFIER Error!"<<endl;
                return false;
            }
            now++;
        }
        if(input[now].value==")"){
            nowNode->newChild("RP"); now++;
            return true;
        } else {
            cerr<<"Syntax Error: WRITE must end of \")\""<<endl;
            return false;
        }
    } else {
        cerr<<"Syntax Error: WRITE must begin with \")\""<<endl;
        return false;
    }
}
bool GrammarAnalyzer::BEGIN(TreeNode* Node) {//<复合语句> → BEGIN<语句>{ ;<语句>} END，注意细节，";"后面还有语句才对，结尾不会是";"
    TreeNode* nowNode=Node->newChild("COMBINED");
    nowNode->newChild("BEGIN");
    now++;//begin
    if(!SENTENCE(nowNode)){
        cerr<<"Syntax Error: BEGIN first SENTENCE Error!"<<endl;
        return false;
    }
    while(input[now].value==";"){
        nowNode->newChild(";");
        now++;
        if(!SENTENCE(nowNode)){
            cerr<<"Syntax Error: BEGIN second SENTENCE Error!"<<endl;
            return false;
        }
    }
    if(input[now].value=="END"){
        nowNode->newChild("END");
        now++;
        return true;
    } else {
        cerr<<"Syntax Error: BEGIN must end of END"<<endl;
        return false;
    }
}
bool GrammarAnalyzer::EPSILON(TreeNode* Node) {//<空语句> → epsilon
    Node->newChild("EMPTY");
    return true;
}
void GrammarAnalyzer::output(){
    string exp=getResult();
    string tmp;
    for (int i = 0; i < exp.length(); i++){
        if (exp[i] == '(' || exp[i] == ')' || exp[i] == ','){
            if(!tmp.empty())tokens.push_back(tmp);
            tmp = "";
            tokens.emplace_back(1, exp[i]);
        } else tmp += exp[i];
    }
    if (!tmp.empty())tokens.push_back(tmp);
    try{
        int id = 0;
        prettydfs(id, 0);
    }
    catch (...){
        cerr << "Error Parsing Expression" << endl;
        exit(-1);
    }
    cout << prettyAns << endl;
}
string GrammarAnalyzer::getResult() {
    TreeNodeToResult(TreeRoot);
    return result;
}
void GrammarAnalyzer::TreeNodeToResult(TreeNode* Node) {
    result+=Node->getStr();
    int len=Node->getChildren().size();
    if(len>0){
        result+="(";
        for (int i=0; i<len; i++){
            TreeNodeToResult(Node->getChildren()[i]);
            if(i!=len-1) result+=',';
        }
        result+=")";
    }
}