#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct mt
{
    int num;            //需要加到多少
    int sum;            //结果
    int current_num;    //当前的数
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};

int increse();

int main(){
    clock_t start, finish;
    double  duration;

    start = clock();
    pid_t id = fork();
    if(id == 0)
    {
        increse();
    }
    else
    {
        wait(NULL);
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf( "程序耗时：%f seconds\n", duration );

        //输出时间用于测算耗时情况
        // FILE *fpAddTime = fopen("time.txt","a");
        // if(fpAddTime == NULL)
        // {
        //     printf("定向输出时间无效\n");
        //     return 0;
        // }else
        // {
        //     // fprintf(fpAddTime,"%d   ",thrd_num);
        //     // fprintf(fpAddTime,"%d   ",num);
        //     fprintf(fpAddTime,"%f\n",duration*1000);
        // }
        // fclose(fpAddTime);
    }
    
    return 0;
}
 
int increse(){
    int process_num = 0;    //进程数量,创建数组
    char str[1][10];        //读取文件暂存数组

    int i;
    struct mt* mm;

    // 建立映射区
    mm = mmap(NULL,sizeof(*mm),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);

    //    printf("-------before memset------\n");
    memset(mm,0,sizeof(*mm));
    //   printf("-------after memset------\n");

    pthread_mutexattr_init(&mm->mutexattr);         // 初始化 mutex 属性
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);  // 修改属性为进程间共享

    pthread_mutex_init(&mm->mutex,&mm->mutexattr);      // 初始化一把 mutex 锁

    //读取文件
    FILE *fpRead=fopen("input.txt","r");  //怎么改成相对路径
    if(fpRead==NULL)
    {  
        printf("读取无效文件");
        return 0;
    }else
    {
        fscanf(fpRead,"%s %s",str[0],str[1]);
        process_num = atoi(&str[0][2]);
        mm->num = atoi(&str[1][2]);
        mm->sum = 0;
        mm->current_num = 1;
    }
    fclose(fpRead);
    printf("进程数量：%d\n累加数：%d\n", process_num,mm->num);

    //创建进程
    int j;
    pid_t status;
    for (j = 0; j < process_num-1; j++)
    {
        status = fork();
        if (status == 0 || status == -1) break;//每次循环时，假设发现是子进程就直接从创建子进程的循环中跳出来。不让你进入循环，这样就保证了每次仅仅有父进程来做循环创建子进程的工作
    }
    if (status == -1)
    {
        printf("创建进程无效\n");
    }
    for( i=0; i<mm->num;i++ )
        {
            pthread_mutex_lock(&mm->mutex);
            if(mm->current_num <= mm->num)
            {
                mm->sum += mm->current_num;
                mm->current_num ++;
                // printf("-child--------------sum    %d\n",mm->sum);
            }
            pthread_mutex_unlock(&mm->mutex);
            // sleep(1);
        }
    // else if (status == 0) //每一个子进程都会运行的代码
    // {
    //     //sub process
    //     for( i=0; i<mm->num;i++ )
    //     {
    //         pthread_mutex_lock(&mm->mutex);
    //         if(mm->current_num <= mm->num)
    //         {
    //             mm->sum += mm->current_num;
    //             mm->current_num ++;
    //             // printf("-child--------------sum    %d\n",mm->sum);
    //         }
    //         pthread_mutex_unlock(&mm->mutex);
    //         // sleep(1);
    //     }
    //     pthread_mutex_unlock(&mm->mutex);
    // }
    // else
    // {
    //     //parent process
    //     for( i=0;i<mm->num;i++)
    //     {
    //         // sleep(1);
    //         pthread_mutex_lock(&mm->mutex);
    //         if(mm->current_num <= mm->num)
    //         {
    //             mm->sum += mm->current_num;
    //             mm->current_num ++;
    //             // printf("-parent--------------sum    %d\n",mm->sum);
    //         }
    //         pthread_mutex_unlock(&mm->mutex);
        
    //     }
    //     wait(NULL);
    // }

    pthread_mutexattr_destroy(&mm->mutexattr);  // 销毁 mutex 属性对象
    pthread_mutex_destroy(&mm->mutex);          // 销毁 mutex 锁

    if(status == 0)
    {
        //sub
    }else
    {
        //parent
        //将结果写入文件
        printf("累加结果为：%ld\n",mm->sum);
        FILE *fpWrite=fopen("output.txt","w");    //怎么改成相对路径
        if(fpWrite == NULL)
        {
            printf("输出无效文件\n");
            return 0;
        }else
        {
            fprintf(fpWrite,"%ld",mm->sum);
        }
        fclose(fpWrite);
    }
    
    return 0;

}