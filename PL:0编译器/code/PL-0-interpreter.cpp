#include <fstream>
#include <stack>
#include "GAandSA.h"

const int maxn=1e7+1;
int RunDataStack[maxn];
vector<TargetCodeItem> codes;
int findAddr(int addr,int level){
    while(level){
        addr=RunDataStack[addr];
        level--;
    }
    return addr;
}
int getint(){
    int x=0,s=1; char ch=' ';
    while(ch<'0' || ch>'9'){ ch=getchar(); if(ch=='-') s=-1;}
    while(ch>='0' && ch<='9'){ x=x*10+ch-'0'; ch=getchar();}
    return x*s;
}
int main(){
    ifstream fin("program.code");
    if(!fin.is_open()){
        cerr<<"Cannot open program.code"<<endl;
        return -1;
    }
    while(!fin.eof()){
        string f; int l,a;
        fin>>f>>l>>a;
        codes.emplace_back(f,l,a);
    }
    int PC=0,SP=0,BP=1;
    TargetCodeItem tci;
    while(true){
        tci=codes[PC++];
        if(tci.f=="LIT") RunDataStack[++SP]=tci.a;
        else if(tci.f=="LOD") RunDataStack[++SP]=RunDataStack[findAddr(BP,tci.l)+tci.a];
        else if(tci.f=="STO") RunDataStack[findAddr(BP,tci.l)+tci.a]=RunDataStack[SP--];
        else if(tci.f=="INT") SP+=tci.a;
        else if(tci.f=="JMP") PC=tci.a;
        else if(tci.f=="JPC"){
            if(!RunDataStack[SP]) PC=tci.a;
            SP--;
        } else if(tci.f=="CAL"){
            RunDataStack[SP+1]=findAddr(BP,tci.l);
            RunDataStack[SP+2]=BP;
            RunDataStack[SP+3]=PC;
            BP=SP+1;
            PC=tci.a;
        } else if(tci.f=="OPR"){
            if(tci.a==Plus) RunDataStack[SP-1]+=RunDataStack[SP],SP--;
            else if(tci.a==Minus) RunDataStack[SP-1]-=RunDataStack[SP],SP--;
            else if(tci.a==Mul) RunDataStack[SP-1]*=RunDataStack[SP],SP--;
            else if(tci.a==Div) RunDataStack[SP-1]/=RunDataStack[SP],SP--;
            else if(tci.a==Neg) RunDataStack[SP]=-RunDataStack[SP];
            else if(tci.a==Odd) RunDataStack[SP]%=2;
            else if(tci.a==Eq) RunDataStack[SP-1]=RunDataStack[SP-1]==RunDataStack[SP],SP--;
            else if(tci.a==Neq) RunDataStack[SP-1]=RunDataStack[SP-1]!=RunDataStack[SP],SP--;
            else if(tci.a==Less) RunDataStack[SP-1]=RunDataStack[SP-1]<RunDataStack[SP],SP--;
            else if(tci.a==Greater) RunDataStack[SP-1]=RunDataStack[SP-1]>RunDataStack[SP],SP--;
            else if(tci.a==Le) RunDataStack[SP-1]=RunDataStack[SP-1]<=RunDataStack[SP],SP--;
            else if(tci.a==Ge) RunDataStack[SP-1]=RunDataStack[SP-1]>=RunDataStack[SP],SP--;
            else if(tci.a==Write) cout<<RunDataStack[SP--]<<endl;
            else if(tci.a==Read) {
                int tempx; cin>>tempx;
                RunDataStack[++SP]=tempx;
            }
            else if(tci.a==Return) {
                SP=BP-1;
                PC=RunDataStack[SP+3];
                BP=RunDataStack[SP+2];
            }
        } else {
            cerr<<"INTERPRETER Error!"<<endl;
            return -1;
        }
        if(PC==0){
            return 0;
        }
    }
    return -1;
}
