#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <map>
#include <climits>
#include <string>

const unsigned int INF = UINT_MAX;

struct Manager {
	private:
		typedef unsigned int		dword;
		typedef unsigned long long	qword;
		dword start_time;
		dword last_time;
	public:
		std::string  TASK;								//-// is staticly set after getting parameters
		std::string  VERSION;							//-// is staticly set at constructor
		std::string  CONNECTION;						//-// is dynamicly set at vk.connect
		std::string  ROOT_USER_NAME;					//-// is staticly set after getting parameters 
		std::string  TARGET_USER_NAME;					//-// is staticly set after getting parameters
		dword ROOT_USER_ID;								//+// is staticly set after getting parameters 
		dword TARGET_USER_ID;							//+// is staticly set after getting parameters, is set to 0, when many targets are used
		std::map<dword, std::string> TARGET_USERS;		//+// is same as single value, but is used when many targets are set
		std::string  CURRENT_USER_NAME;					//-// is dynamicly set after reading from hard buffer and getting name
		dword CURRENT_USER_ID;							//-// is dynamicly set after reading from hard buffer
		dword CURRENT_USER_FRIEND_ID;					//-// is dynamicly set while brutting, used in saving, in order to have looked users in "save.file" 
		dword CURRENT_USER_FRIEND_NUMBER;				//+// is dynamicly set at each iteration over user friends
		dword CURRENT_USER_FRIENDS_TOTAL;				//-// is dynamicly set before iteration over user friends
		dword CURRENT_LEVEL_USER_NUMBER;				//+// is dynamicly set after reading from hard buffer
		dword CURRENT_LEVEL_USERS_TOTAL;				//+// is dynamicly set after changing level
		  signed int CURRENT_LEVEL;						//-// is dynamicly set after reading from hard buffer
		dword NEXT_LEVEL_USERS_TOTAL;					//+// is dynamicly set after changing level
		dword NEXT_NEXT_LEVEL_USERS_TOTAL;				//+// is dynamicly set at each iteration over user friends
		  signed int TOTAL_LEVELS;						//+// is staticly set after getting parameters
		dword DENIED_COUNTER;							//+// is dynamicly set after getting parameters and incremented after skipping user friends
		dword LEFT_COUNTER;								//+// is increased after getting friends, decreased after examining users 
		dword FOUND_COUNTER;							//-// is dynamicly set at vk.get_friends
		dword TOTAL_COUNTER;							//+// is increased after getting friends
		dword SKIPPED_COUNTER;							//+// is incresed if user friends are greater then limit 
		dword EXAMINED_COUNTER;							//+// is incresed after getting friends
		dword DEAD_COUNTER;								//+// is dynamicly set at vk.get_friends in case of error
		dword LEFT_TO_RECONNECT;						//-// is dynamicly change at vk.service call-functions
		dword rPOSITION;								//+// is dynamicly change after reading from buffer
		dword wPOSITION;								//+// is dynamicly set at vk.get_friends, after finishing writing
		dword REPEATED_COUNTER;							//+// is dynamicly change after finding user in "lookup"
		dword NEW_USERS_COUNTER;
		dword SPEED;
		FILE * HARD_BUFFER;								//-// is initialized in constructor
		std::string SAVE_FILENAME;
		std::string FOUND_FILENAME;
		
		Manager() {
			this->TASK = "Bfs walk along vk users";
			this->VERSION = "03.01.02.16";
			this->CONNECTION = "";
			this->ROOT_USER_NAME = "";
			this->TARGET_USER_NAME = "";
			this->ROOT_USER_ID = INF;
			this->TARGET_USER_ID = INF;
			this->DENIED_COUNTER = 0;
			this->CURRENT_USER_NAME = "";
			this->CURRENT_USER_ID = INF;
			this->CURRENT_USER_FRIEND_ID = INF;
			this->CURRENT_USER_FRIEND_NUMBER = 0;
			this->CURRENT_USER_FRIENDS_TOTAL = INF;
			this->CURRENT_LEVEL_USER_NUMBER = 0;
			this->CURRENT_LEVEL_USERS_TOTAL = 1;
			this->CURRENT_LEVEL = -2;
			this->NEXT_LEVEL_USERS_TOTAL = 1;
			this->NEXT_NEXT_LEVEL_USERS_TOTAL = 1;
			this->TOTAL_LEVELS = INF;
			this->LEFT_COUNTER = 0;
			this->FOUND_COUNTER = 0;
			this->TOTAL_COUNTER = 0;
			this->SKIPPED_COUNTER = 0;
			this->EXAMINED_COUNTER = 0;
			this->DEAD_COUNTER = 0;
			this->LEFT_TO_RECONNECT = INF;
			this->rPOSITION = 0;
			this->wPOSITION = 0;
			this->REPEATED_COUNTER = 0;
			this->HARD_BUFFER = NULL;
			this->start_time = clock();
			
			this->SPEED = 0;
			this->NEW_USERS_COUNTER = 0;
			this->last_time = clock();
			
			this->SAVE_FILENAME = "continue.vk";
			this->FOUND_FILENAME = "FOUND.txt";
		}
		
		inline void update_speed() {
			
			if((clock() - this->last_time) / CLOCKS_PER_SEC < 2) return;
			
			this->SPEED = this->TIME() ? this->NEW_USERS_COUNTER / ((clock() - this->last_time) / CLOCKS_PER_SEC) : 0;
			this->NEW_USERS_COUNTER = 0;
			this->last_time = clock();
			return;
		}
		inline dword TIME() {
			return (clock() - this->start_time) / CLOCKS_PER_SEC;			
		}
		inline dword DB_FILE_SIZE() {
			return this->wPOSITION;
		}
		inline qword FUTURE_DB_FILE_SIZE() {
			return qword((this->AVERAGE_FRIENDS() * 4 + 2) * this->LEFT_COUNTER + this->wPOSITION);
		}
		inline dword AVERAGE_FRIENDS() {
			return (this->EXAMINED_COUNTER + this->DENIED_COUNTER) ? this->TOTAL_COUNTER / (this->EXAMINED_COUNTER + this->DENIED_COUNTER) : 0;
		}
		inline dword AVERAGE_USERS_PER_SEC() {
			return this->TIME() ? (this->EXAMINED_COUNTER + this->REPEATED_COUNTER + this->DENIED_COUNTER + this->DEAD_COUNTER) / this->TIME() : 0;
		}
		inline dword USERS_PER_SEC() {
			return this->SPEED;
		}
};
#endif
