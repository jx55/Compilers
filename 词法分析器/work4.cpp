#include <iostream>
#include <cstdio> 
#include <cstring>
#include <map>
#include <vector>
using namespace std;

vector< pair<string,int> > Content_pre;//�洢ÿ�е����ݼ���Ӧ���к� 
vector< pair<string,int> > Content;//�洢�����ÿ�е����� 

map<string,int> Type,ExistWord;//�������͡����� 
map<string,int> VarDef;	//�Ѷ���ı��� 
map<string,int> CalcSymbol;//������� 
map<string,int> RelSymbol;//��ϵ����
map<string,int> PrimSymbol;//��������
map<string,int> Symbol;//����,; 

vector<string> vec;//��ŷָ���ַ��� 
int line;	//��¼�к�
int TypeFlag=0;//���ÿ���Ƿ���ڻ�������
int Annotate=0;//����Ƿ���ڶ���ע��
int Unknown_error=0;//��ǳ����Ƿ���δ֪�Ĵ���
int Annotate_line=0;//��¼����ע�ʹ�����к� 

//����ע��
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
				if(s[j]=='/'&&s[j+1]=='/'){//���д��ڵ���ע��
					string tmp(s,0,j);
					if(tmp!="") 
						Content.push_back(make_pair<string,int>(tmp,Content_pre[i].second));
					IsHandle=1;
					break; 
				}
				else if(s[j]=='/' && s[j+1]=='*'){//���д��ڶ���ע�͵Ŀ�ͷ 
					string tmp(s,0,j); 
					if(tmp!="")
						Content.push_back(make_pair<string,int>(tmp,Content_pre[i].second));
					Annotate=1;
					IsHandle=1;
					break;
				}
				else if(s[j]=='*'&&s[j+1]=='/'){//���д��ڶ���ע�͵Ľ��� 
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
		else{//�Ѿ����ڶ���ע���� 
			for(int j=0;j<s.length();j++)
			{
				if(s[j]=='*'&&s[j+1]=='/'){//�ҵ��˶���ע�͵Ľ�β
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

//��ʼ�� 
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
//�ж��Ƿ�������
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
//�ж��Ƿ��Ǻϸ�ı�ʶ��
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
//�жϱ�ʶ��֮ǰ�Ƿ����
int CheckIdent(string s)
{
	if(VarDef[s]) return 1;
	else return 0;
} 
//�ж�һ���ַ��Ƿ��Ǵ�Сд�������ֻ����»���
int JudgeChar(char ch)
{
	if((ch>='0'&&ch<='9') || (ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z') || ch=='_')
		return 1;
	else 
		return 0;
} 
//������ηָ�ɵ����ĵ��ʣ����浽vec�� 
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
//�ж�һ���ַ����Ƿ�ȫ�ǿո�,ȫ�ǿո񷵻�1�����򷵻�0 
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
//�ֱ���ÿ�����ʣ�����������ڵı�����ϵ 
//Keyword���ؼ��ʡ�Ident����ʶ�� ��Operator������� ��Separator���ָ�����Number������
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
						cout<<"Error:[�������] at line"<<line<<":�ñ����Ѿ����壬�����ظ����塣"<<endl;
					}
				}
				else{
					if(VarDef[s[i]]==0){
						cout<<s[i]<<"\t";
						cout<<"Error:[ʹ�ô���] at line"<<line<<":����δ���塣"<<endl;
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
					cout<<"Error:[�������] at line"<<line<<":��ʶ�����岻���Ϲ涨��12"<<endl;
				} 
			} 
		}
	}
}
void Displayvec(vector<string> v)
{
	cout<<"����������Ϊ��" <<endl;
	for(int i=0;i<v.size();i++)
		cout<<v[i]<<endl;
	cout<<endl;	
}

//���ÿ�еĽ��
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
	char readchar[255]={}; //�����ַ���
	
	FILE *fp;
	fp=fopen("test.txt","r");
	if(fp==NULL){
		cout<<"�ļ���ʧ�ܣ�"<<endl;
		exit(-1);
	}
	cout<<"�ʷ��������,������£�"<<endl;
	//���ж����ַ��������� 
	while( fgets(readchar,255,fp)!=NULL ){
		str=readchar;
		Content_pre.push_back(make_pair<string,int>(str,linenum));
		linenum++;
	}
	FilterAnnotate();
	if(Unknown_error==1 || Annotate==1){
		if(Unknown_error==1){
			cout<<"Error:[ע�ʹ���] at line"<<Annotate_line<<":ע�Ͳ����Ϲ淶��"<<endl;
		}
		else{
			cout<<"Error:[ע�ʹ���] at line"<<linenum-1<<":ע�Ͳ����Ϲ淶��"<<endl;
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