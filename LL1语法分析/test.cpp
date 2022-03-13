#include <iostream>
using namespace std;

int main()
{
	string s="123'sa";
	cout<<s<<endl;
	for(int i=0;i<s.length();i++)
	{
		string tmp;
		tmp.push_back(s[i]);
		if(tmp=="'") cout<<i<<" HHH"<<endl;
	}
	return 0;
} 