/*
输入：文法
处理：左递归消除、左公共因子提取，FIRST、FOLLOW等集合构造，判断LL(1)
输出：是LL(1)的情况输出预测分析表，否则判断不是LL(1)
*/
/*
用#表示空串 
*/
#include <iostream>
#include <vector> 
using namespace std;
typedef pair<string,vector<string> > PSV;

vector<string>  pre_grammar; //存放读入的文法
vector<string> grammar;//存放处理后的文法
int addnum=1; //新增的文法序号，避免重复
vector<string> recurgrammar;//存放左递归处理前的文法
vector<string> norecurgrammar;//存放处理左递归后的文法
vector<string> cutgrammar;//存放拆分后的文法 
vector<PSV> First;	//存放first集合的中间结果 
vector<PSV> First1;	//存放最后first集合 
vector<PSV> Follow;	//存放Follow集
vector<PSV> Follow1;//存放follow集的中间结果 
 
//复制数组，避免之前正确处理的结果被更改
void copyvector(vector<string> vec1,vector<string> &vec2)
{
	for(int i=0;i<vec1.size();i++)
	{
		vec2.push_back(vec1[i]);
	}
} 
//输出数组内容
void Display(vector<string> vec)
{
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl;
} 
//输出pair数组的内容
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
//从文件中读入文法
void read_grammar()
{
	string str;
	char readchar[255]={}; //读入字符串
	
	FILE *fp;
	fp=fopen("input.txt","r");
	if(fp==NULL){
		cout<<"文件打开失败！"<<endl;
		exit(-1);
	}
	cout<<"文法读入成功！"<<endl;
	//逐行读入字符串并处理 
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
//读入文法之后，处理文法中的包含的空格
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
//返回切割后每条文法的因子 
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
//处理每行文法的公共因子,返回该行公共因子的长度 
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
//左公共因子提取 
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
				vector<string> tmp;//新产生的产生式的内容 
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
					if(s1==common_string){//该行文法包含最长公共子串 
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
//判断一个字符是否是字符、数字或'
int judgechar(char ch)
{
	if( (ch>='A'&&ch<='Z') || (ch>='a'&&ch<='z') || (ch>='0'&&ch<='9') || ch=='\'' )
		return 1;
	else 
		return 0;
} 
//从一个因子中分离出第一个符号
void getsymbol(string s,string &res)
{
	for(int i=0;i<s.length();i++)
	{
		if(judgechar(s[i])) res+=s[i];
		else break;
	}
} 
//判断该行文法是否存在左递归，返回因子中与左部相同的个数。 
int judge_recursion(string s)
{
	string head="";
	int flag=0;
	vector<string> v;
	v=analysis_grammar(s); 
	//得到该行文法的左部 
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
//左递归消除
void left_recursion()
{
	for(int i=0;i<recurgrammar.size();i++)
	{
		int n=judge_recursion(recurgrammar[i]);
		string s=recurgrammar[i];
		if(n==0){
			norecurgrammar.push_back(recurgrammar[i]); 
		}
		else{//该行文法存在左递归 
			string head="";
			//得到该行文法的左部 
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
			string s2=head+"'";//新产生的文法 
			string s3=head;//原来的文法 
			string s4=s2;
			s2+="::";
			s3+="::";
			//原来的文法需要修改 
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
			//还需要添加一行新的文法 
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
			s2+="#";//添加一个空串 
			norecurgrammar.push_back(s3);
			norecurgrammar.push_back(s2); 
		}
	}
}
//将一行多个|的拆分 
void cut_sentence(string s)
{
	string head="";
	//得到该行文法的左部 
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
//判断此行文法的左部s是否已经在数组中,并且右部第一个符号是否在，不在则添加 
void head_exist(string s,string s1,vector<PSV> &v)
{
	int flag=0;
	for(int i=0;i<v.size();i++)
	{
		string tmp=v[i].first;
		//左部已经在数组中 
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
//判断一个符号是否是终结符号,不是终结符号返回0 
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
//将非终结符号中没在本数组中的内容添加进来
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
		//该内容在数组中不存在 
		if(flag==0) v1.push_back(s); 
	}
} 
//判断一个字符是否是大写字母或者' 
int jbds(char ch)	//judge_big_dot_symbol
{
	if(ch>='A'&&ch<='Z' || ch=='\'') return 1;
	else return 0;
}
//判断一个字符是否是大写字母
int jbs(char ch)
{
	if(ch>='A'&&ch<='Z') return 1;
	else return 0;
} 
//判断一个字符是小写字母
int jss(char ch)  //judge_small_symbol
{
	if(ch>='a'&&ch<='z') return 1;
	else return 0;
}
//判断一个字符是否是运算符 
int jcs(char ch)	//judge_calc_symbol
{
	if(ch=='+' || ch=='-' || ch=='*' || ch=='/') return 1;
	else return 0;
}
//预处理FIRST集合 
void get_first_pre()
{
	for(int i=0;i<norecurgrammar.size();i++)
		cut_sentence(norecurgrammar[i]);
	for(int i=0;i<cutgrammar.size();i++)
	{
		string s=cutgrammar[i];
		string head="";
		//得到该行文法的左部
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
//复制PSV数组 
void copypsv(vector<PSV> &v1,vector<PSV> v2)
{
	for(int i=0;i<v2.size();i++)
		v1.push_back(v2[i]);
}
//判断最后的First集合是否已经存在该行文法 
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
//最终得到First集合
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
				else {//当前符号不是终结符号 
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
//处理每行文法所有的符号 
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
					if(Follow[m].first==s1){//s1符号已经在Follow集中，可能不完整
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
				if(flag1==0){//当前非终结符号没有在Follow中出现过 
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
			else{	//剩下的情况应该是s2==" "的情况
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
//FOLLOW集合构造
void get_follow()
{
	vector<string> f_grammar;
	copyvector(cutgrammar,f_grammar);
	for(int i=0;i<f_grammar.size();i++)
	{
		string s=f_grammar[i];
		string head="";
		//得到该行文法的左部
		int j=0;
		for(;j<s.length();j++)
		{
			if(s[j]==':'&&s[j+1]==':') break;
			head+=s[j];
		}
		head_exist(head,"$",Follow);
		//处理每行文法，为每行文法后面补一个空格，方便后续处理
		s+=' ';
		f_grammar[i]=s; 
	}
	for(int i=0;i<f_grammar.size();i++)
	{
		string s=f_grammar[i];
		string head="";
		//得到该行文法的左部
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
//构造预测分析表 
void analysis_table()
{
	
}

int main()
{
	read_grammar();/*
	cout<<"读入的文法："<<endl;
	Display(pre_grammar);
	*/
	deal_space();
	cout<<"处理空格后的文法："<<endl; 
	Display(pre_grammar);
	
	common_factor();/*
	cout<<"左公共因子提取后的文法："<<endl;
	Display(grammar);*/
	copyvector(grammar,recurgrammar); 
	left_recursion();/*
	cout<<"消除左递归后的文法："<<endl; 
	Display(norecurgrammar);
	*/
	get_first_pre();/*
	cout<<"分割之后的文法："<<endl; 
	Display(cutgrammar);
	cout<<"简单处理后的First集："<<endl;
	Displaypsv(First);
	*/
	get_first();
	cout<<"最后完整的First集："<<endl;
	Displaypsv(First1);
	
	get_follow();
	cout<<"简单处理后的Follow集："<<endl;
	Displaypsv(Follow);
	return 0;
} 
/*
S::iEtS|iEtSeS|iEtb|a

*/