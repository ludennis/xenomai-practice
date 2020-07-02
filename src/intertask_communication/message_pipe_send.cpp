#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include <alchemy/task.h>
#include <alchemy/pipe.h>

#include <RtMacro.h>

RT_TASK rtWriteTask;

RT_PIPE rtPipe;

int main(int argc, char *argv[])
{
  const char device[] = "/dev/rtp0";

  auto file_descriptor = open(device, O_WRONLY, S_IRWXG);

  if (file_descriptor < 0)
  {
    printf("file descriptor error: %s\n", strerror(errno));
  }

  void *buffer;
  buffer = malloc(RtMacro::kMessageSize);
  char data[] = "hihihi";
  memcpy(buffer, data, RtMacro::kMessageSize);

  auto num_written = write(file_descriptor, buffer, RtMacro::kMessageSize);
  printf("Written %ld bytes(?) into pipe, %s\n", num_written, device);

  if (num_written < 0)
  {
    printf("write error: %s\n", strerror(errno));
  }

  free(buffer);
  close(file_descriptor);

  return 0;
}
