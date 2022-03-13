#include <iostream>
#include <vector>
#include <map>
using namespace std;

typedef pair<string,string> PSS;
map<PSS,string > NFA;
vector<string> Firstline; //存储第一行的输入状态
map<string,int> End;//存储终结符号 
//判断一个字符是否符合要求 
int Regular_char(char ch)
{
	if(ch>='a'&&ch<='z' || ch>='A'&&ch<='Z' || ch>='0'&&ch<='9' || ch==',') 
		return 1;
	else 
		return 0;
}
//从文件中读入，得到NFA的转换表 
void GetTable()
{
	cout<<"请输入终结符号个数及内容：";
	int n;
	cin>>n;
	for(int i=0;i<n;i++)
	{
		string s;
		cin>>s;
		End[s]=1;
	} 
	FILE *fp;
	int line=1;
	fp=fopen("input2.txt","r");
	if(fp==NULL){
		cout<<"文件打开失败！"<<endl;
		exit(-1);
	}
	char readchar[255];
	string str; 
	//逐行读入字符串并存入NFA表中 
	while( fgets(readchar,255,fp)!=NULL ){
		str=readchar;
		str+=' '; 
		if(line!=1){
			int first=0,flag=0;
			string cond;
			string tmp="";
			for(int i=0;i<str.length();i++){
				if(Regular_char(str[i])) tmp+=str[i];
				else if(i!=0 && !Regular_char(str[i]) && Regular_char(str[i-1])){
					if(flag==0){
						cond=tmp;
						flag=1;
					}
					else{
						PSS cond_input(make_pair<string,string>(cond,Firstline[first++]));
						NFA[cond_input]=tmp;
					}
					tmp="";
				}
			}	
		} 
		else{
			string tmp="";
			for(int i=0;i<str.length();i++)
			{
				if(Regular_char(str[i])) tmp+=str[i];
				else if(i!=0 && !Regular_char(str[i]) && Regular_char(str[i-1])){
					Firstline.push_back(tmp);
					tmp="";
				}
			}
			
		}
		line++;
	}
	cout<<"文件读取成功！"<<endl; 
}
//输出NFA表的内容 
void Display()
{
	map<PSS,string>::iterator it;
	for(it=NFA.begin();it!=NFA.end();it++)
	{
		cout<<it->first.first<<'\t'<<it->first.second<<'\t'<<"{ "<<it->second<<" }"<<endl; 
	}
}
//输出数组的内容 
void Show(vector<string> vec)
{
	cout<<"数组内容："<<endl;
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl;
}
//将一个字符串转化为单个字符 
void Str_to_char(string s)
{
	
}
//判断数组中是否存在s字符串 
int Search(vector<string> vec,string s)
{
	int flag=0;
	for(int i=0;i<vec.size();i++)
	{
		if(vec[i]==s){
			flag=1;
			break;
		}
	} 
	//flag=1，即数组中已经存在该串 
	if(flag) return 1;
	else return 0;
}
//处理简单拼接的状态集合 
void Handle_str(string &s)
{
	string tmp="";
	for(int i=0;i<s.length();i++)
	{
		int exist=0;
		if(s[i]!=','){
			if(tmp==""){
				tmp+=s[i];
				exist=1; 
			}
			else{
				for(int j=0;j<tmp.length();j++)
				{
					if(s[i]==tmp[j]){
						exist=1;
						break;
					}
				}
			}
			//将不在的字符拼接上 
			if(exist==0){
				tmp+=',';
				tmp+=s[i];
			}
		}
	}
	s=tmp;
}
//判断一个状态是否是NULL 
int Judge_Null(string s)
{
	if(s=="NULL") return 1;
	else return 0;
} 
int pos=0;//记录数组中当前处理到的个数 
vector<string> process; //暂存正在处理的字符串 
vector<pair<PSS,string> > store_process;	//存储中间过程
string finish;//已经处理的最后一个状态 
//从NFA向DFA的转化过程 
void Conversion_process()
{
	map<PSS,string>::iterator it=NFA.begin();
	process.push_back(it->first.first);
	while(pos<process.size()){
		string tmp="";
		for(int i=0;i<Firstline.size();i++)
		{
			tmp=""; 
			for(int j=0;j<process[pos].length();j++)
			{
				if(process[pos][j]!=','){
					string use;
					use.push_back(process[pos][j]);
					PSS cond_input(make_pair<string,string>(use,Firstline[i]));
					string tmp1=NFA[cond_input];
					//将状态集合中每个状态能到达的状态先简单拼接起来
					if(Judge_Null(tmp1)==0){
						if(tmp=="") tmp=tmp1;
						else{
							tmp+=',';
							tmp+=tmp1;
						} 
					}
				}
			}
			//将拼接的可到达的总状态处理后存起来
			Handle_str(tmp);
			PSS midst(make_pair<string,string>(process[pos],Firstline[i]));
			store_process.push_back(make_pair<PSS,string>(midst,tmp));
			//即这个状态还没在集合中，就将其加入到集合中
			if(!Search(process,tmp)){
				process.push_back(tmp); 
			}
		}
		pos++;
	}
	Show(process);
}
//输出存储的过程
void Show_process()
{
	cout<<"存储的中间过程如下："<<endl; 
	for(int i=0;i<store_process.size();i++)
	{
		PSS pss=store_process[i].first;
		string s=store_process[i].second;
		cout<<pss.first<<'\t'<<pss.second<<'\t'<<"{ "<<s<<" }"<<endl;
	}
	cout<<endl;
}
//在存储的过程中寻找要转移到的状态
int Find_state(string s1,string s2,string &res)
{
	int flag=0;
	for(int i=0;i<store_process.size();i++)
	{
		PSS pss=store_process[i].first;
		string s=store_process[i].second;
		if(pss.first==s1&&pss.second==s2){
			flag=1;
			res=s;
			break;
		}
	}
	if(flag) return 1;//如果找到了，就返回1
	else return 0; 
} 
//判断一个状态中是否包含了终结符号 
int Judge_end_state(string s)
{
	int flag=0;
	for(int i=0;i<s.length();i++)
	{
		string tmp;
		tmp.push_back(s[i]);
		if(End[tmp]){
			flag=1;
			break;
		}
	}
	if(flag) return 1;//即包含了终结符号
	else return 0; 
} 
vector<string> identity; //存储识别过程中用到的状态集 
//判断输入的字符串是否可以被识别 
void Is_identity()
{
	string s;
	cout<<"请输入待识别的字符串："<<endl;
	cin>>s;
	int identity_pos=0; 
	identity.push_back(store_process[0].second);
	int flag=1;
	for(int i=0;i<s.length();i++)
	{
		string res="";
		//为真，即当前状态面临s[i]的转移结果找到了 
		string ss;
		ss.push_back(s[i]);
		if(Find_state( identity[identity_pos],ss,res) ){
			identity.push_back(res);
			identity_pos++; 
		}
		else{//如果中间存在找不到转移结果的 
			flag=0;
			break;
		}
	} 
	//判断中间是否有找不到转移结果情况
	//还要判断最后是否是终结状态
	if(flag==0){
		cout<<"字符串无法识别！"<<endl;
	} 
	else{
		if(Judge_end_state(identity.back()) ){
			cout<<"字符串识别成功！"<<endl;
		}
		else{
			cout<<"字符串识别失败！"<<endl; 
		} 
	}
}
int main()
{
	GetTable();
	//Display();
	Conversion_process();
	Show_process();
	Is_identity();
	return 0;
} 