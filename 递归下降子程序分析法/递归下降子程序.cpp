#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

void E();
void T();
void E_();
void A();
void F();
void M();
void T_();

string s;	//��ɨ�贮
string s1; 
void show(int n)
{
	if(n==1) cout<<"\t"<<s1<<"\t\t"<<s[0]<<"\t\t"<<s<<endl;
	else cout<<"\t\t"<<s1<<"\t\t"<<s[0]<<"\t\t"<<s<<endl;
}
void E()
{
	cout<<"E->TE'";
	show(0);
	T();
	E_();
} 
void T()
{
	cout<<"T->FT'";
	show(0);
	F();
	T_();
}
void E_()
{
	if(s[0]=='+'||s[0]=='-'){
		cout<<"E'->ATE'";
		show(1);
		A();
		T();
		E_();
	}
	else{
		cout<<"E'->#";
		show(0);
	}
	
}
void A()
{
	if(s[0]=='+'){
		s1+=s[0];
		s=s.substr(1,s.length());
		cout<<"A->+";
		show(0);
	}
	else if(s[0]=='-'){
		s1+=s[0];
		s=s.substr(1,s.length());
		cout<<"A->-";
		show(0);
	}
	else{
		cout<<"\n���ַ����Ƿ���"<<endl;
		exit(0); 
	}
} 
void F()
{
	if(s[0]=='('){
		s1+=s[0];
		s=s.substr(1,s.length());
		cout<<"F->(E)";
		show(0);
		E();
		if(s[0]==')'){
			s1+=s[0];
			s=s.substr(1,s.length());
			cout<<"F->(E)";
			show(0);
		}
		else{
			cout<<"\n���ַ����Ƿ���"<<endl;
			exit(0); 
		}
	}
	else if(s[0]=='i'){
		s1+=s[0];
		s=s.substr(1,s.length());
		cout<<"F->i";
		show(0);
	}
	else{
		cout<<"\n���ַ����Ƿ���"<<endl;
		exit(0); 
	}
}
void M()
{
	if(s[0]=='*'){
		s1+=s[0]; 
		s=s.substr(1,s.length());
		cout<<"M->*";
		show(0);
	}
	else if(s[0]=='/'){
		s1+=s[0]; 
		s=s.substr(1,s.length());
		cout<<"M->/";
		show(0);
	}
	else{
		cout<<"\n���ַ����Ƿ���"<<endl;
		exit(0); 
	}
}
void T_()
{
	if(s[0]=='*'||s[0]=='/'){
		cout<<"T'->MFT'";
		show(1);
		M();
		F();
		T_();
	}
	else{
		cout<<"T'->#";
		show(0);
	}
} 
int main()
{
	cout<<"�������ʶ�𴮣�";
	cin>>s;
	s1="";
	cout<<"�ķ�\t\t�ѷ�����\t��ǰ������\tʣ�മ"<<endl;
	E();
	if(s=="$") cout<<"\n�ô���ȷʶ��"<<endl;
	else cout<<s<<endl; 
	return 0;
} 