#include "Headers\VkService.h"

ccus VkService::database = ccus();

bool VkService::connect(Manager &_manager) {
	if(!this->is_connected) {
		tcp::resolver resolver(this->io_service);
		tcp::resolver::query query("api.vk.com", "http");
		this->error = boost::asio::error::host_not_found;
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, this->error), last;
		if(!this->error) {
			for(; endpoint_iterator != last; ++endpoint_iterator) {
				
				_manager.CONNECTION = "Connecting ...";
				apix::Handle::Info("Connecting...");
					
				this->socket.connect(*endpoint_iterator, this->error);
				if(!this->error) {
					_manager.CONNECTION = endpoint_iterator->endpoint().address().to_string() + ':' +
														apix::to_string(endpoint_iterator->endpoint().port());
					apix::Handle::Info("Connected to " + _manager.CONNECTION);
					break;
				}
			}
		}
		else {
			_manager.CONNECTION = "FAILED!";
			apix::Handle::Warning("Connection faild.");
			this->is_connected = false;
			return this->is_connected; 
		}
		boost::asio::socket_base::keep_alive option(true);
		this->socket.set_option(option);
		this->is_connected = true;
	}
	return this->is_connected;			
}
bool VkService::reconnect(Manager &_manager) {
	if(this->is_connected) {
		this->socket.close();
		apix::Handle::Info("Conncection closed. Reconnecting...");
		this->is_connected = false;
		this->call_counter = 0;
		_manager.LEFT_TO_RECONNECT = this->call_limit - this->call_counter;
	}
	return this->connect(_manager);
}
bool VkService::connect() {
	Manager temp;
	return this->connect(temp);
}
bool VkService::reconnect() {
	Manager temp;
	return this->reconnect(temp);
}
VkService::VkService() : socket(this->io_service){
	this->call_counter = 0;
	this->call_limit = 100;
	this->is_connected = false;
}
bool VkService::GetFriends_WriteToHardBuffer(Manager &_manager, const Filter &_filter, VkService::dword &__out_count) {

	__out_count = 0;
	if(!this->is_connected) {
		while(!this->connect(_manager)){};
	}
	 
	this->call_counter ++;
	_manager.LEFT_TO_RECONNECT = this->call_limit - this->call_counter;
	if(this->call_counter == this->call_limit) {
		while(!this->reconnect(_manager)){};
	}
	dword user_id = _manager.CURRENT_USER_FRIEND_ID;
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	boost::asio::streambuf response;
	std::istream response_stream(&response);	
	std::string http_version, status_message, header, status_code;
	
	request_stream << "GET " << "/method/friends.get?user_id=" << user_id << "&v=5.8" << " HTTP/1.0\r\n";
	request_stream << "Host: " << "api.vk.com" << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: keep-alive\r\n\r\n";
	
	boost::asio::write(socket, request);
	
	boost::asio::read_until(socket, response, "\r\n");
	response_stream >> http_version >> status_code;
	std::getline(response_stream, status_message);

	if (!response_stream || http_version.substr(0, 5) != "HTTP/")
	{
		_manager.CONNECTION = "Invalid response.";
		return false;
	}
	if (status_code != "200")
	{
		_manager.CONNECTION = "Response status code " + status_code;
		return false;
	}
	
	boost::asio::read_until(socket, response, "\r\n\r\n");
	while (std::getline(response_stream, header) && header != "\r"){};

	boost::asio::read_until(socket, response, "}}");

	char t = 0;
	dword id, c = 0, level;

	while(t != '\"'){
		response_stream >> t;
	 }
	std::string _maybe_error = "";
	while(8){
		response_stream >> t;
		if(t == '\"') break;
		_maybe_error += t;
	}

	if(_maybe_error == "error"){
		_manager.DEAD_COUNTER++;
		_manager.NEW_USERS_COUNTER++;
		return true;
	}

	while(c != 2){
		response_stream >> t;
		if(t == ':') c++;
	}
		
	level = _manager.CURRENT_LEVEL + 1;
	response_stream >> __out_count;
	fwrite(&level, 4, 1, _manager.HARD_BUFFER);
	fwrite(&user_id, 4, 1, _manager.HARD_BUFFER);
	fwrite(&__out_count, 4, 1, _manager.HARD_BUFFER);

	if(__out_count != 0) {
		while(t != '[') response_stream >> t;
		apix::Handle::Log("####1#### " + apix::to_string(user_id) + " ####1####", "buffer.log");
		dword debug_counter = 0;
		while(t != ']') {
			response_stream >> id >> t;
			if(_manager.TARGET_USER_ID != UINT_MAX) {
				if(id == _manager.TARGET_USER_ID) {
					User current_user_friend = this->GetUser(user_id, _filter.SearcherFields());
					// is not setting user's friends quantity so filter won't detain user by this parameter
					if(_filter.belong(current_user_friend)) {
						ManagerAC::SaveCatch(_manager, current_user_friend.Name(), _manager.TARGET_USER_NAME);
						_manager.FOUND_COUNTER++;
					}
				}
			}
			else {
				std::map<dword, std::string>::iterator it = _manager.TARGET_USERS.find(id);
				if(it != _manager.TARGET_USERS.end()) {
					User current_user_friend = this->GetUser(user_id, _filter.SearcherFields());
					if(_filter.belong(current_user_friend)) {
						ManagerAC::SaveCatch(_manager, current_user_friend.Name(), it->second);
						_manager.FOUND_COUNTER++;
					}
				}
			}
			fwrite(&id, 4, 1, _manager.HARD_BUFFER);
			apix::Handle::Log(apix::to_string(debug_counter) + ": " + apix::to_string(id), "buffer.log");
			debug_counter ++;
		}
	}
	_manager.wPOSITION = ftell(_manager.HARD_BUFFER);
	return true;
}
User VkService::GetUser(VkService::dword _user_id, const std::vector<std::string> &_fields) {
	
	std::string request = "/method/users.get?user_id=" + apix::to_string(_user_id) + "&v=5.8";
	if(_fields.size() > 0) {
		request += "&fields=";
		for(dword i = 0; 8; ++i) {
			request += _fields[i];
			if(i == _fields.size() - 1) break;
			request += ',';
		}
	}
	boost::asio::streambuf response;
	std::istream response_stream(&response);
	this->makeRequest(request, response, response_stream, "]}");
	
	boost::property_tree::ptree tree;
	boost::property_tree::read_json(response_stream, tree);
	User bob(_user_id);
	bob.SetFirstName(tree.get<std::string>("response..first_name", "undefined"));
	bob.SetLastName(tree.get<std::string>("response..last_name", "undefined"));
	
	for(dword i = 0; i < _fields.size(); ++i) {
		if(_fields[i] == "country") {
			bob.SetCountryId(tree.get<dword>("response..country.id", UINT_MAX));
		}
		else if(_fields[i] == "city") {
			bob.SetCityId(tree.get<dword>("response..city.id", UINT_MAX));
		}
		else if(_fields[i] == "sex") {
			bob.SetSex(tree.get<unsigned char>("response..sex", 0) - 1);
		}
		else if(_fields[i] == "bdate") {
			bob.SetAge(tree.get<std::string>("response..bdate", ""));
		}
		else if(_fields[i] == "schools") {
			
			boost::property_tree::ptree a;
			boost::property_tree::ptree subtree = tree.get_child("response..schools", a);
			if(subtree != a) {
				for(boost::property_tree::ptree::iterator it = subtree.begin(); it != subtree.end(); ++it) {
					bob.AddSchool(it->second.get<dword>("id", UINT_MAX));
				}
			}
		}
		else if(_fields[i] == "universities") {
			
			boost::property_tree::ptree a;
			boost::property_tree::ptree subtree = tree.get_child("response..universities", a);
			if(subtree != a) {
				for(boost::property_tree::ptree::iterator it = subtree.begin(); it != subtree.end(); ++it) {
					bob.AddUniversity(it->second.get<dword>("id", UINT_MAX));
				}
			}
		}
		else {
			apix::Handle::Warning(_fields[i] + " invalid user information field, is ignored.");
		}
	}
	
	return bob;
}
User VkService::GetUser(VkService::dword _user_id) {
	
	std::string request = "/method/users.get?user_id=" + apix::to_string(_user_id) + "&v=5.8";
	boost::asio::streambuf response;
	std::istream response_stream(&response);
	this->makeRequest(request, response, response_stream, "]}");
	
	char t = 0;
	unsigned int c = 0;
	std::string first_name, last_name;

	while(c != 2){
		response_stream >> t;
		if(t == '\"') c++;
	}
	c = 0;
	while(c != 3){
		response_stream >> t;
		if(t == ':') c++;
	}
	
	response_stream >> t >> t;	// reading "a	
	while(t != '\"') {
		first_name += t;
		response_stream >> t;
	}
	while(t != ':') {
		response_stream >> t;
	}
	response_stream >> t >> t;	// reading "a
	while(t != '\"') {
		last_name += t;
		response_stream >> t;
	}
	
	return User(_user_id, first_name + ' ' +  last_name);	
}
VkService::dword VkService::GetUniversityId(const std::string &_country, const std::string &_city, const std::string &_university) {
	
	dword university_id = this->database.GetUniversityId(_country, _city, _university);
	if(university_id == UINT_MAX) {
		dword city_id = this->GetCityId(_country, _city);
		if(city_id != UINT_MAX) {
			dword country_id = this->GetCountryId(_country);
			if(country_id != UINT_MAX) {
				std::string request = "/method/database.getUniversities?q=" + apix::to_string_url(_university) + 
										"&city_id=" + apix::to_string(city_id) +
										"&country_id=" + apix::to_string(country_id) + "&v=5.45";
				boost::asio::streambuf response;
				std::istream response_stream(&response);
				this->makeRequest(request, response, response_stream, "}}");
				
				char t = 0;
				while(t != '[') response_stream >> t;
				response_stream >> t;
				if(t == ']') {
					apix::Handle::Warning(_university + " - university not found in " + _city + ", " + _country + '.');
				}
				else {
					while(t != ':') response_stream >> t;
					std::string id = "";
					
					while(8) {
						response_stream >> t;
						if(t == '\"') continue;
						if(t == ',') break;
						id += t;
					}
					if(!apix::is_number(id)) {
						apix::Handle::Warning(id + " - broken university id.");
					}
					else {
						university_id = apix::to_dword(id);
						this->database.AddUniversity(_country, _city, std::pair<std::string, dword> (_university, university_id));
						apix::Handle::Info("University " + _university + " added to database with id " + apix::to_string(university_id) + '.');
					}
				}
			}
			else {
				apix::Handle::Warning("Getting university id faild because of undefined country request.");
			}
		}
		else {
			apix::Handle::Warning("Getting university id faild because of undefined city request.");
		}
	}
	return university_id;
}
VkService::dword VkService::GetSchoolId(const std::string &_country, const std::string &_city, const std::string &_school) {
	
	dword school_id = this->database.GetUniversityId(_country, _city, _school);
	if(school_id == UINT_MAX) {
		dword city_id = this->GetCityId(_country, _city);
		if(city_id != UINT_MAX) {
			dword country_id = this->GetCountryId(_country);
			if(country_id != UINT_MAX) {
				std::string request = "/method/database.getSchools?q=" + apix::to_string_url(_school) + 
										"&city_id=" + apix::to_string(city_id) +
										"&country_id=" + apix::to_string(country_id) + "&v=5.45";
				boost::asio::streambuf response;
				std::istream response_stream(&response);
				this->makeRequest(request, response, response_stream, "}}");
				
				char t = 0;
				while(t != '[') response_stream >> t;
				response_stream >> t;
				if(t == ']') {
					apix::Handle::Warning(_school + " - school not found in " + _city + ", " + _country + '.');
				}
				else {
					while(t != ':') response_stream >> t;
					std::string id = "";
					
					while(8) {
						response_stream >> t;
						if(t == '\"') continue;
						if(t == ',') break;
						id += t;
					}
					if(!apix::is_number(id)) {
						apix::Handle::Warning(id + " - broken school id.");
					}
					else {
						school_id = apix::to_dword(id);
						this->database.AddSchool(_country, _city, std::pair<std::string, dword> (_school, school_id));
						apix::Handle::Info("School " + _school + " added to database with id " + apix::to_string(school_id) + '.');
					}
				}
			}
			else {
				apix::Handle::Warning("Getting school id faild because of undefined country request.");
			}
		}
		else {
			apix::Handle::Warning("Getting school id faild because of undefined city request.");
		}
	}
	return school_id;
}
VkService::dword VkService::GetCityId(const std::string &_country, const std::string &_city) {
	
	dword city_id = this->database.GetCityId(_country, _city);
	if(city_id == UINT_MAX) {
		dword country_id = this->GetCountryId(_country);
		if(country_id != UINT_MAX) {
			std::string request = "/method/database.getCities?q=" + apix::to_string_url(_city) + "&country_id=" + apix::to_string(country_id) + "&v=5.45";
			boost::asio::streambuf response;
			std::istream response_stream(&response);
			this->makeRequest(request, response, response_stream, "}}");
			
			char t = 0;
			while(t != '[') response_stream >> t;
			response_stream >> t;
			if(t == ']') {
				apix::Handle::Warning(_city + " - city not found in " + _country + '.');
			}
			else {
				while(t != ':') response_stream >> t;
				std::string id = "";
				
				while(8) {
					response_stream >> t;
					if(t == '\"') continue;
					if(t == ',') break;
					id += t;
				}
				if(!apix::is_number(id)) {
					apix::Handle::Warning(id + " - broken city id.");
				}
				else {
					city_id = apix::to_dword(id);
					this->database.AddCity(_country, std::pair<std::string, dword> (_city, city_id));
					apix::Handle::Info("City " + _city + " added to database with id " + apix::to_string(city_id) + '.');
				}
			}
		}
		else {
			apix::Handle::Warning("Getting city id faild because of undefined country request.");
		}
	}
	return city_id;
}
VkService::dword VkService::GetCountryId(const std::string &_name) {
	
	dword country_id = this->database.GetCountryId(_name);
	if(country_id == UINT_MAX) {
		std::string country_code = apix::iso3166::getCountryCode(_name);
		if(country_code != "") {
			std::string request = "/method/database.getCountries?code=" + country_code + "&v=5.45";
			boost::asio::streambuf response;
			std::istream response_stream(&response);
			this->makeRequest(request, response, response_stream, "}}");
			
			char t = 0;
			while(t != '[') response_stream >> t;
			while(t != ':') response_stream >> t;
			std::string id = "";
			while(8) {
				response_stream >> t;
				if(t == '\"') continue;
				if(t == ',') break;
				id += t;
			}
			if(!apix::is_number(id)) {
				apix::Handle::Warning(id + " - broken country id.");
			}
			else {
				country_id = apix::to_dword(id);
				this->database.AddCountry(std::pair<std::string, dword> (_name, country_id));
				apix::Handle::Info("Country " + _name + " added to database with id " + apix::to_string(country_id) + '.');
			}
		}
		else {
			apix::Handle::Warning(_name + " - undefined country's code is requested.");
		}
	}
	return country_id;
}

bool VkService::makeRequest(const std::string &_request, boost::asio::streambuf &_response, std::istream &_response_stream, Manager &_manager, const std::string &_until) {
	
	if(!this->is_connected) {
		while(!this->connect(_manager)){};
	}
	 
	this->call_counter ++;
	_manager.LEFT_TO_RECONNECT = this->call_limit - this->call_counter;
	if(this->call_counter == this->call_limit) {
		while(!this->reconnect(_manager)){};
	}
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	std::string http_version, status_message, header, status_code;
	
	request_stream << "GET " << _request << "&lang=en" << " HTTP/1.0\r\n";
	request_stream << "Host: " << "api.vk.com" << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: keep-alive\r\n\r\n";
	
	boost::asio::write(socket, request);
	
	boost::asio::read_until(socket, _response, "\r\n");
	_response_stream >> http_version >> status_code;
	std::getline(_response_stream, status_message);
	
	if (!_response_stream || http_version.substr(0, 5) != "HTTP/")
	{
		_manager.CONNECTION = "Invalid response.";
		apix::Handle::Warning("Invalid response.");
		return false;
	}
	if (status_code != "200")
	{
		_manager.CONNECTION = "Request failed with status code " + status_code;
		apix::Handle::Warning(_manager.CONNECTION);
		return false;
	}
		
	boost::asio::read_until(socket, _response, "\r\n\r\n");
	while (std::getline(_response_stream, header) && header != "\r"){};
	boost::asio::read_until(socket, _response, _until);

	return true;
}
bool VkService::makeRequest(const std::string &_request, boost::asio::streambuf &_response, std::istream &_response_stream, const std::string &_until) {
	Manager temp;
	return this->makeRequest(_request, _response, _response_stream, temp, _until);
}
