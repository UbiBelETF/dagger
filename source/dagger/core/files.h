#pragma once

#include "core/core.h"

static String ReadFromFile(String path_)
{
	FileInputStream stream(path_.c_str());

	if (!stream.good())
		return "";

	String str;

	stream.seekg(0, std::ios::end);
	str.reserve(stream.tellg());
	stream.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());

	return str;
}