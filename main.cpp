#include "Player.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <string>
#include <unistd.h>
using std::cout;
using std::endl;
using namespace ENEM;

int main(void) {
  IPlayerCore *player = new MPG123();
  player->Init(NULL);
  // std::string url = "http://rg3.sycdn.kuwo.cn/resource/n1/25/47/198668168.mp3";
  std::string url = "http://www.170mv.com/kw/other.web.nc01.sycdn.kuwo.cn/resource/n1/12/88/1459100477.mp3";
  sleep(1);
  player->Load(url.c_str());
  cout << "Loaded!" << endl;
  player->SetPlayerStatus(true);
  player->SetVolume(10.0);
  while (!player->GetPlayerEnded())
    ;
  cout << "Player Exited" << endl;
  player->Release(NULL);
  return 0;
}
