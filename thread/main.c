#include <stdio.h>
#include <stdlib.h>
#include <time.h>       //时间计数1
#include<sys/time.h> //时间计数2
#include <pthread.h>

// #define CLOCKS_PER_SEC ((clock_t)1000)

int current_num = 1;  //当前的数
int num = 0; //需要加到多少
long sum = 0; //结果
void increse_num();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    int thrd_num = 0;    //线程数量,创建数组
    char str[4][10];
    ////////////////////////////
    clock_t start, finish;
    double  duration;
    ////////////////////////////
     //定义两个结构体，来记录开始和结束时间
    // struct  timeval start;
    // struct  timeval end;
    // //记录两个时间差
    // unsigned  long duration;
    ////////////////////////////////

    start = clock();
    // gettimeofday(&start,NULL);
    //读取文件
    FILE *fpRead=fopen("input/input.txt","r");  //怎么改成相对路径
    if(fpRead==NULL)
    {  
        printf("读取无效文件");
        return 0;
    }else
    {
        fscanf(fpRead,"%s %s %d %s %s %d",str[1],str[2],&thrd_num,str[3],str[4],&num);
        // fscanf(fpRead,"%d ",&num);
    }
    fclose(fpRead);
    printf("线程数量%d\n需要加到多少%d\n", thrd_num,num);

    //创建线程
    pthread_t thrd[thrd_num];
    int j;
    for(j = 1; j < thrd_num; j++) {
        pthread_create(&thrd[j], NULL, (void *)increse_num, NULL);
    }
    //等待线程执行结束
    for(j = 1; j < thrd_num; j++) {
        pthread_join(thrd[j], NULL);
    }
    printf("sum = %ld\n", sum);
    //将结果写入文件
    FILE *fpWrite=fopen("output/output.txt","w");    //怎么改成相对路径
    if(fpWrite == NULL)
    {
        printf("输出无效文件\n");
        return 0;
    }else
    {
        fprintf(fpWrite,"%ld",sum);
    }
    fclose(fpWrite);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    // gettimeofday(&end,NULL);
    // duration = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf( "程序耗时：%f seconds\n", duration );
    return 0;

}

void increse_num() {
    int i;
    for(i=1; i<=num; i++) {
        /*加锁*/
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
            if(current_num <= num){         //对读操作也要上锁，不然也会造成一定的问题
                sum = sum + current_num;
                current_num++;
            }
        /*解锁*/
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("pthread_mutex_unlock");
            exit(EXIT_FAILURE);
        }
    }
}