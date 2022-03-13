#include<iostream>
#include<string>
using namespace std;

void E(); //E->TG
void G();// G->+TG|-TG G->¦Å
void T(); //T->FS
void S(); // S->*FS|/FS S->¦Å
void F(); //F->(E)|i
string str;
string strC;
int main()
{
    cout << "ÇëÊäÈë×Ö·û´®£¨#ºÅ½áÊø£©£º" << endl;
    cin >> str;
    strC = str;
    cout << "ÎÄ·¨        ·ÖÎö´®     ·ÖÎö×Ö·û        Ê£Óà´®" << endl;
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
        cout << "G->¦Å                " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str << endl;
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
            printf("\n·Ç·¨µÄ·ûºÅ´®!\n");
            exit(0);
        }
    }
    else {
        printf("·Ç·¨µÄ·ûºÅ´®!\n");
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
        cout << "S->¦Å        " << strC.substr(0, strC.length() - str.length() + 1) << "        " << str[0] << "        " << str << endl;
    }
}