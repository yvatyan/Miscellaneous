#include "Headers\\cui.h"

void cui::progressBar::_construct(cui::byte _x, cui::byte _y, cui::byte _len, cui::byte _ex_level, char _fill, char _empty) {
	
	switch(_ex_level) {
		case	0	:	this->extended2 = this->extended3 = false; break;
		case	1	:	this->extended2 = true; this->extended3 = false; break;
		case	2	:	this->extended2 = false; this->extended3 = true; break;
	}
	this->displayed = false;
	this->max_value = this->cur_value = 0;
	this->fill_character = _fill;
	this->empty_character = _empty;
	this->length = _len;
	this->position.X = _x;
	this->position.Y = _y;
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
	this->last_position_x = 0;
	this->correction = true;
	return;
}
void cui::progressBar::show1() {
	
	std::cout << '[';
	int i = -1;
	int points = 0;
	if(this->max_value < this->cur_value) this->cur_value = this->max_value;
	
	if(this->max_value) points = this->cur_value * this->length / this->max_value;
	while(++i < points) {
		std::cout << this->fill_character;
	}
	--i;
	while(++i < this->length) {
		std::cout << this->empty_character;
	}
	return;
}
void cui::progressBar::show2() {
	show1();
	std::cout << '|';
	int persent = 0;
	if(this->max_value) persent = this->cur_value * 100 / this->max_value;

	if(persent < 10) {
		std::cout << "  " << persent << '%';
	}
	else if(persent < 100) {
		std::cout << ' ' << persent << '%';
	}
	else {
		std::cout << persent << '%';
	}
	return;
}
cui::progressBar::progressBar() {
	_construct(0, 0, 20, 0, '.', ' ');
}
cui::progressBar::progressBar(cui::byte _x_pos, cui::byte _y_pos, cui::byte _length, cui::byte _ex_level) {
	_construct(_x_pos, _y_pos, _length, _ex_level, '.', ' ');
}
cui::progressBar::progressBar(cui::byte _x_pos, cui::byte _y_pos, cui::byte _length, cui::byte _ex_level, char _fill_char, char _emp_char) {
	_construct(_x_pos, _y_pos, _length, _ex_level, _fill_char, _emp_char);
}
void cui::progressBar::Show() {
	
	this->displayed = true;
	if(this->correction) SetConsoleCursorPosition(this->hConsole, this->position);
	else {
		_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
		this->position.X = buffer_info.dwCursorPosition.X;
		this->position.Y = buffer_info.dwCursorPosition.Y;
	}
		
	if(this->extended2) {
		this->show2();
		std::cout << ']';				
	}
	else if(this->extended3) {
		this->show2();
		std::cout << "] " << this->cur_value << '/' << this->max_value;
	}
	else {
		this->show1();
		std::cout << ']';
	}
	
	_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
	int len = this->last_position_x - buffer_info.dwCursorPosition.X;
	
	this->last_position_x = buffer_info.dwCursorPosition.X;
	for(int i = 0; i < len; ++i) std::cout << ' ';
	
	COORD f = {last_position_x, this->position.Y};
	SetConsoleCursorPosition(this->hConsole, f);
	
	return;
}
void cui::progressBar::Hide() {

	if(this->displayed) {
		this->displayed = false;
		int len = this->last_position_x - this->position.X;
		SetConsoleCursorPosition(this->hConsole, this->position);
		for(int i = 0; i < len; ++i) std::cout << ' ';
	}
	return;
}
void cui::progressBar::Update(cui::dword _cur_value, cui::dword _max_value) {
	
	this->cur_value = _cur_value;
	this->max_value = _max_value;
	return;
}
void cui::progressBar::Update_n_Show(cui::dword _cur_value, cui::dword _max_value) {
	this->Update(_cur_value, _max_value);
	this->Show();
	return;
}

void cui::textBox::_construct(cui::byte _x, cui::byte _y, std::string _text, cui::byte _tot_len, cui::byte _pad_l, cui::byte _pad_r, cui::alignment::type _t) {
	this->displayed = false;
	this->padding_left = _pad_l;
	this->padding_right = _pad_r;
	switch(_t) {
		case	alignment::left		:	this->alignment = 0; break;
		case	alignment::right	:	this->alignment = 1; break;
		case	alignment::center	:	this->alignment = 2; break;
	};
	this->text = _text;
	this->position.X = _x;
	this->position.Y = _y;
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->last_position_x = 0;
	this->correction = true;
	_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
	if(_x + _tot_len > buffer_info.dwSize.X) this->length = buffer_info.dwSize.X - _x - 1;
	else {
		this->length = _tot_len;
	}
	return;
}
void cui::textBox::show_cutted() {
	
	for(int i = 0; i < this->padding_left; ++i) std::cout << ' ';
	for(int i = 0; i < this->AreaSize() - 3; ++i) std::cout << this->text[i];
	std::cout << "...";
	for(int i = 0; i < this->padding_right; ++i) std::cout << ' ';
	return;
}
void cui::textBox::show_left() {
	
	int index = 0;
	for(int i = 0; i < this->padding_left; ++i, ++index) std::cout << ' ';
	for(int i = 0; i < this->text.length(); ++i, ++index) std::cout << this->text[i];
	for(; index < this->length; ++index) std::cout << ' ';
	return;				
}
void cui::textBox::show_right() {
	
	for(int i = 0; i < this->padding_left + this->AreaSize() - this->text.length(); ++i) std::cout << ' ';
	for(int i = 0; i < this->text.length(); ++i) std::cout << this->text[i];
	for(int i = 0; i < this->padding_right; ++i) std::cout << ' ';
	return;			
}
void cui::textBox::show_center() {
	
//	means: [area / 2] - [word / 2] if(area % 2 == 0) - (word % 2 + area % 2)
	int al_center_spaces = 
		this->AreaSize() / 2 - this->text.length() / 2 - (this->AreaSize() % 2 == 0)*(this->text.length() % 2 + this->AreaSize() % 2);
	int index = 0;
	for(int i = 0; i < this->padding_left + al_center_spaces; ++i, ++index) std::cout << ' ';
	for(int i = 0; i < this->text.length(); ++i, ++index) std::cout << this->text[i];
	for(; index < this->length; ++index) std::cout << ' ';
	return;
}
cui::textBox::textBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text) {											// unlimited
	_construct(_x_pos, _y_pos, _text, 255, 0, 0, alignment::left);
}
cui::textBox::textBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _padding_left, cui::byte _padding_right) {	// unlimited
	_construct(_x_pos, _y_pos, _text, 255, _padding_left, 0, alignment::left);
}
cui::textBox::textBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length) {
	_construct(_x_pos, _y_pos, _text, _total_length, 0, 0, alignment::left);
}
cui::textBox::textBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length, cui::byte _padding_left, cui::byte _padding_right) {
	_construct(_x_pos, _y_pos, _text, _total_length, _padding_left, _padding_right, alignment::left);
}
cui::textBox::textBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length, cui::alignment::type _t) {
	_construct(_x_pos, _y_pos, _text, _total_length, 0, 0, _t);
}
cui::textBox::textBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length, cui::byte _padding_left, cui::byte _padding_right, cui::alignment::type _t) {
	_construct(_x_pos, _y_pos, _text, _total_length, _padding_left, _padding_right, _t);
}
cui::byte cui::textBox::Size() {
	return this->length;
}
cui::byte cui::textBox::AreaSize() {
	return this->length - this->padding_left - this->padding_right;
}
const std::string & cui::textBox::Text() {
	return this->text;
}			
void cui::textBox::Hide() {

	if(this->displayed) {
		this->displayed = false;
		int len = this->last_position_x - this->position.X;
		SetConsoleCursorPosition(this->hConsole, this->position);
		for(int i = 0; i < len; ++i) std::cout << ' ';
	}
	return;
}
void cui::textBox::Update(std::string _text) {
	this->text = _text;
	return;
}
void cui::textBox::Show() {
	
	this->displayed = true;
	if(this->correction) SetConsoleCursorPosition(this->hConsole, this->position);
	else {
		_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
		this->position.X = buffer_info.dwCursorPosition.X;
		this->position.Y = buffer_info.dwCursorPosition.Y;
	}
	
	if(this->text.length() > this->AreaSize()) {
		this->show_cutted();
	}
	else {
		if(this->alignment == 0) {
			this->show_left();
		}
		else if(this->alignment == 1) {
			this->show_right();
		}
		else if(this->alignment == 2) {
			this->show_center();
		}
	}
	
	_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
	int len = this->last_position_x - buffer_info.dwCursorPosition.X;
	this->last_position_x = buffer_info.dwCursorPosition.X;
	for(int i = 0; i < len; ++i) std::cout << ' ';
	
	COORD f = {last_position_x, this->position.Y};
	SetConsoleCursorPosition(this->hConsole, f);
	
	return;
}
void cui::textBox::Update_n_Show(std::string _text) {
	this->Update(_text);
	this->Show();
	return;
}
void cui::textBox::Correction(bool _enable) {
	this->correction = _enable;
	return;
}

std::string cui::nameBox::cut_the_name() {
	
	std::string result;
	
	byte names = 1;
	for(int i = 0; i < this->text.length(); ++i) if(this->text[i] == ' ') names ++;
	byte * names_length = new byte[names];
	byte total_length = 0;
	for(int i = 0, l = 0, w = 0; i < this->text.length(); ++i, ++l) {
	
		if(this->text[i] == ' ' || i == this->text.length() - 1) {
			names_length[w] = l + (i == this->text.length() - 1);
			total_length += l + (i == this->text.length() - 1);
			++i;
			l = 0;
			++w;
		}
	}
				
				int valid_area = this->AreaSize() - names + 1, text_index = 0;

				for(int i = 0; i < names-1; ++i) {
				
					if(total_length <= valid_area) break;
					else {
						result += this->text[text_index];
						result += '.';
						text_index += names_length[i] + 1;
						total_length -= names_length[i] - 2;
					}
				}
				
				while(text_index < this->text.length()) {
					result += this->text[text_index];
					++text_index;
				}
				return result;
			}
std::string cui::nameBox::trim_spaces(const std::string & _str) {

	int size = _str.length();
	std::string new_text;
	int index = -1;
	bool is_name = false;
	
	while(++index < size) {
		if(_str[index] != ' ' && _str[index] != '\t') break;
	}
	--index;
	while(++index < size) {
		if(_str[index] != ' ' && _str[index] != '\t') new_text += _str[index];
		else {
			while(++index < size) {
				if(_str[index] != ' ' && _str[index] != '\t') break;
			}
			if(index != size) {
				new_text += ' ';
				is_name = true;
			}
			--index;
		}
	}
	return new_text;
}
cui::nameBox::nameBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text) :														// unlimited
	textBox(_x_pos, _y_pos, "", 255, 0, 0, alignment::left) 
		{ this->text = this->trim_spaces(this->text); }
cui::nameBox::nameBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _padding_left, cui::byte _padding_right) :				// unlimited
	textBox(_x_pos, _y_pos, "", 255, _padding_left, 0, alignment::left)
		{ this->text = this->trim_spaces(this->text); }
cui::nameBox::nameBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length) :
	textBox(_x_pos, _y_pos, "", _total_length, 0, 0, alignment::left)
		{ this->text = this->trim_spaces(this->text); }
cui::nameBox::nameBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length, cui::byte _padding_left, cui::byte _padding_right) :
	textBox(_x_pos, _y_pos, "", _total_length, _padding_left, _padding_right, alignment::left)
		{ this->text = this->trim_spaces(this->text); }
cui::nameBox::nameBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length, alignment::type _t) :
	textBox(_x_pos, _y_pos, "", _total_length, 0, 0, _t)
		{ this->text = this->trim_spaces(this->text); }
cui::nameBox::nameBox(cui::byte _x_pos, cui::byte _y_pos, std::string _text, cui::byte _total_length, cui::byte _padding_left, cui::byte _padding_right, alignment::type _t) :
	textBox(_x_pos, _y_pos, "", _total_length, _padding_left, _padding_right, _t)
		{ this->text = this->trim_spaces(this->text); }

 void cui::nameBox::Show() {

	if(this->text.length() > this->AreaSize()) {
		std::string saved_full = this->text;
		textBox::Update(this->cut_the_name());
		textBox::Show();
		this->text = saved_full;
	}
	else {
		textBox::Show();
	}
	return;				
}
void cui::nameBox::Update(std::string _text) {
	textBox::Update(this->trim_spaces(_text));
	return;
}

cui::counter::counter(cui::byte _x_pos, cui::byte _y_pos, std::string _format) {
	
	std::string temp = "";
	for(dword i = 0; i < _format.length(); ++i) {
		
		if(_format[i] == '%') {
			this->parts.push_back(temp);
			temp = "";
			++i;
			if(_format[i] != 'd') {
				throw std::invalid_argument("Invalid flag " + _format[i] + '.');
			}
		}
		else {
			temp += _format[i];
		} 
	}
	this->parts.push_back(temp);
	
	counters = std::vector<qword> (this->parts.size()-1, 0);
	this->position.X = _x_pos;
	this->position.Y = _y_pos;
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->last_position_x = 0;
	this->correction = true;
}
void cui::counter::Hide() {
	if(this->displayed) {
		this->displayed = false;
		int len = this->last_position_x - this->position.X;
		SetConsoleCursorPosition(this->hConsole, this->position);
		for(int i = 0; i < len; ++i) std::cout << ' ';
	}
	return;
}
void cui::counter::Show() {
	
	this->displayed = true;
	if(this->correction) SetConsoleCursorPosition(this->hConsole, this->position);
	else {
		_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
		this->position.X = buffer_info.dwCursorPosition.X;
		this->position.Y = buffer_info.dwCursorPosition.Y;
	}
	
	dword i;
	for(i = 0; i < this->counters.size(); ++i) {
		std::cout << this->parts[i] << this->counters[i];
	}
	std::cout << this->parts[i];
	
	_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
	
	int len = this->last_position_x - buffer_info.dwCursorPosition.X;
	this->last_position_x = buffer_info.dwCursorPosition.X;
	for(int i = 0; i < len; ++i) std::cout << ' ';
	
	COORD f = {last_position_x, this->position.Y};
	SetConsoleCursorPosition(this->hConsole, f);
	
	return;
}
void cui::counter::Update(dword _count, ...) {
	
	va_list arguments;
	va_start(arguments, _count);
	
	for(dword i = 0; i < _count && i < this->counters.size(); ++i) {
		this->counters[i] = va_arg(arguments, qword);
	}
	
	va_end(arguments);
	return;
}
void cui::counter::Update_n_Show(dword _count, ...) {
	
	va_list arguments;
	va_start(arguments, _count);
	
	for(dword i = 0; i < _count && i < counters.size(); ++i) {
		counters[i] = va_arg(arguments, qword);
	}
	
	this->Show();
	
	va_end(arguments);
	return;
}
void cui::counter::Correction(bool _enable) {
	this->correction = _enable;
	return;
}

cui::timer::timer(cui::byte _x_pos, cui::byte _y_pos) : 
	counter(_x_pos, _y_pos, std::string("%dh %dm %ds")) {}
void cui::timer::Show() {
	// If position correction is not set, update initial coordinates
	this->displayed = true;
	if(this->correction) SetConsoleCursorPosition(this->hConsole, this->position);
	else {
		_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
		this->position.X = buffer_info.dwCursorPosition.X;
		this->position.Y = buffer_info.dwCursorPosition.Y;
	}
	// Output what must
	dword i;
	for(i = 0; i < this->counters.size(); ++i) {
		std::cout << this->parts[i];
		if(this->counters[i] < 10) std::cout << 0;
		std::cout << this->counters[i];
	}
	std::cout << this->parts[i];
	// Get final coordinates, and if tey are less than previous ones, erase left part of output
	_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
	int len = this->last_position_x - buffer_info.dwCursorPosition.X;
	this->last_position_x = buffer_info.dwCursorPosition.X;
	for(int i = 0; i < len; ++i) std::cout << ' ';
	// Put the cursor next to the last character, in order to output anything else just after it
	COORD f = {last_position_x, this->position.Y};
	SetConsoleCursorPosition(this->hConsole, f);
	
	return;	
}
void cui::timer::Update(cui::dword _h, cui::dword _m, cui::dword _s) {
	
	this->counters[0] = _h;
	this->counters[1] = _m;
	this->counters[2] = _s;
	return;
}
void cui::timer::Update(cui::qword _sec_tot) {
	
	this->counters[2] = _sec_tot % 60;
	this->counters[1] = ((_sec_tot - this->counters[2]) / 60) % 60;
	this->counters[0] = _sec_tot / 3600;
	return;
}
void cui::timer::Update_n_Show(cui::dword _h, cui::dword _m, cui::dword _s) {
	
	this->Update(_h, _m, _s);
	this->Show();
	return;
}
void cui::timer::Update_n_Show(cui::qword _sec_tot) {
	
	this->Update(_sec_tot);
	this->Show();
	return;
}

cui::memory::memory(cui::byte _x_pos, cui::byte _y_pos) : 
	counter(_x_pos, _y_pos, std::string("%d")) {}
void cui::memory::Show() {
	
	this->displayed = true;
	if(this->correction) SetConsoleCursorPosition(this->hConsole, this->position);
	else {
		_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
		this->position.X = buffer_info.dwCursorPosition.X;
		this->position.Y = buffer_info.dwCursorPosition.Y;
	}
	
		 if(this->counters[0] < 1024) std::cout << this->counters[0] << " B";
	else if(this->counters[0] < 1048576) std::cout << this->counters[0] / 1024 << " KB";
	else if(this->counters[0] < 1073741824) std::cout << this->counters[0] / 1048576 << " MB";
	else if(this->counters[0] < 1099511627776LL) std::cout << this->counters[0] / 1073741824 << " GB";
	else if(this->counters[0] < 1125899906842624LL) std::cout << this->counters[0] / 1099511627776LL << " TB";
	else if(this->counters[0] < 1152921504606846976LL) std::cout << this->counters[0] / 1099511627776LL << " PB";
	
	_CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(this->hConsole, &buffer_info);
	int len = this->last_position_x - buffer_info.dwCursorPosition.X;
	this->last_position_x = buffer_info.dwCursorPosition.X;
	for(int i = 0; i < len; ++i) std::cout << ' ';
	
	COORD f;
	f.X = this->last_position_x;
	f.Y = this->position.Y;
	SetConsoleCursorPosition(this->hConsole, f);
	
	return;	
}
void cui::memory::Update(qword _mem_bytes) {
	this->counters[0] = _mem_bytes;
	return;
}
void cui::memory::Update_n_Show(qword _mem_bytes) {
	this->Update(_mem_bytes);
	this->Show();
	return;
}
