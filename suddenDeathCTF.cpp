// suddenDeathCTF.cpp

#include "bzfsAPI.h"

class suddenDeathCTF : public bz_Plugin
{
public:
    virtual const char* Name(){return "suddenDeathCTF";}
    virtual void Init ( const char* /*config*/ );
    virtual void Event(bz_EventData *eventData );
    virtual void Cleanup ( void );
    int gameInProgress=0; // 1 if it is running, 0 if it isn't.
    int gameStatus=0; // If it's paused, we use 1.
};

BZ_PLUGIN(suddenDeathCTF)

void suddenDeathCTF::Init (const char* commandLine) {
    Register(bz_eGameStartEventt);
    Register(bz_eGameResumeEvent);
    Register(bz_eGamePauseEvent);
    Register(bz_eGameEndEvent);
    Register(bz_eCaptureEvent);
}

void suddenDeathCTF::Cleanup (void) {
    Flush();
}

void suddenDeathCTF::Event(bz_EventData *eventData ){
    switch (eventData->eventType) {
     case bz_eGameStartEvent: {
        bz_GameStartEndEventData_V2* gameStartData = (bz_GameStartEndEventData_V2*)eventData;
        gameInProgress=1;
// Data
// ---
// (double)       duration - The duration (in seconds) of the game.
// (int)          playerID - The player who started the /countdown. If no player is tied to this event, this defaults to -1
// (bool)         gameOver - Always false in the start event
// (double)       eventTime - The server time the event occurred (in seconds).
     }break;

     case bz_eGameResumeEvent: {
        bz_GamePauseResumeEventData_V2* gameResumeData = (bz_GamePauseResumeEventData_V2*)eventData;
        gameStatus=0;
// Data
// ---
// (bz_ApiString) actionBy - The callsign of whoever triggered the event. By default, it's "SERVER".
// (int)          playerID - The ID of the player who triggered this action. By default, it's 253.
// (double)       eventTime - The server time the event occurred (in seconds).
     }break;

     case bz_eGamePauseEvent: {
        bz_GamePauseResumeEventData_V2* gamePauseData = (bz_GamePauseResumeEventData_V2*)eventData;
        gameStatus=1;
// Data
// ---
// (bz_ApiString) actionBy - The callsign of whoever triggered the event. By default, it's "SERVER".
// (int)          playerID - The ID of the player who triggered this action. By default, it's 253.
// (double)       eventTime - The server time the event occurred (in seconds).
     }break;

     case bz_eGameEndEvent: {
        bz_GameStartEndEventData_V2* gameEndData = (bz_GameStartEndEventData_V2*)eventData;
        gameStatus=0;
        gameInProgress=0;
// Data
// ---
// (double)       duration - The duration (in seconds) of the game.
// (int)          playerID - The player who triggered the event. If it was triggered automatically, this value will be the server ID (253)
// (bool)         gameOver - Set to true if the game was ended with `/gameover` or `/superkill` or via the API
// (double)       eventTime - The server time the event occurred (in seconds).
     }break;

     case bz_eCaptureEvent: {
        bz_CTFCaptureEventData_V1* capData = (bz_CTFCaptureEventData_V1*)eventData;
        if ((gameStatus==0)&&(gameInProgress=1)) {
            bz_gameOver(BZ_SERVER, capData->teamCapping);
        }
// Data
// ---
// (bz_eTeamType) teamCapped - The team whose flag was captured.
// (bz_eTeamType) teamCapping - The team who did the capturing.
// (int)          playerCapping - The player who captured the flag.
// (float[3])     pos - The world position(X,Y,Z) where the flag has been captured
// (float)        rot - The rotational orientation of the capturing player
// (double)       eventTime - This value is the local server time of the event.
     }break;

     default:{ 
     }break;
    }
}
