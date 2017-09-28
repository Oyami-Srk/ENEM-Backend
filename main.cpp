#include "Netease.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
using namespace ENEM;

int main(void) {
  string posturl = "http://music.163.com/api/song/"
                   "lyric?os=win&id=448157762&lv=-1&kv=-1&tv=-1";
  HTTP http;
  string resp;
  http.Get(posturl, resp);
  cout << resp << endl;
  return 0;
}
