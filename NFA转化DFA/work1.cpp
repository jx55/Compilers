/*
�����Ӽ�����NFA��DFA��ת��
�㷨��
	�����ʾNFA��ת������������ݽṹ
	���DFA��ת����
	�㷨�����ǣ�հ�
���������DFA��ͨ������ת����ϵ���ж�������ַ����Ƿ�ʶ�� 
*/
#include <iostream>
#include <vector>
#include <map>
using namespace std;

typedef pair<string,string> PSS;
map<PSS,string > NFA;
vector<string> Firstline; //�洢��һ�е�����״̬
//�ж�һ���ַ��Ƿ����Ҫ�� 
int Regular_char(char ch)
{
	if(ch>='a'&&ch<='z' || ch>='A'&&ch<='Z' || ch>='0'&&ch<='9' || ch==',') 
		return 1;
	else 
		return 0;
}
//���ļ��ж��룬�õ�NFA��ת���� 
void GetTable()
{
	FILE *fp;
	int line=1;
	fp=fopen("input.txt","r");
	if(fp==NULL){
		cout<<"�ļ���ʧ�ܣ�"<<endl;
		exit(-1);
	}
	char readchar[255];
	string str; 
	//���ж����ַ���������NFA���� 
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
//���NFA������� 
void Display()
{
	map<PSS,string>::iterator it;
	for(it=NFA.begin();it!=NFA.end();it++)
	{
		cout<<it->first.first<<'\t'<<it->first.second<<'\t'<<"{ "<<it->second<<" }"<<endl; 
	}
}
 
//��NFA��DFA��ת������ 
void Conversion_process()
{
	
}
int main()
{
	GetTable();
	//Display();
	return 0;
} 