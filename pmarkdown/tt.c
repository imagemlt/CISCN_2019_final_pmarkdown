#include<sys/types.h>
#include <unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(){
if (execl("/usr/bin/pandoc", "pandoc", "test.md", (char*)0) == -1){
              printf("reading finnished\n");
                          return -1;
                  }
	printf("123421\n");
      }
