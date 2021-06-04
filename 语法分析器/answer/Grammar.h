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
#include "Lexical.h"
using namespace std;
//---------------------------语法树----------------------------
class TreeNode{
public:
    explicit TreeNode(string s){
        str=move(s);
        father=nullptr;
        children.clear();
    }
    TreeNode* newChild(string s){
        TreeNode *temp= new TreeNode(move(s));
        temp->father=this;
        children.push_back(temp);
        return temp;
    }
    string getStr(){return str;}
    TreeNode* getFather(){return father;}
    vector<TreeNode*> getChildren(){return children;}
private:
    string str;
    TreeNode* father;
    vector<TreeNode*> children;
};
//---------------------------语法分析器-------------------------
class GrammarAnalyzer{
public:
    GrammarAnalyzer(){
        now=0; level=0; result.clear(); TreeRoot=nullptr; input.clear();
    };
    explicit GrammarAnalyzer(vector<Word> _input){
        now=0; level=0; result.clear(); TreeRoot=nullptr; input.clear();
        input=move(_input);
    };
    bool analyze();
    bool PROGRAM();//主程序
    bool SUBPROG(TreeNode* Node);//分程序
    bool CONST(TreeNode* Node);//常量说明部分
    bool VAR(TreeNode* Node);//变量说明部分
    bool PROCEDURE(TreeNode* Node);//过程说明部分
    bool SENTENCE(TreeNode* Node);//语句
    bool ASSIGNMENT(TreeNode* Node);//赋值语句
    bool EXPRESSION(TreeNode* Node);//表达式
    bool IF(TreeNode* Node);//条件语句
    bool WHILE(TreeNode* Node);//当型循环语句
    bool CALL(TreeNode* Node);//过程调用语句
    bool READ(TreeNode* Node);//读语句
    bool WRITE(TreeNode* Node);//写语句
    bool BEGIN(TreeNode* Node);//复合语句
    bool EPSILON(TreeNode* Node);//空语句
    bool ITEM(TreeNode* Node);//项
    bool FACTOR(TreeNode* Node);//因子
    bool CONDITION(TreeNode* Node);//条件
    void prettydfs(int &index, int depth);
    void output();
    void TreeNodeToResult(TreeNode* Node);
    string getResult();
    TreeNode* getRoot(){return TreeRoot;}

private:
    string result;
    TreeNode* TreeRoot;
    int now;//当前正在处理的input
    int level;//当前正在处理分程序的循环，最多循环3层
    vector<Word> input;

    //用于规范输出
    vector<string> tokens;
    string prettyAns;
};