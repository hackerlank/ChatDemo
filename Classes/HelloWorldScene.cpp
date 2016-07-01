#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "VoiceManager.hpp"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "res/CloseNormal.png",
                                           "res/CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("res/HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    auto button1 = MenuItemLabel::create(Label::createWithTTF("voice", "fonts/Marker Felt.ttf", 24),  CC_CALLBACK_1(HelloWorld::buttonClick, this, 1));
    button1->setTag(1);
    button1->setPosition(Vec2(260, 200));


    auto button2 = MenuItemLabel::create(Label::createWithTTF("play", "fonts/Marker Felt.ttf", 24),  CC_CALLBACK_1(HelloWorld::buttonClick, this, 2));
    button2->setTag(2);
    button2->setPosition(Vec2(260, 100));


    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, button1, button2, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    _voiceManager = new VoiceManager();
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    CCLOG(">>>>>>>>>>>>>>>>>buttonClick");
    _voiceManager->startPlay();
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
}

void HelloWorld::buttonClick(Ref * sender, int tag)
{
    CCLOG(">>>>>>>>>>>>>>>>>buttonClick %d", tag);
    if (tag == 1) {
        if (!_switch) {
            _voiceManager->startRecord();
            _switch = true;
        }
        else {
            _voiceManager->stopRecord();
            _switch = false;
        }
    }
    else {
        _voiceManager->startPlay();
    }
}

