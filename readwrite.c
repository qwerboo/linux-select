#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#define MAXLEN 1024 
#include "readwrie.h"
  
ssize_t readn(int fd, void *buf, size_t len)  
{  
size_t left = len;  
ssize_t nread;  
char *pbuf = (char *)buf;  
  
  
while(left > 0)  
{  
nread = read(fd, pbuf, left);  
if(nread < 0)  
{  
if(errno == EINTR)        //遇到终端则继续  
continue;  
return -1;  
}  
else if(nread == 0)  
{  
break;//对等方结束发送  
}  
left -= nread;  
pbuf += nread;  
}  
return len - left;  
}  
  
  
ssize_t writen(int fd, void *buf, size_t len)  
{  
size_t left = len;  
ssize_t nwrite;  
char *pbuf = (char *)buf;  
  
  
while(left > 0)  
{  
nwrite = write(fd, pbuf, left);  
if(nwrite <= 0)  
{  
if (nwrite < 0 && errno == EINTR)  
nwrite = 0;  
else 
return -1;  
}
left -= nwrite;  
pbuf += nwrite;  
}  
return len;  
}  
