#pragma once
#include <string>
#include "others/INIReader.h"
#include "types.h"
#include <algorithm>
#include "others/libconvert.h"
#include <functional>

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
    int getInt(const std::string& name, int default_value = 0)
    {
        return int(getRealFromSection(default_section_, name, default_value));
    }
    double getReal(const std::string& name, double default_value = 0.0)
    {
        return getRealFromSection(default_section_, name, default_value);
    }
    std::string getString(const std::string& name, std::string default_value = "")
    {
        return getStringFromSection(default_section_, name, default_value);
    }

    //从指定section提取
    int getIntFromSection(const std::string& section, const std::string& name, int default_value = 0)
    {
        return int(ini_reader_.GetReal(section, name, default_value));
    }
    double getRealFromSection(const std::string& section, const std::string& name, double default_value = 0.0)
    {
        return ini_reader_.GetReal(section, name, default_value);
    }
    std::string getStringFromSection(const std::string& section, const std::string& name, std::string default_value = "");

    //先找公共部分，再找section部分
    int getIntFromSection2(const std::string& section, const std::string& name, int default_value);
    real getRealFromSection2(const std::string& section, const std::string& name, real default_value);
    std::string getStringFromSection2(const std::string& section, const std::string& name, std::string default_value);

    bool hasSection(const std::string section) { return ini_reader_.HasSection(section); }
    bool hasOption(const std::string section, const std::string name) { return ini_reader_.HasOption(section, name); }

    std::vector<std::string> getAllSections() { return ini_reader_.GetAllSections(); }
    void setOption(std::string section, std::string name, std::string value) { ini_reader_.SetOption(section, name, value); }
    void setOptions(std::string section, std::string name_values);
    void setOptions(std::string section, std::vector<std::string> name_values);

    void print() { ini_reader_.print(); }

private:
    template<typename T>
    T getEnumValue(std::string name, std::map<std::string, T> map)
    {
        name = dealString(name);
        if (map.find(name) != map.end())
        {
            return map[name];
        }
        else
        {
            fprintf(stderr, "Undefined type %s, please check the spelling.\n", name.c_str());
            return T(0);
        }
    }

    template<typename T>
    void addMemberVector(std::map <std::string, T> map, std::vector<std::pair<std::string, T>> member)
    {

    }

    //转换关键字为小写，去掉下划线
    template<typename T>
    void dealStringForMap(std::map <std::string, T>& map)
    {
        for (auto member : map)
        {
            map[dealString(member.first)] = member.second;
        }
    }

#define GET_TYPE(type)\
private: \
    std::map<std::string, type> map_##type##_;\
public: \
    type get##type##FromSection(std::string section, std::string name, std::string default_name = "") \
    { \
        if (default_name.empty()) { default_name = map_##type##_.begin()->first; } \
        return getEnumValue(getStringFromSection(section, name, default_name), map_##type##_); \
    } \
    type get##type(std::string name, std::string default_name) \
    { \
        return get##type##FromSection(default_section_, name, default_name); \
    } \
    type get##type(std::string name) \
    { \
        return getEnumValue(name, map_##type##_); \
    }

    GET_TYPE(ActiveFunctionType);
    GET_TYPE(LayerConnectionType);
    GET_TYPE(CostFunctionType);
    GET_TYPE(PoolingType);
    GET_TYPE(CombineType);
    GET_TYPE(RandomFillType);
    GET_TYPE(AdjustLearnRateType);
    GET_TYPE(BatchNormalizationType);
    GET_TYPE(RecurrentType);
    GET_TYPE(RecurrentDirectionType);
    GET_TYPE(RecurrentInputType);
    GET_TYPE(RecurrentAlgoType)
    GET_TYPE(SolverType);
    GET_TYPE(WorkModeType);
    GET_TYPE(PruneType);

#undef GET_TYPE

};

#define OPTION_GET_VALUE_INT(op, v, default_v) v = op->getInt(#v, default_v)
