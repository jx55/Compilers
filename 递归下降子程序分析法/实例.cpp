#include<iostream>
#include<string>
using namespace std;

void E(); //E->TG
void G();// G->+TG|-TG G->��
void T(); //T->FS
void S(); // S->*FS|/FS S->��
void F(); //F->(E)|i
string str;
string strC;
int main()
{
    cout << "�������ַ�����#�Ž�������" << endl;
    cin >> str;
    strC = str;
    cout << "�ķ�        ������     �����ַ�        ʣ�മ" << endl;
    E();
	return 0; 
}
void E()
{
    cout << "E->TG        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
    T();
    G();
}
void T()
{
    cout << "T->FS        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
    F();
    S();

}
void G()
{
    if (str[0] == '+'){
        cout << "G->+TG        " << strC.substr(0, strC.length()-str.length()+1) << "        " << str[0] << "        " << str << endl;
        str = str.substr(1, str.length());
        T();
        G();
    }
    else if (str[0] == '-'){
        cout << "G->-TG        " << strC.substr(0, strC.length() - str.length()+1) << "        " << str[0] << "        " << str << endl;
        str = str.substr(1, str.length());
        T();
        G();
    }
    else{
        cout << "G->��                " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str << endl;
    }
}
void F()
{
    if(str[0] == 'i'){
        cout << "F->i        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
        str = str.substr(1, str.length());
    }
    else if (str[0] == '(') {
        cout << "F->(E)        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
        str = str.substr(1, str.length());
        E();
        if (str[0] == ')') {
            cout << "F->(E)        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
            str = str.substr(1, str.length());
        }
        else {
            printf("\n�Ƿ��ķ��Ŵ�!\n");
            exit(0);
        }
    }
    else {
        printf("�Ƿ��ķ��Ŵ�!\n");
        exit(0);
    }
}
void S()
{
    if (str[0] == '*') {
        cout << "S->*FS        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
        str = str.substr(1, str.length());
        F();
        S();
    }
    else if (str[0] == '/')
    {
        cout << "S->/FS        " << strC.substr(0, strC.length() - str.length() + 1) << "        " + str[0] << "        " << str << endl;
        str = str.substr(1, str.length());
        F();
        S();
    }
    else{
        cout << "S->��        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
    }
}