#include <iostream>
#include <cstdio> 
#include <cstring>
#include <map>
#include <vector>
using namespace std;

vector< pair<string,int> > Content_pre;//存储每行的内容及对应的行号 
vector< pair<string,int> > Content;//存储处理后每行的内容 

map<string,int> Type,ExistWord;//基本类型、条件 
map<string,int> VarDef;	//已定义的变量 
map<string,int> CalcSymbol;//运算符号 
map<string,int> RelSymbol;//关系符号
map<string,int> PrimSymbol;//基本符号
map<string,int> Symbol;//符号,; 

vector<string> vec;//存放分割的字符串 
int line;	//记录行号
int TypeFlag=0;//标记每行是否存在基本类型
int Annotate=0;//标记是否存在多行注释
int Unknown_error=0;//标记程序是否有未知的错误
int Annotate_line=0;//记录出现注释错误的行号 

//过滤注释
void FilterAnnotate()
{
	int n=Content_pre.size();
	for(int i=0;i<n;i++)
	{
		int IsHandle=0;
		string s=Content_pre[i].first;
		if(Annotate==0){
			for(int j=0;j<s.length();j++)
			{
				if(s[j]=='/'&&s[j+1]=='/'){//该行存在单行注释
					string tmp(s,0,j);
					if(tmp!="") 
						Content.push_back(make_pair<string,int>(tmp,Content_pre[i].second));
					IsHandle=1;
					break; 
				}
				else if(s[j]=='/' && s[j+1]=='*'){//该行存在多行注释的开头 
					string tmp(s,0,j); 
					if(tmp!="")
						Content.push_back(make_pair<string,int>(tmp,Content_pre[i].second));
					Annotate=1;
					IsHandle=1;
					break;
				}
				else if(s[j]=='*'&&s[j+1]=='/'){//该行存在多行注释的结束 
					Unknown_error=1;
					Annotate_line=Content_pre[i].second;
					IsHandle=1;
					break;
				}
			}
			if(IsHandle==0){
				Content.push_back(make_pair<string,int>(s,Content_pre[i].second));
			}
		}
		else{//已经存在多行注释了 
			for(int j=0;j<s.length();j++)
			{
				if(s[j]=='*'&&s[j+1]=='/'){//找到了多行注释的结尾
					string tmp(s,j+2,s.length()-j-3);
					if(tmp!="")
						Content.push_back(make_pair<string,int>(tmp,Content_pre[i].second));
					Annotate=0;
					break; 
				} 
			}
		}
	}
}

//初始化 
void Init()
{
	Type["int"]=1;Type["float"]=1;Type["double"]=1;//Type["void"]=1;
	
	ExistWord["if"]=1;ExistWord["else"]=1;ExistWord["do"]=1;ExistWord["while"]=1;
	ExistWord["for"]=1;ExistWord["break"]=1;ExistWord["continue"]=1;ExistWord["return"]=1;
	
	CalcSymbol["+"]=1; CalcSymbol["-"]=1; CalcSymbol["*"]=1; CalcSymbol["/"]=1; 
	CalcSymbol["%"]=1; 
	
	RelSymbol["="]=1; RelSymbol["!="]=1;RelSymbol["=="]=1; RelSymbol["="]=1; 
	RelSymbol["!"]=1; RelSymbol["&&"]=1; RelSymbol["||"]=1; RelSymbol[">"]=1;
	RelSymbol["<"]=1; RelSymbol[">="]=1; RelSymbol["<="]=1; RelSymbol["="]=1; 
	 
	PrimSymbol["("]=1;PrimSymbol[")"]=1;PrimSymbol["["]=1;PrimSymbol["]"]=1;
	PrimSymbol["{"]=1;PrimSymbol["}"]=1;
	
	Symbol[";"]=1;Symbol[","]=1;
} 
//判断是否是数字
int IsNumber(string s)
{
	int flag=1;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]<'0' || s[i]>'9'){
			flag=0;
			break;
		} 
	}
	if(flag==0) return 0;
	else return 1;
}
//判断是否是合格的标识符
int IsIdent(string s)
{
	int flag=1;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='_' || (s[i]>='a'&&s[i]<='z') || (s[i]>='A'&&s[i]<='Z') || (s[i]>='0'&&s[i]<='9') )
			continue;
		else{
			flag=0;
			break;
		}
	}
	if(s[0]>='0'&&s[0]<='9') flag=0;
	if(flag==1) return 1;
	else return 0;
} 
//判断标识符之前是否定义过
int CheckIdent(string s)
{
	if(VarDef[s]) return 1;
	else return 0;
} 
//判断一个字符是否是大小写或者数字或者下划线
int JudgeChar(char ch)
{
	if((ch>='0'&&ch<='9') || (ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z') || ch=='_')
		return 1;
	else 
		return 0;
} 
//将程序段分割成单个的单词，保存到vec中 
void CutStr(string s)
{
	while(vec.size()>0) vec.pop_back(); 
	string tmp="";
	for(int i=0;i<s.length();i++)
	{
		if(JudgeChar(s[i]) ) tmp+=s[i];
		else{
			if(tmp!="" && JudgeChar(tmp[0])){
				vec.push_back(tmp);
				tmp="";
			}
			if(s[i]!=' ') tmp+=s[i];
			if(JudgeChar(s[i+1])&&tmp!=""){
				vec.push_back(tmp);
				tmp="";
			}
		}
		if(s[i]==','||s[i]==';'||s[i]=='{'||s[i]=='}'){
			if(tmp!="" && JudgeChar(tmp[0])){
				vec.push_back(tmp);
				tmp="";
			}
			string ss="";
			ss+=s[i];
			vec.push_back(ss);
		}
	}                         
}
//判断一个字符串是否全是空格,全是空格返回1，否则返回0 
int Judgespace(string s)
{
	int flag=1;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]==' '||s[i]=='\t'){
			continue;
		}
		else{
			flag=0;
			break;
		}
	}
	if(flag) return 1;
	else return 0;
}                        
//分别处理每个单词，输出单词属于的编码体系 
//Keyword：关键词、Ident：标识符 、Operator：运算符 、Separator：分隔符、Number：数字
void Handle(vector<string> s)
{
	TypeFlag=0;
	for(int i=0;i<s.size();i++)
	{		
		if(Type[s[i]]){
			cout<<"< BType,"<<s[i]<<" >"<<endl;
			TypeFlag=1; 
		}
		else if(ExistWord[s[i]]) cout<<"< Keyword, "<<s[i]<<" >"<<endl;
		else if(CalcSymbol[s[i]]) cout<<"< Operator, "<<s[i]<<" >"<<endl;
		else if(RelSymbol[s[i]]) cout<<"< Operator, "<<s[i]<<" >"<<endl;
		else if(PrimSymbol[s[i]]){
			cout<<"< Separator, "<<s[i]<<" >"<<endl;
		}
		else if(Symbol[s[i]]){
			cout<<"< Symbol, "<<s[i]<<" >"<<endl;
		}
		else if(IsNumber(s[i])) cout<<"< Number,"<<s[i]<<" >"<<endl;
		else{
			if(IsIdent(s[i])){
				if(TypeFlag==1){
					if(VarDef[s[i]]==0){
						VarDef[s[i]]=1;
						cout<<"< Ident,"<<s[i]<<" >"<<endl;
					}
					else{
						cout<<s[i]<<"\t";
						cout<<"Error:[定义错误] at line"<<line<<":该变量已经定义，不能重复定义。"<<endl;
					}
				}
				else{
					if(VarDef[s[i]]==0){
						cout<<s[i]<<"\t";
						cout<<"Error:[使用错误] at line"<<line<<":变量未定义。"<<endl;
					}
					else{
						cout<<"< Ident,"<<s[i]<<" >"<<endl;
					} 
				} 
			}
			else{
				if(Judgespace(s[i])) continue;
				else{
					cout<<s[i]<<"\t";
					cout<<"Error:[定义错误] at line"<<line<<":标识符定义不符合规定。12"<<endl;
				} 
			} 
		}
	}
}
void Displayvec(vector<string> v)
{
	cout<<"该数组内容为：" <<endl;
	for(int i=0;i<v.size();i++)
		cout<<v[i]<<endl;
	cout<<endl;	
}

//输出每行的结果
void Display(vector<pair<string,int> > v)
{
	for(int i=0;i<v.size();i++)
	{
		cout<<v[i].first<<" "<<v[i].second<<endl; 
	}
	cout<<endl;
} 
int main()
{
	Init();
	string str;
	int linenum=1;
	char readchar[255]={}; //读入字符串
	
	FILE *fp;
	fp=fopen("test.txt","r");
	if(fp==NULL){
		cout<<"文件打开失败！"<<endl;
		exit(-1);
	}
	cout<<"词法分析完成,结果如下："<<endl;
	//逐行读入字符串并处理 
	while( fgets(readchar,255,fp)!=NULL ){
		str=readchar;
		Content_pre.push_back(make_pair<string,int>(str,linenum));
		linenum++;
	}
	FilterAnnotate();
	if(Unknown_error==1 || Annotate==1){
		if(Unknown_error==1){
			cout<<"Error:[注释错误] at line"<<Annotate_line<<":注释不符合规范。"<<endl;
		}
		else{
			cout<<"Error:[注释错误] at line"<<linenum-1<<":注释不符合规范。"<<endl;
		}
	}
	else{
		for(int i=0;i<Content.size();i++)
		{
			line=Content[i].second; 
			CutStr(Content[i].first);
			//Displayvec(vec); 
			Handle(vec);
		}
	} 
	return 0;
}