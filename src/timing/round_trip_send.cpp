#include <signal.h>
#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/timer.h>

#include <RtMacro.h>

RT_TASK rtSendTask;
RT_TASK rtReceiveTask;

RT_TASK_MCB receiveMessage;
RT_TASK_MCB sendMessage;

void SendRoutine(void*)
{
  auto retval = rt_task_bind(&rtReceiveTask, "rtReceiveTask", TM_INFINITE);
  if (retval < 0)
  {
    rt_printf("rt_task_bind error: %s\n", strerror(-retval));
  }

  rt_printf("Found receiver\n");

  sendMessage.data = (char*) malloc(RtMacro::kMessageSize);

  receiveMessage.data = (char*) malloc(RtMacro::kMessageSize);
  receiveMessage.size = RtMacro::kMessageSize;

  rt_printf("sending\n");

  for (;;)
  {
    const char sendData[] = "ping";
    memcpy(sendMessage.data, sendData, RtMacro::kMessageSize);

    RTIME rtTimer = rt_timer_read();
    auto retval = rt_task_send(&rtReceiveTask, &sendMessage, &receiveMessage, TM_INFINITE);
    rt_printf("send time elpased: %d ns\n", rt_timer_read() - rtTimer);
    if (retval < 0)
    {
      rt_printf("rt_task_send error: %s\n", strerror(-retval));
    }
    else
    {
      rt_printf("received reply: %s\n", receiveMessage.data);
    }

    rt_task_wait_period(NULL);
  }
}

void terminationHandler(int signal)
{
  free(receiveMessage.data);
  free(sendMessage.data);
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

  rt_task_create(&rtSendTask, "rtSendTask", RtMacro::kStackSize,
    RtMacro::kPriority, RtMacro::kMode);
  rt_task_set_periodic(&rtSendTask, TM_NOW,
    rt_timer_ns2ticks(RtMacro::kOneHundredMilliseconds));
  rt_task_start(&rtSendTask, SendRoutine, NULL);

  for (;;)
  {}

  return 0;
}
