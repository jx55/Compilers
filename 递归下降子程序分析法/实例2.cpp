/*
递归下降分析法设计与实现
定义文法：
（1）E→TG
（2）G→ATG
（3）G→ε
（4）T→FH
（5）H→MFH
（6）H→ε
（7）F→i
（8）A→+
（9）A→-
（10）M→*
（11）M→/
*/
#include <iostream>
#include <fstream>
#include <dos.h>
#include <cstring>
#include <stdlib.h>
using namespace std;
char a[100], b[100], d[500], e[10];
char ch;
int n1,i1=0,flag=1,n=5;
 
int E1();
int E();
int G();
int T();
int H();
int F();
int A();
int M();
void input();
void input1();
void output();

int E1()
{
    int f,t;
    cout<<"E->TG\t";
    flag=1;
    input();
    input1();
    f=T();
    if (f==0)
        return(0);
    t=G();
    if (t==0)
        return(0);
    else
        return(1);
}
 
int E()
{
    int f,t;
    cout<<"E->TG\t";
    e[0]='E';
    e[1]='=';
    e[2]=' ';
    e[3]='T';
    e[4]='G';
    e[5]='#';
    output();
    flag=1;
    input();
    input1();
    f=T();
    if (f==0)
        return(0);
    t=G();
    if (t==0)
        return(0);
    else
        return(1);
}
 
int G()
{
    int f,t,g;
    if(ch=='+'||ch=='-')
    {
        cout<<"G→ATG\t";
        e[0]='G';
        e[1]='=';
        e[2]=' ';
        e[3]='A';
        e[4]='T';
        e[5]='G';
        e[6]='#';
        output();
        flag=1;
        input();
        input1();
        f=A();
        if (f==1)
        {
            t=T();
            if(t==0)
            {
                cout<<"error"<<endl;
                return (0);
            }
            g=G();
            if (g==0)
                return(0);
            return(1);
        }
    }
    cout<<"G->^\t";
    e[0]='G';
    e[1]='=';
    e[2]=' ';
    e[3]='^';
    e[4]='#';
    output();
    flag=1;
    input();
    input1();
    return(1);
}
 
int T()
{
    int f,t;
    printf("T->FH\t");
    e[0]='T';
    e[1]='=';
    e[2]=' ';
    e[3]='F';
    e[4]='H';
    e[5]='#';
    output();
    flag=1;
    input();
    input1();
    f=F();
    if (f==0) return(0);
    t=H();
    if (t==0) return(0);
    else return(1);
}

int H()
{
    int f,t,g;
    if(ch=='*'||ch=='/')
    {
        cout<<"H->MFH\t";
        e[0]='H';
        e[1]='=';
        e[2]=' ';
        e[3]='M';
        e[4]='F';
        e[5]='H';
        e[6]='#';
        output();
        flag=1;
        input();
        input1();
        f=M();
        if (f==1)
        {
            t=F();
            if(t==0)
            {
                cout<<"error"<<endl;
                return (0);
            }
            g=H();
            if (g==0)
                return(0);
            return(1);
        }
    }
 
    cout<<"H->^\t";
    e[0]='H';
    e[1]='=';
    e[2]=' ';
    e[3]='^';
    e[4]='#';
    output();
    flag=1;
    a[i1]=ch;
    input();
    input1();
    return(1);
}
 
int F()
{
    if(ch=='i')
    {
        b[i1]=ch;
        cout<<"F->i\t";
        e[0]='F';
        e[1]='=';
        e[2]=' ';
        e[3]='i';
        e[4]='#';
        output();
        flag=0;
        input();
        input1();
        ch=a[++i1];
    }
    else
    {
        cout<<"error\n";
        return(0);
    }
    return(1);
}
 
int A()
{
    if(ch=='+')
    {
        b[i1]=ch;
        cout<<"A->+\t";
        e[0]='A';
        e[1]='=';
        e[2]=' ';
        e[3]='+';
        e[4]='#';
        output();
        flag=0;
        input();
        input1();
        ch=a[++i1];
    }
    else if(ch=='-')
    {
        b[i1]=ch;
        cout<<"A->-\t";
        e[0]='A';
        e[1]='=';
        e[2]=' ';
        e[3]='-';
        e[4]='#';
        output();
        flag=0;
        input();
        input1();
        ch=a[++i1];
    }
    else
    {
        cout<<"error\n";
        return(0);
    }
    return(1);
}
 
int M()
{
    if(ch=='*')
    {
        b[i1]=ch;
        cout<<"M->*\t";
        e[0]='M';
        e[1]='=';
        e[2]=' ';
        e[3]='*';
        e[4]='#';
        output();
        flag=0;
        input();
        input1();
        ch=a[++i1];
    }
    else if(ch=='/')
    {
        b[i1]=ch;
        cout<<"M->/\t";
        e[0]='M';
        e[1]='=';
        e[2]=' ';
        e[3]='/';
        e[4]='#';
        output();
        flag=0;
        input();
        input1();
        ch=a[++i1];
    }
    else
    {
        cout<<"error\n";
        return(0);
    }
    return(1);
}
 
void input()
{
    for (int j=0; j<=i1-flag; j++)
        cout<<b[j];     //输出分析串
    cout<<"\t\t";
    cout<<ch<<"\t\t";   //输出分析字符
}
 
void input1()
{
    for (int j=i1+1-flag; j<n1; j++)
        cout<<a[j];     //输出剩余字符
    cout<<endl;
}
 
//推导式计算
void output()                               
{
    int m,k,j,q;
    int i=0;
    m=0;
    k=0;
    q=0;
    i=n;
    d[n]='=';
    d[n+1]=' ';
    d[n+2]='#';
    n=n+2;
    i=n;
    i=i-2;
    while(d[i]!=' '&&i!=0)
        i=i-1;
    i=i+1;
    while(d[i]!=e[0])
        i=i+1;
    q=i;
    m=q;
    k=q;
    while(d[m]!=' ')
        m=m-1;
    m=m+1;
    while(m!=q)
    {
        d[n]=d[m];
        m=m+1;
        n=n+1;
    }
    d[n]='#';
    for(j=3; e[j]!='#'; j++)
    {
        d[n]=e[j];
        n=n+1;
    }
    k=k+1;
    while(d[k]!='=')
    {
        d[n]=d[k];
        n=n+1;
        k=k+1;
    }
    d[n]='#';
}
 
//递归分析
int main()                      
{
    int p;
    char x;
    d[0]='E';
    d[1]='=';
    d[2]=' ';
    d[3]='T';
    d[4]='G';
    d[5]='#';
    //从文件中读入除换行符之外的字符
    ifstream infile("test4.txt",ios::in);
    if(!infile)
    {
        cerr<<"open error!"<<endl;
        exit(1);
    }
    char c='c',f[50]= {""};
    int i=0,k=0,l=0;
    while(infile.peek()!=EOF)
    {
        infile.get(c);
        if(c!='\n')
        {
            f[i]=c;
            i++;
        }
    }
    //给操作数组a赋初值，依次进行操作
    do
    {
        a[k]=f[l];
        if(f[l]=='#')
        {
            cout<<"从文件读入字符串(长度<50,以#号结束）\n";
            for(int j=0; a[j]!='\0'; j++)
                cout << a[j];
            n1=i;
            ch=b[0]=a[0];
            cout<<"\n文法\t分析串\t\t分析字符\t剩余串\n";
            E1();
            if (ch=='#')
            {
                cout<<"accept"<<endl;
                p=0;
                x=d[p];
                while(x!='#')
                {
                    cout<<x;
                    p=p+1;
                    x=d[p];     //输出推导式
                }
            }
            else
            {
                cout<<"error"<<endl;
                cout<<"回车返回"<<endl;
                return (0);
            }
            cout<<endl;
            cout<<"继续>>>>\n"<<endl;
            memset(a,'\0',sizeof(a));
            memset(b,'\0',sizeof(b));
            memset(e,'\0',sizeof(e));
            i1=0,flag=1,n=5;
            k=-1;
        }
        k++;
        l++;
    }
    while(f[l]!='\0');
    infile.close();
    return 0;
}