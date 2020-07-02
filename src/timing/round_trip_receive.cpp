#include <signal.h>
#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>

#include <RtMacro.h>

RT_TASK rtReceiveTask;
RT_TASK rtSendTask;

RT_TASK_MCB receiveMessage;
RT_TASK_MCB sendMessage;

void ReceiveRoutine(void*)
{
  receiveMessage.data = (char*) malloc(RtMacro::kMessageSize);
  receiveMessage.size = RtMacro::kMessageSize;

  sendMessage.data = (char*) malloc(RtMacro::kMessageSize);
  sendMessage.size = RtMacro::kMessageSize;
  const char sendData[] = "ack";
  memcpy(sendMessage.data, sendData, RtMacro::kMessageSize);

  rt_printf("Receiving ...\n");

  for (;;)
  {
    auto retval = rt_task_receive(&receiveMessage, TM_INFINITE);
    auto flowid = retval;
    if (flowid < 0)
    {
      rt_printf("rt_task_receive error: %s\n", strerror(-retval));
    }
    else
    {
      rt_printf("received: %s\n", receiveMessage.data);
    }

    rt_task_reply(flowid, &sendMessage);
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

  rt_task_create(&rtReceiveTask, "rtReceiveTask", RtMacro::kStackSize,
    RtMacro::kPriority, T_JOINABLE);
  rt_task_start(&rtReceiveTask, ReceiveRoutine, NULL);

  rt_task_join(&rtReceiveTask);

  return 0;
}
