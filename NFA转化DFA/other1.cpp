#include<stdio.h>
#include<malloc.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include<set>

using namespace std;

#define max 50//定义结点最大数量
typedef char vertype;//定义结点值域为字符类型
char buf[1024];//定义读文件内容时，程序缓冲数组
int num;//记录有穷字母表元素的个数
int length;//记录States数组的长度

typedef struct arcnode//图的边信息
{
    int adjvex;
    int weight;//边所对应的权值
    struct arcnode *next;
}arcnode;

typedef struct vnode//图的结点类型定义
{
    vertype data;
    arcnode *next;
}vnode,adjlist[max];

typedef struct//图的定义
{
    adjlist a;
    int vexnum,arcnum;
}algraph;

typedef struct state//状态的定义
{
    set<int> Set;
    char name;
}state;

state States[max];

int change(algraph g,char p)//将图中结点转化为对应下标
{
    int i;
    for(i=0;i<g.vexnum;i++)
    {
        if(p==g.a[i].data)
            return i;
    }
    return -1;
}

void creatph(algraph &g,FILE *fpr)
{
    int line = 0;
    while(!feof(fpr))
    {
        fgets(buf,1024,fpr);
        if(strlen(buf)>1)//获取文件中图的结点个数
        {
            int i = 0;
            while(buf[i]==' ')
            {
                i++;
            }

            g.a[line].data=buf[i];
            g.a[line].next=NULL;
            line++;
        }
    }
    g.vexnum=line;

    rewind(fpr);//将文件指针返回到开头位置
    line = 0;
        arcnode *s;

    while(!feof(fpr))//再次扫描文件将边的信息添上，构造图
    {
        int weight=0;//边所对应的权值,每一行权值都从0开始
        fgets(buf,1024,fpr);
        if(strlen(buf)>1)
        {
            for(int i=0;i<strlen(buf)-1;i++)
            {
                if(buf[i]=='{')
                {
                    weight++;
                    if(num<weight)
                        num=weight;

                    i++;
                    if(buf[i]=='N')
                            i=i+4;

                    while(buf[i]!='}')
                    {
                        if(buf[i]!=',')
                        {
                            //cout<<buf[i];////////////////////////////////
                            int x = change(g,buf[i]);

                            s=(arcnode *)malloc(sizeof(arcnode));
                            s->adjvex=x;
                            s->weight=weight;
                            s->next=g.a[line].next;
                            g.a[line].next=s;
                            //cout<<line;////////////////////////////////
                        }
                        i++;
                    }
                }
            }
            line++;
        }
    }

}

state move(state s,int n,algraph g)//求当前状态集合的转移集合，即求s状态识别字母n之后的状态集合
{
    state temp;
    arcnode *m;
    set<int>::iterator itr;//迭代器
    for(itr = s.Set.begin();itr!=s.Set.end();itr++)//遍历当前s状态中集合元素
    {
        int i = *itr;
        m = g.a[i].next;
        while(m)
        {
            if(m->weight==n)
            {
                temp.Set.insert(m->adjvex);//cout<<m->adjvex<<" ";
            //    temp.name=s.name+1;//cout<<temp.name<<endl;
            }
            m=m->next;
        }
    }
    return temp;
}

int equalSet(state m,state n)//比较两个状态的set集合内容是否相等
{
    int flag = 1;
    if(m.Set.size()!=n.Set.size())
    {
        flag = 0;
        return flag;
    }

    set<int>::iterator itrm;
    set<int>::iterator itrn;
    for(itrm = m.Set.begin(),itrn = n.Set.begin();itrm!=m.Set.end();itrm++,itrn++)
    {
        int m = *itrm;
        int n = *itrn;

        if(m!=n)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

void inStates(state &s)//判断当前状态是否在States数组中存在，若存在，进行改名；若不存在，改名后加入States数组
{
    int flag = 0;
    if(length==0)
    {
        States[0]=s;
        States[0].name='A';
        length++;
    }
    else
    {
        for(int i=0;i<length;i++)
        {
            //cout<<equalSet(States[i],s);
            if(equalSet(States[i],s)==1)//若存在，进行改名
            {
                s.name=States[i].name;
                flag = 1;
                break;
            }
        }

        if(flag == 0)//若不存在，改名后加入States数组
        {
            s.name=States[length-1].name+1;
            States[length]=s;
            length++;
        }
    }
}

void changeToD(algraph g,FILE *fpw)
{
    state s,temp;
    s.Set.insert(0);
    s.name='A';

    inStates(s);

    for(int i=0;i<length;i++)
    {
        cout<<"{";
        fprintf(fpw,"{");

        set<int>::iterator itr;//迭代器
        for(itr = States[i].Set.begin();itr!=States[i].Set.end();itr++)//遍历当前s状态中集合元素
        {
            int i = *itr;
            cout<<g.a[i].data<<",";
            fprintf(fpw,"%c,",g.a[i].data);
        }

        cout<<"}";
        fprintf(fpw,"}");

        cout<<States[i].name;
        fprintf(fpw,"%c",States[i].name);

        for(int j=1;j<=num;j++)
        {
            temp = move(States[i],j,g);
            inStates(temp);
            //查看temp状态的set集合
            /*
            set<int>::iterator itr;//迭代器
            for(itr = temp.Set.begin();itr!=temp.Set.end();itr++)//遍历当前s状态中集合元素
            {
                int i = *itr;
                cout<<i<<" ";
            }*/
            cout<<temp.name;
            fprintf(fpw,"%c",temp.name);
        }
        cout<<endl;
        fprintf(fpw,"\n");
    }
}


int main()
{
    algraph g;

    FILE *fpr = fopen("test.txt","r");
    FILE *fpw = fopen("testOutput.txt","w");


/*    FILE *fpr = fopen("test.txt","r");
    FILE *fpw = fopen("output.txt","w");*/

    creatph(g,fpr);

    //create测试
    /*
    for(int i=0;i<g.vexnum;i++)
    {
        cout<<g.a[i].data<<endl;//////////////////

    }
    */

    changeToD(g,fpw);
    //move测试
    /*
    state s;
    s.Set.insert(0);
    s.Set.insert(2);
    s.Set.insert(3);
    s.mark=1;
    s.name='B';

    move(s,2,g);
    */
    return 0;
}