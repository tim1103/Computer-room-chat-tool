#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <math.h>
#include <algorithm>
#include <set>
#define QLEN 5
#define WSVERS MAKEWORD(2, 0)
#define BUFLEN 2000
#pragma comment(lib,"ws2_32.lib")
#define BANNED_TIME 120
using namespace std;
const char * systemAlert = "\
               Chat Room ver. 1.0\n\
/w(s|a|d) | to see the previous(next\n\
          |   /oldest/latest) messages.\n\
/r        | to refresh the pages.\n\
/cls      | to clear the messages.\n\
/exit     | to exit the chat room.";
SOCKET msock, ssock;
SOCKET  sockets[100] = {NULL};
char na[101][21];
int cc,nai;
time_t now;
char pts[64],tim[64],ptss[64];
char buf[2000],nam[2000];
char *input;
HANDLE hThread1,hThread[100] = {NULL};
clock_t sta,end;
unsigned int threadID,ThreadID[105],number,sen=0;
char *name[1005];
bool GetIn[2000];
double score[2000];
int att[2000];
struct  sockaddr_in fsin;
struct  sockaddr_in Sin;

const int maxn=1e5+5;
char currScore[2000];
char fileName[2000];
clock_t Las[2000];
bool banned[2000];
int warned[2000];
unsigned int __stdcall Chat(PVOID PM) {
    (void) time(&now);
    strftime(pts,sizeof(pts),"%Y-%m-%d",localtime(&now));

    sprintf(fileName,"chat%s.txt",pts);

    FILE *fp=fopen(fileName,"a");
    if((fp=fopen(fileName,"a")) == NULL) {
        printf("ERROR : data file cannot be opened!\n");
        exit(1);
    }
    char buf1[2000];
    char buf2[2000];
    char buf3[2000];
    char buf4[2000];
    char buf15[2000];

    (void) time(&now);
    strftime(pts,sizeof(pts),"%Y,%b,%d %X %a",localtime(&now));

    fprintf(fp,"*-*-*-*-*[%s]*-*-*-*-*\n\n",pts);

    (void) time(&now);
    strftime(pts,sizeof(pts),"%X",localtime(&now));
    sockets[number] = ssock;
    SOCKET  sock = ssock;
    ThreadID[number] = threadID;
    GetIn[number]=true;
    Las[number]=clock();
    banned[number]=false;
    warned[number]=0;
    unsigned int threadid = threadID;
    char namei[20];

    (void) recv(sock,namei, BUFLEN, 0);
    name[number]=namei;
    sprintf(buf2,"[C-0A][NOTE] %s\n%s entered the chat room!",pts,namei);
    printf("%s",buf2);
    fprintf(fp,"%s",buf2);
    for(int i=0; i<=number; i++) {
        if(sockets[i] != NULL && sockets[i] != sock) {
            (void) send(sockets[i],buf2, sizeof(buf2), 0);
            printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
        }
    }
    sprintf(buf2,"[C-0A][NOTE] Chat name : %s\nUser(s) : ",nam);
    bool hasnC=false;
    for(int i=0;i<=number;i++){
        if(!GetIn[i])   continue;
        if(hasnC)   sprintf(buf2,"%s, ",buf2);
        sprintf(buf2,"%s%s",buf2,name[i]);
        hasnC=true;
    }
    sprintf(buf2,"%s\n%s",buf2,systemAlert);
    fprintf(fp,"%s",buf2);
    (void) send(sock,buf2, sizeof(buf2), 0);
    puts("");
    fprintf(fp,"\n");
    memset(currScore,0,sizeof(currScore));
    bool havE=false;
    for(int i=0;i<=number;i++){
        if(!GetIn[i])   continue;
        if(havE)	sprintf(currScore,"%s\n",currScore);
        sprintf(currScore,"%s|-%c-%s",currScore,(banned[i]?'X':'-'),name[i]);
        havE=true;
    }
    for(int i=0;i<=number;i++)
    	if(sockets[i]!=NULL){
    		sprintf(buf2,"[S-00]/cls");
	    	(void)send(sockets[i],buf2,sizeof(buf2),0);
    		sprintf(buf2,"[S-07]Chat Name : %s\nUser : %s\nUser List :\n%s",nam,name[i],currScore);
    		(void)send(sockets[i],buf2,sizeof(buf2),0);
    		printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
		}
	sprintf(buf2,"[T-07]Chat Room");
    (void) send(sock,buf2, sizeof(buf2), 0);
    printf("(成功将消息发送至 %s<%d>)\n",name[number],ThreadID[number]);
flag1:
    cc = recv(sock, buf3, BUFLEN, 0);
    if(cc == SOCKET_ERROR || cc == 0) {

        (void) time(&now);
        strftime(pts,sizeof(pts),"%X",localtime(&now));

        sprintf( buf3,"[C-0A][NOTE] %s\n%s left the chat room!",pts,namei);
		for(int i=0;i<=number;i++)	
			if(sockets[i] == sock){
		        sockets[i] = NULL;
		        name[i] = NULL;
		        GetIn[i]=false;
		        score[i]=0;
		        att[i]=0;
		        banned[i]=false;
		        warned[i]=0;
		        CloseHandle(hThread[i]);
		        break;
			}
        sock = NULL;
        printf("%s", buf3);
        fprintf(fp,"%s", buf3);

        for(int i=0; i<=number; i++) {
            if(sockets[i] != NULL && sockets[i] != sock) {
                (void) send(sockets[i], buf3, sizeof(buf3), 0);
                printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
            }
        }
		memset(currScore,0,sizeof(currScore));
	    bool havE=false;
	    for(int i=0;i<=number;i++){
	        if(!GetIn[i])   continue;
	        if(havE)	sprintf(currScore,"%s\n",currScore);
	        sprintf(currScore,"%s|%c%s",currScore,(banned[i]?'X':'-'),name[i]);
	        havE=true;
	    }
	    for(int i=0;i<=number;i++)
	    	if(sockets[i]!=NULL){
	    		sprintf(buf3,"[S-00]/cls");
	    		(void)send(sockets[i],buf3,sizeof(buf3),0);
	    		sprintf(buf3,"[S-07]Chat Name : %s\nUser : %s\nUser List :\n%s",nam,name[i],currScore);
	    		(void)send(sockets[i],buf3,sizeof(buf3),0);
	    		printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
			}
        puts("");
        fprintf(fp,"\n");

    }

    else if(cc > 0) {
		clock_t currT=clock();
		double tim;int id;
		for(int i=0; i<=number; i++) {
            if(sockets[i] != NULL && sockets[i] == sock) {
            	id=i;break;
            }
    	}
    	tim=(1.0*(currT-Las[id]))/CLOCKS_PER_SEC;
    	printf("- %lf %d\n",tim,strlen(buf3));
    	if(banned[id] && tim<=BANNED_TIME){
    		;
		}
		else if(banned[id] && tim>BANNED_TIME){
			banned[id]=false;Las[id]=currT;
			memset(currScore,0,sizeof(currScore));
		    bool havE=false;
		    for(int i=0;i<=number;i++){
		        if(!GetIn[i])   continue;
		        if(havE)	sprintf(currScore,"%s\n",currScore);
		        sprintf(currScore,"%s|%c%s",currScore,(banned[i]?'X':'-'),name[i],score[i]);
		        havE=true;
		    }
		    for(int i=0;i<=number;i++)
		    	if(sockets[i]!=NULL){
		    		sprintf(buf3,"[S-00]/cls");
		    		(void)send(sockets[i],buf3,sizeof(buf3),0);
		    		sprintf(buf3,"[S-07]Chat Name : %s\nUser : %s\nUser List :\n%s",nam,name[i],currScore);
		    		(void)send(sockets[i],buf3,sizeof(buf3),0);
		    		printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
				}
		}
		else if(tim*10<strlen(buf3) || tim<1.0){
			Las[id]=currT;
    		++warned[id];
    		if(warned[id]>=10){
    			banned[id]=true;warned[id]=0;
    			sprintf(buf4,"[C-0C][NOTE][ER] @%s You will be banned for a while. Please slow your speed.",name[id]);
    			for(int i=0; i<=number; i++) {
		            if(sockets[i] != NULL) {
		                (void) send(sockets[i],buf4, sizeof(buf4), 0);
		                printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
		            }
		    	}
		    	memset(currScore,0,sizeof(currScore));
			    bool havE=false;
			    for(int i=0;i<=number;i++){
			        if(!GetIn[i])   continue;
			        if(havE)	sprintf(currScore,"%s\n",currScore);
			        sprintf(currScore,"%s|-%c-%s",currScore,(banned[i]?'X':'-'),name[i],score[i]);
			        havE=true;
			    }
			    for(int i=0;i<=number;i++)
			    	if(sockets[i]!=NULL){
			    		sprintf(buf4,"[S-00]/cls");
			    		(void)send(sockets[i],buf4,sizeof(buf4),0);
			    		sprintf(buf4,"[S-07]Chat Name : %s\nUser : %s\nUser List :\n%s",nam,name[i],currScore);
			    		(void)send(sockets[i],buf4,sizeof(buf4),0);
			    		printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
					}
			}
			else{
		        (void) time(&now);
		        strftime(pts,sizeof(pts),"%X",localtime(&now));
		        sprintf(buf4,"[C-07][INFO] %s: %s",namei,buf3);
		        for(int i=0; i<=number; i++) {
		            if(sockets[i] != NULL) {
		                (void) send(sockets[i],buf4, sizeof(buf4), 0);
		                printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
		            }
		    	}
		        puts("");
		        fprintf(fp,"\n");
		        if(warned[id]>=5){
		        	sprintf(buf4,"[C-0E][NOTE][WN] Please slow your speed.");
    				(void) send(sock,buf4, sizeof(buf4), 0);
				}
			}
		}
		else{
			Las[id]=currT;
			(void) time(&now);
		    strftime(pts,sizeof(pts),"%X",localtime(&now));
			sprintf(buf4,"[C-07][INFO] %s: %s",namei,buf3);
		    for(int i=0; i<=number; i++) {
		        if(sockets[i] != NULL) {
		            (void) send(sockets[i],buf4, sizeof(buf4), 0);
		            printf("(成功将消息发送至 %s<%d>)\n",name[i],ThreadID[i]);
		        }
			}
		    puts("");
		    fprintf(fp,"\n");
		}
        goto flag1;
    }
    (void) closesocket(sock);

    return 0;
}

int main(int argc, char *argv[]) {
  system("chcp 936");
    int alen;
    WSADATA wsadata;
    char  service[10];

    printf("请输入服务器端口：");
    gets(service);

    //注意：客户端连接服务端，IP为服务端的计算机的IP，端口为上面的service
    printf("请输入对话名字：");
    gets(nam);

    WSAStartup(WSVERS, &wsadata);
    msock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&Sin, 0, sizeof(Sin));

    Sin.sin_family = AF_INET;
    Sin.sin_addr.s_addr = INADDR_ANY;
    Sin.sin_port = htons((u_short)atoi(service));
    bind(msock, (struct sockaddr *)&Sin, sizeof(Sin));
    listen(msock, 5);

    system("cls");

    printf("机房聊天工具【服务端】\n");
    printf("服务器端口：[%s]\n",service);
    printf("对话名字：%s\n",nam);
    (void) time(&now);
    strftime(pts,sizeof(pts),"%Y/%m/%d %X",localtime(&now));
    printf("时间:%s\n-----------------------\n",pts);

    number = -1;
    strftime(tim,sizeof(tim),"%X",localtime(&now));
    while(1) {

        alen = sizeof(struct sockaddr);
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
        ++number;
        hThread[number] = (HANDLE)_beginthreadex(NULL, 0,Chat,NULL, 0, &threadID);

    }

    (void) closesocket(msock);
    WSACleanup();

    return 0;
}
