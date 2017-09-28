#include <cstring>
#include <string>

namespace ENEM {
class HTTP {
public:
  int Get(const std::string &url, std::string &resp);
  int Post(const std::string &url, const std::string &post, std::string & resp);
private:
  bool __debug_flag__ = true;
};

class Netease {
public:
  std::string GetSongUrl(int id);
};
} // namespace ENEM
