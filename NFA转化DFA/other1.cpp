#include<stdio.h>
#include<malloc.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include<set>

using namespace std;

#define max 50//�������������
typedef char vertype;//������ֵ��Ϊ�ַ�����
char buf[1024];//������ļ�����ʱ�����򻺳�����
int num;//��¼������ĸ��Ԫ�صĸ���
int length;//��¼States����ĳ���

typedef struct arcnode//ͼ�ı���Ϣ
{
    int adjvex;
    int weight;//������Ӧ��Ȩֵ
    struct arcnode *next;
}arcnode;

typedef struct vnode//ͼ�Ľ�����Ͷ���
{
    vertype data;
    arcnode *next;
}vnode,adjlist[max];

typedef struct//ͼ�Ķ���
{
    adjlist a;
    int vexnum,arcnum;
}algraph;

typedef struct state//״̬�Ķ���
{
    set<int> Set;
    char name;
}state;

state States[max];

int change(algraph g,char p)//��ͼ�н��ת��Ϊ��Ӧ�±�
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
        if(strlen(buf)>1)//��ȡ�ļ���ͼ�Ľ�����
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

    rewind(fpr);//���ļ�ָ�뷵�ص���ͷλ��
    line = 0;
        arcnode *s;

    while(!feof(fpr))//�ٴ�ɨ���ļ����ߵ���Ϣ���ϣ�����ͼ
    {
        int weight=0;//������Ӧ��Ȩֵ,ÿһ��Ȩֵ����0��ʼ
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

state move(state s,int n,algraph g)//��ǰ״̬���ϵ�ת�Ƽ��ϣ�����s״̬ʶ����ĸn֮���״̬����
{
    state temp;
    arcnode *m;
    set<int>::iterator itr;//������
    for(itr = s.Set.begin();itr!=s.Set.end();itr++)//������ǰs״̬�м���Ԫ��
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

int equalSet(state m,state n)//�Ƚ�����״̬��set���������Ƿ����
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

void inStates(state &s)//�жϵ�ǰ״̬�Ƿ���States�����д��ڣ������ڣ����и������������ڣ����������States����
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
            if(equalSet(States[i],s)==1)//�����ڣ����и���
            {
                s.name=States[i].name;
                flag = 1;
                break;
            }
        }

        if(flag == 0)//�������ڣ����������States����
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

        set<int>::iterator itr;//������
        for(itr = States[i].Set.begin();itr!=States[i].Set.end();itr++)//������ǰs״̬�м���Ԫ��
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
            //�鿴temp״̬��set����
            /*
            set<int>::iterator itr;//������
            for(itr = temp.Set.begin();itr!=temp.Set.end();itr++)//������ǰs״̬�м���Ԫ��
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

    //create����
    /*
    for(int i=0;i<g.vexnum;i++)
    {
        cout<<g.a[i].data<<endl;//////////////////

    }
    */

    changeToD(g,fpw);
    //move����
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