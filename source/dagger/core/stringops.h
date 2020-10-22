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