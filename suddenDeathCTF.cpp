// suddenDeathCTF.cpp

#include "bzfsAPI.h"

class suddenDeathCTF : public bz_Plugin
{
public:
  int gameInProgress=0; // 1 if it is running, 0 if it isn't.
  int gameStatus=0; // If it's paused, we use 1.
  virtual const char* Name(){return "suddenDeathCTF";}
  virtual void Init (const char* commandLine) {
    Register(bz_eGameStartEventt);
    Register(bz_eGameResumeEvent);
    Register(bz_eGamePauseEvent);
    Register(bz_eGameEndEvent);
    Register(bz_eCaptureEvent);
  }
  virtual void Event(bz_EventData *eventData ){
    switch (eventData->eventType) {
      case bz_eGameStartEvent: {
        bz_GameStartEndEventData_V2* gameStartData = (bz_GameStartEndEventData_V2*)eventData;
        gameInProgress=1;
      }break;

      case bz_eGameResumeEvent: {
        bz_GamePauseResumeEventData_V2* gameResumeData = (bz_GamePauseResumeEventData_V2*)eventData;
        gameStatus=0;
      }break;

      case bz_eGamePauseEvent: {
        bz_GamePauseResumeEventData_V2* gamePauseData = (bz_GamePauseResumeEventData_V2*)eventData;
        gameStatus=1;
      }break;

      case bz_eGameEndEvent: {
        bz_GameStartEndEventData_V2* gameEndData = (bz_GameStartEndEventData_V2*)eventData;
        gameStatus=0;
        gameInProgress=0;
      }break;

      case bz_eCaptureEvent: {
        bz_CTFCaptureEventData_V1* capData = (bz_CTFCaptureEventData_V1*)eventData;
        if ((gameStatus==0)&&(gameInProgress=1)) {
          bz_gameOver(BZ_SERVER, capData->teamCapping);
        }
      }break;

      default:{ 
      }break;
    }
  }
  virtual void Cleanup (void) {
    Flush();
  }
};

BZ_PLUGIN(suddenDeathCTF)

