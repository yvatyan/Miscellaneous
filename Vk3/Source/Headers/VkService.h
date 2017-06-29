#ifndef _VKSERVICE_H_
#define _VKSERVICE_H_

#ifdef WIN32
	#ifndef _WIN32_WINNT
	    #define _WIN32_WINNT 0x0501
	#endif
#endif

class ManagerAC;

#include "Manager.hpp"
#include "Filter.h"
#include "Appendix.hpp"
#include "ccus.h"
#include <map>
#include <climits>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::ip::tcp;

class VkService {
	private:
		typedef unsigned int	dword;
		
		boost::asio::io_service   io_service;
		boost::system::error_code error;
		tcp::socket          socket;
		bool is_connected;
		dword call_counter;
		dword call_limit;
		
		static ccus database;
		
		bool connect(Manager &_manager);
		bool reconnect(Manager &_manager);
		bool VkService::makeRequest(const std::string &_request, boost::asio::streambuf &_response, std::istream &_response_stream, Manager &_manager, const std::string &_until);
		
		bool connect();
		bool reconnect();
		bool VkService::makeRequest(const std::string &_request, boost::asio::streambuf &_response, std::istream &_response_stream,const std::string &_until);
	public:
		VkService();
		bool GetFriends_WriteToHardBuffer(Manager &_manager, const Filter &_filter, dword &__out_count);
		User GetUser(dword _user_id, const std::vector<std::string> &_fields);
		User GetUser(dword _user_id);
		dword GetUniversityId(const std::string &_country, const std::string &_city, const std::string &_university);
		dword GetSchoolId(const std::string &_country, const std::string &_city, const std::string &_shool);
		dword GetCityId(const std::string &_country, const std::string &_city);
		dword GetCountryId(const std::string &_name);
		~VkService() {
			this->socket.close();
			apix::Handle::Info("Conncection closed. Object deleted.");
		}
};

	#ifdef MANAGERAC_H_
		#include "..\VkService.cpp"
	#else
		#include "ManagerAC.h"
		#include "..\VkService.cpp"
	#endif
#endif
