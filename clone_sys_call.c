/*************************************************************************
    > File Name: clone_sys_call.c
    > Author: duanjun
    > Mail: duanjun@asiainfo.com 
    > Created Time: Sat 19 Sep 2015 04:44:21 AM EDT
	> Description:docker的容器clone系统调用case
 ************************************************************************/
#define _GNU_SOURCE
#include<stdio.h>

#include<sys/types.h>
#include<sys/wait.h>

#include<sched.h>
#include<signal.h>
#include<unistd.h>
#define STACK_SIZE (1024*1024)
static char container_stack[STACK_SIZE];

char * const container_args[] = {
	"/bin/bash",
	NULL
};

int container_main(void* args)
{
	printf("Container - inside the container!\n");
	execv(container_args[0],container_args);
	printf("Something is wrong !\n");
	return 1;
}

int main()
{
	printf("Parent - start a container!\n");
	int container_pid = clone(container_main, container_stack+STACK_SIZE, SIGCHLD, NULL);
	waitpid(container_pid, NULL, 0);
	printf("Parent - container stopped!\n");
	return 0;
}


