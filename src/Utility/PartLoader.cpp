#include "PartLoader.h"
#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/writer.h"
#include "../../include/rapidjson/stringbuffer.h"

using namespace rapidjson;

int PartLoader::load_parts(Bundle* parts) {
    
    std::vector<uint8_t> raw = parts->load_raw_data("parts/database.json");
    std::string json(raw.begin(),raw.end());

    Document d;
    d.Parse(json.c_str(),json.size());

    if (!d.HasMember("Banks") || !d["Banks"].IsArray()) {
        printf("E 13418: Missing part banks!\n");
        return 2;
    }

    const rapidjson::Value& banks = d["Banks"];

    for (rapidjson::SizeType i = 0; i < banks.Size(); i++) {
        const rapidjson::Value& bank = banks[i];
        
        folders.emplace_back(std::string(bank.GetString()));
    }
    
    printf("READ RECURS\n");

    std::vector<std::string> ls = parts->ls("parts");

    for (std::string &s : ls) {
        printf("GOT %s\n",s.c_str());
    }


    return 0;
}