#include <signal.h>
#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/queue.h>
#include <alchemy/heap.h>

#include <RtMacro.h>

RT_TASK rtReceiveTask;

RT_QUEUE rtQueue;
RT_QUEUE bindingRtQueue;

RT_QUEUE_INFO rtQueueInfo;

RT_HEAP rtHeap;

void ReceiveRoutine(void*)
{
  rt_queue_create(&rtQueue, "rtQueue", RtMacro::kQueuePoolSize,
    RtMacro::kQueueMessageLimit, Q_FIFO);
  rt_queue_inquire(&rtQueue, &rtQueueInfo);

  rt_printf("queue, %s, created\n", rtQueueInfo.name);

  // allocate heap for storing messages since -pshared is enabled
  rt_heap_create(&rtHeap, "rtHeap", RtMacro::kQueuePoolSize, H_SINGLE);

  for (;;)
  {
    rt_queue_inquire(&rtQueue, &rtQueueInfo);
    if (rtQueueInfo.nmessages > 0)
    {
      void *blockPointer;
      auto retval = rt_heap_alloc(&rtHeap, RtMacro::kQueuePoolSize, TM_NONBLOCK, &blockPointer);
      if (retval < 0)
        rt_printf("rt_heap_alloc error: %s\n", strerror(-retval));

      auto bytesRead = rt_queue_read(&rtQueue, blockPointer,
        RtMacro::kQueuePoolSize, TM_NONBLOCK);

      char* message = (char*) malloc(sizeof(blockPointer));
      memcpy(message, blockPointer, sizeof(blockPointer));
      rt_heap_free(&rtHeap, blockPointer);

      if (bytesRead >= 0)
      {
        rt_printf("TaskTwo bytesRead: %ld\n", bytesRead);
        rt_printf("TaskTwo received: %s\n", message);
        rt_printf("\n");
      }

    }

    auto e3 = rt_task_wait_period(NULL);
    if (e3)
      printf("rt_task_wait_period error: %d\n", e3);
  }
}

void terminationHandler(int signal)
{
  rt_heap_delete(&rtHeap);
  rt_queue_delete(&rtQueue);
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

  rt_task_create(&rtReceiveTask, "rtReceiveTask", RtMacro::kStackSize,
    RtMacro::kPriority, RtMacro::kMode);
  rt_task_set_periodic(&rtReceiveTask, TM_NOW,
    rt_timer_ns2ticks(RtMacro::kOneHundredMilliseconds));
  rt_task_start(&rtReceiveTask, ReceiveRoutine, NULL);

  for (;;)
  {}

  return 0;
}
