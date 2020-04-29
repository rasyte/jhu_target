#ifndef _common_h_
#define _common_h_

//#include <QPoint>
//#include <QMap>


enum CMDS { CMD_HRT_BEAT = 0, CMD_UPDATE_STATE, CMD_SUGGEST, CMD_ACCUSE, CMD_INIT, CMD_PLAYER_JOIN, CMD_SHUTDOWN, 
    CMD_GAME_BEGIN, CMD_GAME_SELECT, CMD_MOVE, CMD_GAME_OVER, CMD_TURN, CMD_TURN_OVER, CMD_ACCUSE_RSP, CMD_SUGGEST_RSP, CMD_UNUSED };

static const short HDR_LEN = 3;
static const int NBR_SUSPECTS = 6;

static const char* lpszSuspects[] = { "Miss Scarlet", "Col. Mustard", "Mrs. White","Mr. Green","Mrs. Peacock", "Prof. Plum" };
static const char* lpszRooms[] = { "Study", "Hall", "Lounge", "Library", "Billiard Room", "Dining Room", "Conservatory", "Ballroom", "Kitchen" };
static const char* lpszWeapons[] = {"Knife", "Candlestick", "Revolver", "Rope", "Lead Pipe", "Wrench"};

static const int cntSuspects = sizeof(lpszSuspects) / sizeof(lpszSuspects[0]);
static const int cntWeapons = sizeof(lpszWeapons) / sizeof(lpszSuspects[0]);
static const int cntRooms = sizeof(lpszRooms) / sizeof(lpszRooms[0]);

//static const QPoint  startLoc[6] = { {460, 30}, {640, 190}, {460, 585}, {210, 585}, {30, 430},{30, 190} };

#ifdef __WIN
#pragma pack(show)
typedef struct _tag_hdr
{
    short      msgLen;
    char       chCode;
    char       szMsg[128];              // more than we really need
} msgT, *pMsgT;

#else
typedef struct _tag_hdr
{
    short      msgLen;
    char       chCode;
    char       szMsg[128];              // more than we really need
} __attribute__((packed)) msgT, *pMsgT;

#endif


#endif