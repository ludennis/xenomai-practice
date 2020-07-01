#include <alchemy/task.h>

#include <RtMacro.h>

RT_TASK rtReceiveTask;

int main(int argc, char *argv[])
{
  rt_task_create(&rtReceiveTask, "rtReceiveTask", RtMacro::kStackSize,
    RtMacro::kPriority, RtMacro::kMode);
  return 0;
}
