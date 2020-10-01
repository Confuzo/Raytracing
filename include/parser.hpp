#ifndef _PARSER_
#define _PARSER_
#include "../lib/pugixml.hpp"
#include <string>
#include <map>
#include <iostream>
#include <variant>
#include <vector>
#include "paramset.hpp"
#include "api.hpp"

class Parser
{

private:
    Paramset<std::string, std::string> cameraParams;
    Paramset<std::string, std::string> filmParams;
    Paramset<std::string, std::string> backgroundParams;
    Paramset<std::string, std::string> materialParams;
    std::vector<Paramset<std::string, std::string>> objectParams;

    void addItemToParamSet(pugi::xml_node *node, Paramset<std::string, std::string> *ps)
    {
        for (pugi::xml_attribute attr : (*node).attributes())
        {
            ps->add(attr.name(), attr.value());
        }
    }

    std::string path;
    Api* api;

public:
    Parser(std::string path, Api &api)
    {
        this->path = path;
        this->api = &api;
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
                this->addItemToParamSet(&n, &cameraParams);
            }
            if(!aux.compare("lookat"))
            {
                this->addItemToParamSet(&n, &cameraParams);
            }
            else if (!aux.compare("film"))
            {
                this->addItemToParamSet(&n, &filmParams);
            }
            else if (!aux.compare("background"))
            {
                this->addItemToParamSet(&n, &backgroundParams);
            }
            else if(!aux.compare("material")){
                this->addItemToParamSet(&n, &materialParams);
            }
            else if(!aux.compare("object")){
                Paramset<std::string, std::string> object_;
                this->addItemToParamSet(&n, &object_);
                objectParams.push_back(object_);
            }
        }

        api->CAMERA(cameraParams);
        api->FILM(filmParams);
        api->BACKGROUND(backgroundParams);
        api->MATERIAL(materialParams);
        api->OBJECTS(objectParams);

        return 0;
    }

    void printLoadedScene(){
        std::cout << "Camera params\n" << std::endl;
        cameraParams.print();
        std::cout << std::endl;
        std::cout << "Film params\n" << std::endl;
        filmParams.print();
        std::cout << std::endl;
        std::cout << "Background params\n" << std::endl;
        backgroundParams.print();
    }
};

#endif