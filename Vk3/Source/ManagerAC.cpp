#include "Headers\ManagerAC.h"
#include "Headers\ManagerIF.hpp"

void ManagerAC::level0_handle(signed char _parity, signed char _temp) {
	if(_temp == -1) {
		apix::Handle::Error("Saved data is broken");
	}
	else if(_temp != _parity) {
		apix::Handle::Error("Saved data is written not completely at level 0.");
	}
}
bool ManagerAC::level1_handle(signed char _parity, signed char _temp) {
	if(_temp == -1) {
		apix::Handle::Error("Saved data is broken");
	}
	else if(_temp != _parity) {
		apix::Handle::Warning("Saved data is written not completely at level 1.");
		return false;
	}
	return true;
}
bool ManagerAC::level2_handle(signed char _parity, signed char _temp) {
	if(_temp == -1) {
		apix::Handle::Error("Saved data is broken");
	}
	else if(_temp != _parity) {
		apix::Handle::Warning("Saved data is written not completely at level 2.");
		return false;
	}
	return true;
}
void ManagerAC::save_dyn_manager(const Manager &_manager) {
	
	apix::Handle::Info("Starting to save manager\'s dynamic part...");
	static bool parity = true;
	FILE * fsave = fopen(_manager.SAVE_FILENAME.c_str(), "rb+");

	byte byte5[5];
	byte hash[] = {'S', 'A', 'V', 'E'};
	
	fseek(fsave, 0, SEEK_SET);
	fwrite(hash, 1, 4, fsave);
	fwrite(apix::to_byte5(byte5, _manager.wPOSITION, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.rPOSITION, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.CURRENT_USER_FRIEND_NUMBER, parity), 1, 5, fsave);
	
	fwrite(apix::to_byte5(byte5, _manager.CURRENT_LEVEL_USER_NUMBER, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.CURRENT_LEVEL_USERS_TOTAL, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.NEXT_LEVEL_USERS_TOTAL, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.NEXT_NEXT_LEVEL_USERS_TOTAL, parity), 1, 5, fsave);
	
	fwrite(apix::to_byte5(byte5, _manager.LEFT_COUNTER, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.TOTAL_COUNTER, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.SKIPPED_COUNTER, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.EXAMINED_COUNTER, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.DEAD_COUNTER, parity), 1, 5, fsave);
	fwrite(apix::to_byte5(byte5, _manager.REPEATED_COUNTER, parity), 1, 5, fsave);
	
	fclose(fsave);
	parity = !parity;
	apix::Handle::Info("Manager\'s dynamic part has been saved.");
	return;
}
void ManagerAC::save_st_manager(const Filter &_filter, const Manager &_manager) {
	
	apix::Handle::Info("Starting to save manager\'s static part...");
	FILE *fsave = fopen(_manager.SAVE_FILENAME.c_str(), "wb");
	if(fsave == NULL) {
		apix::Handle::Error("Problems with openening save file.");
	}
	else {
		dword temp1;
		byte temp2;
		fseek(fsave, 128, SEEK_SET);
		fwrite(&_manager.TOTAL_LEVELS, 4, 1, fsave);
		fwrite(&_manager.ROOT_USER_ID, 4, 1, fsave);
		fwrite(&(temp1 = _filter.FriendLimit()), 4, 1, fsave);
		fwrite(&(temp1 = _filter.AgeLimit().first), 4, 1, fsave);
		fwrite(&(temp1 = _filter.AgeLimit().second), 4, 1, fsave);
		fwrite(&(temp2 = _filter.Sex()), 1, 1, fsave);
		
		std::set<User> users = _filter.Users();
		fwrite(&(temp1 = users.size()), 4, 1, fsave);
		for(std::set<User>::iterator it = users.begin(); it != users.end(); ++it) {
			fwrite(&(temp1 = it->Id()), 4, 1, fsave);
		}
		
		std::set<dword> countries = _filter.Countries();
		fwrite(&(temp1 = countries.size()), 4, 1, fsave);
		for(std::set<dword>::iterator it = countries.begin(); it != countries.end(); ++it) {
			fwrite(&(*it), 4, 1, fsave);
		}
		
		std::set<dword> cities = _filter.Cities();
		fwrite(&(temp1 = cities.size()), 4, 1, fsave);
		for(std::set<dword>::iterator it = cities.begin(); it != cities.end(); ++it) {
			fwrite(&(*it), 4, 1, fsave);
		}
		
		std::set<dword> universities = _filter.Universities();
		fwrite(&(temp1 = universities.size()), 4, 1, fsave);
		for(std::set<dword>::iterator it = universities.begin(); it != universities.end(); ++it) {
			fwrite(&(*it), 4, 1, fsave);
		}
		
		std::set<dword> schools = _filter.Schools();
		fwrite(&(temp1 = schools.size()), 4, 1, fsave);
		for(std::set<dword>::iterator it = schools.begin(); it != schools.end(); ++it) {
			fwrite(&(*it), 4, 1, fsave);
		}
		
		std::vector<User> targets = ManagerAC::GetTargetUsers(_manager);
		fwrite(&(temp1 = targets.size()), 4, 1, fsave);
		for(dword i = 0; i < targets.size(); ++i) {
			fwrite(&(temp1 = targets[i].Id()), 4, 1, fsave);
		}

		fwrite(&(temp2 = _filter.Undef()), 1, 1, fsave);
		fclose(fsave);
	}
	apix::Handle::Info("Manager\'s static part has been saved.");
	return;
}
void ManagerAC::show_manager(Manager &_manager) {

	std::cout << "Task                      : " << _manager.TASK << std::endl; // "";
	std::cout << "Version                   : " << _manager.VERSION << std::endl; // "03.01.02.16";
	std::cout << "Connection                : " << _manager.CONNECTION << std::endl; // "";
	std::cout << "Root User Name            : " << _manager.ROOT_USER_NAME << std::endl; // "";
	std::cout << "Target User Name          : " << _manager.TARGET_USER_NAME << std::endl; // "";
	std::cout << "Root User Id              : " << _manager.ROOT_USER_ID << std::endl; // INF;
	std::cout << "Target User Id            : " << _manager.TARGET_USER_ID << std::endl; // INF;
	std::cout << "Denied Counter            : " << _manager.DENIED_COUNTER << std::endl; // 0;
	std::cout << "Current User Name         : " << _manager.CURRENT_USER_NAME << std::endl; // "";
	std::cout << "Current User Id           : " << _manager.CURRENT_USER_ID << std::endl; // INF;
	std::cout << "Current User Friend Id    : " << _manager.CURRENT_USER_FRIEND_ID << std::endl; // INF;
	std::cout << "Current User Friend Number: " << _manager.CURRENT_USER_FRIEND_NUMBER << std::endl; // 0;
	std::cout << "Cuurent User Friends Total: " << _manager.CURRENT_USER_FRIENDS_TOTAL << std::endl; // INF;
	std::cout << "Current Level User Number : " << _manager.CURRENT_LEVEL_USER_NUMBER << std::endl; // 0;
	std::cout << "Current Level Users Total : " << _manager.CURRENT_LEVEL_USERS_TOTAL << std::endl; // 1;
	std::cout << "Current Level             : " << _manager.CURRENT_LEVEL << std::endl; // -2;
	std::cout << "Next Level Users          : " << _manager.NEXT_LEVEL_USERS_TOTAL << std::endl; // 1;
	std::cout << "Next Next Level Users     : " << _manager.NEXT_NEXT_LEVEL_USERS_TOTAL << std::endl; // 1;
	std::cout << "Total Levels              : " << _manager.TOTAL_LEVELS << std::endl; // INF;
	std::cout << "Left Counter              : " << _manager.LEFT_COUNTER << std::endl; // 0;
	std::cout << "Found Counter             : " << _manager.FOUND_COUNTER << std::endl; // 0;
	std::cout << "Total Counter             : " << _manager.TOTAL_COUNTER << std::endl; // 0;
	std::cout << "Skipped Counter           : " << _manager.SKIPPED_COUNTER << std::endl; // 0;
	std::cout << "Examined Counter          : " << _manager.EXAMINED_COUNTER << std::endl; // 0;
	std::cout << "New users Counter         : " << _manager.NEW_USERS_COUNTER << std::endl; // 0;
	std::cout << "Dead Counter              : " << _manager.DEAD_COUNTER << std::endl; // 0;
	std::cout << "Left To Reconnect         : " << _manager.LEFT_TO_RECONNECT << std::endl; // INF;
	std::cout << "Read Position             : " << _manager.rPOSITION << std::endl; // 0;
	std::cout << "Write Position            : " << _manager.wPOSITION << std::endl; // 0;
	std::cout << "Repeated Counter          : " << _manager.REPEATED_COUNTER << std::endl; // 0;
	std::cout << "Hard Buffer Pointer       : " << _manager.HARD_BUFFER << std::endl << std::endl; // NULL;
	std::cout << "Target Users Names        :\n";
	std::vector<User> us = ManagerAC::GetTargetUsers(_manager);
	for(dword i = 0; i < us.size(); ++i) {
		std::cout << '\t' << us[i].Name() << std::endl;
	}
	
	std::cout << "Passed time               : " << _manager.TIME() << std::endl; // NULL;
	std::cout << "Data base file size       : " << _manager.DB_FILE_SIZE() << std::endl; // NULL;
	std::cout << "Data base future file size: " << _manager.FUTURE_DB_FILE_SIZE() << std::endl; // NULL;
	std::cout << "Average friends           : " << _manager.AVERAGE_FRIENDS() << std::endl; // NULL;
	std::cout << "Average speed             : " << _manager.AVERAGE_USERS_PER_SEC() << std::endl; // NULL;
	std::cout << "Current speed             : " << _manager.USERS_PER_SEC() << std::endl;
	
	return;
}
void ManagerAC::load_save_file(Filter &_filter, Manager &_manager, std::set<dword> &_looked_users) {

	apix::Handle::Info("Loading " + _manager.SAVE_FILENAME + " as save file.");
	FILE * fload = fopen(_manager.SAVE_FILENAME.c_str(), "rb");
	if(fload != NULL) {
		ManagerAC::load_save_file_dyn(_manager, fload);
		ManagerAC::load_save_file_st(_filter, _manager, _looked_users, fload);
		fclose(fload);
	}
	else {
		apix::Handle::Error("Can not open save file .");
	}
	return;
}
void ManagerAC::load_save_file_dyn(Manager &_manager, FILE *_fload) {

	apix::Handle::Info("Loading save file\'s dynamic part.");
	byte buffer[5];
	
	fread(buffer, 1, 4, _fload);		// reading hash
	// Level 0
	fread(buffer, 1, 5, _fload);
	signed char parity = apix::from_byte5(buffer, _manager.wPOSITION), temp;
	if(parity == -1) apix::Handle::Error("Saved data is broken");
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.rPOSITION);
	level0_handle(parity, temp);
	
	_manager.HARD_BUFFER = fopen("users.buf", "rb");
	if(_manager.HARD_BUFFER == NULL) {
		apix::Handle::Error("Hard buffer doesn't exist.");
	}
	fseek(_manager.HARD_BUFFER, _manager.rPOSITION, SEEK_SET);
	fread(&_manager.CURRENT_LEVEL, 4, 1, _manager.HARD_BUFFER);
	fclose(_manager.HARD_BUFFER);
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.CURRENT_USER_FRIEND_NUMBER);
	level0_handle(parity, temp);
	
	// Level 1
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.CURRENT_LEVEL_USER_NUMBER);
	if(!level1_handle(parity, temp)) {
		_manager.CURRENT_LEVEL_USER_NUMBER = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.CURRENT_LEVEL_USERS_TOTAL);
	if(!level1_handle(parity, temp)) {
		_manager.CURRENT_LEVEL_USERS_TOTAL = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.NEXT_LEVEL_USERS_TOTAL);
	if(!level1_handle(parity, temp)) {
		_manager.NEXT_LEVEL_USERS_TOTAL = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.NEXT_NEXT_LEVEL_USERS_TOTAL);
	if(!level1_handle(parity, temp)) {
		_manager.NEXT_NEXT_LEVEL_USERS_TOTAL = INF;
	}
	
	// Level 2
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.LEFT_COUNTER);
	if(!level2_handle(parity, temp)) {
		_manager.LEFT_COUNTER = INF;
	}
	
	std::ifstream fin;
	_manager.FOUND_COUNTER = 0;
	try {
		fin.open(_manager.FOUND_FILENAME.c_str());
	}
	catch(std::exception &_e) {
		apix::Handle::Warning(_e.what());
	}
	if(fin.is_open()) {
		std::string str;
		getline(fin, str);
		if(str != "FOUND") {
			apix::Handle::Warning("Found users file is fake.");
		}
		else {
			while(getline(fin, str)) {
				_manager.FOUND_COUNTER ++;
			}
		}
		fin.close();
	}
	else {
		apix::Handle::Warning("Found users file hasn\'t been found.");
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.TOTAL_COUNTER);
	if(!level2_handle(parity, temp)) {
		_manager.TOTAL_COUNTER = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.SKIPPED_COUNTER);
	if(!level2_handle(parity, temp)) {
		_manager.SKIPPED_COUNTER = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.EXAMINED_COUNTER);
	if(!level2_handle(parity, temp)) {
		_manager.EXAMINED_COUNTER = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.DEAD_COUNTER);
	if(!level2_handle(parity, temp)) {
		_manager.DEAD_COUNTER = INF;
	}
	
	fread(buffer, 1, 5, _fload);
	temp = apix::from_byte5(buffer, _manager.REPEATED_COUNTER);
	if(!level2_handle(parity, temp)) {
		_manager.REPEATED_COUNTER = INF;
	}
	
	apix::Handle::Info("Save.file\'s dynamic part has been loaded.");
	return;
}
void ManagerAC::load_save_file_st(Filter &_filter, Manager &_manager, std::set<dword> &_looked_users, FILE *_fload) {

	apix::Handle::Info("Loading save file\'s static part.");
	VkService service;
	dword temp1, temp2;
	byte  temp3;
	fseek(_fload, 128, SEEK_SET);
	
	fread(&_manager.TOTAL_LEVELS, 4, 1, _fload);
	fread(&_manager.ROOT_USER_ID, 4, 1, _fload);
	_manager.ROOT_USER_NAME = service.GetUser(_manager.ROOT_USER_ID).Name();
	
	fread(&temp1, 4, 1, _fload);
	_filter.SetLimit(temp1);
	fread(&temp1, 4, 1, _fload);
	fread(&temp2, 4, 1, _fload);
	_filter.SetAge(temp1, temp2);
	fread(&temp3, 1, 1, _fload);
	_filter.SetSex(temp1);
	fread(&temp1, 4, 1, _fload);

	for(dword i = 0; i < temp1; ++i) {
		fread(&temp2, 4, 1, _fload);
		if(temp2 != 0) _filter.AddUser(service.GetUser(temp2));
	}
	fread(&temp1, 4, 1, _fload);
	for(dword i = 0; i < temp1; ++i) {
		fread(&temp2, 4, 1, _fload);
		_filter.AddCountry(temp2);
	}
	fread(&temp1, 4, 1, _fload);
	for(dword i = 0; i < temp1; ++i) {
		fread(&temp2, 4, 1, _fload);
		_filter.AddCity(temp2);
	}
	fread(&temp1, 4, 1, _fload);
	for(dword i = 0; i < temp1; ++i) {
		fread(&temp2, 4, 1, _fload);
		_filter.AddUniversity(temp2);
	}
	fread(&temp1, 4, 1, _fload);
	for(dword i = 0; i < temp1; ++i) {
		fread(&temp2, 4, 1, _fload);
		_filter.AddSchool(temp2);
	}
	fread(&temp1, 4, 1, _fload);
	for(dword i = 0; i < temp1; ++i) {
		fread(&temp2, 4, 1, _fload);
		ManagerAC:: AddTargetUser(_manager, service.GetUser(temp2));
	}
	fread(&temp3, 1, 1, _fload);
	temp3 ? _filter.AdmitUndef() : _filter.DetainUndef();
	
	while(fread(&temp1, 4, 1, _fload)) {
		_looked_users.insert(temp1);
	}
	apix::Handle::Info("Save.file\'s static part has been loaded.");
	return;
}
void ManagerAC::load_filter_file(Filter &_filter, Manager &_manager, const std::string &_filename) {
	
	apix::Handle::Info("Loading " + _filename + " as filter file...");
	boost::property_tree::ptree tree;
	boost::property_tree::read_json(_filename, tree);
	VkService service;

	_manager.ROOT_USER_ID = tree.get<unsigned int>("root", UINT_MAX);
	if(_manager.ROOT_USER_ID == UINT_MAX) {
		apix::Handle::Error("Root user should be set.");
	}
	_manager.ROOT_USER_NAME = service.GetUser(_manager.ROOT_USER_ID).Name();
	_filter.SetAge(tree.get<unsigned int>("age from", UINT_MAX), tree.get<unsigned int>("age_to", UINT_MAX));
	_filter.SetSex(tree.get<unsigned char>("sex", UCHAR_MAX));
	_manager.TOTAL_LEVELS = tree.get<signed int>("level limit", UINT_MAX);
	_filter.SetLimit(tree.get<unsigned int>("friends limit", UINT_MAX));
	tree.get<std::string>("detain undefined users", "no") == "no" ? _filter.DetainUndef() : _filter.AdmitUndef();
	
	apix::Handle::Info("Getting places ids...");
	for(boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it) {
		
		if(it->first == "places") {
			for(boost::property_tree::ptree::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				
				std::string country = it2->second.get<std::string>("country", "");
				std::string city = it2->second.get<std::string>("city", "");
				std::string university =  it2->second.get<std::string>("university", "");
				std::string school =  it2->second.get<std::string>("school", "");
				if(university != "") {
					if(city != "") {
						if(country != "") {
							dword university_id = service.GetUniversityId(country, city, university);
							if(university_id != UINT_MAX) {
								_filter.AddUniversity(university_id);
							}
							else {
								apix::Handle::Warning(country + ", " + city + ", " + university + ": bad data.");
							}
						}
						else {
							apix::Handle::Warning(university + ": country is NOT given.");
						}
					}
					else {
						apix::Handle::Warning(university + ": city is NOT given.");
					}
				}
				if(school != "") {
					if(city != "") {
						if(country != "") {
							dword school_id = service.GetSchoolId(country, city, school);
							if(school_id != UINT_MAX) {
								_filter.AddSchool(school_id);
							}
							else {
								apix::Handle::Warning(country + ", " + city + ", " + school + ": bad data.");
							}
						}
						else {
							apix::Handle::Warning(school + ": country is NOT given.");
						}
					}
					else {
						apix::Handle::Warning(school + ": city is NOT given.");
					}
				}
				
				if(school == "" && university == "") {
					if(city != "") {
						if(country != "") {
							dword city_id = service.GetCityId(country, city);
							if(city_id != UINT_MAX) {
								_filter.AddCity(city_id);
							}
							else {
								apix::Handle::Warning(country + ", " + city + ": bad data, city NOT found.");
							}
						}
						else {
							apix::Handle::Warning(city + ": country is NOT given.");
						}
					}
					else if(city == "" && country != "") {
						dword country_id = service.GetCountryId(country);
						if(country_id != UINT_MAX) {
							_filter.AddCountry(country_id);
						}
						else {
							apix::Handle::Warning(country + ": bad data, country NOT found.");
						}
					}
				}
			}
		}
		else if(it->first == "targets") {
			for(boost::property_tree::ptree::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				ManagerAC::AddTargetUser(_manager, service.GetUser(it2->second.get<dword>("", UINT_MAX)));
			}
		}
		else if(it->first == "denies") {
			for(boost::property_tree::ptree::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				_filter.AddUser(service.GetUser(it2->second.get<dword>("", UINT_MAX)));
			}
		}
	}
	apix::Handle::Info("Filter file loading is complete.");
	return;
}
std::vector<User> ManagerAC::GetTargetUsers(const Manager &_manager) {

	std::vector<User> result;
	if(_manager.TARGET_USER_ID == UINT_MAX && !_manager.TARGET_USERS.empty()) {
		std::map<dword, std::string>::const_iterator it_c = _manager.TARGET_USERS.begin();
		std::map<dword, std::string>::const_iterator it_e = _manager.TARGET_USERS.end();
		while(it_c != it_e) {
			result.push_back(User(it_c->first, it_c->second));
			++it_c;
		}
	}
	else if(_manager.TARGET_USER_ID != UINT_MAX) {
		result.push_back(User(_manager.TARGET_USER_ID, _manager.TARGET_USER_NAME));
	}
	return result;
}
void ManagerAC:: AddTargetUser(Manager &_manager, const User &_user) {
	
	std::string username = _user.Name();
	dword userid = _user.Id();
	
	if(_manager.TARGET_USER_ID == UINT_MAX && _manager.TARGET_USERS.empty()) {
		_manager.TARGET_USER_ID = userid;
		_manager.TARGET_USER_NAME = username;
	}
	else if(_manager.TARGET_USER_ID != UINT_MAX && _manager.TARGET_USERS.empty()) {
		_manager.TARGET_USERS.insert(std::pair<dword, std::string>(_manager.TARGET_USER_ID, _manager.TARGET_USER_NAME));
		_manager.TARGET_USERS.insert(std::pair<dword, std::string>(userid, username));
		_manager.TARGET_USER_ID = UINT_MAX;
	}
	else if(!_manager.TARGET_USERS.empty()) {
		_manager.TARGET_USERS.insert(std::pair<dword, std::string>(userid, username));
	}
	return;
}
void ManagerAC::Exec(Manager _manager, Filter _filter) {
	
	static ManagerIF st_interface;
	ManagerAC::save_dyn_manager(_manager);
	// ManagerAC::show_manager(_manager);
	st_interface.Update_n_Show(_manager, _filter);
	return;
}
void ManagerAC::PreSave(Filter _filter, Manager _manager) {
	ManagerAC::save_st_manager(_filter, _manager);
	return;
}
Filter ManagerAC::parseArguments(int _argv, const char **_argc, Manager &_manager, std::set<dword> &_looked_users, bool &_create_buffer) {
	
	apix::Handle::Info("Starting to parse commandline arguments...");
	Filter filter;
	VkService service;
	std::stack<byte> commands;
	std::stack<std::string> parameters;
	
	byte com = 0, count = 0;
	std::string filter_filename = "";
	std::string save_filename = "";
	std::string found_filename = "";
	int i = 1;
	while(8) {
		if(i == _argv) {
			if(com != 0) {
				commands.push(count);
				commands.push(com);
			}
			break;
		}
		if(_argc[i][0] == '-') {
			if(strlen(_argc[i]) > 2) {
				apix::Handle::Warning(std::string(_argc[i]) + " argument's length is to big. Excess part will be ignored.");
			}
			if(com != 0) {
				commands.push(count);
				commands.push(com);
			}
			switch(_argc[i][1]) {
				case	'e'	:	com = 0; ++i; found_filename = _argc[i]; break;
				case	'c'	:	com = 0; ++i; save_filename = _argc[i]; break;
				case	'C' :	com = 0; save_filename = "continue.vk"; break;
				case	'f'	:	com = 0; ++i; filter_filename = _argc[i]; break;
				case	'F'	:	com = 0; filter_filename = "Resources\\filter.json"; break;
				default		:	count = 0; com = _argc[i][1]; break;
			};
		}
		else {
			if(com == 0) {
				apix::Handle::Warning(std::string(_argc[i]) + " excess parameter, will be ignored.");
			}
			else {
				count ++;
				parameters.push(_argc[i]);
			}
		}
		++i;
	}
	if(filter_filename != "") {
		commands.push(1);
		commands.push('f');
		parameters.push(filter_filename);
	}
	if(save_filename != "") {
		commands.push(1);
		commands.push('c');
		parameters.push(save_filename);
		_create_buffer = false;
	}
	else {
		_create_buffer = true;
	}
	if(found_filename != "") {
		commands.push(1);
		commands.push('e');
		parameters.push(found_filename);
	} 
	apix::Handle::Info("Argument checking is complete.");
	
	while(!commands.empty()) {
		byte com = commands.top(); commands.pop();
		byte count = commands.top(); commands.pop();
		if(com == 'g') {
			if(count == 1) {
				std::string par = parameters.top(); parameters.pop();
				if(!apix::is_number(par)) {
					apix::Handle::Error("-g parameter should be valid id.");
				}
				else {
					dword age_to = apix::to_dword(par);
					filter.SetAge(UINT_MAX, age_to);
				}
			}
			else {
				if(count > 3) {
					apix::Handle::Warning("-g should have 1 or 2 parameters, excess parameters will be ignored.");
				}
				std::string par1 = parameters.top(); parameters.pop();
				std::string par2 = parameters.top(); parameters.pop();
				if(!apix::is_number(par1) || !apix::is_number(par2)) {
					apix::Handle::Error("-g parameter should be valid number.");
				}
				else {
					dword age_from = apix::to_dword(par1);
					dword age_to = apix::to_dword(par2);
					filter.SetAge(age_from, age_to);
					for(byte i = 2; i < count; ++i) {
						par1 = parameters.top(); parameters.pop();
					}
				}
			}
		}
		else if(com == 'd' || com == 't' || com == 'r' || com == 'l' || com == 'm') {
			for(byte i = 0; i < count; ++i) {
				std::string par = parameters.top(); parameters.pop();
				if(!apix::is_number(par)) {
					apix::Handle::Error("user id\\limit should be valid number.");
				}
				else {
					dword num = apix::to_dword(par);
						 if(com == 'd') filter.AddUser(service.GetUser(num));
					else if(com == 't') ManagerAC:: AddTargetUser(_manager, service.GetUser(num));
					else if(com == 'r') {
						if(count > 1) {
							apix::Handle::Warning("root user should be one, excess users will be ignored.");
						}
						User us = service.GetUser(num);
						_manager.ROOT_USER_ID = us.Id();
						_manager.ROOT_USER_NAME = us.FirstName() + ' ' + us.LastName();
						break;
					}
					else if(com == 'l' || com == 'm') {
						if(count > 1) {
							apix::Handle::Warning("limit should be one, excess numbers will be ignored.");
						}
							 if(com == 'l') _manager.TOTAL_LEVELS = num;
						else if(com == 'm') filter.SetLimit(num);
						break;
					}
				}
			}
		}	
		else if(com == 'u' || com == 'h' || com == 'i' || com == 'o') {
			for(byte i = 0; i < count; ++i) {
				std::string par = parameters.top(); parameters.pop();
				if(apix::is_number(par)) {
					if(com == 'u') {
						filter.AddUniversity(apix::to_dword(par));
					}
					else if(com == 'h') {
						filter.AddSchool(apix::to_dword(par));
					}
					else if(com == 'i') {
						filter.AddCity(apix::to_dword(par));
					}
					else if(com == 'o') {
						filter.AddCountry(apix::to_dword(par));
					}
				}
				else {
					std::string country, city, institution;
					bool valid = true;
						 if(com == 'u' || com == 'h') valid = apix::to_university(par, country, city, institution);
					else if(com == 'i')				  valid = apix::to_city(par, country, city);
					if(!valid) {
						apix::Handle::Error(par + " is invalid insitution/place string.");
					}
					else {
						if(com == 'u') {
							filter.AddUniversity(service.GetUniversityId(country, city, institution));
						}
						else if(com == 'h') {
							filter.AddSchool(service.GetSchoolId(country, city, institution));
						}
						else if(com == 'i') {
							filter.AddCity(service.GetCityId(country, city));
						}
						else if(com == 'o') {
							filter.AddCountry(service.GetCountryId(country));
						}
					}
				}
			}
		}
		// filename is set in manager's def constructor, here it is only redefined if user point his own file
		else if(com == 'e') {
			std::string filename = parameters.top(); parameters.pop();
			_manager.FOUND_FILENAME = filename;
		}
		else if(com == 'c') {
			std::string filename = parameters.top(); parameters.pop();
			_manager.SAVE_FILENAME = filename;
			ManagerAC::load_save_file(filter, _manager, _looked_users);
		}
		else if(com == 'f') {
			std::string filename = parameters.top(); parameters.pop();
			ManagerAC::load_filter_file(filter, _manager, filename);
		}
	}
	apix::Handle::Info("Argument parsing is complete.");
	return filter;
}
void ManagerAC::SaveCatch(const Manager &_manager, const std::string &_cur_username, const std::string &_target_username) {

	FILE *file = fopen(_manager.FOUND_FILENAME.c_str(), "rb");
	fseek(file, 0, SEEK_END);
	dword size = ftell(file);
	fclose(file);
	
	std::ofstream fout;
	fout.open(_manager.FOUND_FILENAME.c_str(), std::ofstream::app);
	if(size == 0) {
		fout << "FOUND\n";
	}
	fout << _cur_username << '(' << _target_username << ')' << std::endl;
	fout.close();
	return;
}
