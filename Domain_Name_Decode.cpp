# include <iostream>
# include <string>
# include <fstream>
using namespace std;

# define FileName "out_come.txt"
# define OutName "out_come.txt"
# define Max 128

//域名解析器

// www.baidu.com
// bjut.edu.cn

/*
*----menu	//开始菜单
*----file_read()//读取文件并构造域名树
*----file_output()//将域名树保存到文件中
*----insert()//进行插入操作
*----Find()//查找域名 IP地址
*----Delete()//删除域名
*----Print()//打印到屏幕
*----Check_Domain_Name()//检测域名输入是否合法
*----Check_IP_Adress()//检测IP地址输入是否合法
*----Transfer()//域名反转
*----Destroy()//销毁域名树
*/

struct Node{	//域名树的节点
	bool flag;  //表明状态  1表示节点 0表示IP
	string name;//保存域名信息
	Node* next; //指向兄弟
	Node* child; //指向孩子
	Node(){
		flag = 1;
		name = "";
		next = NULL;
		child = NULL;
	}
	Node(string n, int f):name(n),flag(f){
		next = NULL;
		child = NULL;
	}
};

class DND{	//Domain Name Decode 域名解析器
	public:
		DND();
		~DND();
		void menu();
		void Find();
		void Print();
		void Insert();//递归实现
		//void Delete();//递归实现
	private:
		Node head;	// head Node without information  头节点 不包含信息
		fstream in;
		fstream out;
		void Print(Node* p, char *temp, int count);
		Node* Destroy(Node* p);		//
		Node* file_read(Node* p, string & domain_name, string & IP_name, int& count);	//读取文件自动开始
		void file_output(Node* p, char * temp, int count);
		bool Transfer(string& name);
		bool Find(Node*p, string& name, int& count);
		bool Check_IP_Adress(string& name);
		bool Insert(string& name, Node* &p, int& count);
		//bool Delete(string& name, Node* &p);
		bool Check_Domain_Name(string& name);
};

DND::DND(){
	in.open(FileName,ios::in);

	if( in.bad() ){
		cout << "can't run this program";
		exit(0);
	}

	string domain_name = "";
	string IP_name = "";

	while(true){
		char temp;
		domain_name = "";
		IP_name = "";

		temp = in.get();
		while( temp != 32 ){
			//cout << temp;
			//system("pause");
			if( in.eof() ) break;
			domain_name += temp;
			temp = in.get();
		}
		if( in.eof() )	break;

		temp = in.get();
		while( temp != '\n'){
			if( in.eof() ) break;
			IP_name += temp;
			temp = in.get();
		}
		if( in.eof() ) break;
		cout << domain_name << " " << IP_name << endl;
		int count = 0;
		head.child = file_read(head.child, domain_name, IP_name, count);
	}
	in.close();
	Print();
}

DND::~DND(){
	out.open(OutName,ios::out);

	if( out.bad() ){
		cout << "can't run this program";
		head.child = Destroy(head.child);
		exit(0);
	}

	char temp[Max];
	int count = 0;
	file_output(head.child, temp, count);
	head.child = Destroy(head.child);
	out.close();
}

Node* DND::Destroy(Node* p){
	if( p == NULL )	return NULL;
	p->child = Destroy(p->child);
	p->next = Destroy(p->next);
	delete p;
	return NULL;
}

bool DND::Check_IP_Adress(string &IP_name){
	int size = IP_name.size();
	if( size > 15 || size < 7) return 1; // Is illegal

	unsigned short IP_number = 0;
	int count = 0;

	do{
		if( IP_name[count] > 57 || IP_name[count] < 46) return 1;
		//else if( IP_name[count] == 47)	return 1;
		IP_number = 0;
		while( IP_name[count] != '\0' && IP_name[count]!= '.'){
			if( IP_name[count] == 47 )	return 1;
			IP_number = IP_number*10 + IP_name[count]-48;
			count++;
		}
		cout << IP_number << endl;
		system("pause");
		if( IP_number > 255 || IP_number < 0)	return 1;
	}while( IP_name[count++] != '\0');

	return 0;
}

bool DND::Check_Domain_Name(string & temp){
	/*
		字符长度不能超过256位
		不允许特殊符号 只能由大写字母 小写字母 数字 .组成
		.号不能出现在首位以及末位
		不合法返回1
		合法返回0
	*/
	const int size = 64;
	int number = temp.size();
	if( number == 0 || number > size )	return 1;
	int count = 0;

	while(temp[count] != '\0')
		if( temp[count++] < 45 || temp[count] > 122 || temp[count] > 57 && (temp[count] < 65) || (temp[count] > 90 && temp[count] < 97))
			return 1;
	return 0;
}

bool DND::Transfer(string& temp){
	if( Check_Domain_Name(temp) ) //if is illegal
		return 0;

	string outcome = "";
	string record;

	char name;
	int count = -1;
	int size;

	size = temp.size()-1;

	while( size >= 0 ){
		name = temp[size--];
		if( name == '.' ){
			while( count >= 0 )
				outcome += record[count--];
			outcome += '.';
			record = "";
			count = -1;
		}
		else{
			record += name;
			++count;
		}
	}
	while( count >= 0)
		outcome += record[count--];

	//cout << outcome << endl;
	temp = outcome;
	return 1;
}

void DND::Insert(){
	string name;
	cout << "请输入域名:";
	cin >> name;

	while( !Transfer(name) ){
		cout << "域名非法 请重新输入：";
		name = "";
		cin >> name;
	}

	int count = 0;
	if( Insert(name, head.child, count) ){	//插入成功
		cout << "插入成功" << endl;
		return;
	}
	else{
		cout << "插入失败" << endl;	//为什么会插入失败 可能存在bug
		return;
	}


bool DND::Insert(string& name, Node* &root, int& count){// ����Ϊ www.baidu.com �Ѿ�ת��Ϊcom.baidu.www

	Node *p = root;//p is active pointer

	int flag = 1;
	int create = 0;

	if( name[count] == '\0' ){
		string IP_Adress = "";

		if( p == NULL ){
			cout << "������Ӧ��IP��ַ:";
			cin >> IP_Adress;
			while( Check_IP_Adress(IP_Adress) ){
				IP_Adress = "";
				cout << "input is illegal! enter again. if you want to quit input 0";
				cin >> IP_Adress;
				if(IP_Adress[0] == '0' && IP_Adress[1] == '\0'){
					cout << "Insert unsuccess\n";
					return 0;//	insert unsuccess
				}
			}
			p = new Node(IP_Adress,0);// flag = 0 means it equal to IP Node
			root = p;
			return 1;// insert success
		}
		else if( p->flag ){
			cout << p->name << endl;
			cout << "this node is just a Link Node. there must be something wrong\n";
			return 0;
		}
		else{
			cout << "Do you want to change this IP Adress? Change enter IP, Quit enter 0:";
			do{
				cin >> IP_Adress;
				if(IP_Adress[0] == '0' && IP_Adress[1] == '\0'){
					cout << "Insert unsuccess\n";
					return 0;//	insert unsuccess
				}
			}while( Check_IP_Adress(IP_Adress));
			p->name = IP_Adress;
			return 1;
		}
	}
	string cut_name = "";
	while( name[count] != '\0' && name[count] != '.')
		cut_name += name[count++];
	if( name[count] == '.')
		++count;

	if( p == NULL ){// new insert
		p = new Node(cut_name,1);//means equal to link node
		create = 1;
	}
	else if( cut_name == p->name){
		//continue to next without doing anything
	}
	else if( cut_name != p->name){
		Node * brother = p->next;
		while( brother != NULL && brother->name != cut_name){ // find next brother
			//pre = p;
			p = brother;
			brother = p->next;
		}
		if( brother == NULL ){
			p->next = new Node(cut_name,1);//means equal to link node
			p = p->next;
			create = 1;
		}
		else{ //if has same link node
			//pre = p;
			p = brother;
		}
		flag = 0;
	}

	if( Insert(name,p->child,count) ){//insert success
		if( flag )	root = p;
		cout << "success" << endl;
		return 1;
	}
	else{	//insert unsuccess
		if(	create ) delete p;
		cout << "unsuccess" << endl;
		return 0;
	}
}

void DND::Find(){
	string name;
	cout << "请输入域名:";
	cin >> name;

	while( !Transfer(name) ){
		cout << "域名非法 请重新输入：";
		name = "";
		cin >> name;
	}

	int count = 0;
	if( Find(head.child, name, count) )
		cout << "成功找到" << endl;
	else
		cout << "未找到" << endl;
}

bool DND::Find(Node* p, string & temp, int & count){
	if( p == NULL )
		return 0;
	if( temp[count] == '\0' ){
		if( p->flag ){
			cout << "I don't know why but it is a link node";
			return 0;
		}
		else{
			cout << p->name << endl;
			return 1;
		}
	}

	string cut_name = "";
	while( temp[count] != '\0' && temp[count] != '.')
		cut_name += temp[count++];
	if( temp[count] == '.')
		++count;

	if( cut_name != p->name){
		Node* brother = p->next;
		while( brother != NULL && cut_name != brother->name){
			p = brother;
			brother = p->next;
		}
		if( brother == NULL){
			cout << " the domain name isn't excit" << endl;
			return 0;
		}
		else{
			p = brother;
		}
	}

	if( Find(p->child, temp, count))
		return 1;
	else
		return 0;
}

void DND::Print(){
	char temp[Max];
	Print(head.child,temp,0);
}

void DND::Print(Node* p, char* temp, int count){//, char * temp, int count){
	if( p == NULL )
		return;
	int size = p->name.size();
	int save = count;
	for(int i = 0; i < size; ++i)
		temp[count++] = p->name[i];

	if( p->flag == 0){
		temp[count] = '\0';
		cout << temp << endl;
		//cout << p->name << endl;
		return;
	}
	if( p->child->flag ){
		temp[count++] = '.';
		//cout << p->name << ".";
	}
	else{
		//cout << p->name << " ";
		temp[count++] = ' ';
	}
	//cout << count << endl;
	Print(p->child, temp, count);
	//cout << "count:" << count << endl;
	Print(p->next, temp, save);
	//cout << "count next:" << count << endl;
}

void DND::file_output(Node* p, char* temp, int count){
	if( p==NULL )
		return;
	int save = count;
	const int size = p->name.size();

	for(int i = 0; i < size; ++i)
		temp[count++] = p->name[i];

	if(p->flag == 0){
		temp[count] = '\0';
		out << temp << endl;
		return;
	}
	else if(p->child->flag)
		temp[count++] = '.';
	else
		temp[count++] = ' ';

	file_output(p->child, temp, count);
	file_output(p->next, temp, save);
}

void DND::menu(){
	int choose;
	cout << "1.Insert\n";
	cout << "2.Find\n";
	cout << "3.Print\n";
	cout << "4.quit\n";
	cout << "choose one to use:";
}

Node* DND::file_read(Node* p, string & domain_name, string & IP_name, int& count){
	if( domain_name[count] == '\0' ){
		p = new Node(IP_name, 0);
		return p;
	}
	int create = 0;
	Node * save = p;
	string cut_name = "";
	while( domain_name[count] != '\0' && domain_name[count] != '.')
		cut_name += domain_name[count++];
	cout << cut_name << endl;

	if( domain_name[count] == '.')
		++count;

	if( p == NULL ){
		p = new Node(cut_name, 1);
	}
	else if( p->name != cut_name){
		Node* brother = p->next;
		while( brother != NULL && brother->name != cut_name){
			p = brother;
			brother = p->next;
		}
		if( brother == NULL){
			brother = new Node(cut_name, 1);
			p->next = brother;
			p = p->next;
		}
		else
			p = brother;
		create = 1;
	}
	else{
	}

	p->child = file_read(p->child,domain_name, IP_name, count);
	if( create )
		return save;
	else
		return p;
}

int main(){
	DND DN_decode;
	return 0;
}
