#include <cstdio>

namespace ENEM {

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

class MPG123 : IPlayerCore {
public:
  int SetPlayerStatus(bool Flag);
  int SetVolume(float Volume);
  int Init(void *);
  int Release(void *);
  int Load(const char *url);

  int GetPlayerStatus(PlayerStatus *pStatus);

private:
  PlayerStatus m_Status;
  FILE* pPIPE = NULL;
};
} // namespace ENEM
