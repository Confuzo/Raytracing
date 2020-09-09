#ifndef _PARSER_
#define _PARSER_
#include "../lib/pugixml-1.10/src/pugixml.hpp"
#include <string>
#include <map>
#include <iostream>
#include <variant>
#include "paramset.hpp"


class Parser
{

private:
    ParamSet<std::string, std::string> *cameraParams;
    ParamSet<std::string, std::string> *filmParams;
    ParamSet<std::string, std::string> *backgroundParams;

    void addItemToParamSet(pugi::xml_node *node, ParamSet<std::string, std::string> *ps)
    {
        for (pugi::xml_attribute attr : (*node).attributes())
        {
            ps->add(attr.name(), attr.value());
        }
    }

    std::string path;

public:
    Parser(std::string path)
    {
        this->path = path;
    }

    ~Parser() = default;

    int loadScene()
    {

        pugi::xml_document document;

        if (!document.load_file(path.c_str()))
        {
            return -1;
        }

        pugi::xml_node nodes = document.child("RT3");

        for (pugi::xml_node n : nodes)
        {
            std::string aux = n.name();
            if (!aux.compare("camera"))
            {
                this->addItemToParamSet(&n, cameraParams);
            }
            else if (!aux.compare("film"))
            {
                this->addItemToParamSet(&n, filmParams);
            }
            else if (!aux.compare("background"))
            {
                this->addItemToParamSet(&n, backgroundParams);
            }
        }

        return 0;
    }

    // void print(){
    //     for(auto const& pair : *filmParams){
    //         std::cout << pair.first << " " << pair.second << std::endl;
    //     }
    // }
};

#endif