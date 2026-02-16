#pragma once
#include "../globals.h"

struct PartModel {
    ModelGroup group;
    std::vector<ngl_object*> models; //Supports multiple objects in an obj, such as flags on capsules, handrails and such
};

class PartLoader {
public:
    int load_parts(Bundle* _parts);

    int get_part_by_id(unsigned int id);
    //Make this a dict?
    std::vector<PartModel> list;
private:
    std::vector<std::string> folders;
};
    