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
  std::string url = "http://rg03.sycdn.kuwo.cn/resource/n1/25/47/198668168.mp3";
  player->Load(url.c_str());
  cout << "Loaded!" << endl;
  player->SetPlayerStatus(true);
  player->SetVolume(10.0);
  sleep(100);

  return 0;
}
