#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#define MAX_TRANS 100
using namespace std;

vector<char>characters;//�����ַ���
int DFA_state_number=0;//DFA��״̬����
vector<vector<int> >transition(MAX_TRANS);//��¼��̬�����Ʊ仯(һ�Զ�)
int final_count=0;//��̬����
int trans_count=0;//transition��С

//�õ���״̬����
struct States
{
    vector<int>jset;
};

//DFA״̬ת����ϵ
struct relation
{
    States* pre;
    char jchar;
    States* next;
};
//�õ�s�հ�
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
//�ܹ���״̬��s����ͨ���ַ�tcת�������NFA״̬���ϣ��������t��
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
//���״̬�Ӽ�
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
//ȥ���ظ���DFAת����ϵ
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

//״̬���ƺϲ���������
void Rename(vector<States*>allSet,States* tS,int &newS)
{
    //��1��ʼ����
    for(int i=0;i<allSet.size();i++)
    {
        if(tS->jset==allSet[i]->jset)
            newS=i+1;
    }
}

//����ȥ��
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

//��Ҫ����
int* NFA2DFA(bigCell NFA,vector<vector<int>>&dfaT)
{
    States *newS=new States();
    newS->jset.push_back(NFA.start.id);//��ʼ״̬
    Eclosure(NFA,newS);//�õ���ʼ״̬�ıհ�I
    vector<States*>allSet(1,newS);//DFA����״̬���ϣ���ֵΪI��Eclosure

    int sizeOfStrVec=1;

    vector<States*>strVec(1,newS);//�洢ÿ�αհ����������״̬����
    vector<relation*>relVec;//״̬ת����ϵ�ļ���

    //��������״̬
    while(sizeOfStrVec)
    {
        int oldAllSet=allSet.size();//ԭ������״̬��
        for(int j=0;j<sizeOfStrVec;j++)
        {
            //cout<<j;

            //�������������ַ�
            for(int i=0;i<NFA.count;i++)
            {
                States *dest=new States();

                //���������ַ��ı�
                if(NFA.edges[i].symbol!='?')
                {
                    move(NFA.edges[i].symbol,NFA,strVec[j],dest);//��strVec[j]����ͨ��symbol���������״̬����dest��
                    Eclosure(NFA,dest);//U=dest

                    //U����״̬�ܼ���
                    if(add(allSet,dest)&&dest->jset.size()!=0)
                    {
                        allSet.push_back(dest);
                    }

                    //��U����DFAת����ϵ��
                    if(dest->jset.size()!=0)
                    {
                        relation* rDest=new relation();
                        rDest->pre=strVec[j];
                        rDest->jchar=NFA.edges[i].symbol;
                        rDest->next=dest;

                        //�µ�ת����ϵ�������е��ظ�,�����relVec
                        bool isIn=addRel(relVec,rDest->pre,rDest->jchar,rDest->next);
                        if(isIn)
                        {
                            relVec.push_back(rDest);
                        }
                    }
                }
            }
        }

        //������״̬����strVec��
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

    /*�������´�������ת������*/
//    cout<<"״̬ת������:"<<endl;
//    vector<relation*>::iterator it;
//    for(it=relVec.begin();it!=relVec.end();it++)
//    {
//        ���ת��ǰ״̬
//        for(int i=0;i<(*it)->pre->jset.size()-1;i++)
//        {
//            cout<<(*it)->pre->jset[i]<<",";
//        }
//
//        ��������ַ�
//        cout<<(*it)->pre->jset[(*it)->pre->jset.size()-1]<<"     "<<(*it)->jchar<<"     ";
//
//        ���ת����״̬
//        for(int i=0;i<(*it)->next->jset.size()-1;i++)
//        {
//            cout<<(*it)->next->jset[i]<<",";
//        }
//        cout<<(*it)->next->jset[(*it)->next->jset.size()-1]<<endl;
//    }
//    cout<<endl;
//
//    ���״̬������ǰ������
//    int newName[MAX];
//    memset(newName,0,sizeof(char)*MAX);
//    cout<<"״̬���������£�"<<endl;
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
    //��������������±߼���
    for(int i=0;i<relVec.size();i++)
    {
        relation* newRel=new relation();
        //����
        int newPre,newNext;
        Rename(allSet,relVec[i]->pre,newPre);
        Rename(allSet,relVec[i]->next,newNext);
        //�����±�
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
    //ȥ���ַ����ظ�Ԫ��
    sort(characters.begin(),characters.end());
    characters.erase(unique(characters.begin(), characters.end()), characters.end());

    //��̬
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
                    transition[i+1].push_back(NFA.end[k].id);//��¼��̬������ǰ��
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
    cout<<"DFA��Ϣ:"<<endl;
    for(int i=1;i<=DFA_state_number;i++)
    {
        for(int j=0;j<characters.size();j++)
        {
            char c=characters[j];
            cout<<i<<" "<<c<<" "<<dfaT[i][c]<<endl;
        }
    }

    cout<<"�ɽ���״̬Ϊ��"<<endl;
    for(int i=0;i<final_count;i++)
    {
        cout<<accepted[i]<<" ";
    }
    cout<<endl<<endl;
}
