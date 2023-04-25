# pragma once

# include <Siv3D.hpp>

namespace Common
{

struct Music
{
    String Album, ID, Title, Desc, Staff, Tools;
    Music(String album, String title, String desc, String staff, String tools):
    Album(album),
    ID(album+U"_"+title),
    Title(title),
    Desc(desc),
    Staff(staff),
    Tools(tools)
    {}
    
    void draw(Vec2 pos, int32 id) const;
    RectF getRegion(Vec2 pos) const;
};

struct Album
{
    String Title, Auth, Desc;
    Array<Music>m_Musics;
    Transition FadeTransition;
    
    Album(){}
    Album(String title, String auth, String desc, const Array<Music>& musics)
    :Title(title),
    Auth(auth),
    Desc(desc),
    m_Musics(musics),
    FadeTransition(0.1s, 0.1s){}
    
    void update(Vec2 pos);
    void draw(Vec2 pos) const;
    RectF getRegion(Vec2 pos) const;
};

struct Player
{
    const Circle PlayStop{ 1120, 600, 50 };
    const Circle Back{ 1010, 600, 40 };
    const Circle Next{ 1230, 600, 40 };
    Array<Music>Queue;
    Optional<Music> Current;
    Array<Music>History;
    void update();
    void draw() const;
    void AddQueue(Music& music);
};

};

struct Data
{
    Data();
    Common::Player player;
    Common::Album Detailing;
    void LoadMusics();
    Array<Common::Album>Albums;
};

using App = SceneManager<String, Data>;
