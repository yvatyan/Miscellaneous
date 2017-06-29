#ifndef USER_HPP_
#define USER_HPP_

#include <set>
#include <string>
#include <climits>

#include "Appendix.hpp"

class User {
	private:
		typedef unsigned int dword;
		typedef unsigned char byte;
		
		dword id;
		dword country;
		dword city;
		std::set<dword> universities;	// optional
		std::set<dword> schools;		// optional
		dword university;
		dword school;
		dword age;
		byte sex;
		dword friends_qty;
		std::string first_name;
		std::string last_name;
		User() {};
	public:
		User(dword _id) {
				this->id = _id;
				this->school = this->university = this->city = this->country = UINT_MAX;
				this->age = UINT_MAX;
				this->sex = UCHAR_MAX;
				this->friends_qty = UINT_MAX;
				this->first_name = "";
				this->last_name = "";
		};
		User(dword _id, const std::string &_name) {
			this->id = _id;
			this->school = this->university = this->city = this->country = UINT_MAX;
			this->age = UINT_MAX;
			this->sex = UCHAR_MAX;
			this->friends_qty = UINT_MAX;
			
			dword index = 0;
			while(!apix::is_space(_name[index])) {
				this->first_name += _name[index];
				++index;
			}
			while(apix::is_space(_name[index])) ++index;
			while(index < _name.length()) {
				this->last_name += _name[index];
				++index;
			}
		}
		dword Id() const {
			return this->id;
		}
		dword Country() const {
			return this->country;
		}
		dword City() const {
			return this->city;
		}
		std::set<dword> Universities() const {
			
			if(this->universities.empty()) {
				std::set<dword> temp;
				temp.insert(this->university);
				return temp;
			}
			else {
				return this->universities;
			}
		}
		std::set<dword> Schools() const {
			
			if(this->schools.empty()) {
				std::set<dword> temp;
				temp.insert(this->school);
				return temp;
			}
			else {
				return this->schools;
			}
		}
		dword School() const {
			return this->school;
		}
		dword University() const {
			return this->university;
		}
		dword Age() const {
			return this->age;
		}
		byte Sex() const {
			return this->sex;
		}
		dword FriendQuantity() const {
			return this->friends_qty;
		}
		std::string FirstName() const {
			return this->first_name;
		}
		std::string LastName() const {
			return this->last_name;
		}
		std::string Name() const {
			return this->first_name + ' ' + this->last_name;
		}
		
		bool MultiSchool() const {
			return !this->schools.empty();
		}
		bool MultiUniversity() const {
			return !this->universities.empty();
		}
		
		void SetFirstName(const std::string &_first_name) {
			this->first_name = _first_name;
		}
		void SetLastName(const std::string &_last_name) {
			this->last_name = _last_name;
		}
		void SetName(const std::string &_name) {
			dword index = 0;
			while(!apix::is_space(_name[index])) {
				this->first_name += _name[index];
				++index;
			}
			while(apix::is_space(_name[index])) ++index;
			while(index < _name.length()) {
				this->last_name += _name[index];
				++index;
			}
			return;
		}
		void SetSex(byte _sex) {
			this->sex = _sex;
		}
		void SetAge(dword _age) {
			this->age = _age;
		}
		void SetAge(const std::string &_bdate) { // in format d(d).m(m).yyyy
			
			std::string temp;
			dword index1 = 0, index2 = 0;
			bool correct = false;
			dword bday = 0, bmonth = 0, byear = 0;
			
			while(index1 != 3 && index2 < _bdate.length()) {
				while(_bdate[index2] != '.' && index2 < _bdate.length()) {
					temp += _bdate[index2];
					++ index2;
				}
				switch(index1) {
					case	0	:	bday = apix::to_dword(temp); break;
					case	1	:	bmonth = apix::to_dword(temp); break;
					case	2	:	byear = apix::to_dword(temp); correct = true; break;
				};
				temp = "";
				++ index1;
				++ index2;
			}
			
			if(!correct) {
				this->age = UINT_MAX;
			}
			else {
				this->age = apix::count_age(bday, bmonth, byear);
			}
			return;
		}
		void SetCountryId(dword _country_id) {
			this->country = _country_id;
		}
		void SetCityId(dword _city_id) {
			this->city = _city_id;
		}		
		void AddUniversity(dword _university_id) {
			
			if(this->university != _university_id) {
				if(this->university == UINT_MAX) {
					this->university = _university_id;
				}
				else if(this->university != UINT_MAX && this->universities.empty() && this->university != _university_id) {
					this->universities.insert(this->university);
					this->universities.insert(_university_id);
				}
				else {
					this->universities.insert(_university_id);
				}
			}
			return;
		}
		void AddSchool(dword _school_id) {
			
			if(this->school != _school_id) {
				if(this->school == UINT_MAX) {
					this->school = _school_id;
				}
				else if(this->school != UINT_MAX && this->schools.empty()) {
					this->schools.insert(this->school);
					this->schools.insert(_school_id);
				}
				else {
					this->schools.insert(_school_id);
				}
			}
			return;
		}
		void SetFriendsQuantity(dword _par) {
			this->friends_qty = _par;
		}
		
		bool operator<(const User &_user) const {
			return this->id < _user.id;
		}
};
#endif
