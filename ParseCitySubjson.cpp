#include <vector>
#include <string>
#include "json.h"

using namespace std;

struct Language;
struct City;
struct Country;

struct CityData {
    string name;
    string iso_code;
    string phone_code;
};

struct CountryInfo {
    string name;
    string iso_code;
    string phone_code;
    string time_zone;
    vector<Language> languages;
};

vector<City> ParseCitySubjson(const Json& json, const CountryInfo& country_info) {
    vector<City> cities;
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({
            city_obj["name"s].AsString(),
            city_obj["iso_code"s].AsString(),
            country_info.phone_code + city_obj["phone_code"s].AsString(),
            country_info.name,
            country_info.iso_code,
            country_info.time_zone,
            country_info.languages
        });
    }
    return cities;
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        
        CountryInfo country_info {
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            {}
        };
        
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country_info.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        
        countries.push_back({
            country_info.name,
            country_info.iso_code,
            country_info.phone_code,
            country_info.time_zone,
            country_info.languages
        });
        
        auto country_cities = ParseCitySubjson(country_obj["cities"s], country_info);
        cities.insert(cities.end(), country_cities.begin(), country_cities.end());
    }
}
