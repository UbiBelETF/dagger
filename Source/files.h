#pragma once

#include <string>
#include <fstream>
#include <streambuf>

static std::string ReadFromFile(std::string path_)
{
	std::ifstream t(path_.c_str());

	if (!t.good())
		return "";

	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	
	return str;
}