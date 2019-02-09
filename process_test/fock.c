/*
    互斥量 实现 多进程 之间的同步 
*/
 
#include<unistd.h>
#include<sys/mman.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
 
struct mt
{
    int num;
    int sum;
    int current_num;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};
 
 
int main(void)
{
    
    int i;
    struct mt* mm;
 
    pid_t pid;
 
    // 建立映射区
    mm = mmap(NULL,sizeof(*mm),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
 
    //    printf("-------before memset------\n");
    memset(mm,0,sizeof(*mm));
    //   printf("-------after memset------\n");
 
    pthread_mutexattr_init(&mm->mutexattr);         // 初始化 mutex 属性
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);               // 修改属性为进程间共享
 
    pthread_mutex_init(&mm->mutex,&mm->mutexattr);      // 初始化一把 mutex 锁
    
    pid = fork();
    mm->num = 10;
    mm->sum = 0;
    mm->current_num = 1;
    if( pid == 0 )          // 子进程
    {
        for( i=0; i<mm->num;i++ )
        {
            pthread_mutex_lock(&mm->mutex);
            if(mm->current_num <= mm->num)
            {
                mm->sum += mm->current_num;
                mm->current_num ++;
                printf("-child--------------num++    %d\n",mm->sum);
            }
            pthread_mutex_unlock(&mm->mutex);
            // sleep(1);
        }
    
    }
    else 
    {
        for( i=0;i<mm->num;i++)
        {
            // sleep(1);
            pthread_mutex_lock(&mm->mutex);
            if(mm->current_num <= mm->num)
            {
                mm->sum += mm->current_num;
                mm->current_num ++;
                printf("-parent--------------num++    %d\n",mm->sum);
            }
            pthread_mutex_unlock(&mm->mutex);
        
        }
        wait(NULL);
 
    }
    pthread_mutexattr_destroy(&mm->mutexattr);  // 销毁 mutex 属性对象
    pthread_mutex_destroy(&mm->mutex);          // 销毁 mutex 锁
 
    return 0;
}
