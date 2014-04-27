#include "String.h"
#include "Array.h"
#include <string>
#include <iomanip>
#include <cstring>

namespace n
{
namespace core
{

char String::nullString = 0;

String operator+(const char *cst, String s) {
	return String(cst) + s;
}

String::Concat::Concat(const String &a, const String &b) : tokens(4) {
	tokens.append(a);
	tokens.append(b);
}

uint String::Concat::size() const {
	uint s = 0;
	for(const String &str : tokens) {
		s += str.size();
	}
	return s;
}

String::Concat &String::Concat::operator+(const String &s) {
	tokens.append(s);
	return *this;
}

String::Concat &String::Concat::operator+(const Concat &c) {
	for(const String &s : c.tokens) {
		tokens.append(s);
	}
	return *this;
}

Array<String> String::Concat::getTokens() const {
	return tokens;
}

String::String() : String(0, 0) {
}

String::String(const char *cst) : String(cst, cst ? strlen(cst) : 0) {
}

String::String(const char c) : String(&c, 1) {
}

String::String(const char *cst, uint l) : length(l), count(l ? new uint(1) : 0), data(0) {
	if(l) {
		data = (char *)malloc((length + 1) * sizeof(char));
		if(cst) {
			memcpy(data, cst, length * sizeof(char));
		}
		data[length] = '\0';
	}
}

String::String(const Concat &sc) : length(sc.size()), count(new uint(1)) {
	char *d = data = (char *)malloc((length + 1) * sizeof(char));
	for(const String &str : sc.getTokens()) {
		memcpy(d, str.data, str.size() * sizeof(char));
		d += str.size();
	}
	data[length] = '\0';
}

String::String(const String &str) : length(str.length), count(str.count), data(str.data) {
	if(count) {
		(*count)++;
	}
}

String::String(String &&str) : length(0), count(0), data(0) {
	swap(str);
}

String::~String() {
	if(data) {
		if(*count == 1) {
			delete count;
			free(data);
		} else {
			(*count)--;
		}
	} else {
		delete count;
	}
}


void String::replace(const String &oldS, const String &newS) {
	while(true) {
		uint index = find(oldS);
		if(index == (uint)-1) {
			break;
		}
		operator=(subString(0, index) + newS + subString(index + oldS.size()));
	}
}

String String::replaced(const String &oldS, const String &newS) const {
	String str = *this;
	while(true) {
		uint index = str.find(oldS);
		if(index == (uint)-1) {
			break;
		}
		str = str.subString(0, index) + newS + str.subString(index + oldS.size());
	}
	return str;
}

void String::clear() {
	detach(0);
	length = 0;
}

uint String::size() const {
	return length;
}

bool String::isEmpty() const {
	return !length;
}

char const *String::toChar() const {
	if(!data) {
		return &nullString;
	}
	return data;
}

uint String::find(char c, uint from) const {
	if(!(from < length)) {
		return -1;
	}
	for(uint i = from; i != length; i++) {
		if(data[i] == c) {
			return i;
		}
	}
	return -1;
}

uint String::find(const String &str, uint from) const {
	if(from + str.size() > length) {
		return -1;
	}
	for(uint i = from, l = length - str.length; i <= l; i++) {
		bool matched = true;
		for(uint j = 0; j != str.length; j++) {
			if(data[i + j] != str.data[j]) {
				matched = false;
				break;
			}
		}
		if(matched) {
			return i;
		}
	}
	return -1;
}

bool String::contains(char c) const {
	return find(c) < length;
}

bool String::contains(String &str) const {
	return find(str) < length;
}

String String::subString(uint beg, uint len) const {
	return String(data + beg, len);
}

String String::subString(uint beg) const {
	return subString(beg, length - beg);
}

void String::detach() {
	detach(length);
}

void String::swap(String &str) {
	uint l = str.length;
	char *ch = str.data;
	uint *c = str.count;
	str.length = length;
	str.data = data;
	str.count = count;
	count = c;
	data = ch;
	length = l;
}

Array<String> String::split(const String &str) const {
	Array<String> arr;
	uint p = -1;
	uint from = 0;
	while((p = find(str, from)) != (uint)-1) {
		arr.append(subString(from, p - from));
		from = p + str.size();
	}
	arr.append(subString(from));
	return arr;
}

float String::toFloat() const {
	return toDouble();
}

double String::toDouble() const {
	return atof(data);
}

int String::toInt() const {
	return atoi(data);
}

String::operator std::string() const {
	return std::string(data);
}

std::string String::toStdString() const {
	return std::string(data);
}

String &String::operator+=(const String &s) {
	detach(length + s.length);
	memmove(data + length, s.data, s.length); // just in case of s = s;
	data[length += s.length] = '\0';
	return *this;
}

String &String::operator+=(String &&s) {
	detach(length + s.length);
	memcpy(data + length, s.data, s.length);
	data[length += s.length] = '\0';
	return *this;
}

String::Concat String::operator+(const String &s) const {
	return Concat(*this, s);
}

String String::operator+(char c) {
	if(!data) {
		return String(c);
	}
	if(length == 0) {
		return String(&c, 1);
	}
	data[length] = c;
	String s(data, length + 1);
	data[length] = '\0';
	return s;
}

bool String::operator==(const String &str) const {
	if(str.length != length) {
		return false;
	}
	for(uint i = 0; i != length; i++) {
		if(data[i] != str.data[i]) {
			return false;
		}
	}
	return true;
}

bool String::operator==(const char *str) const {
	if(length != strlen(str)) {
		return false;
	}
	for(uint i = 0; i != length; i++) {
		if(data[i] != str[i]) {
			return false;
		}
	}
	return true;
}

bool String::operator!=(const String &str) const {
	return !operator==(str);
}

bool String::operator!=(const char *str) const {
	return !operator==(str);
}

const String &String::operator=(const String &s) {
	clear();
	data = s.data;
	count = s.count;
	length = s.length;
	if(count) {
		(*count)++;
	}
	return *this;
}


const String &String::operator=(String &&s) {
	swap(s);
	return *this;
}

const String &String::operator=(const Concat &sc) {
	char *d = detach(sc.size());
	length = sc.size();
	for(const String &str : sc.getTokens()) {
		memcpy(d, str.data, str.size() * sizeof(char));
		d += str.size();
	}
	data[length] = '\0';
	return *this;
}

bool String::operator<(const String &s) const {
	if(data && s.data) {
		for(uint i = 0, min = std::min(length, s.length); i != min; i++) {
			if(data[i] < s.data[i]) {
				return true;
			} else if(data[i] > s.data[i]) {
				return false;
			}
		}
	}
	return length < s.length;
}

String::operator const char *() const {
	return toChar();
}

char const *String::begin() const {
	return data;
}

char const *String::end() const {
	return data + length;
}

String &String::operator<<(const String &s) {
	return this->operator+=(s);
}

uint String::getHash() const {
	return length && data ? hash(data, length) : 0;
}

char *String::detach(uint s) {
	if(!count) {
		if(s) {
			count = new uint(1);
			data = (char *)malloc((s + 1) * sizeof(char));
			*data = '\0';
			return data;
		}
		return data = 0;
	}
	if(*count != 1) {
		(*count)--;
		if(!s) {
			count = 0;
			return data = 0;
		}
		char *d = (char *)malloc((++s) * sizeof(char));
		count = new uint(1);
		memcpy(d, data, std::min(s, length + 1) * sizeof(char));
		return data = d;
	}
	if(!s) {  // count = 0 at this point
		free(data);
		delete count;
		count = 0;
		return data = 0;
	}
	return data = (char *)realloc(data, (s + 1) * sizeof(char));
}

} //core
} //n
