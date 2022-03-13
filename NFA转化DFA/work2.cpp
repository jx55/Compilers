#include <iostream>
#include <vector>
#include <map>
using namespace std;

typedef pair<string,string> PSS;
map<PSS,string > NFA;
vector<string> Firstline; //�洢��һ�е�����״̬
map<string,int> End;//�洢�ս���� 
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
	cout<<"�������ս���Ÿ��������ݣ�";
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
	cout<<"�ļ���ȡ�ɹ���"<<endl; 
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
//������������ 
void Show(vector<string> vec)
{
	cout<<"�������ݣ�"<<endl;
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl;
}
//��һ���ַ���ת��Ϊ�����ַ� 
void Str_to_char(string s)
{
	
}
//�ж��������Ƿ����s�ַ��� 
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
	//flag=1�����������Ѿ����ڸô� 
	if(flag) return 1;
	else return 0;
}
//�����ƴ�ӵ�״̬���� 
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
			//�����ڵ��ַ�ƴ���� 
			if(exist==0){
				tmp+=',';
				tmp+=s[i];
			}
		}
	}
	s=tmp;
}
//�ж�һ��״̬�Ƿ���NULL 
int Judge_Null(string s)
{
	if(s=="NULL") return 1;
	else return 0;
} 
int pos=0;//��¼�����е�ǰ�����ĸ��� 
vector<string> process; //�ݴ����ڴ�����ַ��� 
vector<pair<PSS,string> > store_process;	//�洢�м����
string finish;//�Ѿ���������һ��״̬ 
//��NFA��DFA��ת������ 
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
					//��״̬������ÿ��״̬�ܵ����״̬�ȼ�ƴ������
					if(Judge_Null(tmp1)==0){
						if(tmp=="") tmp=tmp1;
						else{
							tmp+=',';
							tmp+=tmp1;
						} 
					}
				}
			}
			//��ƴ�ӵĿɵ������״̬����������
			Handle_str(tmp);
			PSS midst(make_pair<string,string>(process[pos],Firstline[i]));
			store_process.push_back(make_pair<PSS,string>(midst,tmp));
			//�����״̬��û�ڼ����У��ͽ�����뵽������
			if(!Search(process,tmp)){
				process.push_back(tmp); 
			}
		}
		pos++;
	}
	Show(process);
}
//����洢�Ĺ���
void Show_process()
{
	cout<<"�洢���м�������£�"<<endl; 
	for(int i=0;i<store_process.size();i++)
	{
		PSS pss=store_process[i].first;
		string s=store_process[i].second;
		cout<<pss.first<<'\t'<<pss.second<<'\t'<<"{ "<<s<<" }"<<endl;
	}
	cout<<endl;
}
//�ڴ洢�Ĺ�����Ѱ��Ҫת�Ƶ���״̬
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
	if(flag) return 1;//����ҵ��ˣ��ͷ���1
	else return 0; 
} 
//�ж�һ��״̬���Ƿ�������ս���� 
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
	if(flag) return 1;//���������ս����
	else return 0; 
} 
vector<string> identity; //�洢ʶ��������õ���״̬�� 
//�ж�������ַ����Ƿ���Ա�ʶ�� 
void Is_identity()
{
	string s;
	cout<<"�������ʶ����ַ�����"<<endl;
	cin>>s;
	int identity_pos=0; 
	identity.push_back(store_process[0].second);
	int flag=1;
	for(int i=0;i<s.length();i++)
	{
		string res="";
		//Ϊ�棬����ǰ״̬����s[i]��ת�ƽ���ҵ��� 
		string ss;
		ss.push_back(s[i]);
		if(Find_state( identity[identity_pos],ss,res) ){
			identity.push_back(res);
			identity_pos++; 
		}
		else{//����м�����Ҳ���ת�ƽ���� 
			flag=0;
			break;
		}
	} 
	//�ж��м��Ƿ����Ҳ���ת�ƽ�����
	//��Ҫ�ж�����Ƿ����ս�״̬
	if(flag==0){
		cout<<"�ַ����޷�ʶ��"<<endl;
	} 
	else{
		if(Judge_end_state(identity.back()) ){
			cout<<"�ַ���ʶ��ɹ���"<<endl;
		}
		else{
			cout<<"�ַ���ʶ��ʧ�ܣ�"<<endl; 
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