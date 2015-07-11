#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#define MAXLEN 1024 

struct packet  
{  
int len;  
char content[MAXLEN];  
};  

size_t readn(int fd, void *buf, size_t len);
size_t writen(int fd, void *buf, size_t len);
