#include<stdio.h>
#include<sys/types.h>
#include <unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

int mysystem(char* cmdstring, char* buf, int len)
{
int   fd[2]; pid_t pid;
int   n, count;
memset(buf, 0, len);
if (pipe(fd) < 0)
return -1;
if ((pid = fork()) < 0)
return -1;
else if (pid > 0)     /* parent process */
{
close(fd[1]);     /* close write end */
count = 0;
while ((n = read(fd[0], buf + count, len)) > 0 && count > len)
count += n;
printf("%d\n",count);
printf("adf\n");
close(fd[0]);
if (waitpid(pid, NULL, 0) > 0)
return -1;
}
else    /* child process */
{
close(fd[0]);     /* close read end */
if (fd[1] != STDOUT_FILENO)
{
if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
{
return -1;
}
close(fd[1]);
}
if (execl("/usr/bin/pandoc", "pandoc", cmdstring, (char*)0) == -1)
return -1;
}
return 0;
}

int call_pandoc(char* filename, char* buf, int len)
  {
	  printf(filename);
	  printf("\n");
      int   fd[2];
      pid_t pid;
      int   n, count;
      memset(buf, 0, len);
	  printf("loop loop go!\n");
      if (pipe(fd) < 0)
          return -1;
      if ((pid = fork()) < 0)
          return -1;
      else if (pid > 0)
      {
		  printf("father process\n");
          close(fd[1]);
          count = 0;
        /*  while ((n =read(fd[0], buf + count, len)) > 0 && count > len){
              count += n;

			  printf(count);
			  printf("total\n");
		  }
          */
	  read(fd[0],buf,len);
		  close(fd[0]);
          if (waitpid(pid, NULL, 0) > 0)
              return -1;
      }
      else
      {
		  printf("child process\n");
          close(fd[0]);
		  printf("closed a channel\n");
         if (fd[1] != STDOUT_FILENO)
          {
			  printf("asd\n");
              if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
              {
		      printf("adsf\n");
                  return -1;
              }
	      printf("close fd1\n");
              close(fd[1]);
          }
	  
	  printf("asda\n");
	  if (execl("/bin/ls", "ls", filename, (char*)0) == -1){
              printf("reading finnished\n");
			  return -1;
		  }
      }
      
      return 0;
  }

int read_syscall(const char* filename, char* pResult, int size)
{
   int fd[2];
   if(pipe(fd))   {
      printf("pipe error!\n");
      return -1;
   }
   int bak_fd = dup(STDOUT_FILENO);
   int new_fd = dup2(fd[1], STDOUT_FILENO);
   char command[1024];
   snprintf(command,1024,"./%s",filename);
   //the output of `pCmd` is write into fd[1]
 //  int ret=execl("/bin/cat","cat", command, NULL); 
   	system("whoami");
	read(fd[0], pResult, size-1);
   	
	pResult[strlen(pResult)-1] = 0; //resume stdout
   	dup2(bak_fd, new_fd);
	printf("asdfdsafs\n");
	return 0;
   
   //dup2(bak_fd, new_fd);
   //return -1;
}


int main(){
	char buf[10240];
	memset(buf,0,10240);
	mysystem("test.md",buf,10240);
	//call_pandoc("test.md",buf,10230);
	//printf(buf);
	return 0;
}

