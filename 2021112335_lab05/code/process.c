#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>

#define HZ  100

#define THREAD_NO   (9)

struct arg_struct {
  int last;
  int cpu_time;
  int io_time;
} args[THREAD_NO] = {
  { 30, 1, 9 },
  { 30, 2, 8 },
  { 30, 3, 7 },
  { 30, 4, 6 },
  { 30, 5, 5 },
  { 30, 6, 4 },
  { 30, 7, 3 },
  { 30, 8, 2 },
  { 30, 9, 1 },
};


void cpuio_bound(int last, int cpu_time, int io_time);

int main(int argc, char * argv[]) {
  int pids[THREAD_NO];
  int i = 0;

  for (i = 0; i < THREAD_NO; ++i) {
    int pid = fork();
    if (pid == 0) {
      cpuio_bound(args[i].last, args[i].cpu_time, args[i].io_time);
      return 0;
    }
    pids[i] = pid;
  }

  for (i = 0; i < THREAD_NO; ++i) {
    while (waitpid(pids[i], NULL, WUNTRACED) != pids[i])
      continue;
    printf("process %5d exited\n", pids[i]);
  }

  return 0;
}

/*
 * 此函数按照参数占用CPU和I/O时间
 * last: 函数实际占用CPU和I/O的总时间，不含在就绪队列中的时间，>=0是必须的
 * cpu_time: 一次连续占用CPU的时间，>=0是必须的
 * io_time: 一次I/O消耗的时间，>=0是必须的
 * 如果last > cpu_time + io_time，则往复多次占用CPU和I/O
 * 所有时间的单位为秒
 */
void cpuio_bound(int last, int cpu_time, int io_time) {
  struct tms start_time, current_time;
  clock_t utime, stime;
  int sleep_time;

  while (last > 0) {
    /* CPU Burst */
    times(&start_time);
    /* 其实只有t.tms_utime才是真正的CPU时间。但我们是在模拟一个
     * 只在用户状态运行的CPU大户，就像“for(;;);”。所以把t.tms_stime
     * 加上很合理。*/
    do {
      times(&current_time);
      utime = current_time.tms_utime - start_time.tms_utime;
      stime = current_time.tms_stime - start_time.tms_stime;
    } while ( ( (utime + stime) / HZ )  < cpu_time );
    last -= cpu_time;

    if (last <= 0 )
      break;

    /* IO Burst */
    /* 用sleep(1)模拟1秒钟的I/O操作 */
    sleep_time = 0;
    while (sleep_time < io_time) {
      sleep(1);
      sleep_time++;
    }
    last -= sleep_time;
  }
}
