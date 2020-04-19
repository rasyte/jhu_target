#include "manager.h"
#include "game.h"

#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>



#ifdef __WIN
#include <ctime>
#else
#include <sys/timerfd.h>
#include <unistd.h>
#endif


extern std::vector<pconnInfoT>  g_conns;
extern std::mutex               g_mutexque;
extern bool                     g_bForce;
extern bool                     g_bMgrRun;
/*
This function provides the implementation of the manager.  This function does:
(1) periodically checks the size of the queue  
(2) starts a new game if it can
(3) monitors the state of `bForce` to force a game to start
*/
void manage()
{
  int ret;
  int fd = -1;
  struct itimerspec timeout;
  unsigned long long missed;
  
   std::cout << "[manager] Manager thread alive and well" << std::endl;

   fd = timerfd_create(CLOCK_MONOTONIC, 0);      // create a timer

   if (fd > 0)
   {
     ret = fcntl(fd, F_SETFL, O_NONBLOCK);
     if (!ret)
     {
       timeout.it_value.tv_sec = 5;            // 5 second time out
       timeout.it_value.tv_nsec = 0;
       timeout.it_interval.tv_sec = 5;         // recurring
       timeout.it_interval.tv_nsec = 0;

       ret = timerfd_settime(fd, 0, &timeout, NULL);
       if (!ret)
       {
           while (g_bMgrRun)
           {
	            std::vector<pconnInfoT> players;
	            //bool bBuilding = false;

	   
	            unsigned int nSize = g_conns.size();

	            if(((nSize >= 3) && (nSize <= 6)) || g_bForce)
	            {
	                g_mutexque.lock();                                     // lock the queue
	                for(int ndx = nSize-1; ndx >=0; ndx--)
	                {
	                    std::cout << "[manager] Moving player: " << ndx << " to game" <<std::endl;
                        (g_conns.at(ndx))->player = ndx;                     // fill in player number;
	                    players.push_back(g_conns.at(ndx));                  // move player from connections queue to players
	                    g_conns.erase(g_conns.begin() + ndx);                // remove the connection from the queue
	                }  

	                g_conns.erase(g_conns.begin(), g_conns.end());
	                g_bForce = false;

	                g_mutexque.unlock();
	                std::cout << "[manager] Spawning a new game" << std::endl;
	                std::thread  temp(game, players);
	                temp.detach();                                    // detach from the game thread
	   
	     
	            }
	   
	            sleep(5);
	
	       }  // end of while loop
       }    // end of 'if(!ret)' block
       else
       {
	       std::cerr << "[manager] falied to set timer duration, error is " << errno << std::endl;
       }
     }
     else
     {
       std::cerr << "[manager] Failed to set to non-blocking mode, error  " << errno << std::endl;
     }
   }
   else
   {
     std::cerr << "[manager] Failed to create timer, error is: " << errno <<std::endl;
   }

   std::cout << "[manager] manager thread exiting" << std::endl;
}



