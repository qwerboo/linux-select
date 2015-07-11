#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#include "readwrie.h"
#define MAXLEN 1024  
  
  

  
  
size_t readn(int fd, void *buf, size_t len)  
{  
size_t left = len;  
int ret;  
char *pbuf = (char *)buf;  
  
  
while(left > 0)  
{  
ret = read(fd, pbuf, left);  
if(ret < 0)  
{  
if(errno == EINTR)        //遇到终端则继续  
continue;  
return -1;  
}  
else if(ret == 0)  
{  
return len - left;//对等方结束发送  
}  
left -= ret;  
pbuf += ret;  
}  
return len;  
}  
  
  
size_t writen(int fd, void *buf, size_t len)  
{  
size_t left = len;  
int ret;  
char *pbuf = (char *)buf;  
  
  
while(left > 0)  
{  
ret = write(fd, pbuf, left);  
if(ret < 0)  
{  
if (errno == EINTR)  
continue;  
return 1;  
}  
else if(ret == 0)  
{  
continue; // 没有写入，则继续  
}  
left -= ret;  
pbuf += ret;  
}  
return len;  
}  
