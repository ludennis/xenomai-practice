#include <signal.h>
#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/queue.h>

#include <RtMacro.h>
#include <MotorMessage.h>

RT_TASK rtSendTask;

RT_QUEUE rtQueue;

void SendRoutine(void*)
{
  for (;;)
  {
    rt_printf("TaskOne: sending hi to rtQueue\n\n");

    void *message = rt_queue_alloc(&rtQueue, sizeof(MotorMessage));
    if (message == NULL)
      rt_printf("rt_queue_alloc error\n");
    char *sendData = (char*) malloc(sizeof(MotorMessage));
    MotorMessage *motorMessageData = new MotorMessage{5000, 1, 0, 10, 20};
    memcpy(message, motorMessageData, sizeof(MotorMessage));

    auto retval = rt_queue_send(&rtQueue, message, sizeof(MotorMessage), Q_NORMAL);
    if (retval < 0)
    {
      rt_printf("rt_queue_send error: %s\n", strerror(-retval));
    }

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

  rt_task_create(&rtSendTask, "rtSendTask", RtMacro::kStackSize,
    RtMacro::kPriority, RtMacro::kMode);
  rt_task_set_periodic(&rtSendTask, TM_NOW, rt_timer_ns2ticks(RtMacro::kOneSecond));
  rt_task_start(&rtSendTask, SendRoutine, NULL);

  for (;;)
  {}

  return 0;
}
