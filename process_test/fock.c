// /*
//     互斥量 实现 多进程 之间的同步 
// */
 
// #include<unistd.h>
// #include<sys/mman.h>
// #include<pthread.h>
// #include<sys/types.h>
// #include<sys/wait.h>
// #include<fcntl.h>
// #include<string.h>
// #include<stdlib.h>
// #include<stdio.h>
 
// struct mt
// {
//     int num;
//     int sum;
//     int current_num;
//     pthread_mutex_t mutex;
//     pthread_mutexattr_t mutexattr;
// };
 
 
// int main(void)
// {
    
//     int i;
//     struct mt* mm;
 
//     pid_t pid;
 
//     // 建立映射区
//     mm = mmap(NULL,sizeof(*mm),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
 
//     //    printf("-------before memset------\n");
//     memset(mm,0,sizeof(*mm));
//     //   printf("-------after memset------\n");
 
//     pthread_mutexattr_init(&mm->mutexattr);         // 初始化 mutex 属性
//     pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);               // 修改属性为进程间共享
 
//     pthread_mutex_init(&mm->mutex,&mm->mutexattr);      // 初始化一把 mutex 锁
    
//     pid = fork();
//     mm->num = 10;
//     mm->sum = 0;
//     mm->current_num = 1;
//     if( pid == 0 )          // 子进程
//     {
//         for( i=0; i<mm->num;i++ )
//         {
//             pthread_mutex_lock(&mm->mutex);
//             if(mm->current_num <= mm->num)
//             {
//                 mm->sum += mm->current_num;
//                 mm->current_num ++;
//                 printf("-child--------------num++    %d\n",mm->sum);
//             }
//             pthread_mutex_unlock(&mm->mutex);
//             // sleep(1);
//         }
    
//     }
//     else 
//     {
//         for( i=0;i<mm->num;i++)
//         {
//             // sleep(1);
//             pthread_mutex_lock(&mm->mutex);
//             if(mm->current_num <= mm->num)
//             {
//                 mm->sum += mm->current_num;
//                 mm->current_num ++;
//                 printf("-parent--------------num++    %d\n",mm->sum);
//             }
//             pthread_mutex_unlock(&mm->mutex);
        
//         }
//         wait(NULL);
 
//     }
//     pthread_mutexattr_destroy(&mm->mutexattr);  // 销毁 mutex 属性对象
//     pthread_mutex_destroy(&mm->mutex);          // 销毁 mutex 锁
 
//     return 0;
// }


/*--------------------------fork.c------------------------------*/ 
/* mikespook */ 
/* exercise function fork() and wait()*/ 
/* 2002.5.28 */ 

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#define FAC_N 100

/* 子进程调用的函数，这里我为了模拟一个很大的后台操作用了一个循环。 */ 
void big_loop(int n); 
/* 父进程调用的函数，其实不放到函数里也可以，不过为了程序的结构更好看还是放到函数里的好 */ 
void input_information(); 

int main() 
{ 
  /* 进程号 */ 
  pid_t pid =1; 
  /* 程序在这里“分叉”，新的进程创建了 */ 
  for(int k=0;k<2;k++)
  {
  /* 通过fork()的返回值来判断是父进程还是子进程 */ 
  switch(pid){ 
    /* 返回 -1，很不幸，创建进程失败了。可能是没有足够的内存空间，也可能已经开起了太多的进程。 */ 
    case -1: 
      perror("fork\n"); 
      break; 
    /* 返回 0，现在是在子进程里运行，那就调用子进程的操作函数。 */ 
    case 0: 
      /* 一个运行65535次的循环，如果你的机子太快，不能看清楚两个进程同时运行的效果，那就再加大循环次数。或用sleep()函数 */ 
    //   big_loop(FAC_N); 
    printf("sub\n");
      /* 取得子进程的PID，你可以看清楚子进程和父进程的PID是不同的（子进程的PID比父进程的要大，因为是在父进程运行后才创建的）。*/ 
      printf("PID:%d\n", getpid()); 
      break; 
    /* 哈哈，返回的即不是错误，又不是子进程，那就是父进程喽。*/ 
    default: 
      /* 等着吧，子进程不退出，你父进程也不能退出的。 */    
        // wait(NULL); 
        printf("fock_begin\n");
        pid = fork(); 
        wait(NULL);
        printf("fock_finish\n");
        printf("parent\n");
      /* 这里让用户输入了4个数 */ 
    //   input_information(); 
      /* 取得子进程的PID。*/ 
      printf("PID:%d\n", getpid()); 
      break; 
  }
  } 
      if(pid == -1)
    {
        printf("___________创建进程无效\n");
    }else if(pid == 0)
    {
        //sub
        printf("___________我是sub\n");
    }else
    {
        //parent
        wait(NULL);
        printf("___________我是parent\n");
    } 
    // wait(NULL);
//   exit(0); 
    return 0;
} 

/*big_loop: 简单，一看就明白，不解释了。*/ 
void big_loop(int n) 
{ 
  int i; 
  for(i = 0; i < n; i++){ 
    switch(i % 4){ 
      case 0: 
        putchar('-'); 
        break; 
      case 1: 
        putchar('/'); 
        break; 
      case 2: 
        putchar('|'); 
        break; 
      case 3: 
        putchar('\\'); 
        break; 
    } 
    putchar('\n'); 
  } 
  wait(NULL);
} 

/*input_information: 简单，一看就明白，也不解释了。*/ 
void input_information() 
{ 
  int n_table[4], i; 

  for(i = 0; i < 4; i++){ 
    printf("Number %d:\t", i); 
    scanf("%d", &n_table[i]); 
  } 

   printf("Number1\tNumber2\tNumber3\tNumber4\n"); 
   printf("%d\t%d\t%d\t%d\n", n_table[0], n_table[1], n_table[2], n_table[3]); 
} 
/*--------------------------fork.c------------------------------*/ 