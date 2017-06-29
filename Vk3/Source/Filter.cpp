#include "Headers\Filter.h"

Filter::Filter() : user(0) {
	this->include_undefined = true;
	this->country = this->UNDEF;
	this->city = this->UNDEF;
	this->university = this->UNDEF;
	this->school = this->UNDEF;
	this->limit = this->UNDEF;
	this->age_from = this->UNDEF;
	this->age_to = this->UNDEF;
	this->sex = UCHAR_MAX;
}
void Filter::AddUser(const User &_user) {
	
	if(_user.Id() != this->UNDEF && this->user.Id() != _user.Id()) {
		if(this->user.Id() == this->UNDEF) {
			this->user = _user;
		}
		else if(this->user.Id() != this->UNDEF && this->users.empty()) {
			this->users.insert(this->user);
			this->users.insert(_user);
		}
		else {
			this->users.insert(_user);
		}
	}
	return;
}
void Filter::AddCountry(dword _country_id) {
	
	if(_country_id != this->UNDEF && this->country != _country_id) {
		if(this->country == this->UNDEF) {
			this->country = _country_id;
			this->searcher_fields.push_back("country");
		}
		else if(this->country != this->UNDEF && this->countries.empty()) {
			this->countries.insert(this->country);
			this->countries.insert(_country_id);
		}
		else {
			this->countries.insert(_country_id);
		}
	}
	return;
}
void Filter::AddCity(dword _city_id) {
	
	if(_city_id != this->UNDEF && this->city != _city_id) {
		if(this->city == this->UNDEF) {
			this->city = _city_id;
			this->searcher_fields.push_back("city");
		}
		else if(this->city != this->UNDEF && this->cities.empty()) {
			this->cities.insert(this->city);
			this->cities.insert(_city_id);
		}
		else {
			this->cities.insert(_city_id);
		}
	}
	return;
}
void Filter::AddUniversity(dword _university_id) {
	
	if(_university_id != this->UNDEF && this->university != _university_id) {
		if(this->university == this->UNDEF) {
			this->university = _university_id;
			this->searcher_fields.push_back("universities");
		}
		else if(this->university != this->UNDEF && this->universities.empty()) {
			this->universities.insert(this->university);
			this->universities.insert(_university_id);
		}
		else {
			this->universities.insert(_university_id);
		}
	}
	return;
}
void Filter::AddSchool(dword _school_id) {
	
	if(_school_id != this->UNDEF && this->school != _school_id) {
		if(this->school == this->UNDEF) {
			this->school = _school_id;
			this->searcher_fields.push_back("schools");
		}
		else if(this->school != this->UNDEF && this->schools.empty()) {
			this->schools.insert(this->school);
			this->schools.insert(_school_id);
		}
		else {
			this->schools.insert(_school_id);
		}
	}
	return;
}
void Filter::SetAge(dword _age_from, dword _age_to) {
	
	if(this->age_from == this->UNDEF && _age_from != this->UNDEF) {
		this->age_from = _age_from;
		if(this->age_to == this->UNDEF) this->searcher_fields.push_back("bdate");
	}
	if(this->age_to == this->UNDEF && _age_to != this->UNDEF) {
		this->age_to = _age_to;
		if(this->age_from == this->UNDEF) this->searcher_fields.push_back("bdate");
	}
	return;
}
void Filter::SetSex(byte _sex) {

	if(this->sex == UCHAR_MAX && _sex != UCHAR_MAX) {
		this->sex = _sex;
		this->searcher_fields.push_back("sex");
	}
	return;
}
std::set<User> Filter::Users() const {
	if(this->users.empty()) {
		std::set<User> temp;
		temp.insert(this->user);
		return temp;
	}
	else {
		return this->users;
	}
}
std::set<Filter::dword> Filter::Countries() const {
	if(this->countries.empty()) {
		std::set<dword> temp;
		temp.insert(this->country);
		return temp;
	}
	else {
		return this->countries;
	}
}
std::set<Filter::dword> Filter::Cities() const {
	if(this->cities.empty()) {
		std::set<dword> temp;
		temp.insert(this->city);
		return temp;
	}
	else {
		return this->cities;
	}
}
std::set<Filter::dword> Filter::Universities() const {
	if(this->universities.empty()) {
		std::set<dword> temp;
		temp.insert(this->university);
		return temp;
	}
	else {
		return this->universities;
	}
}
std::set<Filter::dword> Filter::Schools() const {
	if(this->schools.empty()) {
		std::set<dword> temp;
		temp.insert(this->school);
		return temp;
	}
	else {
		return this->schools;
	}
}
bool Filter::belong(const User &_user) const {
	
	bool result = true;
	if(this->sex != UCHAR_MAX) {
		result &= ((_user.Sex() != UCHAR_MAX && _user.Sex() == this->sex) || (_user.Sex() == UCHAR_MAX && this->include_undefined));
	}
	if(this->age_from != UINT_MAX || this->age_to != UINT_MAX) {
		
		if(this->age_from == UINT_MAX) {
			result &= ((_user.Age() != UINT_MAX && (_user.Age() >= 0 && _user.Age() <= this->age_to)) || (_user.Age() == UINT_MAX && this->include_undefined));
		}
		else {
			result &= ((_user.Age() != UINT_MAX && (_user.Age() >= this->age_from && _user.Age() <= this->age_to)) || (_user.Age() == UINT_MAX && this->include_undefined));
		}
	}
	if(this->limit != UINT_MAX) {
		result &= ((_user.FriendQuantity() != UINT_MAX && _user.FriendQuantity() <= this->limit) || (_user.FriendQuantity() == UINT_MAX && this->include_undefined));
	}
	if(this->user.Id() != UINT_MAX) {
		
		if(this->users.empty()) {
			result &= ((_user.Id() != UINT_MAX && _user.Id() != this->user.Id()) || (_user.Id() == UINT_MAX && this->include_undefined));
		}
		else {
			result &= ((_user.Id() != UINT_MAX && this->users.find(_user.Id()) == this->users.end()) || (_user.Id() == UINT_MAX && this->include_undefined));
		}
	}
	if(this->country != UINT_MAX) {
		
		if(this->countries.empty()) {
			result &= ((_user.Country() != UINT_MAX && _user.Country() == this->country) || (_user.Country() == UINT_MAX && this->include_undefined));
		}
		else {
			result &= ((_user.Country() != UINT_MAX && this->countries.find(_user.Country()) != this->countries.end()) || (_user.Country() == UINT_MAX && this->include_undefined));
		}
	}
	if(this->city != UINT_MAX) {
		
		if(this->cities.empty()) {
			result &= ((_user.City() != UINT_MAX && _user.City() == this->city) || (_user.City() == UINT_MAX && this->include_undefined));
		}
		else {
			result &= ((_user.City() != UINT_MAX && this->cities.find(_user.City()) != this->cities.end()) || (_user.City() == UINT_MAX && this->include_undefined));
		}
	}
	if(this->school != UINT_MAX) {
		
		bool school_result = false;
		if(this->schools.empty()) {
			
			if(_user.MultiSchool()) {
				
				std::set<dword> user_schools = _user.Schools();
				for(std::set<dword>::iterator it = user_schools.begin(); it != user_schools.end(); ++it) {
					school_result |= (*it == this->school);
				}
			}
			else {
				school_result |= ((_user.School() != UINT_MAX && _user.School() == this->school) || (_user.School() == UINT_MAX && this->include_undefined));
			}
		}
		else {

			if(_user.MultiSchool()) {
				std::set<dword> user_schools = _user.Schools();
				for(std::set<dword>::iterator it = user_schools.begin(); it != user_schools.end(); ++it) {
					school_result |= (this->schools.find(*it) != this->schools.end());
				}
			}
			else {
				school_result |= ((_user.School() != UINT_MAX && this->schools.find(_user.School()) != this->schools.end()) || (_user.School() == UINT_MAX && this->include_undefined));
			}
		}
		result &= school_result;
	}
	
	if(this->university != UINT_MAX) {
		
		bool university_result = false;
		if(this->universities.empty()) {
			
			if(_user.MultiUniversity()) {
				
				std::set<dword> user_universities = _user.Universities();
				for(std::set<dword>::iterator it = user_universities.begin(); it != user_universities.end(); ++it) {
					university_result |= (*it == this->university);
				}
			}
			else {
				university_result |= ((_user.University() != UINT_MAX && _user.University() == this->university) || (_user.University() == UINT_MAX && this->include_undefined));
			}
		}
		else {

			if(_user.MultiUniversity()) {
				std::set<dword> user_universities = _user.Universities();
				for(std::set<dword>::iterator it = user_universities.begin(); it != user_universities.end(); ++it) {
					university_result |= (this->universities.find(*it) != this->universities.end());
				}
			}
			else {
				university_result |= ((_user.University() != UINT_MAX && this->universities.find(_user.University()) != this->universities.end()) || (_user.University() == UINT_MAX && this->include_undefined));
			}
		}
		result &= university_result;
	}
	return result;
}
