#include <signal.h>
#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/queue.h>
#include <alchemy/heap.h>

RT_TASK rtTaskOne;
RT_TASK rtTaskTwo;

RT_QUEUE rtQueue;
RT_QUEUE_INFO rtQueueInfo;

RT_HEAP rtHeap;

auto constexpr kStackSize = 0;
auto constexpr kPriority = 99;
auto constexpr kMode = 0;
auto constexpr kOneSecond = 1e9;
auto constexpr kOneHundredMilliseconds = 1e8;
auto constexpr kOneMillisecond = 1e6;
auto constexpr kQueuePoolSize = 4096u;
auto constexpr kQueueMessageLimit = 10u;
auto constexpr kMessageSize = 256u;

void TaskOneRoutine(void*)
{
  for (;;)
  {
    rt_printf("TaskOne: sending hi to rtQueue\n\n");
    void *message = rt_queue_alloc(&rtQueue, kMessageSize);
    if (message == NULL)
      rt_printf("rt_queue_alloc error\n");
    rt_queue_inquire(&rtQueue, &rtQueueInfo);
    char sendData[] = "yo";
    memcpy(message, sendData, sizeof(char) * kMessageSize);

    rt_queue_send(&rtQueue, message, sizeof(char) * kMessageSize, Q_NORMAL);
    rt_task_wait_period(NULL);
  }
}

void TaskTwoRoutine(void*)
{
  for (;;)
  {
    auto e1 = rt_queue_inquire(&rtQueue, &rtQueueInfo);
    if (e1)
      rt_printf("rt_queue_inquire error\n");

    void **blockPointer;
    auto retval = rt_heap_alloc(&rtHeap, kQueuePoolSize, TM_NONBLOCK, blockPointer);
    if (retval < 0)
      rt_printf("rt_heap_alloc error: %s\n", strerror(-retval));

    auto bytesRead = rt_queue_read(&rtQueue, blockPointer, kQueuePoolSize, TM_NONBLOCK);
    if (bytesRead >= 0)
    {
      rt_printf("TaskTwo bytesRead: %ld\n", bytesRead);
      rt_printf("TaskTwo received: %s\n", blockPointer);
      rt_printf("\n");
    }

    auto e3 = rt_task_wait_period(NULL);
    if (e3)
      printf("rt_task_wait_period error: %d\n", e3);
  }
}

void terminationHandler(int signal)
{
  printf("Caught ctrl + c termination signal. Exiting.\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  mlockall(MCL_CURRENT|MCL_FUTURE);

  struct sigaction action;
  action.sa_handler = terminationHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);

  rt_queue_create(&rtQueue, "rtQueue", kQueuePoolSize, kQueueMessageLimit, Q_FIFO);

  // allocate heap for storing messages since -pshared is enabled
  rt_heap_create(&rtHeap, "rtHeap", kQueuePoolSize, H_SINGLE);

  rt_task_create(&rtTaskOne, "rtTaskOne", kStackSize, kPriority, kMode);
  rt_task_set_periodic(&rtTaskOne, TM_NOW, rt_timer_ns2ticks(kOneSecond));
  rt_task_start(&rtTaskOne, TaskOneRoutine, NULL);

  rt_task_create(&rtTaskTwo, "rtTaskTwo", kStackSize, kPriority, kMode);
  rt_task_set_periodic(&rtTaskTwo, TM_NOW, rt_timer_ns2ticks(kOneHundredMilliseconds));
  rt_task_start(&rtTaskTwo, TaskTwoRoutine, NULL);

  for (;;)
  {}

  return 0;
}
