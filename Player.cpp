#include "Player.h"
#include <cstdio>
#include <cstring>
#include <string>

using namespace ENEM;
using std::string;

int MPG123::Init(void *InitArg) {
  const char *mpg123_arguments = static_cast<const char *>(InitArg);
  string CommandString = "mpg123 -R ";
  CommandString.append(mpg123_arguments);
  pPIPE = popen(CommandString.c_str(), "r+");
  m_Status.PlayerName = "mpg123";
  m_Status.PlayingFlag = false;
  m_Status.Volume = 100;
  return 0;
}

int MPG123::Release(__attribute__((unused)) void *ReleseArg) {
  pclose(pPIPE);
  pPIPE = NULL;
  return 0;
}

int MPG123::SetPlayerStatus(bool Flag) {
  if (m_Status.PlayingFlag != Flag)
    fputs("P\n", pPIPE);
  return 0;
}

int MPG123::SetVolume(float Volume) {
  if (Volume >= 0.0 && Volume <= 100.0) {
    char buf[32];
    sprintf(buf, "V %f\n", Volume);
    fputs(buf, pPIPE);
  }
  return 0;
}

int MPG123::Load(const char *url) {
  m_Status.ResourceUrl = url;
  char buf[1024];
  sprintf(buf, "L %s\nP\n", url);
  fputs(buf, pPIPE);
  return 0;
}

int MPG123::GetPlayerStatus(PlayerStatus *pStatus) {
  memcpy(pStatus, &m_Status, sizeof(PlayerStatus));
  return 0;
}
