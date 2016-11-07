# ifndef __NovelLayer__
# define __NovelLayer__

# include "../LayerBase.h"

# include "TextLabels.h"
# include "TextChunkManager.h"

namespace User
{
    class SwitchBoolean
    {
    public:
        SwitchBoolean( ) : frag( false ) { }
        SwitchBoolean( bool frag ) : frag( frag ) { }
    public:
        void on( ) { frag = true; }
        void off( ) { frag = false; }
    public:
        operator bool( ) { return frag; }
        bool operator!( ) { return !frag; }
    private:
        bool frag;
    };

    class NovelLayer : public LayerBase
    {
    public:
        CREATE_ARGS_FUNC( NovelLayer );
        NovelLayer( std::string const& novelPath );
        ~NovelLayer( );
        bool init( ) override;
        void setup( ) override;
        void update( float delta )override;
    public:
        void on( );
        void off( );
    public:
        void setNextChild( std::string const& name );
        void setDelayTime( double delayTime ) { textChunkManager.setDelayTime( delayTime ); }
        // 選択肢でシナリオの読み込み停止機能のスイッチ
        SwitchBoolean systemStop;
        void textUpdate( );
    private:
        std::string novelPath;
        // 読み込み機能を停止するかどうか。
        // テキストを読み込み途中の場合はtrueになります。
        void readingProceedUpdate( );
        void textNextRead( );
        void textActionStop( );
        // テキストデータを空にする。
        void textClear( );
        // 読み込んだテキストデータをノベルレイヤーに貼り付ける。
        void textPasting( );
        // 読み込みが停止されるまで読み込むかのスイッチ
        SwitchBoolean systemRead;
        // 高速に読み飛ばす機能のスイッチ
        SwitchBoolean readProceed;

        TextLabels textLabels;
        TextChunkManager textChunkManager;
    };
}

# endif // __NovelLayer__
