#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#include <string.h>  
#include "readwrite.h"  
  
void do_srvce(int conn)  
{  
int nready;  
fd_set rset;  
FD_ZERO(&rset);  
int fdstdin = fileno(stdin);  
int maxfd = conn > fdstdin ? conn:fdstdin;  
  
struct packet sendBuf;  
struct packet recvBuf;  
  
while(1)  
{  
FD_SET(conn, &rset);  
FD_SET(fdstdin, &rset);  
  
nready = select(maxfd + 1, &rset, NULL, NULL, NULL);  
if(nready == -1)  
{  
perror("select ret -1");  
exit(-1);  
}  
else if(nready == 0)  
continue;  
if(FD_ISSET(conn, &rset))  
{  
memset(&recvBuf, 0, sizeof(recvBuf));  
int ret = readn(conn, &recvBuf.len, 4);  
if(ret == -1)  
{  
perror("conn ret -1");  
exit(-1);  
}  
else if(ret < 4)  
{  
perror("client close.");  
break;  
}  
int n = ntohl(recvBuf.len);  
ret = readn(conn, recvBuf.content, n);  
if(ret == -1)  
{  
perror("conn ret -1");  
exit(-1);  
}  
else if(ret < n)  
{  
perror("client close.");  
break;  
}  
fputs(recvBuf.content, stdout);  
}  
if(FD_ISSET(fdstdin, &rset))  
{  
if(fgets(sendBuf.content, sizeof(sendBuf.content), stdin) == NULL)  
break;  
sendBuf.len = htonl(strlen(sendBuf.content));  
writen(conn, &sendBuf, strlen(sendBuf.content) + 4);  
memset(&sendBuf, 0, sizeof(sendBuf));  
}  
}  
close(conn);  
}  
  
int main()  
{  
int listenfd;  
if((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  
perror("socket error!");  
  
struct sockaddr_in servaddr;  
memset(&servaddr, 0, sizeof(servaddr));  

servaddr.sin_family = AF_INET;  
servaddr.sin_port = htons(6000);  
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)  
perror("bind error!");  
if(listen(listenfd, SOMAXCONN) < 0)  
perror("listen");  
struct sockaddr_in peeraddr;  
socklen_t peerlen = sizeof(peeraddr);  
int conn;  
  
if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)  
{  
perror("accept error!");  
exit(1);   
}  
  
do_srvce(conn);  
close(listenfd);  
return 0;  
}  
