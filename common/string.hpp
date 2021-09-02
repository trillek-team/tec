#pragma once
/**
* String handling stuff
*/

#include <string>
#include <vector>

namespace tec {

/**
	* \brief Convert a wide Unicode string to an UTF8 string
	*
	* \param wstr wide string on unicode. On Windows -> UTF16. On Linux -> UTF32
	* \return std::string with UTF-8 encoding. Empty string if fails
	*/
std::string utf8_encode(const std::wstring& wstr);

/**
	* \brief Convert an UTF8 string to a wide Unicode String
	*
	* \param str UTF8 string
	* \return std::wstring on unicode system encoding. Empty string if fails
	*/
std::wstring utf8_decode(const std::string& str);

std::vector<std::string> SplitString(std::string args, std::string deliminator = " ");

} // namespace tec
