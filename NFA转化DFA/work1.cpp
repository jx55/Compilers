/*
基于子集法的NFA到DFA的转换
算法：
	输入表示NFA的转换表或其他数据结构
	输出DFA的转换表
	算法核心是ｅ闭包
根据输出的DFA，通过遍历转换关系，判断输入的字符串是否被识别 
*/
#include <iostream>
#include <vector>
#include <map>
using namespace std;

typedef pair<string,string> PSS;
map<PSS,string > NFA;
vector<string> Firstline; //存储第一行的输入状态
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
	FILE *fp;
	int line=1;
	fp=fopen("input.txt","r");
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
 
//从NFA向DFA的转化过程 
void Conversion_process()
{
	
}
int main()
{
	GetTable();
	//Display();
	return 0;
} 