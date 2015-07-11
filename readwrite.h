#include <unistd.h>  
#include <stdio.h>  
#define MAXLEN 1024 

struct packet  
{  
int len;  
char content[MAXLEN];  
};  

size_t readn(int fd, void *buf, size_t len);
size_t writen(int fd, void *buf, size_t len);
