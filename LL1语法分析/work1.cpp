#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
typedef pair<pair<string,string>,string> PSSS;

string res;//��ƥ�䴮 
string res1;//��ƥ�䴮 
vector<PSSS> Table;	//���Ԥ�������
stack<string> st;

//����LL1�ķ��ж�����Ľ��
void read_grammar()
{
	string str;
	char readchar[255]={}; //�����ַ���
	
	FILE *fp;
	fp=fopen("../LL1�ķ��ж�/output1.txt","r");
	if(fp==NULL){
		cout<<"�ļ���ʧ�ܣ�"<<endl;
		exit(-1);
	}
	cout<<"�ķ�����ɹ���"<<endl;
	//���ж����ַ��������� 
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
	//����s3��Ļس� 
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
//���ջ������
void Display(stack<string> st)
{
	//cout<<"��ǰջ��Ԫ��Ϊ��"<<endl;
	while(!st.empty()){
		cout<<right<<st.top();
		st.pop();
	}
}
//�ж�һ���ַ��Ƿ��Ǵ�д��ĸ
int jbs(char ch)
{
	if(ch>='A'&&ch<='Z') return 1;
	else return 0;
} 
//�ж�ջ��Ԫ���Ƿ�Ϸ�,�Ϸ�����1�����򷵻�0 
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
				cout<<"\n�ô�ʶ��ʧ�ܣ�"<<endl;
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
			cout<<"ƥ��"<<res[0]<<endl; 
		}
		else if(s1=="#"){
			st.pop();
			show();
			cout<<endl;
		}
		else{
			cout<<"\n�ô�ʶ��ʧ�ܣ�"<<endl;
			exit(0);
		}
	}
	
	if(st.top()=="$") cout<<"\n�ô�ʶ��ɹ���"<<endl;
	else cout<<"\n�ô�ʶ��ʧ�ܣ�"<<endl;
}
int main()
{
	read_grammar();
	cout<<"�������ʶ��(��$��β)��";
	cin>>res;
	res1=""; 
	cout<<"��ƥ��\tջ\t���봮\t����"<<endl; 
	LL1();
	return 0;
} 