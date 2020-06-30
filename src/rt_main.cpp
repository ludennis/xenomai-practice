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

int main(int argc, char *argv[])
{
  rt_task_create(&rtTask, "RtTask", kStackSize, kPriority, kMode);
  rt_task_set_periodic(&rtTask, TM_NOW, rt_timer_ns2ticks(kOneSecond));
  rt_task_start(&rtTask, Routine, NULL);

  for (;;)
  {}

  return 0;
}
