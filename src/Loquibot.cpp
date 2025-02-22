#include "ServerListener.h"
#include "Loquibot.h"
#include "BlockLevelAlertProtocol.h"
#include "BlockCreatorAlertProtocol.h"
#include "BlockRequesterAlertProtocol.h"
#include <signal.h>
#include "GlobalVars.h"
#include <Geode/Geode.hpp>
#include "RequestsLayer.h"
#include "GJGameLevel.h"
#include "BlockMenu.h"
#include "YouTubeMenu.h"
#include "LevelInfoLayer.h"

using namespace cocos2d;

std::string notice = "Please open <cg>loquibot</c> to continue. If it is open, try again";
std::string title = "loquibot Not Opened!";
std::string confirm = "Okay";

Loquibot* Loquibot::instance = nullptr;

void hideButtons(CCObject* obj) {

    CCMenuItemSpriteExtra* button = reinterpret_cast<CCMenuItemSpriteExtra*>(obj);
    
    auto menu = button->getParent();
    if(menu){
        if (menu->getTag() == 4324) {
            menu = menu->getParent();
        }

        if (menu->getTag() == 4323) {

            menu->setVisible(false);

            auto winSize = CCDirector::sharedDirector()->getWinSize();


            CCSprite* loadingSprite = CCSprite::create("loadingCircle.png");


            loadingSprite->setPosition({ winSize.width - 80, winSize.height / 2 + 70 });
            loadingSprite->setTag(76532);
            loadingSprite->setScale(0.5);
            loadingSprite->setBlendFunc({ GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA });

            loadingSprite->runAction(CCRepeatForever::create(
                CCRotateBy::create(1.0f, 360)
                )
            );

            menu->getParent()->addChild(loadingSprite);
        }
    }
}

void Loquibot::showButtons() {

    CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

	LevelInfoLayer* layer = dynamic_cast<LevelInfoLayer*>(currentScene->getChildren()->objectAtIndex(0));
    
    if(layer){

        auto menu = layer->getChildByTag(4323);

        if(menu){
            menu->setVisible(true);

            if(menu->getParent()->getChildByTag(76532)){
                menu->getParent()->removeChildByTag(76532);
            }
        }
    }
}

void Loquibot::goToLevel(CCObject* obj) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();
    
    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!GlobalVars::getSharedInstance()->loquiOpen) {
        alertLayer->show();
    }
    else {

        ServerListener::sendMessage("current_get");
        GlobalVars::getSharedInstance()->isButtonPressed = true;

        hideButtons(obj);
    }
}

void Loquibot::goToNextLevel(CCObject* obj) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!GlobalVars::getSharedInstance()->loquiOpen) {
        alertLayer->show();
    }
    else {

        ServerListener::sendMessage("next_get");
        GlobalVars::getSharedInstance()->isButtonPressed = true;

        hideButtons(obj);
    }
}

void Loquibot::goToTopLevel(CCObject* obj) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!GlobalVars::getSharedInstance()->loquiOpen) {
        alertLayer->show();
    }
    else {

        ServerListener::sendMessage("top_get");
        GlobalVars::getSharedInstance()->isButtonPressed = true;

        hideButtons(obj);
    }
}


void Loquibot::goToRandomLevel(CCObject* obj) {
   
    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!GlobalVars::getSharedInstance()->loquiOpen) {
        alertLayer->show();
    }
    else {
        ServerListener::sendMessage("random_get");
        GlobalVars::getSharedInstance()->isButtonPressed = true;

        hideButtons(obj);
    }
}

void Loquibot::goToUndoLevel(CCObject* obj) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!GlobalVars::getSharedInstance()->loquiOpen) {
        alertLayer->show();
    }
    else {
        ServerListener::sendMessage("undo_get");
        hideButtons(obj);

        GlobalVars::getSharedInstance()->isButtonPressed = true;
    }
}


void Loquibot::blockLevel(CCObject*) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(new BlockLevelAlertProtocol, "Block Level?", ("Block <cr>" + std::to_string(GlobalVars::getSharedInstance()->currentID) + "</c>?"), "Cancel", "Okay", 300);
    
    alertLayer->show();
}

void Loquibot::blockCreator(CCObject*) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(new BlockCreatorAlertProtocol, "Block Creator?", ("Block <cr>" + GlobalVars::getSharedInstance()->creator + "</c>?"), "Cancel", "Okay", 300);

    alertLayer->show();
}

void Loquibot::blockRequester(CCObject*) {

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(new BlockRequesterAlertProtocol, "Block Requester?", ("Block <cr>" + GlobalVars::getSharedInstance()->requester + "</c>?"), "Cancel", "Okay", 300);

    alertLayer->show();
}

void Loquibot::goToMainScene(CCObject*) {
    auto layer = MenuLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    
    auto transition = CCTransitionFade::create(0.5f, scene);
   
    CCDirector::sharedDirector()->pushScene(transition);
}

void Loquibot::openLevelMenu(CCObject*){

    if (!GlobalVars::getSharedInstance()->loquiOpen) ServerListener::connectAsync();

    ServerListener::sendMessage("list_get");
    
}

void Loquibot::openYoutube(CCObject*){

    ServerListener::sendMessage("get_selected_yt");
}

void Loquibot::showBlockMenu(CCObject*){

    BlockMenu* menu = BlockMenu::create();
    menu->show();

}

void Loquibot::showYouTube(LevelInfoLayer* LevelInfoLayer){

    if(LevelInfoLayer){

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        CCNode* title = LevelInfoLayer->getChildByID("title-label");

        float titleWidth = title->getContentSize().width;

        auto youtubeButtonSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
        auto youtubeButton = CCMenuItemSpriteExtra::create(youtubeButtonSprite, LevelInfoLayer,
            menu_selector(Loquibot::openYoutube));
        youtubeButton->ignoreAnchorPointForPosition(true);

        CCMenu* youtubeButtonMenu = CCMenu::create();
        youtubeButtonMenu->setContentSize({30,30});
        youtubeButtonMenu->ignoreAnchorPointForPosition(false);
        youtubeButtonMenu->setPosition({title->getPositionX() + (titleWidth * title->getScaleX())/2.0f + 20, title->getPositionY()-2});
        youtubeButtonMenu->setScale(0.5f);
        youtubeButtonMenu->addChild(youtubeButton);

        LevelInfoLayer->addChild(youtubeButtonMenu);
    }
}
