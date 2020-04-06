#ifndef _common_h_
#define _common_h_

enum CMDS { CMD_HRT_BEAT = 0, CMD_UPDATE_STATE, CMD_SUGGEST, CMD_ACCUSE, CMD_INIT, CMD_PLAYER_JOIN, CMD_SHUTDOWN, 
            CMD_GAME_BEGIN, CMD_GAME_SELECT, CMD_MOVE, CMD_GAME_OVER, CMD_TURN, CMD_TURN_OVER, CMD_UNUSED};

static const short HDR_LEN = 3;
static const int NBR_SUSPECTS = 6;
static const char* lpszSuspects[] = { "Col. Mustard","Prof. Plum","Mr. Green","Mrs. Peacock","Miss Scarlet", "Mrs. White" };
static const char* lpszRooms[] = {"Hall", "Lounge", "Dining Room", "Kitchen", "Ball Room","Conservatory", "Billard Room", "Library", "Study"};
static const char* lpszWeapons[] = {"Knife", "Candlestick", "Revolver", "Rope", "Lead Pipe", "Wrench"};

static const int cntSuspects = sizeof(lpszSuspects) / sizeof(lpszSuspects[0]);
static const int cntWeapons = sizeof(lpszWeapons) / sizeof(lpszSuspects[0]);
static const int cntRooms = sizeof(lpszRooms) / sizeof(lpszRooms[0]);

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