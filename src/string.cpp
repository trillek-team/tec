#include "string.hpp"

#include <algorithm>

#include <locale>
#include <codecvt>


namespace tec {

// Encoding conversion stuff thanks to : http://stackoverflow.com/a/18374698

//setup converter between UTF-8 <-> wstring encoding
typedef std::codecvt_utf8<wchar_t> convert_utf8;

std::string utf8_encode(const std::wstring &wstr) {
	if (wstr.empty()) {
		return std::string();
	}
	std::wstring_convert<convert_utf8, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return converter.to_bytes(wstr);
}

std::wstring utf8_decode(const std::string &str) {
	if (str.empty()) {
		return std::wstring();
	}
	std::wstring_convert<convert_utf8, wchar_t> converter;
	
	return converter.from_bytes(str);
}

}
