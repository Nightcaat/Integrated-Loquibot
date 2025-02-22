#pragma once

#ifndef __GLOBALVARS_H
#define __GLOBALVARS_H

#include <Geode/Geode.hpp>

class GlobalVars {


protected:
    static GlobalVars* instance;
public:
    GJGameLevel* levelData;

    std::string creator;
    std::string requester;
    int accountID;
    int currentID;

    bool isEmpty;
    bool isStartLevel;
    bool isButtonPressed;
    bool isLoquiMenu;
    bool deleting;
    bool loquiOpen;
    bool isSearchScene;
    bool isViewer;
    bool onReqScene;
    int idWithYouTube;

    matjson::Array currentLevelList;

    static GlobalVars* getSharedInstance(){

        if (!instance) {
            instance = new GlobalVars();
        };
        return instance;
    }

};


#endif