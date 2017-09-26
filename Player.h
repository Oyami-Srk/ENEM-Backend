#include <cstdio>

namespace ENEM {

const int PIPE_WRITE = 1;
const int PIPE_READ = 0;

typedef struct _PlayerStatus {
  const char *PlayerName;
  const char *ResourceUrl;
  bool PlayingFlag;
  float Volume;
  float CurTime;
  float TotTime;
  float CurFrame;
  float TotFrame;
} PlayerStatus;

class IPlayerCore {
public:
  // Controller
  virtual int SetPlayerStatus(bool Flag) = 0; // T->Play, F->Pause
  virtual int SetVolume(float Volume) = 0;    // in percent
  virtual int Init(void *) = 0;               // return 0 or ercode
  virtual int Release(void *) = 0;            // return 0 or ercode
  virtual int Load(const char *url) = 0;      // Load resource file

  // Status
  virtual int GetPlayerStatus(PlayerStatus *pStatus) = 0;
};

class MPG123 : public IPlayerCore {
public:
  int SetPlayerStatus(bool Flag);
  int SetVolume(float Volume);
  int Init(void *);
  int Release(void *);
  int Load(const char *url);

  int GetPlayerStatus(PlayerStatus *pStatus);

private:
  int __create_proc__(void);

  PlayerStatus m_Status;
  int PIPE_STDIN[2];
  int PIPE_STDOUT[2];
  int PIPE_STDERR[2];
};
} // namespace ENEM
