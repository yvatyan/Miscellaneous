#include "Headers\ccus.h"

void ccus::AddCountry(ccus::place_pair _country) {
	
	this->dbase.insert (
		std::pair<std::string, std::pair<dword, std::map<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > > > (
			_country.first, std::pair<dword, std::map<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > > (
				_country.second, std::map<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > (
					// empty
				)
			)
		)
	);
	return;
}
ccus::dword ccus::GetCountryId(const std::string &_name) {
	
	country_iterator it = this->dbase.find(_name);
	if(it != this->dbase.end()) return it->second.first;
	else return UINT_MAX;
}

void ccus::AddCity(ccus::place_pair _country, ccus::place_pair _city) {
	
	country_iterator it = this->dbase.find(_country.first);
	if(it == this->dbase.end()) {
		this->AddCountry(_country);
		it = this->dbase.find(_country.first);
	}
	it->second.second.insert (
		std::pair<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > (
			_city.first, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > (
				_city.second, std::map<std::string, dword>(), std::map<std::string, dword> (
					// empty
				)
			)
		)
	);
	return;
}
bool ccus::AddCity(const std::string &_country, ccus::place_pair _city) {
	
	country_iterator it = this->dbase.find(_country);
	if(it == this->dbase.end()) {
		return false;
	}
	it->second.second.insert (
		std::pair<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > (
			_city.first, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > (
				_city.second, std::map<std::string, dword>(), std::map<std::string, dword> (
					// empty
				)
			)
		)
	);
	return true;
}
ccus::dword ccus::GetCityId(const std::string &_name) {
	
	country_iterator begin = this->dbase.begin();
	country_iterator it = begin;
	country_iterator end = this->dbase.end();
	while(it != end) {
		city_iterator it2 = it->second.second.find(_name);
		if(it2 != it->second.second.end()) return it2->second.get<0>();
		++it;
	}
	return UINT_MAX;
}
ccus::dword ccus::GetCityId(const std::string &_country_name, const std::string &_city_name) {
	
	country_iterator it = this->dbase.find(_country_name);
	if(it == this->dbase.end()) return UINT_MAX;
	else {
		city_iterator it2 = it->second.second.find(_city_name);
		if(it2 == it->second.second.end()) return UINT_MAX;
		else return it2->second.get<0>();
	}
}

void ccus::AddUniversity(ccus::place_pair _country, ccus::place_pair _city, ccus::place_pair _university) {
	
	country_iterator it = this->dbase.find(_country.first);
	if(it == this->dbase.end()) {
		this->AddCountry(_country);
		it = this->dbase.find(_country.first);
	}
	city_iterator it2 = it->second.second.find(_city.first);
	if(it2 == it->second.second.end()) {
		this->AddCity(_country, _city);
		it2 = it->second.second.find(_city.first);
	}
	it2->second.get<1>().insert (
		std::pair<std::string, dword> (
			_university.first, _university.second
		)
	);
	return;
}
bool ccus::AddUniversity(const std::string &_country, const std::string &_city, place_pair _university) {
	
	country_iterator it = this->dbase.find(_country);
	if(it == this->dbase.end()) {
		return false;
	}
	city_iterator it2 = it->second.second.find(_city);
	if(it2 == it->second.second.end()) {
		return false;
	}
	it2->second.get<1>().insert (
		std::pair<std::string, dword> (
			_university.first, _university.second
		)
	);
	return true;
}
ccus::dword ccus::GetUniversityId(const std::string &_name) {
	
	country_iterator begin = this->dbase.begin();
	country_iterator it = begin;
	country_iterator end = this->dbase.end();
	while(it != end) {
		city_iterator begin2 = it->second.second.begin();
		city_iterator it2 = begin2;
		city_iterator end2 = it->second.second.end();
		while(it2 != end2) {
			institution_iterator it3 = it2->second.get<1>().find(_name);
			if(it3 != it2->second.get<1>().end()) return it3->second;
			++it2;
		}
		++it;
	}
	return UINT_MAX;
}
ccus::dword ccus::GetUniversityId(const std::string &_city_name, const std::string &_university_name) {
	
	country_iterator begin = this->dbase.begin();
	country_iterator it = begin;
	country_iterator end = this->dbase.end();
	city_iterator it2;
	while(8) {
		if(it == end) return UINT_MAX;
		it2 = it->second.second.find(_city_name);
		if(it2 != it->second.second.end()) break;
		++it;
	}
	institution_iterator it3 = it2->second.get<1>().find(_university_name);
	if(it3 == it2->second.get<1>().end()) return UINT_MAX;
	else return it3->second;
}
ccus::dword ccus::GetUniversityId(const std::string &_country_name, const std::string &_city_name, const std::string &_university_name) {
	
	country_iterator it = this->dbase.find(_country_name);
	if(it == this->dbase.end()) return UINT_MAX;
	else {
		city_iterator it2 = it->second.second.find(_city_name);
		if(it2 == it->second.second.end()) return UINT_MAX;
		else {
			institution_iterator it3 = it2->second.get<1>().find(_university_name);
			if(it3 == it2->second.get<1>().end()) return UINT_MAX;
			else return it3->second;
		}
	}
}

void ccus::AddSchool(ccus::place_pair _country, ccus::place_pair _city, ccus::place_pair _school) {
	
	country_iterator it = this->dbase.find(_country.first);
	if(it == this->dbase.end()) {
		this->AddCountry(_country);
		it = this->dbase.find(_country.first);
	}
	city_iterator it2 = it->second.second.find(_city.first);
	if(it2 == it->second.second.end()) {
		this->AddCity(_country, _city);
		it2 = it->second.second.find(_city.first);
	}
	it2->second.get<2>().insert (
		std::pair<std::string, dword> (
			_school.first, _school.second
		)
	);
	return;
}
bool ccus::AddSchool(const std::string &_country, const std::string &_city, ccus::place_pair _school) {
	
	country_iterator it = this->dbase.find(_country);
	if(it == this->dbase.end()) {
		return false;
	}
	city_iterator it2 = it->second.second.find(_city);
	if(it2 == it->second.second.end()) {
		return false;
	}
	it2->second.get<2>().insert (
		std::pair<std::string, dword> (
			_school.first, _school.second
		)
	);
	return true;
}
ccus::dword ccus::GetSchoolId(const std::string &_name) {
	
	country_iterator begin = this->dbase.begin();
	country_iterator it = begin;
	country_iterator end = this->dbase.end();
	while(it != end) {
		city_iterator begin2 = it->second.second.begin();
		city_iterator it2 = begin2;
		city_iterator end2 = it->second.second.end();
		while(it2 != end2) {
			institution_iterator it3 = it2->second.get<2>().find(_name);
			if(it3 != it2->second.get<2>().end()) return it3->second;
			++it2;
		}
		++it;
	}
	return UINT_MAX;
}
ccus::dword ccus::GetSchoolId(const std::string &_city_name, const std::string &_university_name) {
	
	country_iterator begin = this->dbase.begin();
	country_iterator it = begin;
	country_iterator end = this->dbase.end();
	city_iterator it2;
	while(8) {
		if(it == end) return UINT_MAX;
		it2 = it->second.second.find(_city_name);
		if(it2 != it->second.second.end()) break;
		++it;
	}
	institution_iterator it3 = it2->second.get<2>().find(_university_name);
	if(it3 == it2->second.get<2>().end()) return UINT_MAX;
	else return it3->second;
}
ccus::dword ccus::GetSchoolId(const std::string &_country_name, const std::string &_city_name, const std::string &_university_name) {
	
	country_iterator it = this->dbase.find(_country_name);
	if(it == this->dbase.end()) return UINT_MAX;
	else {
		city_iterator it2 = it->second.second.find(_city_name);
		if(it2 == it->second.second.end()) return UINT_MAX;
		else {
			institution_iterator it3 = it2->second.get<2>().find(_university_name);
			if(it3 == it2->second.get<2>().end()) return UINT_MAX;
			else return it3->second;
		}
	}
}
