#include "Player.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <unistd.h>

#define LOCAL_MPG_123 "/usr/local/bin/mpg123"
#define MPG_123 "/usr/bin/mpg123"

using namespace ENEM;
using std::string;

int MPG123::__create_proc__(void) {
  pid_t pid = fork();
  if (pid < 0)
    throw new std::runtime_error("Cannot fork process");
  if (pid)
    return pid;

  dup2(PIPE_STDOUT[PIPE_WRITE], STDOUT_FILENO);
  dup2(PIPE_STDIN[PIPE_READ], STDIN_FILENO);
  dup2(PIPE_STDERR[PIPE_WRITE], STDERR_FILENO);
  close(PIPE_STDOUT[PIPE_READ]);
  close(PIPE_STDIN[PIPE_WRITE]);
  close(PIPE_STDERR[PIPE_READ]);

  execl(LOCAL_MPG_123, "mpg123", "-R", NULL);
  execl(MPG_123, "mpg123", "-R", NULL);
  execlp("mpg123", "mpg123", "-R", NULL);

  throw new std::runtime_error("Cannot create mpg123 process");
  exit(20);
}

int MPG123::Init(__attribute__((unused)) void *InitArg) {
  if (pipe(PIPE_STDIN) == -1 || pipe(PIPE_STDOUT) == -1 ||
      pipe(PIPE_STDERR) == -1) {
    throw new std::runtime_error("Cannot create pipes");
  }

  __create_proc__();

  m_Status.PlayerName = "mpg123";
  m_Status.PlayingFlag = false;
  m_Status.Volume = 100;
  return 0;
}

int MPG123::Release(__attribute__((unused)) void *ReleseArg) {
  write(PIPE_STDIN[PIPE_WRITE], "Q\n", strlen("Q\n"));
  close(PIPE_STDOUT[PIPE_WRITE]);
  close(PIPE_STDIN[PIPE_READ]);
  close(PIPE_STDERR[PIPE_WRITE]);
  return 0;
}

int MPG123::SetPlayerStatus(bool Flag) {
  if (m_Status.PlayingFlag != Flag)
    write(PIPE_STDIN[PIPE_WRITE], "P\n", strlen("P\n"));
  return 0;
}

int MPG123::SetVolume(float Volume) {
  if (Volume >= 0.0 && Volume <= 100.0) {
    char buf[32];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "V %f\n", Volume);
    write(PIPE_STDIN[PIPE_WRITE], buf, strlen(buf));
  }
  return 0;
}

int MPG123::Load(const char *url) {
  if (!url)
    return 1;
  m_Status.ResourceUrl = url;
  char buf[1024];
  sprintf(buf, "LP %s\n", url);
  write(PIPE_STDIN[PIPE_WRITE], buf, strlen(buf));
  return 0;
}

int MPG123::GetPlayerStatus(PlayerStatus *pStatus) {
  memcpy(pStatus, &m_Status, sizeof(PlayerStatus));
  return 0;
}
