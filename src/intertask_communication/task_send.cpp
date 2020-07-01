#include <sys/mman.h>

#include <alchemy/task.h>

#include <RtMacro.h>

RT_TASK rtSendTask;

int main(int argc, char *argv[])
{
  mlockall(MCL_CURRENT|MCL_FUTURE);

  rt_task_create(&rtSendTask, "rtSendTask", RtMacro::kStackSize,
    RtMacro::kPriority, RtMacro::kMode);

  return 0;
}
