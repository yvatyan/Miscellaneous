#ifndef FILTER_H_
#define FILTER_H_

#include "User.hpp"
#include <set>
#include <vector>
#include <climits>

class Filter {
	private:
		typedef unsigned int dword;
		typedef unsigned char byte;
		static const dword UNDEF;
		
		bool include_undefined;
		dword limit;
		dword age_from;
		dword age_to;
		byte sex;
		// single valued 
		User  user;
		dword country;
		dword city;
		dword university;
		dword school;
		// multi valued
		std::set<User>  users;
		std::set<dword> countries;
		std::set<dword> cities;
		std::set<dword> universities;
		std::set<dword> schools;
		
		std::vector<std::string> searcher_fields;
	public:
		Filter();
		
		void AddUser(const User &_user);			
		void AddCountry(dword _country_id);				// changes Searcher fields
		void AddCity(dword _city_id);					// changes Searcher fields
		void AddUniversity(dword _university_id);		// changes Searcher fields
		void AddSchool(dword _school_id);				// changes Searcher fields
		void SetLimit(dword _friend_limit) {
			this->limit = _friend_limit;
		}			
		void SetAge(dword _age_from, dword _age_to);	// changes Searcher fields
		void SetSex(byte _sex);							// changes Searcher fields
		void DetainUndef() {
			this->include_undefined = false;
		}
		void AdmitUndef() {
			this->include_undefined = true;
		}
		
		dword FriendLimit() const {
			return this->limit;
		}
		std::pair<dword, dword> AgeLimit() const {
			return std::pair<dword, dword>(this->age_from, this->age_to);
		}
		byte Sex() const {
			return this->sex;
		}
		bool Undef() const {
			return this->include_undefined;
		}
		std::set<User> Users() const;
		std::set<dword> Countries() const ;
		std::set<dword> Cities() const;
		std::set<dword> Universities() const;
		std::set<dword> Schools() const;
		const std::vector<std::string> & SearcherFields() const {
			return this->searcher_fields;
		}
		
		bool belong(const User &_user_to_test) const;
};

const Filter::dword Filter::UNDEF(UINT_MAX);

#include "..\Filter.cpp"

#endif
