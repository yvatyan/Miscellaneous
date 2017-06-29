#ifndef	APPENDIX_HPP_
#define APPENDIX_HPP_

#include <string>
#include <fstream>
#include <exception>
#include <iostream>
#include <cstdio>
#include <ctime>

namespace apix {

	typedef unsigned int	dword;
	typedef unsigned char 	byte;
	
	int length_of(int _number);
	bool is_number(const std::string &_str);
	bool is_space(char _char);
	std::string to_string(int _number);
	dword to_dword(const std::string &_str);
	signed char from_byte5(byte *_buffer, dword &_par);
	byte *to_byte5(byte *_buffer, dword _par, bool _parity);
	bool to_university(const std::string &_chaos_str, std::string &__in_country, std::string &__in_city, std::string &__in_institution);
	bool to_city(const std::string &_chaos_str, std::string &__in_country, std::string &__in_city);
    bool regcmp(const std::string &_str1, const std::string &_str2);
	dword count_age(dword _byear, dword _bmonth, dword _bday);
	
	class exception : public std::exception {
		private:
			std::string message;
		public:
			exception(const std::string &_mesg) {
				this->message = _mesg;
			}
			const char * what() const throw() {
				return this->message.c_str();
			}
			~exception() throw() {
			}
	};
	class Handle {
		private:
			static bool error_messaging;
			static bool warning_messaging;
			static bool info_messaging;
			static bool log_messaging;
		public:
			static void Error(const std::string &_error) {
				if(error_messaging) {
					exception e("[ERROR:] " + _error + '\n');
					throw e;
				}
				return;
			}
			static void Warning(const std::string &_warning) {
				if(warning_messaging) {
					std::cout << "[WARNING:] " << _warning << std::endl;
				}
				return;
			}
			static void Info(const std::string &_info) {
				if(info_messaging) {
					std::cout << "[INFO:] " << _info << std::endl;
				}
				return;
			}
			static void Log(const std::string &_msg, const std::string &_log_filename) {
				if(log_messaging) {
					std::fstream fs;
					fs.open(_log_filename.c_str(), std::fstream::out | std::fstream::app);
					fs << _msg << std::endl;
					fs.close();
				}
				return;
			}
			static void Errors(bool _enable) {
				Handle::error_messaging = _enable;
			}
			static void Warnings(bool _enable) {
				Handle::warning_messaging = _enable;
			}
			static void Infos(bool _enable) {
				Handle::info_messaging = _enable;
			}
			static void Logs(bool _enable) {
				Handle::log_messaging = _enable;
			}
	};
	bool  Handle::error_messaging = true;
	bool  Handle::warning_messaging = true;
	bool  Handle::info_messaging = true;
	bool  Handle::log_messaging = true;
	
	int length_of(int _number) {
		int count = 0;
		if(_number == 0) return count+1;
		while(_number > 0) {
			count ++;
			_number /= 10;
		}
		return count;
	}
	std::string to_string(int _number) {
		
		if(_number == 0) return "0";

		std::string result = "";		
		while(_number > 0) {
			result += char(_number % 10 + '0');
			_number /= 10;
		}
		int n = result.length();
		for(int i = 0; i < n / 2; ++i) std::swap(result[i], result[n - i - 1]);
		return result;
	}
	dword to_dword(const std::string &_number) {
		if(!is_number(_number)) {
			return 0;
		}
		else {

			dword result = 0;
			dword length = _number.length(), i = 0;
			while(8) {
				result += (_number[i] - '0');
				if(i == length - 1) break;
				result *= 10;
				++i;
			}
			return result;
		}
	}
	bool is_number(const std::string &_str) {
		for(int i = 0; i < _str.length(); ++i) {
			if(_str[i] < '0' || _str[i] > '9') {
				return false;
			}
		}
		return true;
	}
	bool is_space(char _chr) {
		return _chr == ' ' || _chr == '\n' || _chr == '\t' || _chr == '\r';
	}
byte * to_byte5(byte *_buffer, dword _par, bool _parity) {

	for(int i = 0; i < 5; ++i) {
		_buffer[i] = (byte)((_par << 1)  | _parity);
		_par >>= 7;
	}
	return _buffer;
}
signed char from_byte5(byte *_buffer, dword &_par) {

	_par = 0;
	signed char parity = _buffer[4] & 1;
	for(int i = 4; i > -1; --i) {
		if(_buffer[i] & 1 != parity) {
			return -1;
		}
		_par |= (dword)(_buffer[i] >> 1);
		if(i != 0) _par <<= 7;
	}
	return parity;
}
	bool to_university(const std::string &_chaos_str, std::string &__in_country, std::string &__in_city, std::string &__in_institution) {
		
		dword length = _chaos_str.length();
		dword count = 0;
		for(dword i = 0; i < length; ++i) {
			if(_chaos_str[i] == ',') count ++;
		}
		if(count != 2) return false;
		
		dword index = (dword)-1;
		__in_country = __in_city = __in_institution = "";
		
		while(is_space(_chaos_str[++index]));
		do {
			__in_country += _chaos_str[index];
		} while(_chaos_str[++index] != ',');
		
		while(is_space(_chaos_str[++index]));
		do {
			__in_city += _chaos_str[index];
		} while(_chaos_str[++index] != ',');
		
		while(is_space(_chaos_str[++index]));
		do {
			__in_institution += _chaos_str[index];
		} while(!is_space(_chaos_str[++index]) && index < length);
		
		return true;
    }
	bool to_city(const std::string &_chaos_str, std::string &__in_country, std::string &__in_city) {
		
		dword length = _chaos_str.length();
		dword count = 0;
		for(dword i = 0; i < length; ++i) {
			if(_chaos_str[i] == ',') count ++;
		}
		if(count != 1) return false;
		
		dword index = (dword)-1;
		__in_country = __in_city = "";
		
		while(is_space(_chaos_str[++index]));
		do {
			__in_country += _chaos_str[index];
		} while(_chaos_str[++index] != ',');
		
		while(is_space(_chaos_str[++index]));
		do {
			__in_city += _chaos_str[index];
		} while(!is_space(_chaos_str[++index]) && index < length);
		
		return true;
    }
	std::string to_string_url(const std::string _str) {
		
		dword length = _str.length();
		std::string result;
		for(dword i = 0; i < length; ++i) {
			
			if(_str[i] == ' ') {
				result += "%20";
				continue;
			}
			result += _str[i];
		}
		return result;
	}
	bool regcmp(const std::string &_str1, const std::string &_str2) {	// register safe compare
		
		dword length = _str1.length();
		if(length != _str2.length()) return false;
		
		for(int i = 0; i < length; ++i) {
			if(_str1[i] - _str2[i] != 0 && _str1[i] - _str2[i] != 32 && _str1[i] - _str2[i] != -32) {
				return false;
			}
		}
		return true;
	}
	
	dword count_age(dword _bday, dword _bmonth, dword _byear) {
		
		time_t t = time(0);
		struct tm * cur_time = localtime(&t);
		
		dword year = 1900 + cur_time->tm_year;
		dword month = cur_time->tm_mon + 1;
		dword day = cur_time->tm_mday;
		
		dword age = year - _byear - 1;
		age += (month >= _bmonth ? (day > _bday ? true : false) : false);
		
		return age;
	}
	
	namespace iso3166 {
		std::string getCountryCode(const std::string &_name) {
			
			FILE * iso3166_file = fopen("Resources\\ISO31661A2.dat", "rb");
			if(iso3166_file == NULL) {
				Handle::Error("Can not find ISO31661A2.dat file in current directory.");
			}
			else {
				byte offset = 0;
				fseek(iso3166_file, 24, SEEK_SET);
				fread(&offset, 1, 1, iso3166_file);
				byte buffer_size = (offset < 24 ? 24 : offset);
				char buffer[buffer_size+1];
				buffer[buffer_size] = 0;
				
				fseek(iso3166_file, 0, SEEK_SET);
				fread(buffer, 1, buffer_size, iso3166_file);
				if(std::string(buffer) != "ISO 3166-1 alpha-2") {
					Handle::Error("File ISO3166A2.dat is fake.");
				}
				else {				
					dword count = 0, i = 0;
					std::string country;
					do {
						fseek(iso3166_file, 25 + i*offset, SEEK_SET);
						count = fread(buffer, 1, buffer_size, iso3166_file);
						country = std::string(buffer + 3);
						if(regcmp(country, _name)) {
							std::string result;
							result += buffer[0];
							result += buffer[1];
							fclose(iso3166_file);
							return result;
						}
						++i;
					} while(count == buffer_size);
				}
			}
			fclose(iso3166_file);
			return "";
		}
	}
}

#endif
