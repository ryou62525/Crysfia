#include "ScriptSystem.h"

#include "ScriptStaticData.h"
#include "OptionalValues.h"

#include "SystemLayer.h"
#include "NameLayer.h"
#include "BackgroundLayer.h"
#include "HumanLayer.h"
#include "NovelLayer.h"
#include "SelectLayer.h"

#include "ScriptHuman.h"
#include "ScriptBackground.h"
#include "ScriptName.h"

USING_NS_CC;

namespace User
{
    size_t ScriptSystem::novelIndex = 0;

    ScriptSystem::ScriptSystem( cocos2d::Layer* layer )
        : ScriptBase( layer )
    {
        funcs.insert( std::make_pair( u8"SETUP", [ this ] ( ArgumentList const& args ) { SETUP( ); } ) );

        funcs.insert( std::make_pair( u8"l", [ this ] ( ArgumentList const& args ) { l( ); } ) );
        funcs.insert( std::make_pair( u8"select", [ this ] ( ArgumentList const& args ) { select( args ); } ) );
        funcs.insert( std::make_pair( u8"stop", [ this ] ( ArgumentList const& args ) { stop( args ); } ) );

        funcs.insert( std::make_pair( u8"bgm", [ this ] ( ArgumentList const& args ) { bgm( args ); } ) );
        funcs.insert( std::make_pair( u8"se", [ this ] ( ArgumentList const& args ) { se( args ); } ) );
        funcs.insert( std::make_pair( u8"name", [ this ] ( ArgumentList const& args ) { name( args ); } ) );
        funcs.insert( std::make_pair( u8"human", [ this ] ( ArgumentList const& args ) { human( args ); } ) );
        funcs.insert( std::make_pair( u8"background", [ this ] ( ArgumentList const& args ) { background( args ); } ) );
    }
    ScriptSystem::~ScriptSystem( )
    {

    }
    void ScriptSystem::SETUP( )
    {
        auto systemLayer = dynamic_cast<SystemLayer*>( layer );
        humanLayer = systemLayer->getLayer<HumanLayer>( );
        backgroundLayer = systemLayer->getLayer<BackgroundLayer>( );
        nameLayer = systemLayer->getLayer<NameLayer>( );
        novelLayer = systemLayer->getLayer<NovelLayer>( );
        selectLayer = systemLayer->getLayer<SelectLayer>( );
    }
    void ScriptSystem::l( )
    {
        novelIndex = lineSize;
    }
    void ScriptSystem::select( ArgumentList const & args )
    {
        l( );

        auto novel = dynamic_cast<NovelLayer*>( novelLayer );
        novel->switchIsSelectStopping( );

        auto visibleSize = Director::getInstance( )->getVisibleSize( );

        //ボタンを2つ作成
        std::vector<MenuItemLabel*> buttons;
        buttons.resize( args.size( ) );
        for ( size_t i = 0; i < buttons.size( ); ++i )
        {
            auto label = Label::createWithTTF( args[i], u8"res/fonts/F910MinchoW3.otf", OptionalValues::fontSize );
            buttons[i] = MenuItemLabel::create( label, [ = ] ( Ref* pSender )
            {
                novel->setNextChild( args[i] );
                // テキストの中身を消します。
                novel->textClear( );
                // 読み込みを開始します。
                novel->switchIsSystemRead( );
            } );
        }

        //2つのボタン間の隙間を50に、縦並びにして画面中央に配置
        Menu* menu = nullptr;
        switch ( buttons.size( ) )
        {
        case 2:
            menu = Menu::create( buttons[0], buttons[1], nullptr );
            break;
        case 3:
            menu = Menu::create( buttons[0], buttons[1], buttons[2], nullptr );
            break;
        case 4:
            menu = Menu::create( buttons[0], buttons[1], buttons[2], buttons[3], nullptr );
            break;
        case 5:
            menu = Menu::create( buttons[0], buttons[1], buttons[2], buttons[3], buttons[4], nullptr );
            break;
        default:
            break;
        }
        
        if ( menu )
        {
            menu->setPosition( Vec2( visibleSize.width * 0.5F, visibleSize.height * 0.5F ) );
            menu->alignItemsVerticallyWithPadding( OptionalValues::fontSize );
            selectLayer->addChild( menu );
        }
    }
    void ScriptSystem::stop( ArgumentList const & args )
    {

    }
    void ScriptSystem::name( ArgumentList const & args )
    {
        std::string str;
        if ( 1 == args.size( ) )
        {
            str = args[0];
            std::string humanName = str;
            auto pos = str.find( u8"名前" );
            if ( pos != std::string::npos ) humanName = str.substr( pos + std::string( u8"名前" ).size( ) );

            auto script = std::make_unique<ScriptName>( nameLayer, humanName, u8"F910MinchoW3.otf" );
            ScriptStaticData::addData( std::make_pair( str, std::move( script ) ) );
        }
    }
    void ScriptSystem::background( ArgumentList const & args )
    {
        std::string str;
        if ( 1 == args.size( ) )
        {
            str = args[0];
            ScriptStaticData::addData( std::make_pair( str, std::make_unique<ScriptBackground>( backgroundLayer, str + u8".png" ) ) );
        }
    }
    void ScriptSystem::bgm( ArgumentList const & args )
    {

    }
    void ScriptSystem::se( ArgumentList const & args )
    {

    }
    void ScriptSystem::human( ArgumentList const & args )
    {
        std::string str;
        if ( 1 == args.size( ) )
        {
            str = args[0];
            ScriptStaticData::addData( std::make_pair( str, std::make_unique<ScriptBackground>( humanLayer, str + u8".png" ) ) );
        }
    }
}
