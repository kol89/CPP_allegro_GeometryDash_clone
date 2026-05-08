#include "include.hpp"

level load_level(std::string file_name){
    std::ifstream inp(file_name);
    std::ostringstream ss;
    ss << inp.rdbuf(); 
    inp.close();
    std::string err;
    json11::Json l = json11::Json::parse(ss.str(),err);

    std::vector<block> colidable;
    for (const auto& b : l["Block"].array_items()){
        colidable.push_back(block(b["x"].number_value(), b["y"].number_value()));
    }

    std::vector<spike> danger;
    for (const auto& b : l["Spike"].array_items()){
        danger.push_back(spike(b["x"].number_value(), b["y"].number_value()));
    }

    std::vector<portal> portals;
    for (const auto& b : l["Portal"].array_items()){
        char mode = b["mode"].string_value()[0];
        double x = b["x"].number_value();
        double y = b["y"].number_value();
        portals.push_back(portal(x, y, mode));
    }

    std::vector<speed_portal> speed;
    for (const auto& b : l["Speed"].array_items()){
        char spd = b["spd"].number_value();
        double x = b["x"].number_value();
        double y = b["y"].number_value();
        speed.push_back(speed_portal(x, y, spd));
    }

    std::vector<orb> orbs;
    for (const auto& b : l["Orb"].array_items()){
        char type = b["type"].string_value()[0];
        double x = b["x"].number_value();
        double y = b["y"].number_value();
        orbs.push_back(orb(x, y, type));
    }

    std::vector<pad> pads;
    for (const auto& b : l["Pad"].array_items()){
        char type = b["type"].string_value()[0];
        double x = b["x"].number_value();
        double y = b["y"].number_value();
        pads.push_back(pad(x, y, type));
    }

    char mode = l["mode"].string_value()[0];
    double sp = l["SP"].number_value();
    double end = l["end"].number_value();

    return level(danger,colidable,portals,speed,orbs,pads,end,mode,sp);
}