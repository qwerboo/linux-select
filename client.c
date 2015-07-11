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
  
size_t doit(int sock)  
{  
fd_set rset;  
FD_ZERO(&rset);  
  
int fdmax;  
int nready;  
int fd_stdin = fileno(stdin);  
fdmax = sock > fd_stdin ? sock : fd_stdin;  
int n;  
struct packet recvBuf;  
struct packet sendBuf;  
  
while(1)  
{  
FD_SET(sock, &rset);  
FD_SET(fd_stdin, &rset);  
nready = select(fdmax + 1, &rset, NULL, NULL, NULL);  
  
if(nready == -1)  
{  
perror("select return -1.");  
exit(1);  
}  
else if(nready == 0)  
continue;// actually, nready == 0 would NEVER happen, because timeval is NULL  
else  
{  
if(FD_ISSET(sock, &rset))  
{  
memset(&recvBuf, 0, sizeof(recvBuf));  
int ret = readn(sock, &recvBuf.len, 4);  
if(ret == -1)  
{  
perror("sock ret -1");  
exit(-1);  
}  
else if(ret < 4)  
{  
perror("client close.");  
break;  
}  
n = ntohl(recvBuf.len);  
ret = readn(sock, recvBuf.content, n);  
if(ret == -1)  
{  
perror("sock ret -1");  
exit(-1);  
}  
else if(ret < n)  
{  
perror("client close.");  
break;  
}  
fputs(recvBuf.content, stdout);  
}  
if(FD_ISSET(fd_stdin, &rset))  
{  
if(fgets(sendBuf.content, sizeof(sendBuf.content), stdin) == NULL)  
break;  
n = strlen(sendBuf.content);  
sendBuf.len = htonl(n);  
writen(sock, &sendBuf, n + 4);  
memset(&sendBuf, 0, sizeof(sendBuf));  
}  
}  
}  
close(sock);  
} 
  
int main()  
{  
int sock;  
if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  
perror("socket error!");  

struct sockaddr_in servaddr;  
memset(&servaddr, 0, sizeof(servaddr));  

servaddr.sin_family = AF_INET;  
servaddr.sin_port = htons(6000);  
servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
  
if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)  
{  
perror("sockect error.");  
exit(1);  
}  
  
doit(sock);   
  
return 0;  
}  
