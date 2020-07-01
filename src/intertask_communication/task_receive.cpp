#include <sys/mman.h>

#include <iostream>

#include <alchemy/task.h>

#include <RtMacro.h>

RT_TASK rtReceiveTask;
RT_TASK rtSendTask;

void ReceiveRoutine(void*)
{
  rt_printf("Waiting for messages\n");

  for (;;)
  {
    RT_TASK_MCB receiveMessage;

    receiveMessage.data = (char*) malloc(RtMacro::kMessageSize);

    RT_TASK_MCB sendMessage;

    sendMessage.data = (char*) malloc(RtMacro::kMessageSize);
    char sendData[] = "ack";
    memcpy(sendMessage.data, sendData, sizeof(char) * RtMacro::kMessageSize);
    sendMessage.size = RtMacro::kMessageSize;

    auto retval = rt_task_receive(&receiveMessage, TM_INFINITE);
    if (retval < 0)
    {
      rt_printf("Error with rt_task_receive: %s\n", strerror(-retval));
    }
    auto flowid = retval;
    rt_printf("Flow id of received message: %d\n", flowid);
    rt_printf("Size of received message: %d\n", receiveMessage.size);

    retval = rt_task_reply(flowid, &sendMessage);
    if (retval < 0)
    {
      rt_printf("Error with rt_task_reply: %s\n", strerror(-retval));
    }

    rt_printf("message received: %s\n", receiveMessage.data);
    rt_printf("Sending back reply: %s\n", sendMessage.data);


    rt_task_bind(&rtSendTask, "rtSendTask", TM_INFINITE);
    rt_task_join(&rtSendTask);

    rt_task_wait_period(NULL);

    free(receiveMessage.data);
    free(sendMessage.data);
  }
}

int main(int argc, char *argv[])
{
  mlockall(MCL_CURRENT|MCL_FUTURE);

  rt_task_create(&rtReceiveTask, "rtReceiveTask", RtMacro::kStackSize,
    RtMacro::kPriority, T_JOINABLE);
  rt_task_set_periodic(&rtReceiveTask, TM_NOW,
    rt_timer_ns2ticks(RtMacro::kOneSecond));
  rt_task_start(&rtReceiveTask, ReceiveRoutine, NULL);

  rt_task_join(&rtReceiveTask);

  for (;;)
  {}

  return 0;
}
