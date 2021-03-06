#include <stdio.h>
#include <stdlib.h>
#include <time.h>     
#include <pthread.h>

int current_num = 1;  //当前的数
int num = 0; //需要加到多少
long sum = 0; //结果
void increse_num();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    int thrd_num = 0;    //线程数量,创建数组
    char str[1][10];
    clock_t start, finish;
    double  duration;

    start = clock();
    //读取文件
    FILE *fpRead=fopen("input.txt","r");  //怎么改成相对路径
    if(fpRead==NULL)
    {  
        printf("读取无效文件");
        return 0;
    }else
    {
        fscanf(fpRead,"%s %s",str[0],str[1]);
        thrd_num = atoi(&str[0][2]);
        num = atoi(&str[1][2]);
    }
    fclose(fpRead);
    printf("线程数量：%d\n累加数：%d\n", thrd_num,num);

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
    printf("累加结果: %ld\n", sum);
    //将结果写入文件
    FILE *fpWrite=fopen("output.txt","w");    //怎么改成相对路径
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
    printf( "程序耗时：%f mseconds\n", duration*1000 );

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