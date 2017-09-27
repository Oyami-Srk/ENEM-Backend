#include "Player.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <unistd.h>

#define LOCAL_MPG_123 "/usr/local/bin/mpg123"
#define MPG_123 "/usr/bin/mpg123"
#define PIPE_WRITE 1
#define PIPE_READ 0

using namespace ENEM;
using std::string;

int MPG123::__lock__(void) {
  while (m_Status_lock)
    ;
  m_Status_lock = true;
  return 0;
}

int MPG123::__unlock__(void) {
  m_Status_lock = false;
  return 0;
}

int MPG123::__create_proc__(void) {
  pid_t pid = fork();
  if (pid < 0)
    throw new std::runtime_error("Cannot fork process");
  if (pid)
    return pid;

  //dup2(PIPE_STDOUT[PIPE_WRITE], STDOUT_FILENO);
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

int MPG123::__parse_out__(void) {
  pid_t pid = fork();
  if (pid < 0)
    throw new std::runtime_error("Cannot fork process");
  if (pid)
    return pid;

  __lock__();
  bool isPlaying = m_Status.PlayingFlag;
  __unlock__();

  while (isPlaying) {
    //printf("?, ");
    // hadnle stdout
    __lock__();
    isPlaying = m_Status.PlayingFlag;
    __unlock__();
  }
  return 0;
}

int MPG123::__deal_with__(char cmd, char *buf) {
  switch (cmd) {
  case 'P':
    switch (buf[2]) {
    case 0:
      // NotLoaded
      break;
    case 1:
      // Pause
      break;
    case 2:
      // Play
      break;
    }
    break;
  }
  return 0;
}

int MPG123::__parse_cmd__(void) {
  char c = 0;
  char input_buffer[256];
  int rcount = 0;
  int icount = 0;
  while (((rcount = read(PIPE_STDOUT[PIPE_READ], &c, 1)) <= 0) && (c != '@'))
    ;
  while (((rcount = read(PIPE_STDOUT[PIPE_READ], &c, 1)) <= 0) && (c != '\n'))
    input_buffer[icount++] = c;
  input_buffer[icount] = '\0';
  __deal_with__(input_buffer[0], input_buffer);
  return input_buffer[0];
}

int MPG123::Init(__attribute__((unused)) void *InitArg) {
  if (pipe(PIPE_STDIN) == -1 || pipe(PIPE_STDOUT) == -1 ||
      pipe(PIPE_STDERR) == -1) {
    throw new std::runtime_error("Cannot create pipes");
  }

  __create_proc__();

  __lock__();
  m_Status.PlayerName = "mpg123";
  m_Status.PlayingFlag = false;
  m_Status.Volume = 100;
  __unlock__();
  return 0;
}

int MPG123::Release(__attribute__((unused)) void *ReleseArg) {
  __lock__();
  m_Status.PlayingFlag = false;
  __unlock__();
  write(PIPE_STDIN[PIPE_WRITE], "Q\n", strlen("Q\n"));
  close(PIPE_STDOUT[PIPE_WRITE]);
  close(PIPE_STDIN[PIPE_READ]);
  close(PIPE_STDERR[PIPE_WRITE]);
  return 0;
}

int MPG123::SetPlayerStatus(bool Flag) {
  if (m_Status.PlayingFlag != Flag) {
    write(PIPE_STDIN[PIPE_WRITE], "P\n", strlen("P\n"));
    __lock__();
    m_Status.PlayingFlag = Flag;
    __unlock__();
    if (Flag)
      __parse_out__();
  }
  return 0;
}

int MPG123::SetVolume(float Volume) {
  if (Volume >= 0.0) {
    __lock__();
    m_Status.Volume = Volume;
    __unlock__();
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
  __lock__();
  m_Status.ResourceUrl = url;
  __unlock__();
  char buf[1024];
  sprintf(buf, "LP %s\n", url);
  write(PIPE_STDIN[PIPE_WRITE], buf, strlen(buf));
  // Check
  sleep(1);
  m_Status.isSongLoaded = true;
  return 0;
}

int MPG123::GetPlayerStatus(PlayerStatus *pStatus) {
  __lock__();
  memcpy(pStatus, &m_Status, sizeof(PlayerStatus));
  __unlock__();
  return 0;
}

bool MPG123::GetPlayerEnded(void) {
  bool ret = false;
  __lock__();
  ret = m_Status.PlayingFlag;
  printf("%d, ", ret);
  __unlock__();
  return !ret;
}
