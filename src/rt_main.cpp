#include <signal.h>

#include <iostream>

#include <alchemy/task.h>

RT_TASK rtTask;

auto constexpr kStackSize = 0;
auto constexpr kPriority = 99;
auto constexpr kMode = 0;
auto constexpr kOneSecond = 1000000000.0;

void Routine(void*)
{
  for (;;)
  {
    printf("hi\n");
    rt_task_wait_period(NULL);
  }
}

void terminationHandler(int signal)
{
  printf("Caught ctrl + c termination signal. Exiting.\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  rt_task_create(&rtTask, "RtTask", kStackSize, kPriority, kMode);
  rt_task_set_periodic(&rtTask, TM_NOW, rt_timer_ns2ticks(kOneSecond));
  rt_task_start(&rtTask, Routine, NULL);

  struct sigaction action;
  action.sa_handler = terminationHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);

  for (;;)
  {}

  return 0;
}
