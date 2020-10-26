#pragma once

#include <string>
#include <regex>

std::string ltrim(const std::string& s)
{
    static const std::regex lws{ "^[[:space:]]*", std::regex_constants::extended };
    return std::regex_replace(s, lws, "");
}

std::string rtrim(const std::string& s)
{
    static const std::regex tws{ "[[:space:]]*$", std::regex_constants::extended };
    return std::regex_replace(s, tws, "");
}

std::string trim(const std::string& s)
{
    return ltrim(rtrim(s));
}

bool ends_with(std::string const& fullString, std::string const& ending) 
{
    if (fullString.length() >= ending.length()) 
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else 
    {
        return false;
    }
}
