#include <signal.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/pipe.h>

#include <RtMacro.h>

RT_TASK rtReadTask;
RT_TASK rtWriteTask;

RT_PIPE rtPipe;

void terminationHandler(int signal)
{
  rt_pipe_delete(&rtPipe);
  exit(1);
}

void ReadRoutine(void*)
{
  void *buffer;
  buffer = malloc(RtMacro::kMessageSize);

  rt_printf("Reading from rtPipe\n");

  auto retval = rt_pipe_read(&rtPipe, buffer, RtMacro::kMessageSize, TM_INFINITE);
  if (retval < 0)
  {
    rt_printf("rt_pipe_read error: %s\n", strerror(-retval));
  }

  rt_printf("read pipe into buffer: %s\n", buffer);

  free(buffer);
}

int main(int argc, char *argv[])
{
  struct sigaction action;
  action.sa_handler = terminationHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);

  auto retval = rt_pipe_create(&rtPipe, "rtPipe", 0, RtMacro::kQueuePoolSize);
  if (retval < 0)
  {
    rt_printf("rt_pipe_create error: %s\n", strerror(-retval));
  }
  auto minor_number = retval;
  rt_printf("/dev/rtp%d pipe is opened\n", minor_number);

  rt_task_create(&rtReadTask, "rtReadTask", RtMacro::kStackSize,
    RtMacro::kPriority, T_JOINABLE);
  rt_task_start(&rtReadTask, ReadRoutine, NULL);

  rt_task_join(&rtReadTask);

  rt_pipe_delete(&rtPipe);

  return 0;
}
