#pragma once
#include "../globals.h"
#include <unordered_map>

struct ProtoPart {
    ModelGroup group;
    std::vector<ngl_object*> models; //Supports multiple objects in an obj, such as flags on capsules, handrails and such
    std::string path;   //Path of part data
    uint16_t shared_id;
};

class PartLoader {
public:
    int load_parts(Bundle* _parts);

    int get_part_by_id(unsigned int id);
    //Make this a dict?

private:
    
    int push_raw(Bundle* parts);

    void config_part(ProtoPart* _pt);


    Bundle* _parts;
    std::vector<std::string> folders;
    std::vector<ProtoPart> raw_parts;
    std::unordered_map<int, size_t> tracked_parts;
};
    