#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#define MAX_TRANS 100
using namespace std;

vector<char>characters;//输入字符集
int DFA_state_number=0;//DFA中状态数量
vector<vector<int> >transition(MAX_TRANS);//记录终态的名称变化(一对多)
int final_count=0;//终态数量
int trans_count=0;//transition大小

//得到的状态集合
struct States
{
    vector<int>jset;
};

//DFA状态转换关系
struct relation
{
    States* pre;
    char jchar;
    States* next;
};
//得到s闭包
int Eclosure(bigCell NFA,States *s)
{
    for(int i=0;i<s->jset.size();i++)
    {
        for(int j=0;j<NFA.count;j++)
        {
            if(NFA.edges[j].source.id==s->jset[i] && NFA.edges[j].symbol=='?')
            {
                s->jset.push_back(NFA.edges[j].target.id);
            }
        }
    }
}
//能够从状态集s出发通过字符tc转换到达的NFA状态集合，结果放在t中
void move(char tc,bigCell NFA,States* s,States* t)
{
    for(int i=0;i<s->jset.size();i++)
    {
        for(int j=0;j<NFA.count;j++)
        {
            if(NFA.edges[j].source.id==s->jset[i] && NFA.edges[j].symbol==tc)
                t->jset.push_back(NFA.edges[j].target.id);
        }
    }
}
//添加状态子集
bool add(vector<States*>allSet,States *newS)
{
    bool b=true;
    for(int i=0;i<allSet.size();i++)
    {
        if(allSet[i]->jset==newS->jset)
            b=false;
    }
    return b;
}
//去除重复的DFA转换关系
bool addRel(vector<relation*>relVec,States* preS,char c,States*nextS)
{
    bool b=true;
    for(int i=0;i<relVec.size();i++)
    {
        if(relVec[i]->pre->jset==preS->jset && relVec[i]->next->jset==nextS->jset
           && relVec[i]->jchar==c)
           b=false;
    }
    return b;
}

//状态名称合并并重命名
void Rename(vector<States*>allSet,States* tS,int &newS)
{
    //从1开始命名
    for(int i=0;i<allSet.size();i++)
    {
        if(tS->jset==allSet[i]->jset)
            newS=i+1;
    }
}

//数组去重
void only(int *a,int n,int* b)
{
    memset(b,0,sizeof(int)*n);
    char first=a[0];
    for(int i=0;i<n;i++)
    {
        b[a[i]-first]=a[i];
    }
    final_count=a[n-1]-first+1;
}

//主要函数
int* NFA2DFA(bigCell NFA,vector<vector<int>>&dfaT)
{
    States *newS=new States();
    newS->jset.push_back(NFA.start.id);//初始状态
    Eclosure(NFA,newS);//得到初始状态的闭包I
    vector<States*>allSet(1,newS);//DFA所有状态集合，初值为I的Eclosure

    int sizeOfStrVec=1;

    vector<States*>strVec(1,newS);//存储每次闭包操作多出的状态集合
    vector<relation*>relVec;//状态转换关系的集合

    //还有新增状态
    while(sizeOfStrVec)
    {
        int oldAllSet=allSet.size();//原本所有状态数
        for(int j=0;j<sizeOfStrVec;j++)
        {
            //cout<<j;

            //遍历所有输入字符
            for(int i=0;i<NFA.count;i++)
            {
                States *dest=new States();

                //含有输入字符的边
                if(NFA.edges[i].symbol!='?')
                {
                    move(NFA.edges[i].symbol,NFA,strVec[j],dest);//从strVec[j]出发通过symbol到达的所有状态存在dest中
                    Eclosure(NFA,dest);//U=dest

                    //U不在状态总集中
                    if(add(allSet,dest)&&dest->jset.size()!=0)
                    {
                        allSet.push_back(dest);
                    }

                    //将U加入DFA转换关系中
                    if(dest->jset.size()!=0)
                    {
                        relation* rDest=new relation();
                        rDest->pre=strVec[j];
                        rDest->jchar=NFA.edges[i].symbol;
                        rDest->next=dest;

                        //新的转换关系不与已有的重复,则加入relVec
                        bool isIn=addRel(relVec,rDest->pre,rDest->jchar,rDest->next);
                        if(isIn)
                        {
                            relVec.push_back(rDest);
                        }
                    }
                }
            }
        }

        //将新增状态加入strVec中
        strVec.clear();
        int newAllSet=allSet.size();
        for(int i=oldAllSet;i<allSet.size();i++)
        {
            States* dest=new States();
            dest=allSet[i];
            strVec.push_back(dest);
        }
        sizeOfStrVec=strVec.size();
    }

    /*加上以下代码可输出转换过程*/
//    cout<<"状态转换如下:"<<endl;
//    vector<relation*>::iterator it;
//    for(it=relVec.begin();it!=relVec.end();it++)
//    {
//        输出转换前状态
//        for(int i=0;i<(*it)->pre->jset.size()-1;i++)
//        {
//            cout<<(*it)->pre->jset[i]<<",";
//        }
//
//        输出输入字符
//        cout<<(*it)->pre->jset[(*it)->pre->jset.size()-1]<<"     "<<(*it)->jchar<<"     ";
//
//        输出转换后状态
//        for(int i=0;i<(*it)->next->jset.size()-1;i++)
//        {
//            cout<<(*it)->next->jset[i]<<",";
//        }
//        cout<<(*it)->next->jset[(*it)->next->jset.size()-1]<<endl;
//    }
//    cout<<endl;
//
//    输出状态重命名前后名称
//    int newName[MAX];
//    memset(newName,0,sizeof(char)*MAX);
//    cout<<"状态重命名如下："<<endl;
//    for(int i=0;i<allSet.size();i++)
//    {
//        newName[i]=i+1;
//        cout<<newName[i]<<":";
//        for(int j=0;j<allSet[i]->jset.size()-1;j++)
//        {
//            cout<<allSet[i]->jset[j]<<",";
//        }
//        cout<<allSet[i]->jset[allSet[i]->jset.size()-1]<<endl;
//    }
    DFA_state_number=allSet.size();

    vector<relation*>DFA;
    //创建重命名后的新边集合
    for(int i=0;i<relVec.size();i++)
    {
        relation* newRel=new relation();
        //改名
        int newPre,newNext;
        Rename(allSet,relVec[i]->pre,newPre);
        Rename(allSet,relVec[i]->next,newNext);
        //创建新边
        States* tpre=new States();
        States* tnext=new States();
        newRel->pre=tpre;
        newRel->next=tnext;
        newRel->pre->jset.push_back(newPre);
        newRel->next->jset.push_back(newNext);
        newRel->jchar=relVec[i]->jchar;
        DFA.push_back(newRel);

    }

    vector<relation*>::iterator rit;
    for(rit=DFA.begin();rit!=DFA.end();rit++)
    {
        dfaT[(*rit)->pre->jset[0]][(*rit)->jchar]=(*rit)->next->jset[0];
        characters.push_back((*rit)->jchar);
    }
    //去除字符集重复元素
    sort(characters.begin(),characters.end());
    characters.erase(unique(characters.begin(), characters.end()), characters.end());

    //终态
    int l=allSet.size();
    int *accepted=new int[l];
    //memset(accepted,0,sizeof(int)*l);
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<allSet[i]->jset.size();j++)
        {
            //cout<<allSet[i]->jset[j]<<endl;
            for(int k=0;k<NFA.end.size();k++)
            {
                if(allSet[i]->jset[j]==NFA.end[k].id)
                {
                    accepted[final_count++]=i+1;
                    transition[i+1].push_back(NFA.end[k].id);//记录终态重命名前后
                    trans_count=i+2;
                    //cout<<i+1<<" "<<transition[i+1][1]<<endl;
                }
            }
        }
    }
    int *outs=new int[final_count];
    only(accepted,final_count,outs);
    delete []accepted;
    delete newS;
    return outs;
}
int showDFA(vector<vector<int>>dfaT,int *accepted)
{
    cout<<"DFA信息:"<<endl;
    for(int i=1;i<=DFA_state_number;i++)
    {
        for(int j=0;j<characters.size();j++)
        {
            char c=characters[j];
            cout<<i<<" "<<c<<" "<<dfaT[i][c]<<endl;
        }
    }

    cout<<"可接受状态为："<<endl;
    for(int i=0;i<final_count;i++)
    {
        cout<<accepted[i]<<" ";
    }
    cout<<endl<<endl;
}
