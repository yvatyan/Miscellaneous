#include "Source\Headers\User.hpp"
#include "Source\Headers\Filter.h"
#include "Source\Headers\Manager.hpp"
#include "Source\Headers\ManagerAC.h"
#include "Source\Headers\VkService.h"
#include "Source\Headers\Appendix.hpp"

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <set>

typedef unsigned int dword;
typedef unsigned char byte;

namespace Temp {
	void Exec(Manager &_manager, Filter &_filter) {
		_manager.update_speed();
		ManagerAC::Exec(_manager, _filter);
		return;
	}
}

///////////////////////////////////////// TODO REPORT //////////////////////////////////////////
// add threads
// test saving file name which differs from "continue.vk"
// test found file name which differs from "FOUND.txt"
// test filter file name which differs from "Filter.json"
// problem with sighed counters
// save denied counter
// show and save( save = loaded_looked.size() ) reaped counter
// ugly average speed after loading
// test saved looked_users
// test bad data loading in levels 0, 1, 2
// think about place to output warnings, infos, errors while runing
// normilize -args names and write help
// let user to enable or disable logs, infos, warnings (not errors: they're always allowed)
// add filter that controles users (not dounded friends)
////////////////////////////////////////////////////////////////////////////////////////////////

int main(int _argc, const char **_argv) {
	
	try {	
		Manager manager;
		std::set<dword> looked_users;
		bool create_buffer;
		Filter filter = ManagerAC::parseArguments(_argc, _argv, manager, looked_users, create_buffer);

		ManagerAC::PreSave(filter, manager);	// save static part
		std::vector<dword> cur_user_friends;
		VkService service;
		
		if(create_buffer) {
			manager.HARD_BUFFER = fopen("users.buf", "wb");
			for(int i = -1; i < 2; ++i) {
				fwrite(&i, 4, 1, manager.HARD_BUFFER);
			}
			fwrite(&manager.ROOT_USER_ID, 4, 1, manager.HARD_BUFFER);
			manager.wPOSITION = ftell(manager.HARD_BUFFER);
			manager.LEFT_COUNTER++;
			fclose(manager.HARD_BUFFER);
			manager.FOUND_FILENAME = "FOUND.txt";
			FILE *file = fopen(manager.FOUND_FILENAME.c_str(), "wb");
			fclose(file);
		}
		
		apix::Handle::Warnings(false);
		apix::Handle::Infos(false);
		apix::Handle::Logs(false);

		system("pause");
		system("cls");
		Temp::Exec(manager, filter); // outputting titles and save dyn part
		
		while(8) {
			manager.HARD_BUFFER = fopen("users.buf", "rb");
			if(manager.HARD_BUFFER == NULL) {
				apix::Handle::Error("Hard buffer doesn't exist.");
			}
			fseek(manager.HARD_BUFFER, manager.rPOSITION, SEEK_SET);
			int save = manager.CURRENT_LEVEL;
			fread(&manager.CURRENT_LEVEL, 4, 1, manager.HARD_BUFFER);
			
			if(save != manager.CURRENT_LEVEL) {
				manager.CURRENT_LEVEL_USER_NUMBER = 0;
				manager.CURRENT_LEVEL_USERS_TOTAL = manager.NEXT_LEVEL_USERS_TOTAL;
				manager.NEXT_LEVEL_USERS_TOTAL =  manager.NEXT_NEXT_LEVEL_USERS_TOTAL;
				manager.NEXT_NEXT_LEVEL_USERS_TOTAL = 0;
			}
			fread(&manager.CURRENT_USER_ID, 4, 1, manager.HARD_BUFFER);
			fread(&manager.CURRENT_USER_FRIENDS_TOTAL, 4, 1, manager.HARD_BUFFER);
			if(manager.CURRENT_USER_ID != 0) {
				User current_user = service.GetUser(manager.CURRENT_USER_ID, filter.SearcherFields());
				current_user.SetFriendsQuantity(manager.CURRENT_USER_FRIENDS_TOTAL);
				manager.CURRENT_USER_NAME = current_user.FirstName() + ' ' + current_user.LastName();
				
				apix::Handle::Log("####0#### " + manager.CURRENT_USER_NAME + " ####0####", "buffer.log");
				
				if(manager.CURRENT_LEVEL == manager.TOTAL_LEVELS) {
					Temp::Exec(manager, filter);
					break;	// EXIT
				}
				// if(!filter.belong(current_user)) {
				if(current_user.FriendQuantity() >= filter.FriendLimit()) {

					int trash;
					for(int i = 0; i < manager.CURRENT_USER_FRIENDS_TOTAL; ++i) {
						fread(&trash, 4, 1, manager.HARD_BUFFER);
					}
					manager.LEFT_COUNTER -= manager.CURRENT_USER_FRIENDS_TOTAL;
					manager.SKIPPED_COUNTER += manager.CURRENT_USER_FRIENDS_TOTAL;
					manager.DENIED_COUNTER ++;
					manager.NEW_USERS_COUNTER++;
					manager.rPOSITION = ftell(manager.HARD_BUFFER);
					manager.CURRENT_LEVEL_USER_NUMBER ++;
					
					Temp::Exec(manager, filter);
					apix::Handle::Log(apix::to_string(current_user.Id()), std::string(_argv[0]) + ".log");
					fclose(manager.HARD_BUFFER);
					if(manager.rPOSITION == manager.DB_FILE_SIZE()) break;
					continue;
				}
			}
			else {
				manager.CURRENT_USER_NAME = "Starting...";
			}
			
			cur_user_friends.resize(manager.CURRENT_USER_FRIENDS_TOTAL);
			for(int i = 0; i < manager.CURRENT_USER_FRIENDS_TOTAL; ++i) {
				fread(&cur_user_friends[i], 4, 1, manager.HARD_BUFFER);
			}
			dword rPosition_saved = ftell(manager.HARD_BUFFER);
			fclose(manager.HARD_BUFFER);
			
			manager.HARD_BUFFER = fopen("users.buf", "r+b");
			fseek(manager.HARD_BUFFER, manager.wPOSITION, SEEK_SET);
			
			dword _out_count;

			for(Temp::Exec(manager, filter); 
				manager.CURRENT_USER_FRIEND_NUMBER < manager.CURRENT_USER_FRIENDS_TOTAL;
				++manager.CURRENT_USER_FRIEND_NUMBER, Temp::Exec(manager, filter))
			{  

				manager.CURRENT_USER_FRIEND_ID = cur_user_friends[manager.CURRENT_USER_FRIEND_NUMBER];
				if(!(looked_users.find(manager.CURRENT_USER_FRIEND_ID) == looked_users.end())) {
					manager.CURRENT_USER_FRIEND_ID = UINT_MAX;
					-- manager.LEFT_COUNTER;

					manager.NEW_USERS_COUNTER++;
					manager.REPEATED_COUNTER ++;
					continue;
				}
				looked_users.insert(manager.CURRENT_USER_FRIEND_ID);
				service.GetFriends_WriteToHardBuffer(manager, filter, _out_count);
				
				manager.TOTAL_COUNTER += _out_count;
				manager.EXAMINED_COUNTER ++;
				manager.NEW_USERS_COUNTER++;
				if(manager.CURRENT_LEVEL + 1 < manager.TOTAL_LEVELS) manager.LEFT_COUNTER += _out_count;
				-- manager.LEFT_COUNTER;
				
				manager.NEXT_NEXT_LEVEL_USERS_TOTAL += _out_count;
			}
			manager.rPOSITION = rPosition_saved;
			manager.CURRENT_LEVEL_USER_NUMBER ++;
			manager.CURRENT_USER_FRIEND_NUMBER = 0;
			manager.CURRENT_USER_FRIEND_ID = 0;
			fclose(manager.HARD_BUFFER);
		}
	}
	catch(std::exception &_e) {
		apix::Handle::Error(_e.what());
	}	
	
	return 0;
}
