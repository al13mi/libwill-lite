// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "ini.h"
#include "INIReader.h"
#include <string>

using std::string;

void INIReader::load(std::string content)
{
    //convert::replaceAllString(content, "\r", "\n");
    //content += "\n";
    _error = ini_parse_content(content.c_str(), ValueHandler, this);
}

int INIReader::ParseError() const
{
    return _error;
}

string INIReader::Get(string section, string name, string default_value) const
{
    string key = MakeKey(section, name);
    // Use _values.find() here instead of _values.at() to support pre C++11 compilers
    return _values.count(key) ? _values.find(key)->second : default_value;
}

long INIReader::GetInteger(string section, string name, long default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::GetReal(string section, string name, double default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::GetBoolean(string section, string name, bool default_value) const
{
    string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
    {
        return true;
    }
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
    {
        return false;
    }
    else
    {
        return default_value;
    }
}

bool INIReader::HasSection(std::string section)
{
    for (auto& value : _values)
    {
        if (value.first.find_first_of(section + "=") == 0)
        {
            return true;
        }
    }
    return false;
}

bool INIReader::HasOption(std::string section, std::string name)
{
    for (auto& value : _values)
    {
        if (value.first == section + "=" + name)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> INIReader::GetAllSections()
{
    std::vector<std::string> ret;
    for (auto& value : _values)
    {
        auto p = value.first.find_first_of("=");
        if (p != std::string::npos)
        {
            ret.push_back(value.first.substr(0, p));
        }
    }
    std::sort(ret.begin(), ret.end());
    ret.erase(std::unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

void INIReader::SetOption(std::string section, std::string name, std::string value)
{
    string key = MakeKey(section, name);
    _values[key] = value;
}

void INIReader::print()
{
    for (auto& value : _values)
    {
        fprintf(stdout, "%s, %s\n", value.first.c_str(), value.second.c_str());
    }
}

string INIReader::MakeKey(string section, string name)
{
    //remove "_"
    int pos = name.find("_");
    while (pos >= 0)
    {
        name.erase(pos, 1);
        pos = name.find("_", pos);
    }
    string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
    const char* value)
{
    INIReader* reader = (INIReader*)user;
    string key = MakeKey(section, name);
    //if (reader->_values[key].size() > 0)
    //{ reader->_values[key] += "\n"; }
    reader->_values[key] = value;
    return 1;
}
