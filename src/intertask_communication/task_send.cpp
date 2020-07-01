#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>

#include <RtMacro.h>

RT_TASK rtSendTask;
RT_TASK rtReceiveTask;

RT_TASK_MCB receiveMessage;

void SendRoutine(void*)
{
  rt_task_bind(&rtReceiveTask, "rtReceiveTask", TM_INFINITE);

  RT_TASK_MCB sendMessage;

  sendMessage.data = (char*) malloc(RtMacro::kMessageSize);
  char sendData[] = "hello";
  memcpy(sendMessage.data, sendData, sizeof(char) * RtMacro::kMessageSize);
  sendMessage.size = RtMacro::kMessageSize;

  receiveMessage.data = (char*) malloc(RtMacro::kMessageSize);
  receiveMessage.size = RtMacro::kMessageSize;

  auto retval = rt_task_send(&rtReceiveTask, &sendMessage, &receiveMessage, TM_INFINITE);

  if (retval < 0)
  {
    rt_printf("Error with rt_task_send: %s\n", strerror(-retval));
  }

  rt_printf("Received reply: %s\n", receiveMessage.data);

  free(receiveMessage.data);
  free(sendMessage.data);
}

int main(int argc, char *argv[])
{
  mlockall(MCL_CURRENT|MCL_FUTURE);

  rt_task_create(&rtSendTask, "rtSendTask", RtMacro::kStackSize,
    RtMacro::kPriority, T_JOINABLE);
  rt_task_start(&rtSendTask, SendRoutine, NULL);

  rt_task_join(&rtSendTask);

  return 0;
}
