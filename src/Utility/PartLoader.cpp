#include "PartLoader.h"
#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/writer.h"
#include "../../include/rapidjson/stringbuffer.h"


using namespace rapidjson;


//Get part->json
void PartLoader::config_part(ProtoPart* _pt) {
    Part* part = &_pt->default_data;

    //files (no paths)
    auto full_list = _parts->ls(_pt->path);

    //Detect parts.json in the folder
    if (!(std::find(full_list.begin(),full_list.end(),"part->json") != full_list.end()))
        return;


    std::vector<uint8_t> raw = _parts->load_raw_data((_pt->path + "/part->json").c_str());

    std::string json(raw.begin(),raw.end());

    Document d;
    d.Parse(json.c_str(),json.size());


    if (d.HasMember("id") && d["id"].IsInt()) {
        int id = d["id"].GetInt();
        printf("ID %d\n",id);
    }

    std::string ICON;
    if (d.HasMember("staging_icon") && d["staging_icon"].IsString()) ICON = d["staging_icon"].GetString();

    part->icon = Staging_Icons::CAPSULE;
    //WIP
    // switch (ICON) {
    //     case ("Command")
    // }

    //COMMON
    if (d.HasMember("id") && d["id"].IsInt()) _pt->shared_id = part->shared_id = d["id"].GetInt();
    if (d.HasMember("name") && d["name"].IsString()) part->name = d["name"].GetString();
    if (d.HasMember("description") && d["description"].IsString()) part->description = d["description"].GetString();
    if (d.HasMember("category") && d["category"].IsString()) part->category = d["category"].GetString();
    if (d.HasMember("model") && d["model"].IsString()) _pt->objname = d["model"].GetString();


    if (d.HasMember("mass") && d["mass"].IsNumber()) part->mass = d["mass"].GetFloat();
    if (d.HasMember("drag_min") && d["drag_min"].IsNumber()) part->drag_min = d["drag_min"].GetFloat();
    if (d.HasMember("drag_max") && d["drag_max"].IsNumber()) part->drag_max = d["drag_max"].GetFloat();
    if (d.HasMember("drag_ang") && d["drag_ang"].IsNumber()) part->drag_ang = d["drag_ang"].GetFloat();
    if (d.HasMember("temp_max") && d["temp_max"].IsNumber()) part->temp_max = d["temp_max"].GetFloat();

}

int PartLoader::load_parts(Bundle* parts) {
    _parts = parts;
    //Get part paths
    if (push_raw(parts) != 0) return 4;

    //Read config
    for (ProtoPart &p : raw_parts) {

        config_part(&p);
    }



    //Load model

    //Track parts
    printf("TRACK PARTS!!\n");

    return 0;
}

//Get part data paths
int PartLoader::push_raw(Bundle* parts) {
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

    //Find all parts, push them to raw
    for (std::string &s : folders) {
        std::string folder_path = "parts/" + s;
        std::vector<std::string> ls = parts->ls(folder_path);
        for (std::string &s2 : ls) {
            ProtoPart part;
            if (s2.find_first_of('.') != std::string::npos) continue; //Skip stray files
            std::string part_path = folder_path + "/" + s2;
            part.path = part_path;
            raw_parts.push_back(std::move(part));
        }

    }
    return 0;
}