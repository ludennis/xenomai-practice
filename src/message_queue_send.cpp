#include <signal.h>
#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/queue.h>

RT_TASK rtSendTask;

RT_QUEUE rtQueue;

RT_QUEUE_INFO rtQueueInfo;

auto constexpr kStackSize = 0;
auto constexpr kPriority = 99;
auto constexpr kMode = 0;
auto constexpr kOneSecond = 1e9;
auto constexpr kOneHundredMilliseconds = 1e8;
auto constexpr kOneMillisecond = 1e6;
auto constexpr kQueuePoolSize = 4096u;
auto constexpr kQueueMessageLimit = 10u;
auto constexpr kMessageSize = 256u;

void SendRoutine(void*)
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

void terminationHandler(int signal)
{
  rt_printf("Caught ctrl + c termination signal, Exiting.\n");
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

  rt_printf("Getting queue descripter with name 'rtQueue'\n");

  // bind the queue by name (wait forever)
  auto retval = rt_queue_bind(&rtQueue, "rtQueue", TM_INFINITE);

  if (retval != 0)
  {
    rt_printf("queue not found, exiting\n");
    rt_printf("Error: %s\n", strerror(-retval));
    return -1;
  }

  rt_printf("Queue found!\n");

  rt_task_create(&rtSendTask, "rtSendTask", kStackSize, kPriority, kMode);
  rt_task_set_periodic(&rtSendTask, TM_NOW, rt_timer_ns2ticks(kOneSecond));
  rt_task_start(&rtSendTask, SendRoutine, NULL);

  for (;;)
  {}

  return 0;
}
