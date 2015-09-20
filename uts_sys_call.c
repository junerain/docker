/*************************************************************************
    > File Name: uts_sys_call.c
    > Author: duanjun
    > Mail: duanjun@asiainfo.com 
    > Created Time: Sat 19 Sep 2015 05:44:24 AM EDT
 ************************************************************************/

#define _GNU_SOURCE
#include<stdio.h>

#include<sys/types.h>
#include<sys/wait.h>

#include<sched.h>
#include<signal.h>
#include<unistd.h>
/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024*1024)
static char container_stack[STACK_SIZE];

char * const container_args[] = {
	"/bin/bash",
	NULL
};

int container_main(void* args)
{
	printf("Container - inside the container!\n");
	sethostname("container1", 10);/*设置容器名称*/

	/* 直接执行一个shell，以便我们观察这个进程空间里的资源是否被隔离了 */
	execv(container_args[0],container_args);
	printf("Something is wrong !\n");
	return 1;
}

int main()
{
	printf("Parent - start a container!\n");
	/* 调用clone函数，其中传出一个函数，还有一个栈空间的（为什么传尾指针，因为栈是反着的） */
	int container_pid = clone(container_main, container_stack+STACK_SIZE, CLONE_NEWUTS|CLONE_NEWIPC|SIGCHLD, NULL);
	/* 等待子进程结束 */
	waitpid(container_pid, NULL, 0);
	printf("Parent - container stopped!\n");
	return 0;
}
