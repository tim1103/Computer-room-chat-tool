#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <math.h>
#include <conio.h>
#include <vector>
#include <string>
#include <cctype>
using namespace std;
#define    BUFLEN        2000                  // 缓冲区大小
#define WSVERS        MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 指明winsock 2.0 Llibrary
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

/*------------------------------------------------------------------------
 * main - TCP client for DAYTIME service
 *------------------------------------------------------------------------
 */
void colset(int a){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}
void Goto(int X,int Y){
    COORD C; C.X=X; C.Y=Y;   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),C);   
}
void hideCursor(){
	HANDLE h_GAME =GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(h_GAME,&cursor_info);
	cursor_info.bVisible=false;
	SetConsoleCursorInfo(h_GAME,&cursor_info);
}
void showCursor(){
	HANDLE h_GAME =GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(h_GAME,&cursor_info);
	cursor_info.bVisible=true;
	SetConsoleCursorInfo(h_GAME,&cursor_info);
}
SOCKET    sock, sockets[100] = { NULL };                          /* socket descriptor            */
//    int    cc;                                /* recv character count            */
char    *packet = NULL;               /* buffer for one line of text    */
char *pts, *input;
HANDLE hThread;
unsigned threadID;
int gameType;
char    host[20];
char service[20];
char   namei[20];
struct MessageBlock{
	vector<string> info;
	vector<int> col;
	vector<vector<pair<int,int> > >atR;
	int hei,wid,x,y;
	char* nam;
	int upl,mxx;
	bool newMess;
	bool lines;
	bool enableHL;
	void clear(){
		info.clear();info.push_back("");
		atR.clear();atR.push_back(vector<pair<int,int> >{});
		col.clear();col.push_back(7);upl=mxx=0;newMess=false;
	}
	void printSide(){
		if(hei==0 || wid==0)	return;
		for(int i=x-1;i<=x+hei;i++){
			Goto(y-2,i);
			for(int j=y-2;j<=y+wid;j+=2){
				bool f1=(i==x-1) || (i==x+hei);
				bool f2=(j==y-2) || (j==y+wid+1);
				if(i==x-1 && j==y-2)	printf("┏");
				else if(i==x+hei && j==y-2)	printf("┗");
				else if(i==x-1 && j==y+wid)	printf("┓");
				else if(i==x+hei && j==y+wid)	printf("┛");
				else if(i==x-1 || i==x+hei)	printf("━━");
				else if(j==y-2 || j==y+wid)	printf("┃");
				else	printf("  "); 
			}
		}
		Goto(y,x-1);colset(7*16);
		printf("%s",nam);
		colset(7);
	}
	MessageBlock(int hei=0,int wid=0,int x=0,int y=0,char* nam=NULL,bool lines=false,bool enableHL=false)
		:hei(hei),wid(wid),x(x),y(y),nam(nam),lines(lines),enableHL(enableHL){
		printSide();clear();
	}
	void addHL(int pp,int l,int r){
		if(!enableHL)	return;
		if(l/(wid-1)==r/(wid-1))
			atR[pp+l/(wid-1)].push_back(make_pair(l%(wid-1),r%(wid-1)));
		else	addHL(pp,l,(l/(wid-1)+1)*(wid-1)-1),addHL(pp,(l/(wid-1)+1)*(wid-1),r);
	}
	void addMessage(char* buf,int c){
		if(upl==max(0,(int)info.size()-1-hei))
			upl=-1,newMess=false;
		else	newMess=true;
		col.back()=c;int idxx=info.size()-1;
		for(int i=0;buf[i];i++){
			if(buf[i]=='\r')	continue;
			else if(buf[i]=='\n'){
				info.push_back("");
				col.push_back(c);
				atR.push_back(vector<pair<int,int> >{});
			}
			else{
				if(info.back().size()==wid-1){
					info.push_back("");
					col.push_back(c);
					atR.push_back(vector<pair<int,int> >{});
				}
				info.back()+=buf[i];
			}
		}
		char p[25];
		sprintf(p,"@%s",namei);
		int idx=0;
		for(int i=0;buf[i];i++){
			if(buf[i]==p[idx])	++idx;
			else{
				idx=0;
				if(buf[i]==p[idx])	++idx;
			}
			if(p[idx]=='\0' && (isspace(buf[i+1]) || buf[i+1]=='\0'))
				addHL(idxx,i-strlen(p)+1,i);
		}
		info.push_back("");
		col.push_back(c);
		atR.push_back(vector<pair<int,int> >{});
		mxx=max(0,(int)info.size()-1-hei);
		if(upl==-1)	upl=mxx;
	}
	void prevMessage(){
		upl-=hei*2/3;
		if(upl<0)	upl=0;
	}
	void nextMessage(){
		upl+=hei*2/3;
		if(upl>=mxx){
			newMess=false;
			upl=mxx;
		}
	}
	void oldestMessage(){
		upl=0;
	}
	void newestMessage(){
		upl=mxx;newMess=false;
	}
	void printMessage(){
		int p=x;
		for(int i=x;i<x+hei;i++){
			Goto(y,i);
			for(int j=1;j<wid;j++)	
				putchar(' ');
		}
		if(!enableHL){
			for(int i=upl;i<upl+hei && i<(int)info.size()-1;i++){
				Goto(y,p++);colset(col[i]);
				printf("%s",info[i].c_str());
			}
		}
		else{
			for(int i=upl,currP;i<upl+hei && i<(int)info.size()-1;i++){
				Goto(y,p++);currP=0;
				for(int j=0;j<(int)atR[i].size();j++){
					colset(col[i]);
					while(currP<atR[i][j].first)
						putchar(info[i][currP++]);
					colset(14*16);
					while(currP<=atR[i][j].second)
						putchar(info[i][currP++]);
				}
				colset(col[i]);
				while(info[i][currP])
					putchar(info[i][currP++]);
			}
		}
		if(newMess){
			colset(63);
			Goto(y,x+hei-1);
			for(int j=1;j<wid;j++)
				putchar(' ');
			Goto(y,x+hei-1);
			printf("> New Message! <");
		}
		if(lines){
			for(int i=0;i<hei;i++){
				Goto(y+wid-1,x+i);
				if((int)info.size()-1<=hei)	colset(9),printf("┃ ");
				else if((i*1.0)/hei*((int)info.size()-1)>=upl
					 && (i*1.0)/hei*((int)info.size()-1)<upl+hei)
					 	colset(9),printf("┃ ");
				else	colset(7),printf("┃ ");
			}
		}
		colset(7);
	}
}ChatBox,TopBox,StatBox,InfoBox;
void flushWindow(int typ){
	hideCursor();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	if(typ&1)	TopBox.printMessage();
	if(typ&2)	ChatBox.printMessage();
	if(typ&4)	StatBox.printMessage();
	Goto(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y); 
	showCursor();
}
void refreshWindow(){
    hideCursor();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	ChatBox.printSide();
    StatBox.printSide();
    InfoBox.printSide();
    TopBox.printSide();
    TopBox.printMessage();
	ChatBox.printMessage();
	StatBox.printMessage();
	Goto(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y); 
	showCursor();	
}
bool checkAt(char* str){
	char p[25];
	sprintf(p,"@%s",namei);
	int idx=0;
	for(int i=0;str[i];i++){
		if(str[i]==p[idx])	++idx;
		else	idx=0;
		if(p[idx]=='\0' && (isspace(str[i+1]) || str[i+1]=='\0'))	return true;
	}
	return false;
}
int toDec(char ch){
	if(isdigit(ch))	return ch-'0';
	return ch-'A'+10;
}
unsigned int __stdcall Chat(PVOID PM)
{
    time_t    now;
    (void)time(&now);
    pts = ctime(&now);
    char buf[2000];
	char buffer[2000];
    while (1){
        int cc = recv(sock, buf, BUFLEN, 0);   //cc为接收的字符数
        if (cc == SOCKET_ERROR || cc == 0){
            sprintf(buffer,"[ERROR] Error Code : %d\nCannot find the service.",GetLastError());
            ChatBox.addMessage(buffer,12);
            TopBox.addMessage("Cannot find the service",12);
            flushWindow(3);
            CloseHandle(hThread);
            (void)closesocket(sock);
            break;
        }
        else if (cc > 0){
            if(!strcmp(buf+6,"/cls")){
            	if(buf[1]=='T')	TopBox.clear(),flushWindow(1);
	            if(buf[1]=='C')	ChatBox.clear(),flushWindow(2);
	            if(buf[1]=='S')	StatBox.clear(),flushWindow(4);
            }
            else{
	            int col=toDec(buf[3])*16+toDec(buf[4]);
	            if(buf[1]=='T')	TopBox.addMessage(buf+6,col),flushWindow(1);
	            if(buf[1]=='C')	ChatBox.addMessage(buf+6,col),flushWindow(2);
	            if(buf[1]=='S')	StatBox.addMessage(buf+6,col),flushWindow(4);
			}
        }
    }
    return 0;
}
int main(int argc, char *argv[])
{
  system("chcp 936");
    time_t    now;
    (void)time(&now);
    pts = ctime(&now);
    char ale[2000];
    system("mode con cols=86 lines=26");
    TopBox=MessageBlock(1,30,1,54,"Status");
    TopBox.addMessage("Please enter a chat room",12);
    TopBox.printMessage();
    ChatBox=MessageBlock(20,48,1,2,"Chat Room",true,true);
    StatBox=MessageBlock(17,30,4,54,"User Info.");
    InfoBox=MessageBlock(1,82,23,2,"Input Area");
    Goto(2,23);
    ChatBox.addMessage("Host IP : ",7);
    ChatBox.printMessage();
    Goto(2,23);gets(host);
    sprintf(ale,"[NOTE] Get Host IP : %s",host);
    ChatBox.addMessage(ale,10);
    ChatBox.printMessage();
    InfoBox.addMessage("",0);
    InfoBox.printMessage();
	ChatBox.addMessage("Service Port : ",7);
    ChatBox.printMessage();
    Goto(2,23);gets(service);
    sprintf(ale,"[NOTE] Get Service Port : %s",service);
    ChatBox.addMessage(ale,10);
    ChatBox.printMessage();
    InfoBox.addMessage("",0);
    InfoBox.printMessage();
	ChatBox.addMessage("Your Name : ",7);
    ChatBox.printMessage();
    Goto(2,23);gets(namei);
    InfoBox.addMessage("",0);
    InfoBox.printMessage();
    ChatBox.clear();
    ChatBox.printMessage();
    Goto(2,23);
    //const char *service = "50520";
    struct  sockaddr_in sin;            /* an Internet endpoint address    */
    WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);       /* 启动某版本Socket的DLL        */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons((u_short)atoi(service));    //atoi：把ascii转化为int. htons：主机序(host)转化为网络序(network), s--short
    sin.sin_addr.s_addr = inet_addr(host);           //如果host为域名，需要先用函数gethostbyname把域名转化为IP地址

    sock = socket(PF_INET, SOCK_STREAM, 0);

    connect(sock, (struct sockaddr *)&sin, sizeof(sin));
    (void) send(sock,namei, sizeof(namei), 0);;
    hThread = (HANDLE)_beginthreadex(NULL, 0, Chat, NULL, 0, &threadID);

    while (1)
    {
        char buf1[2000];
        gets(buf1);
        if(strlen(buf1)==0){
        	InfoBox.addMessage("",0);
        	InfoBox.printMessage();
        	Goto(2,23);
		}
        else if (!strcmp(buf1, "/exit"))
            goto end;
        else{
        	InfoBox.addMessage("",0);
        	InfoBox.printMessage();
        	if(!strcmp(buf1,"/cls")){
        		ChatBox.clear();
        		flushWindow(2);
			}
        	else if(!strcmp(buf1,"/w")){
        		ChatBox.prevMessage();
        		flushWindow(2);
			}
			else if(!strcmp(buf1,"/s")){
        		ChatBox.nextMessage();
        		flushWindow(2);
			}
			else if(!strcmp(buf1,"/a")){
        		ChatBox.oldestMessage();
        		flushWindow(2);
			}
			else if(!strcmp(buf1,"/d")){
        		ChatBox.newestMessage();
        		flushWindow(2);
			}
			else if(!strcmp(buf1,"/r")){
        		refreshWindow();
			}
			else
           		(void)send(sock, buf1, sizeof(buf1), 0);
		}
        (void)time(&now);
        pts = ctime(&now);
    }

end:    CloseHandle(hThread);
    closesocket(sock);
    WSACleanup();

    printf("Press Enter to contine...");
    getchar();
    return 0;
}
