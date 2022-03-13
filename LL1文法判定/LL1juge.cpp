/*
���룺�ķ�
������ݹ��������󹫹�������ȡ��FIRST��FOLLOW�ȼ��Ϲ��죬�ж�LL(1)
�������LL(1)��������Ԥ������������жϲ���LL(1)
*/
/*
��#��ʾ�մ� 
*/
#include <iostream>
#include <vector> 
using namespace std;
typedef pair<string,vector<string> > PSV;

vector<string>  pre_grammar; //��Ŷ�����ķ�
vector<string> grammar;//��Ŵ������ķ�
int addnum=1; //�������ķ���ţ������ظ�
vector<string> recurgrammar;//�����ݹ鴦��ǰ���ķ�
vector<string> norecurgrammar;//��Ŵ�����ݹ����ķ�
vector<string> cutgrammar;//��Ų�ֺ���ķ� 
vector<PSV> First;	//���first���ϵ��м��� 
vector<PSV> First1;	//������first���� 
vector<PSV> Follow;	//���Follow��
vector<PSV> Follow1;//���follow�����м��� 
 
//�������飬����֮ǰ��ȷ����Ľ��������
void copyvector(vector<string> vec1,vector<string> &vec2)
{
	for(int i=0;i<vec1.size();i++)
	{
		vec2.push_back(vec1[i]);
	}
} 
//�����������
void Display(vector<string> vec)
{
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl;
} 
//���pair���������
void Displaypsv(vector<PSV> vec) 
{
	for(int i=0;i<vec.size();i++)
	{
		string s=vec[i].first;
		vector<string> v=vec[i].second;
		cout<<s<<"  { ";
		for(int j=0;j<v.size();j++)
		{
			cout<<v[j];
			if(j!=v.size()-1) cout<<',';
		}
		cout<<" }"<<endl;
	}
	cout<<endl;
}
//���ļ��ж����ķ�
void read_grammar()
{
	string str;
	char readchar[255]={}; //�����ַ���
	
	FILE *fp;
	fp=fopen("input.txt","r");
	if(fp==NULL){
		cout<<"�ļ���ʧ�ܣ�"<<endl;
		exit(-1);
	}
	cout<<"�ķ�����ɹ���"<<endl;
	//���ж����ַ��������� 
	while( fgets(readchar,255,fp)!=NULL ){
		str=readchar;
		 pre_grammar.push_back(str);
	}
	for(int i=0;i< pre_grammar.size();i++)
	{
		if(i== pre_grammar.size()-1) break;
		string s( pre_grammar[i],0, pre_grammar[i].length()-1);
		 pre_grammar[i]=s;
	} 
}
//�����ķ�֮�󣬴����ķ��еİ����Ŀո�
void deal_space()
{
	string s;
	for(int i=0;i<pre_grammar.size();i++)
	{
		string t=pre_grammar[i];
		string res="";
		for(int j=0;j<t.length();j++)
		{
			if(t[j]!=' ') res+=t[j];
		}
		pre_grammar[i]=res;
	}
}
//�����и��ÿ���ķ������� 
vector<string> analysis_grammar(string s)
{
	vector<string> v; 
	vector<int> pos; 
	int l,r;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]==':') pos.push_back(i);
		else if(s[i]=='|') pos.push_back(i);
	}
	int last_pos;
	for(int i=2;i<pos.size();i++)
	{
		string tmp(s,pos[i-1]+1,pos[i]-pos[i-1]-1);
		v.push_back(tmp);
	}
	string tmp1(s,pos[pos.size()-1]+1,pos.size()-pos[pos.size()-1]-1);
	v.push_back(tmp1); 
	return v;
}
void common_prefix(string s1,string s2,int &l,string &s)
{
	int len=min(s1.length(),s2.length());
	for(int i=0;i<len;i++)
	{
		if(s1[i]==s2[i]){
			s+=s1[i];
			l++;
		}
		else break;
	}
} 
//����ÿ���ķ��Ĺ�������,���ظ��й������ӵĳ��� 
void get_common_factor(string s,int &maxcommonlen,string &common_string)
{
	vector<string> v;
	v=analysis_grammar(s);
	for(int i=0;i<v.size();i++)
	{
		for(int j=0;j<v.size();j++)
		{
			if(i==j) continue;
			int l=0;
			string s="";
			common_prefix(v[i],v[j],l,s);
			if(l>maxcommonlen){
				maxcommonlen=l;
				common_string=s;
			}
		}
	}
} 
//�󹫹�������ȡ 
void common_factor()
{
	for(int i=0;i< pre_grammar.size();i++)
	{
		int end=1;
		while(end){
			int maxcommonlen=0;
			string common_string="";
			get_common_factor( pre_grammar[i],maxcommonlen,common_string);
			if(maxcommonlen==0) {
				grammar.push_back( pre_grammar[i]);
			}
			else{
				vector<string> v;
				v=analysis_grammar( pre_grammar[i]);
				string new_grammar="";
				for(int k=0;k< pre_grammar[i].length();k++)
				{
					if( pre_grammar[i][k]==':'&& pre_grammar[i][k+1]==':'){
						new_grammar+="::";
						break;
					}
					else new_grammar+= pre_grammar[i][k];
				}
				vector<string> tmp;//�²����Ĳ���ʽ������ 
				for(int j=0;j<v.size();j++)
				{
					if(v[j].length()<maxcommonlen){
						if(new_grammar[new_grammar.length()-1]==':') new_grammar+=v[j];
						else{
							new_grammar+="|";
							new_grammar+=v[j]; 
						}
						continue;
					}
					string s1(v[j],0,maxcommonlen);
					if(s1==common_string){//�����ķ�����������Ӵ� 
						if(v[j].length()==maxcommonlen){
							string s2="#";
							tmp.push_back(s2);
						}
						else{
							string s2(v[j],maxcommonlen,v[j].length()-maxcommonlen);
							tmp.push_back(s2);
						}
					}
					else{
						if(new_grammar[new_grammar.length()-1]==':') new_grammar+=v[j];
						else{
							new_grammar+="|";
							new_grammar+=v[j]; 
						}
						continue;
					}
				}
				if(new_grammar[new_grammar.length()-1]!=':') new_grammar+="|";			
				new_grammar+=common_string;
				for(int m=0;m< pre_grammar[i].length();m++){
					if( pre_grammar[i][m]==':') break;
					else new_grammar+= pre_grammar[i][m]; 
				}
				for(int m=0;m<addnum;m++) new_grammar+="'";
				 pre_grammar[i]=new_grammar;
				string s4="";
				for(int m=0;m< pre_grammar[i].length();m++){
					if( pre_grammar[i][m]==':') break;
					else s4+= pre_grammar[i][m]; 
				}
				for(int m=0;m<addnum;m++) s4+="'";
				s4+="::";
				for(int m=0;m<tmp.size();m++){
					if(s4[s4.length()-1]==':') s4+=tmp[m];
					else{
						s4+='|';
						s4+=tmp[m];
					} 
				}
				grammar.push_back(s4);
				addnum++;
			}
			end=maxcommonlen;
		}
	}
}
//�ж�һ���ַ��Ƿ����ַ������ֻ�'
int judgechar(char ch)
{
	if( (ch>='A'&&ch<='Z') || (ch>='a'&&ch<='z') || (ch>='0'&&ch<='9') || ch=='\'' )
		return 1;
	else 
		return 0;
} 
//��һ�������з������һ������
void getsymbol(string s,string &res)
{
	for(int i=0;i<s.length();i++)
	{
		if(judgechar(s[i])) res+=s[i];
		else break;
	}
} 
//�жϸ����ķ��Ƿ������ݹ飬����������������ͬ�ĸ����� 
int judge_recursion(string s)
{
	string head="";
	int flag=0;
	vector<string> v;
	v=analysis_grammar(s); 
	//�õ������ķ����� 
	for(int i=0;i<s.length();i++)
	{
		if(s[i]==':') break;
		head+=s[i];
	}
	for(int i=0;i<v.size();i++)
	{
		string res="";
		getsymbol(v[i],res);
		if(res==head) flag++; 
	} 
	if(flag) return flag;
	else return 0;
} 
//��ݹ�����
void left_recursion()
{
	for(int i=0;i<recurgrammar.size();i++)
	{
		int n=judge_recursion(recurgrammar[i]);
		string s=recurgrammar[i];
		if(n==0){
			norecurgrammar.push_back(recurgrammar[i]); 
		}
		else{//�����ķ�������ݹ� 
			string head="";
			//�õ������ķ����� 
			for(int i=0;i<s.length();i++)
			{
				if(s[i]==':') break;
				head+=s[i];
			}
			vector<string> nochange;
			vector<string> change;
			vector<string> v;
			v=analysis_grammar(s);
			for(int j=0;j<v.size();j++)
			{
				if(v[j].length()<head.length()){
					nochange.push_back(v[j]);
					continue;
				}
				else{
					string tmp(v[j],0,head.length());
					if(tmp==head){
						string s1(v[j],head.length(),v[j].length()-head.length());
						change.push_back(s1);
					}
					else{
						nochange.push_back(v[j]);
						continue;
					}
				}
			}
			string s2=head+"'";//�²������ķ� 
			string s3=head;//ԭ�����ķ� 
			string s4=s2;
			s2+="::";
			s3+="::";
			//ԭ�����ķ���Ҫ�޸� 
			for(int k=0;k<nochange.size();k++)
			{
				if(s3[s3.length()-1]==':'){
					s3+=nochange[k];
					s3+=s4;
				}
				else{
					s3+='|';
					s3+=nochange[k];
					s3+=s4;
				}
			}
			//����Ҫ���һ���µ��ķ� 
			for(int k=0;k<change.size();k++)
			{
				if(s2[s2.length()-1]==':'){
					s2+=change[k];
					s2+=s4;
				}
				else{
					s2+='|';
					s2+=change[k];
					s2+=s4;
				}
			}
			s2+="|";
			s2+="#";//���һ���մ� 
			norecurgrammar.push_back(s3);
			norecurgrammar.push_back(s2); 
		}
	}
}
//��һ�ж��|�Ĳ�� 
void cut_sentence(string s)
{
	string head="";
	//�õ������ķ����� 
	for(int i=0;i<s.length();i++)
	{
		if(i>0 && s[i]!=':' &&s[i-1]==':') break;
		head+=s[i];
	}
	string tmp="";
	int flag=0;
	for(int i=1;i<s.length();i++)
	{
		if(s[i]!=':'&&s[i-1]==':') flag=1;
		if(flag==1){
			if(s[i]=='|'){
				tmp=head+tmp;
				cutgrammar.push_back(tmp);
				tmp="";
			}
			else {
				tmp+=s[i];
			}
		} 
	}
	tmp=head+tmp;
	if(tmp!="") cutgrammar.push_back(tmp); 
}
//�жϴ����ķ�����s�Ƿ��Ѿ���������,�����Ҳ���һ�������Ƿ��ڣ���������� 
void head_exist(string s,string s1,vector<PSV> &v)
{
	int flag=0;
	for(int i=0;i<v.size();i++)
	{
		string tmp=v[i].first;
		//���Ѿ��������� 
		if(tmp==s){
			flag=1;
			vector<string> vec=v[i].second;
			int flag1=0;
			for(int j=0;j<vec.size();j++) 
			{
				if(vec[j]==s1){
					flag1=1;
					break;
				}
			}
			if(flag1==0) v[i].second.push_back(s1);
			break;
		}
	}
	if(flag==0){
		vector<string> vec1;
		if(s1 != "") vec1.push_back(s1);
		PSV psv;
		psv.first=s;
		psv.second=vec1;
		v.push_back(psv); 
	}
}
//�ж�һ�������Ƿ����ս����,�����ս���ŷ���0 
int judge_over(string s)
{
	int flag=0;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]>='A'&&s[i]<='Z' || s[i]=='\'') continue;
		else {
			flag=1;
			break;
		}
	}
	if(flag) return 0;
	else return 1;
} 
//�����ս������û�ڱ������е�������ӽ���
void add_over(vector<string> &v1,vector<string> v2)
{
	for(int i=0;i<v2.size();i++)
	{
		string s=v2[i];
		int flag=0;
		for(int j=0;j<v1.size();j++)
		{
			if(v1[j]==s){
				flag=1;
				break;
			}
		}
		//�������������в����� 
		if(flag==0) v1.push_back(s); 
	}
} 
//�ж�һ���ַ��Ƿ��Ǵ�д��ĸ����' 
int jbds(char ch)	//judge_big_dot_symbol
{
	if(ch>='A'&&ch<='Z' || ch=='\'') return 1;
	else return 0;
}
//�ж�һ���ַ��Ƿ��Ǵ�д��ĸ
int jbs(char ch)
{
	if(ch>='A'&&ch<='Z') return 1;
	else return 0;
} 
//�ж�һ���ַ���Сд��ĸ
int jss(char ch)  //judge_small_symbol
{
	if(ch>='a'&&ch<='z') return 1;
	else return 0;
}
//�ж�һ���ַ��Ƿ�������� 
int jcs(char ch)	//judge_calc_symbol
{
	if(ch=='+' || ch=='-' || ch=='*' || ch=='/') return 1;
	else return 0;
}
//Ԥ����FIRST���� 
void get_first_pre()
{
	for(int i=0;i<norecurgrammar.size();i++)
		cut_sentence(norecurgrammar[i]);
	for(int i=0;i<cutgrammar.size();i++)
	{
		string s=cutgrammar[i];
		string head="";
		//�õ������ķ�����
		int j=0;
		for(;j<s.length();j++)
		{
			if(s[j]==':'&&s[j+1]==':') break;
			head+=s[j];
		}
		string tmp="";
		int flag=0;
		for(;j<s.length();j++)
		{
			if(s[j]!=':'&&s[j-1]==':'){
				tmp+=s[j];
				flag=1;
			}
			else if(flag==1&&jbds(tmp[tmp.length()-1])&&s[j]=='\'') 
				tmp+=s[j];
			else if(flag==1&&jss(tmp[tmp.length()-1])&&jss(s[j]))
				tmp+=s[j];
			else flag=0;
		}
		head_exist(head,tmp,First);
	}
}
//����PSV���� 
void copypsv(vector<PSV> &v1,vector<PSV> v2)
{
	for(int i=0;i<v2.size();i++)
		v1.push_back(v2[i]);
}
//�ж�����First�����Ƿ��Ѿ����ڸ����ķ� 
int exist_first(string s)
{
	int flag=0;
	for(int i=0;i<First1.size();i++)
	{
		if(First1[i].first==s){
			flag=1;
			break;
		}
	}
	if(flag) return 1;
	else return 0;
}
//���յõ�First����
void get_first()
{
	vector<PSV> First_tmp; 
	copypsv(First_tmp,First);
	int n=First.size();
	while(n!=First1.size()){
		for(int i=0;i<First_tmp.size();i++)
		{
			int flag=0;
			string s=First_tmp[i].first;
			if(exist_first(s)) continue;
			vector<string> v=First_tmp[i].second;
			vector<string> res;
			for(int j=0;j<v.size();j++)
			{
				if(judge_over(v[j])==0){
					res.push_back(v[j]);
					continue;
				}
				else {//��ǰ���Ų����ս���� 
					flag=1;
					string s1=v[j];
					for(int k=0;k<First_tmp.size();k++)
					{
						if(First_tmp[k].first==s1){
							vector<string> v1=First_tmp[k].second;
							add_over(res,v1);
							break;
						}
					}
				}
			}
			First_tmp[i].second=res;
			if(flag==0) First1.push_back(First_tmp[i]);
		}
	} 
}
//����ÿ���ķ����еķ��� 
void get_follow_continue(string head,vector<string> sym)
{
	sym.push_back(" "); 
	for(int i=0;i<sym.size()-1;i++)
	{
		string s1=sym[i];
		string s2=sym[i+1];
		if(jbs(s1[0])){
			if(jbs(s2[0])){
				int flag1=0;
				for(int m=0;m<Follow.size();m++)
				{
					if(Follow[m].first==s1){//s1�����Ѿ���Follow���У����ܲ�����
						flag1=1;
						for(int n=0;n<First1.size();n++)
						{
							if(First1[n].first==s2){
								vector<string> v3;
								v3=First1[n].second;
								for(int o=0;o<v3.size();o++)
								{
									if(v3[o]!="#"){
										int flag2=0;
										vector<string> v4=Follow[m].second;
										for(int q=0;q<v4.size();q++)
										{
											if(v4[q]==v3[o]){
												flag2=1;
												break;
											}
										}
										if(flag2==0){
											v4.push_back(v3[o]);
											Follow[m].second=v4;
										}
									}
								}
								break;
							}
						}
						break; 
					}
				}
				/*
				if(flag1==0){//��ǰ���ս����û����Follow�г��ֹ� 
					PSV p;
					p.first=s1;
					vector<string> v;
					for(int j=0;j<First.size();j++)
					{
						if(First[j].first==s2){
							vector<string> v1;
							for(int k=0;k<v1.size();k++)
							{
								if(v1[k]!="#"){
									int flag=0;
									for(int l=0;l<v.size();l++)
									{
										if(v[l]==v1[k]){
											flag=1;
											break;
										} 
									}
									if(flag==0){
										v.push_back(v1[k]);
									}
								}
							}
						}
					}
					p.second=v;
					Follow.push_back(p);
				}
				*/
			}
			else if(s2!=" "){
				//cout<<s2<<"  AAA"<<endl;
				int flag=0;
				for(int j=0;j<Follow.size();j++)
				{
					if(Follow[j].first==s1){
						flag=1;
						vector<string> v1;
						v1=Follow[j].second;
						int flag1=0;
						for(int k=0;k<v1.size();k++)
						{
							if(v1[k]==s2){
								flag1=1;
								break;
							}
						}
						if(flag1==0){
							v1.push_back(s2);
							Follow[j].second=v1; 
						}
						break;
					}
				}
				if(flag==0){
					PSV p;
					p.first=s1;
					vector<string> v;
					v.push_back(s2);
					p.second=v;
					Follow.push_back(p);
				}
			}
			else{	//ʣ�µ����Ӧ����s2==" "�����
				//cout<<s2<<"  CCC"<<endl;
				int flag=0;
				for(int j=0;j<Follow.size();j++)
				{
					if(Follow[j].first==s1){
						flag=1;
						vector<string> v1;
						v1=Follow[j].second;
						int flag1=0;
						for(int k=0;k<v1.size();k++)
						{
							if(v1[k]==head){
								flag1=1;
								break;
							}
						}
						if(flag1==0){
							v1.push_back(head);
							Follow[j].second=v1;
						}
						break;
					}
				}
				if(flag==0){
					PSV p;
					p.first=s1;
					vector<string> v;
					v.push_back(head);
				}
			}
		}
	}
}
//FOLLOW���Ϲ���
void get_follow()
{
	vector<string> f_grammar;
	copyvector(cutgrammar,f_grammar);
	for(int i=0;i<f_grammar.size();i++)
	{
		string s=f_grammar[i];
		string head="";
		//�õ������ķ�����
		int j=0;
		for(;j<s.length();j++)
		{
			if(s[j]==':'&&s[j+1]==':') break;
			head+=s[j];
		}
		head_exist(head,"$",Follow);
		//����ÿ���ķ���Ϊÿ���ķ����油һ���ո񣬷����������
		s+=' ';
		f_grammar[i]=s; 
	}
	for(int i=0;i<f_grammar.size();i++)
	{
		string s=f_grammar[i];
		string head="";
		//�õ������ķ�����
		int j=0;
		for(;j<s.length();j++)
		{
			if(s[j]==':'&&s[j+1]==':') break;
			head+=s[j];
		}
		j+=2;
		string tmp="";
		int flag=0;
		vector<string> sym;
		for(;j<s.length()-1;j++)
		{
			if(s[j]=='\'') tmp+=s[j];
			else if(jbs(s[j])){
				if(tmp=="") tmp+=s[j];
				else{
					sym.push_back(tmp);
					tmp="";
					tmp+=s[j];
				}
			}
			else if(jss(s[j])){
				if(tmp=="") tmp+=s[j];
				else{
					if(jss(tmp[tmp.length()-1])) tmp+=s[j];
					else{
						sym.push_back(tmp);
						tmp="";
						tmp+=s[j];
					}
				} 
			}
			else if(jcs(s[j])){
				if(tmp=="") tmp+=s[j];
				else{
					if(jcs(tmp[tmp.length()-1])) tmp+=s[j];
					else{
						sym.push_back(tmp);
						tmp="";
						tmp+=s[j]; 
					} 
				}
			}
			else if(s[j]=='#'){
				if(tmp!=""){
					sym.push_back(tmp);
					tmp="";
				}
				else sym.push_back("#");
			}
			else{
				if(tmp!=""){
					sym.push_back(tmp);
					tmp="";
				}
				string stmp="";
				stmp+=s[j];
				sym.push_back(stmp);
			}
		}
		if(tmp!="") sym.push_back(tmp);
		tmp="";
		get_follow_continue(head,sym);
	}
}
//����Ԥ������� 
void analysis_table()
{
	
}

int main()
{
	read_grammar();/*
	cout<<"������ķ���"<<endl;
	Display(pre_grammar);
	*/
	deal_space();
	cout<<"����ո����ķ���"<<endl; 
	Display(pre_grammar);
	
	common_factor();/*
	cout<<"�󹫹�������ȡ����ķ���"<<endl;
	Display(grammar);*/
	copyvector(grammar,recurgrammar); 
	left_recursion();/*
	cout<<"������ݹ����ķ���"<<endl; 
	Display(norecurgrammar);
	*/
	get_first_pre();/*
	cout<<"�ָ�֮����ķ���"<<endl; 
	Display(cutgrammar);
	cout<<"�򵥴�����First����"<<endl;
	Displaypsv(First);
	*/
	get_first();
	cout<<"���������First����"<<endl;
	Displaypsv(First1);
	
	get_follow();
	cout<<"�򵥴�����Follow����"<<endl;
	Displaypsv(Follow);
	return 0;
} 
/*
S::iEtS|iEtSeS|iEtb|a

*/