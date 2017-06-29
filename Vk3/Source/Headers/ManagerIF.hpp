#ifndef MANAGERAC_H_
	#include "ManagerAC.h"
#else
	#ifndef MANAGERIF_H_
	#define MANAGERIF_H_
	
	#include "cui.h"
	#include "Manager.hpp"
	#include "User.hpp"
	#include "Filter.h"

	#include <vector>
	#include <string>
	#include <iostream>

	class ManagerIF {
		
		private:
			cui::dword field_offset;
			
			void _output_titles() {
				
				std::cout << "Task:" << std::endl; //                   Find Vk.com user
				std::cout << "Version:" << std::endl; //                00.00.01.16
				std::cout << "Connection:" << std::endl; //             api.vk.com at 127.0.0.0
				std::cout << "Root:" << std::endl; //                   Name Surname
				std::cout << "Targets:" << std::endl; //                 Name Surname
				std::cout << "Friends limit:" << std::endl; //                 < 1000 friends
				std::cout << "Skipped users:\n" << std::endl; //           23

				std::cout << "Elapsed time:\n\n" << std::endl; //           1h 04m 45s

				
				std::cout << "Current level:" << std::endl; //          [...........           | 55%] 100/750
				std::cout << "Commonal level:" << std::endl; //         [....                  |  4%] 1/8
				std::cout << "Users left:\n" << std::endl; //             [...................   | 90%] 12897

				std::cout << "Found:" << std::endl; //                  13
				std::cout << "Total users:" << std::endl; //            1350 (75)/1343272
				std::cout << "Deadaccs:" << std::endl; //               19
				std::cout << "Average friends:" << std::endl; //        289
				std::cout << "Average speed:" << std::endl; //
				std::cout << "Current speed:" << std::endl; //
				std::cout << "Left to reconnect:" << std::endl; //
				std::cout << "Database file:" << std::endl; //
				return;
			}
			void _init_fields() {
				
				this->__taskText       = new cui::textBox(field_offset, 0, "");
				this->__versionText    = new cui::textBox(field_offset, 1, "");
				this->__connectionText = new cui::textBox(field_offset, 2, "");
				this->__rootName       = new cui::nameBox(field_offset, 3, "");
				this->__targetsText    = new cui::textBox(field_offset, 4, "");
				this->__frLimitCounter = new cui::counter(field_offset, 5, "< %d friends");
				this->__skippedCounter = new cui::counter(field_offset, 6, "%d");
				
				this->__elapsedTimer   = new cui::timer  (field_offset, 8);
				
				this->__currentName     = new cui::nameBox(0, 10, "", field_offset, 0, 2);
				this->__currentUserBar  = new cui::progressBar(field_offset, 10, 20, 2);
				this->__currentLevelBar = new cui::progressBar(field_offset, 11, 20, 2);
				this->__commonLevelBar  = new cui::progressBar(field_offset, 12, 20, 2);			
				this->__leftUserBar     = new cui::progressBar(field_offset, 13, 20, 1);
				this->__leftUserCounter = new cui::counter(0, 0, " %d");
				this->__leftUserCounter->Correction(false);
				
				this->__foundCounter      = new cui::counter(field_offset, 15, "%d");
				this->__totalUsersCounter = new cui::counter(field_offset, 16, "%d (%d)/%d");
				this->__deadCounter       = new cui::counter(field_offset, 17, "%d");
				this->__averageFrCounter  = new cui::counter(field_offset, 18, "%d");
				this->__averageSpCounter  = new cui::counter(field_offset, 19, "%d");
				this->__currentSpCounter  = new cui::counter(field_offset, 20, "%d");
				this->__reconnectCounter  = new cui::counter(field_offset, 21, "%d");
				this->__bufferMemory      = new cui::memory (field_offset, 22);
				this->__bufferFtMemory    = new cui::memory (field_offset, 23);
				this->__bufferFtMemory->Correction(false);
				
				return;
			}
		public:
			cui::textBox 	 * __taskText;
			cui::textBox 	 * __versionText;
			cui::textBox 	 * __connectionText;
			cui::nameBox 	 * __rootName;
			cui::textBox 	 * __targetsText;
			cui::counter 	 * __frLimitCounter;
			cui::counter 	 * __skippedCounter;
			
			cui::timer	 	 * __elapsedTimer;
			
			cui::nameBox 	 * __currentName;
			cui::progressBar * __currentUserBar;
			cui::progressBar * __currentLevelBar;
			cui::progressBar * __commonLevelBar;
			cui::progressBar * __leftUserBar;
			cui::counter	 * __leftUserCounter;
			
			cui::counter	 * __foundCounter;
			cui::counter	 * __totalUsersCounter;
			cui::counter	 * __deadCounter;
			cui::counter	 * __averageFrCounter;
			cui::counter	 * __averageSpCounter;
			cui::counter	 * __currentSpCounter;
			cui::counter	 * __reconnectCounter;
			cui::memory 	 * __bufferMemory;
			cui::memory 	 * __bufferFtMemory;
			
			ManagerIF() {
				system ("mode con cols=80 lines=35");
				this->field_offset = 25;
				this->_output_titles();
				this->_init_fields();
			}
			void Update_n_Show(Manager &_manager, const Filter &_filter) {
				
				std::string targets_names_string = "";
				std::vector<User> users = ManagerAC::GetTargetUsers(_manager);
				
				for(cui::dword i = 0; i < users.size(); ++i) {
					targets_names_string += users[i].Name();
					if(i != users.size() - 1) {
						targets_names_string += ", ";
					}
				}
				
				__taskText->Update_n_Show(_manager.TASK);
				__versionText->Update_n_Show(_manager.VERSION);
				__connectionText->Update_n_Show(_manager.CONNECTION);
				__rootName->Update_n_Show(_manager.ROOT_USER_NAME);
				__targetsText->Update_n_Show(targets_names_string);
				__frLimitCounter->Update_n_Show(1, (cui::qword)_filter.FriendLimit());
				__skippedCounter->Update_n_Show(1, (cui::qword)_manager.SKIPPED_COUNTER);
				
				__elapsedTimer->Update_n_Show(_manager.TIME());
				
				__currentName->Update_n_Show(_manager.CURRENT_USER_NAME);
				__currentUserBar->Update_n_Show(_manager.CURRENT_USER_FRIEND_NUMBER, _manager.CURRENT_USER_FRIENDS_TOTAL);
				__currentLevelBar->Update_n_Show(_manager.CURRENT_LEVEL_USER_NUMBER, _manager.CURRENT_LEVEL_USERS_TOTAL);
				__commonLevelBar->Update_n_Show(_manager.CURRENT_LEVEL, _manager.TOTAL_LEVELS);
				
				static cui::qword max_value = 0;
				if(_manager.CURRENT_LEVEL + 1 < _manager.TOTAL_LEVELS) {
					max_value = _manager.LEFT_COUNTER;
					__leftUserBar->Update_n_Show(0, max_value);
				}
				else {
					__leftUserBar->Update_n_Show(max_value - _manager.LEFT_COUNTER, max_value);
				}
				__leftUserCounter->Update_n_Show(1,(cui::qword)_manager.LEFT_COUNTER);
				
				__foundCounter->Update_n_Show(1, (cui::qword)_manager.FOUND_COUNTER);
				__totalUsersCounter->Update_n_Show(3, (cui::qword)_manager.EXAMINED_COUNTER, (cui::qword)_manager.DENIED_COUNTER, (cui::qword)_manager.TOTAL_COUNTER);
				__deadCounter->Update_n_Show(1, (cui::qword)_manager.DEAD_COUNTER);
				__averageFrCounter->Update_n_Show(1, (cui::qword)_manager.AVERAGE_FRIENDS());
				__averageSpCounter->Update_n_Show(1, (cui::qword)_manager.AVERAGE_USERS_PER_SEC());
				__currentSpCounter->Update_n_Show(1, (cui::qword)_manager.USERS_PER_SEC());
				__reconnectCounter->Update_n_Show(1, (cui::qword)_manager.LEFT_TO_RECONNECT);
				__bufferMemory->Update_n_Show((cui::qword)_manager.DB_FILE_SIZE()); std::cout << '/';
				__bufferFtMemory->Update_n_Show(_manager.FUTURE_DB_FILE_SIZE());
				
				return;
			}
	};
	#endif
#endif
