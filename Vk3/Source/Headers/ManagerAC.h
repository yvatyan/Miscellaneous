#ifndef MANAGERAC_H_
#define MANAGERAC_H_

#include "Appendix.hpp"
#include "Filter.h"
#include "Manager.hpp"
#include "User.hpp"
#include <set>
#include <stack>
#include <string>
#include <fstream>
#include <climits>

class ManagerAC {
	private:
		typedef unsigned int  dword;
		typedef unsigned char byte;
		static const unsigned int INF;
		
		static void level0_handle(signed char _parity, signed char _temp);
		static bool level1_handle(signed char _parity, signed char _temp);
		static bool level2_handle(signed char _parity, signed char _temp);
		static void save_dyn_manager(const Manager &_manager);
		static void save_st_manager(const Filter &_filter, const Manager &_manager);
		static void load_save_file(Filter &_filter, Manager &_manager, std::set<dword> &_looked_users);
		static void load_save_file_dyn(Manager &_manager, FILE *_fload);
		static void load_save_file_st(Filter &_filter, Manager &_manager, std::set<dword> &_looked_users, FILE *_fload);
		static void load_filter_file(Filter &_filter, Manager &_manager, const std::string &_filename);
	public:
		static void show_manager(Manager &_manager);
		static std::vector<User> GetTargetUsers(const Manager &_manager);
		static void  AddTargetUser(Manager &_manager, const User &_user);
		static void Exec(Manager _manager, Filter _filter);
		static void PreSave(Filter _filter, Manager _manager);
		static Filter parseArguments(int _argv, const char **_argc, Manager &_manager, std::set<dword> &_looked_users, bool &_create_buffer);
		static void SaveCatch(const Manager &_manager, const std::string &_cur_username, const std::string &_target_username);
};

const unsigned int ManagerAC::INF = UINT_MAX;

	#ifdef VKSERVICE_H_
		#include "..\ManagerAC.cpp"
	#else
		#include "VkService.h"
		#include "..\ManagerAC.cpp"
	#endif
#endif
