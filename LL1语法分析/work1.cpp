#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
typedef pair<pair<string,string>,string> PSSS;

string res;//待匹配串 
string res1;//已匹配串 
vector<PSSS> Table;	//存放预测分析表
stack<string> st;

//读入LL1文法判定处理的结果
void read_grammar()
{
	string str;
	char readchar[255]={}; //读入字符串
	
	FILE *fp;
	fp=fopen("../LL1文法判定/output1.txt","r");
	if(fp==NULL){
		cout<<"文件打开失败！"<<endl;
		exit(-1);
	}
	cout<<"文法读入成功！"<<endl;
	//逐行读入字符串并处理 
	while( fgets(readchar,255,fp)!=NULL ){
		str=readchar;
		str+=' '; 
		string tmp="";
		string s1="",s2="",s3="";
		for(int i=0;i<str.length();i++)
		{
			if(str[i]!=' '){
				tmp+=str[i];
			}
			else{
				if(s1=="") s1=tmp;
				else if(s2=="") s2=tmp;
				else if(s3=="") s3=tmp;
				tmp="";
			}
		}
		PSSS p;
		p.first.first=s1;
		p.first.second=s2;
		p.second=s3;
		Table.push_back(p);
	}
	//处理s3后的回车 
	for(int i=0;i<Table.size();i++)
	{
		string s4=Table[i].second;
		string s5="";
		for(int j=0;j<s4.length()-1;j++)
		{
			s5+=s4[j]; 
		}
		Table[i].second=s5;
	}
}
//输出栈的内容
void Display(stack<string> st)
{
	//cout<<"当前栈中元素为："<<endl;
	while(!st.empty()){
		cout<<right<<st.top();
		st.pop();
	}
}
//判断一个字符是否是大写字母
int jbs(char ch)
{
	if(ch>='A'&&ch<='Z') return 1;
	else return 0;
} 
//判断栈顶元素是否合法,合法返回1，否则返回0 
int judgetop(string s)
{
	if(s=="$") return 0;
	
	if(jbs(s[0]) || (!jbs(s[0]) && s[0]==res[0]) || s=="#") return 1;
	else return 0;
}
void show()
{
	cout<<res1<<'\t';
	Display(st);
	cout<<'\t'<<res<<'\t';
}
void LL1()
{
	string s="";
	s+='$'; 
	st.push(s);
	s=Table[0].first.first[0];
	st.push(s);
	while(judgetop(st.top())){
		string s1=st.top();
		if(jbs(s1[0])){
			st.pop();
			string stop=st.top();
			if(stop[0]=='\''){
				s1+='\'';
				st.pop();
			}
			int flag=0;
			string sres; 
			for(int i=0;i<Table.size();i++)
			{
				if(Table[i].first.first==s1 && Table[i].first.second[0]==res[0]){
					flag=1;
					sres=Table[i].second;
					break;
				}
			}
			if(flag==0){
				cout<<"\n该串识别失败！"<<endl;
				exit(0);
			}
			sres+=' ';
			int j=0;
			for(;j<sres.length();j++)
			{
				if(sres[j]!=':'&&sres[j-1]==':') break;
			}
			vector<string> vectmp; 
			for(;j<sres.length()-1;j++)
			{
				if(sres[j]=='\''||sres[j]==' '||sres[j]=='\n') continue;
				string s3="";
				s3+=sres[j];
				if(sres[j+1]=='\'') s3+=sres[j+1];
				if(s3!="") vectmp.push_back(s3);
			}
			for(int i=vectmp.size()-1;i>=0;i--)
			{
				st.push(vectmp[i]);
			}
			show();
			cout<<sres<<endl; 
		}
		else if(s1[0]==res[0]){
			res1+=res[0];
			st.pop();
			res=res.substr(1,res.length());
			show();
			cout<<"匹配"<<res[0]<<endl; 
		}
		else if(s1=="#"){
			st.pop();
			show();
			cout<<endl;
		}
		else{
			cout<<"\n该串识别失败！"<<endl;
			exit(0);
		}
	}
	
	if(st.top()=="$") cout<<"\n该串识别成功！"<<endl;
	else cout<<"\n该串识别失败！"<<endl;
}
int main()
{
	read_grammar();
	cout<<"请输入待识别串(以$结尾)：";
	cin>>res;
	res1=""; 
	cout<<"已匹配\t栈\t输入串\t动作"<<endl; 
	LL1();
	return 0;
} 