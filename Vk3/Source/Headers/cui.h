#ifndef CUI_H_
#define CUI_H_

#ifdef WIN32
	#ifndef WINVER
	    #define WINVER 0x0501
	#endif
#endif

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>
#include <stdarg.h>

namespace cui {

	typedef unsigned int dword;
	typedef unsigned long long qword;
	typedef unsigned char byte;
	
	namespace alignment {
		enum type { left, right, center };
	}
	
	class progressBar {
		
		private:
			bool extended2;
			bool extended3;
			bool displayed;
			dword max_value;
			dword cur_value;
			char fill_character;
			char empty_character;
			byte length;
			COORD position;
			HANDLE hConsole;
			dword last_position_x;
			bool correction;
			
			void _construct(byte _x, byte _y, byte _len, byte _ex_level, char _fill, char _empty);
			void show1();
			void show2();
			
			progressBar(const progressBar & _copy) {};
			progressBar & operator= (const progressBar & _copy) {};
		public:
			progressBar();
			progressBar(byte _x_pos, byte _y_pos, byte _length, byte _ex_level);
			progressBar(byte _x_pos, byte _y_pos, byte _length, byte _ex_level, char _fill_char, char _emp_char);
			void Show();
			void Hide();
			void Update(dword _cur_value, dword _max_value);
			void Update_n_Show(dword _cur_value, dword _max_value);
	};
	
	class textBox {
		
		protected:
			std::string text;
			byte padding_left;
			byte padding_right;
			bool displayed;
			byte alignment;			// 0 stands for left, 1 right, 2 center
			byte length;
			COORD position;
			HANDLE hConsole;
			dword last_position_x;
			bool correction;
		private:	
			textBox() {};
			textBox(const textBox &copy) {};
			textBox & operator= (const textBox &copy) {};
			
			void _construct(byte _x, byte _y, std::string _text, byte _tot_len, byte _pad_l, byte _pad_r, alignment::type _t);
			void show_cutted();
			void show_left();
			void show_right();
			void show_center();
		public:
			textBox(byte _x_pos, byte _y_pos, std::string _text);
			textBox(byte _x_pos, byte _y_pos, std::string _text, byte _padding_left, byte _padding_right);
			textBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length);
			textBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length, byte _padding_left, byte _padding_right);
			textBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length, alignment::type _t);
			textBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length, byte _padding_left, byte _padding_right, alignment::type _t);
			virtual byte Size();
			virtual byte AreaSize();
			virtual const std::string & Text();
			
			virtual void Hide();
			virtual void Update(std::string _text);
			virtual void Show();
			virtual void Update_n_Show(std::string _text);
			virtual void Correction(bool _enable);
	};	
	class nameBox : public textBox {
		
		private:
			std::string cut_the_name();
			std::string trim_spaces(const std::string & _str);
		public:
			nameBox(byte _x_pos, byte _y_pos, std::string _text);
			nameBox(byte _x_pos, byte _y_pos, std::string _text, byte _padding_left, byte _padding_right);
			nameBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length);
			nameBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length, byte _padding_left, byte _padding_right);
			nameBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length, alignment::type _t);
			nameBox(byte _x_pos, byte _y_pos, std::string _text, byte _total_length, byte _padding_left, byte _padding_right, alignment::type _t);
			
			virtual void Show();
			virtual void Update(std::string _text);
	};

	class counter {
		
		protected:
			std::vector<qword> counters;
			std::vector<std::string> parts;
			bool displayed;
			COORD position;
			HANDLE hConsole;
			dword last_position_x;
			bool correction;
		private:
			counter() {};
			counter(const counter &_copy) {};
			counter & operator= (const counter &_copy) {};
		public:
			counter(byte _x_pos, byte _y_pos, std::string _format);
			virtual void Show();
			virtual void Hide();
			virtual void Update(dword _count, ...); // counters are qwords
			virtual void Update_n_Show(dword _count, ...); // counters are qwords
			virtual void Correction(bool _enable);
	};
	class timer : public counter {
		
		public:
			timer(byte _x_pos, byte _y_pos);
			virtual void Show();
			virtual void Update(dword _h, dword _m, dword _s);
			virtual void Update(qword _sec_tot);
			virtual void Update_n_Show(dword _h, dword _m, dword _s);
			virtual void Update_n_Show(qword _sec_tot);
	};	
	class memory : public counter {

		public:
			memory(byte _x_pos, byte _y_pos);
			virtual void Show();
			virtual void Update(qword _mem_bytes);
			virtual void Update_n_Show(qword _mem_bytes);
	};
}

#include "..\\cui.cpp"

#endif
