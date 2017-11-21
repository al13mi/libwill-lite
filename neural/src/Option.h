#pragma once
#include <string>
#include "others/INIReader.h"
#include "types.h"
#include <algorithm>
#include "others/libconvert.h"
#include <functional>
#include <typeinfo>

//该类用于读取配置文件，并转换其中的字串设置为枚举
//注意实数只获取双精度数，如果是单精度模式会包含隐式转换
//获取整数的时候，先获取双精度数并强制转换
struct Option
{
    Option();
    Option(const std::string& filename) : Option() { loadIniFile(filename); }

    void initMaps();

    ~Option() { }

private:
    INIReader ini_reader_;
    std::string default_section_ = "will";

    std::string dealString(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        convert::replaceAllString(str, "_", "");
        return str;
    }

public:
    //载入ini文件
    void loadIniFile(const std::string& filename);
    void loadIniString(const std::string& content);

    //默认section
    void setDefautlSection(const std::string& section) { default_section_ = section; }
    const std::string& getDefautlSection() { return default_section_; }

    //从默认section提取
    int getInt(const std::string& key, int default_value = 0)
    {
        return int(getRealFromSection(default_section_, key, default_value));
    }
    double getReal(const std::string& key, double default_value = 0.0)
    {
        return getRealFromSection(default_section_, key, default_value);
    }
    std::string getString(const std::string& key, std::string default_value = "")
    {
        return getStringFromSection(default_section_, key, default_value);
    }

    //从指定section提取
    int getIntFromSection(const std::string& section, const std::string& key, int default_value = 0)
    {
        return int(ini_reader_.GetReal(section, key, default_value));
    }
    double getRealFromSection(const std::string& section, const std::string& key, double default_value = 0.0)
    {
        return ini_reader_.GetReal(section, key, default_value);
    }
    std::string getStringFromSection(const std::string& section, const std::string& key, std::string default_value = "");

    //先找公共部分，再找section部分
    int getIntFromSection2(const std::string& section, const std::string& key, int default_value);
    real getRealFromSection2(const std::string& section, const std::string& key, real default_value);
    std::string getStringFromSection2(const std::string& section, const std::string& key, std::string default_value);

    bool hasSection(const std::string section) { return ini_reader_.HasSection(section); }
    bool hasOption(const std::string section, const std::string key) { return ini_reader_.HasOption(section, key); }

    std::vector<std::string> getAllSections() { return ini_reader_.GetAllSections(); }
    void setOption(std::string section, std::string key, std::string value) { ini_reader_.SetOption(section, key, value); }
    void setOptions(std::string section, std::string pairs);
    void setOptions(std::string section, std::vector<std::string> pairs);

    void print() { ini_reader_.print(); }

private:
    std::map<std::pair<std::string, std::string>, int> map_;
    template<typename T>
    void registerEnum(std::vector<std::pair<std::string, T>> members)
    {
        for (auto m : members)
        {
            map_[{typeid(T).name(), dealString(m.first)}] = m.second;
        }
    }
public:
    template<typename T>
    T transEnum(std::string value_str)
    {
        return T(map_[{ typeid(T).name(), value_str }]);
    }

    template<typename T>
    T getEnum(std::string key, T default_value = T(0))
    {
        return getEnumFromSection<T>(default_section_, key, default_value);
    }

    template<typename T>
    T getEnumFromSection(std::string section, std::string key, T default_value = T(0))
    {
        std::string value_str = getStringFromSection(section, dealString(key), "");
        value_str = dealString(value_str);
        std::pair<std::string, std::string> p = { typeid(T).name(), value_str };
        if (map_.count(p) > 0)
        {
            return T(map_[p]);
        }
        else
        {
            if (!value_str.empty())
            {
                fprintf(stderr, "Undefined value %s for %s, please check the spelling.\n", value_str.c_str(), key.c_str());
            }
            return default_value;
        }
    }

    template<typename T>
    T getEnumFromSection2(std::string section, std::string key, T default_value = T(0))
    {
        T a = getEnum(key, default_value);
        a = getEnumFromSection(section, key, a);
        return a;
    }

};

#define OPTION_GET_VALUE_INT(op, v, default_v) v = op->getInt(#v, default_v)
