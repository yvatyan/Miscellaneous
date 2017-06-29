#ifndef CCUS_H_
#define CCUS_H_

#include <iostream>
#include <map>
#include <climits>
#include <boost/tuple/tuple.hpp>

class ccus {
	private:
		typedef unsigned int dword;
		typedef std::pair<std::string, dword> place_pair;
		typedef std::map<std::string, std::pair<dword, std::map<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > > >::iterator country_iterator;
		typedef std::map<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > >::iterator city_iterator;
		typedef std::map<std::string, dword>::iterator institution_iterator;
		std::map<std::string, std::pair<dword, std::map<std::string, boost::tuple<dword, std::map<std::string, dword>, std::map<std::string, dword> > > > >  dbase;		
	public:
		/*Add new or change existed                                */ void  AddCountry(place_pair _country);
		/*Get country id by name                                   */ dword GetCountryId(const std::string &_name);
		
		/*Add new or change existed (with parent places)           */ void  AddCity(place_pair _country, place_pair _city);
		/*Add new or change existed                                */ bool  AddCity(const std::string &_country, place_pair _city);
		/*Get city id by name                                      */ dword GetCityId(const std::string &_name);
        /*Get city id by name with pointing country                */ dword GetCityId(const std::string &_country_name, const std::string &_city_name);

		/*Add new or change existed (with parent places)           */ void  AddUniversity(place_pair _country, place_pair _city, place_pair _university);
		/*Add new or change existed                                */ bool  AddUniversity(const std::string &_country, const std::string &_city, place_pair _university);
		/*Get university id by name                                */ dword GetUniversityId(const std::string &_name);
		/*Get university id by name with pointing city             */ dword GetUniversityId(const std::string &_city_name, const std::string &_university_name);
		/*Get university id by name with pointing city and country */ dword GetUniversityId(const std::string &_country_name, const std::string &_city_name, const std::string &_university_name);

		/*Add new or change existed (with parent places)           */ void  AddSchool(place_pair _country, place_pair _city, place_pair _school);
		/*Add new or change existed                                */ bool  AddSchool(const std::string &_country, const std::string &_city, place_pair _school);
		/*Get school id by name                                    */ dword GetSchoolId(const std::string &_name);
		/*Get school id by name with pointing city                 */ dword GetSchoolId(const std::string &_city_name, const std::string &_university_name);
		/*Get school id by name with pointing city and country     */ dword GetSchoolId(const std::string &_country_name, const std::string &_city_name, const std::string &_university_name);
};

#include "..\ccus.cpp"
#endif
