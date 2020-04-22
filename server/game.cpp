#include "manager.h"
#include "../common/common.h"

#include <ctime>

#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

const char* szWelcomeMsg = "Welcome to Clue-less, game starting soon\0";

void game(std::vector<pconnInfoT> vecPlayers)
{
    bool bWinner = false;
    std::ostream_iterator<int> out_it(std::cout, ", ");
    char           file[3];                // case file, three cards
    char           deck[18];               // deck after case file has been removed 6 suspect + 6 weapons + 9 rooms = 21. 21-3 = 18
    int            cntPlayers;
     
    unsigned char  avatar[6] = { 0x61, 0x61, 0x61, 0x61, 0x61, 0x61};  // 0x61 - avatar is available, 0x75 avatar is not available

    // initialize the random number generator
    srand(time(NULL));

    cntPlayers = (int)vecPlayers.size();
    std::cout << "[game] nbr of players: " << cntPlayers << std::endl;

    std::vector<pconnInfoT>::iterator    iter = vecPlayers.begin();
    while(vecPlayers.end() != iter)
    {
        unsigned char  newAvatar[6];             // list returned from player
        int fdClient = (*iter)->connfd; 
        msgT  msg;
            
        // build welcome message...
        std::cout << "[game] sending welcome message to player " << std::endl;
        msg.msgLen = (short)(3 + strlen(szWelcomeMsg));
        msg.chCode = CMD_GAME_BEGIN;
        strcpy(msg.szMsg, szWelcomeMsg);
            
        send(fdClient, (const char*)&msg, msg.msgLen, 0);           // sent welcome message

        // build avatar select message...
        std::cout << "[game] sending select message to player " << std::endl;
        std::cout << "[game] available avatars are: ";
        std::copy(avatar, avatar + NBR_SUSPECTS, out_it);
        std::cout << std::endl;
        msg.msgLen = 3 + NBR_SUSPECTS;
        msg.chCode = CMD_GAME_SELECT;
        memcpy(msg.szMsg, avatar, NBR_SUSPECTS);
        send(fdClient, (const char*)&msg, msg.msgLen, 0);           // send init message

        recv(fdClient, (char*)&msg, msg.msgLen, 0);                 // get response from the client
        memcpy(newAvatar, msg.szMsg, NBR_SUSPECTS);                 // get list returned from client

        // compare new list to old list, difference is the avatar this player took...
        for (int ndx = 0; ndx < NBR_SUSPECTS; ndx++)
        {
            if (avatar[ndx] != newAvatar[ndx])
            {
                (*iter)->avatar = ndx;
                break;
            }
        }

        std::cout << "[game] player " << (*iter)->player << " choose avatar " << (*iter)->avatar << std::endl;

        memcpy(avatar, newAvatar, NBR_SUSPECTS);                    // replace original list with new list
        std::cout << "[game] new list of avatars are: ";
        std::copy(avatar, avatar + NBR_SUSPECTS, out_it);
        std::cout << std::endl;

        ++iter;
    }

    std::cout << "[game] generating case file ..." << std::endl;
 
    file[0] = rand() % cntSuspects;
    file[1] = rand() % cntWeapons;
    file[2] = rand() % cntRooms;

    std::cout << "[game] the case file is (" << (int)file[0] << ", " << (int)file[1] << ", " << (int)file[2] << ")" << std::endl;

    // we treat suspects as 0-5; weapons as 10-15; and rooms as 20-28..client will interpret correctly
    // build deck (without case-file cards)
    int lims[4] = { 0, 5, 10, 18 };
    for (int j = 0; j < 3; j++)
    {
        int cardNbr = 10 * j;
        for (int ndx = lims[j]; ndx < lims[j+1]; ndx++)
        {
            //if (file.nSuspect == cardNbr)   // skip if in case-file
            if ((file[j] + 10 * j) == cardNbr)
                cardNbr = cardNbr + 1;
            deck[ndx] = cardNbr++;

        }
    }
    
    std::cout << "[game] shuffling remaing cards..." << std::endl;
    for (int ndx = 0; ndx < 18; ndx++)
    {
        int target = rand() % 18;               // pick a card at random
        std::swap(deck[ndx], deck[target]);     // swap cards
    }


    std::cout << "[game] dealing cards to players...." << std::endl ;
    
    bool bDeal = true;
    int  nbrCard = 0;

    while (bDeal)
    {
        iter = vecPlayers.begin();
        while (vecPlayers.end() != iter)
        {
            if (nbrCard > 17) { bDeal = false; break; }  // out of cards we are done.
            (*iter)->cards.push_back(deck[nbrCard++]);

            ++iter;
        }
    }

    std::cout << "[game] sending cards to players..." << std::endl;
    iter = vecPlayers.begin();
    while (vecPlayers.end() != iter)
    {
        msgT   msg;
        int clisoc = (*iter)->connfd; 

        msg.chCode = CMD_INIT;
        msg.msgLen = 3 + (*iter)->cards.size();
        memcpy(msg.szMsg, (*iter)->cards.data(), (*iter)->cards.size());
        send(clisoc, (const char*)&msg, msg.msgLen, 0);           // send init message

        ++iter;
    }

    // sort vecPlayers on avatar, and then start with lowest avatar

    std::sort(vecPlayers.begin(), vecPlayers.end(), [](const pconnInfoT m1, const pconnInfoT m2) {return m1->avatar < m2->avatar;});
    int player = 0;                  

    while(!bWinner)
    {
        int  ret;                   // return value from select
        fd_set   rdfs;              // input descriptors to listen on
        struct timeval tv;
       
        int clisoc = (vecPlayers.at(player))->connfd;             // get socket to appropriate client
        
        //send message to play to announce turn
	    std::cout << "[game] sending turn announcement to player: " << player << std::endl;
	    msgT    msg;
	    msg.chCode = CMD_TURN;
	    msg.msgLen = 3 + 12;                      // TODO: hard-coded length of string, chage to dynamic string
	    strcpy(msg.szMsg, "start turn!");
	    int nRet = send(clisoc, (const char*)&msg, msg.msgLen, 0);

        //TODO : build a select loop to multiplex over players turn 
        bool bTurn = true;

        while (bTurn)
        {
            tv.tv_sec = 5;            // set timeout for 5 sec
            tv.tv_usec = 0;

            FD_ZERO(&rdfs);
            FD_SET(0, &rdfs);
            FD_SET(clisoc, &rdfs);

            ret = select(clisoc + 1, &rdfs, nullptr, nullptr, &tv);
            if (-1 == ret)
            {
                std::cerr << "select failed" << std::endl;
                bTurn = false;
            }
            else if (ret > 0)
            {
                if (FD_ISSET(clisoc, &rdfs))             // client sent us data
                {
                
                    //  +--------------+-------------------+---------------------------+
                    //  | len (2 bytes)| msg code (1 byte) | msg content (variable len |
                    //  +--------------+-------------------+---------------------------+
                    char  hdr[HDR_LEN];

                    recv(clisoc, &hdr[0], HDR_LEN, 0);                // read in header...
                    short msgLen = hdr[0];                            // get message length
                    unsigned char cmd = hdr[2];                       // get command from header
                    try
                    {
                        if (msgLen > 3)                                  // remember message length includes header
                        {
                            char* buf = new char[msgLen - 3];
                            recv(clisoc, buf, msgLen - 3, 0);

                            switch (cmd)
                            {
                                case CMD_MOVE:
                                {
                                    // TODO : rebroadcast to all playing
                                }
                                case CMD_SUGGEST:                               // got a suggestion from server
                                {
                                    std::cout << "[game] read from client is: " << buf << std::endl;
                                    // TODO : prove or disprove suggestion
                                    // TODO : rebroadcast to all playing, along with results
                                    break;
                                }
                                case CMD_ACCUSE:                                // got an accusation from server
                                {
                                    std::cout << "[game] read from client is: " << buf << std::endl;
                                    // TODO : prove or disprove accusation
                                    // TODO : rebroadcast to all playing, along with results
                                    break;
                                }
                                case CMD_TURN_OVER:
                                {
                                    std::cout << "Players" << (*iter)->player << " turn is over " << std::endl;
                                    bTurn = false;                             // signal turn is over
                                }
                                default:
                                    std::cout << "[game] Unknown command:" << cmd << std::endl;
                            }
                            delete[] buf;
                        }
                    }
                    catch (std::bad_alloc)
                    {
                        std::cout << "[game] failed to allocate buffer of message contents";
                        bTurn = false;
                        bWinner = true;
                    }
                }
            }
            else
            {
                std::cout << "timeout occured" << std::endl;
            }
        }

        player = player + 1;                                // increment to next player..
        if (player < cntPlayers) player = 0;
        std::cout << "player " << player << "'s turn" << std::endl;
    }
 
    // TODO : send game closing message
    // TODO : close all player sockets

    std::cout << "[game] bottom of game loop" << std::endl;
}
