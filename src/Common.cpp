#include "Common.hpp"

Data::Data()
{
    LoadMusics();
}

void Data::LoadMusics()
{
    for (const auto& home : FileSystem::DirectoryContents(U"Data/Musics", Recursive::No))
    {
        if(not FileSystem::IsDirectory(home)) continue;
        
        Array<Common::Music>musics;
        
        const INI Aini{ home + U"album.ini" };
        
        TextureAsset::Register(Aini[U"Album.title"], home + Aini[U"Album.image"]);
        
        for (const auto& path : FileSystem::DirectoryContents(home, Recursive::No))
        {
            if(not FileSystem::IsDirectory(path)) continue;
            const INI ini{ path + U"config.ini" };
            
            AudioAsset::Register(Aini[U"Album.title"] + U"_" + ini[U"Music.title"], path + ini[U"Music.path"], Loop::Yes);
            
            musics << Common::Music{
                Aini[U"Album.title"],
                ini[U"Music.title"],
                ini[U"Music.desc"],
                ini[U"Music.staff"],
                ini[U"Music.tools"]
            };
        }
        
        Albums << Common::Album{
            Aini[U"Album.title"],
            Aini[U"Album.name"],
            Aini[U"Album.desc"],
            musics
        };
    }
}

void Common::Album::update(Vec2 pos)
{
    const RectF region = getRegion(pos);
    FadeTransition.update(region.mouseOver());
    if (region.mouseOver())
    {
        
    }
}

void Common::Album::draw(Vec2 pos) const
{
    const RoundRect Jacket{ pos.x, pos.y, 200, 200, 5 };
    
    Jacket.draw(Color{ U"#E6E6EB" }).drawFrame(0, 1, Color{ U"#414252" });
    if(not TextureAsset(Title)) {
        FontAsset(U"font")(U"No Image").drawAt(pos+Vec2{ 100, 100 }, Color{ U"#414252" });
    }
    else
    {
        Jacket(TextureAsset(Title)).draw().drawFrame(0, 1, Color{ U"#414252" });
    }
    Jacket.draw(Color{ U"#414252" }.withAlpha(FadeTransition.value() * 0.3 * 255));
    FontAsset(U"font")(Title).draw(24, pos + Vec2{ 0, 200 });
    FontAsset(U"font")(Auth).draw(16, pos + Vec2{ 0, 230 });
}


RectF Common::Album::getRegion(Vec2 pos) const
{
    return RectF{ pos, 200, 250 };
}

void Common::Player::AddQueue(Music& music)
{
    Queue << music;
}

void Common::Player::update()
{
    const Rect Seek{ 970, 510, 300, 20 };
    if(not Current)
    {
        if(not Queue.empty())
        {
            Current = Queue.front();
            Queue.pop_front();
            AudioAsset(Current->ID).play();
        }
    }
    else
    {
        if((size_t)AudioAsset(Current->ID).loopCount())
        {
            AudioAsset(Current->ID).stop();
            Current.reset();
        }
    
        if (PlayStop.leftClicked())
        {
            if(AudioAsset(Current->ID).isPlaying()) AudioAsset(Current->ID).pause();
            else AudioAsset(Current->ID).play();
        }
        if (Back.leftClicked())
        {
            AudioAsset(Current->ID).seekSamples(0);
        }
        if (Next.leftClicked())
        {
            AudioAsset(Current->ID).stop();
            Current.reset();
        }
        if (Seek.leftClicked())
        {
            int32 pos = AudioAsset(Current->ID).samples() * (Cursor::Pos().x - 970) / 300;
            AudioAsset(Current->ID).seekSamples(pos);
        }
    }
}

void Common::Player::draw() const
{
    const RoundRect image{ 970, 50, 300, 300, 5 };
    Rect{ 960, 0, 320, 720 }.draw(Color{ U"#E6E6EB" });
    if(Current)
    {
        String MusicName = Current->ID;
        if (TextureAsset(Current->Album))
        {
            image(TextureAsset(Current->Album)).draw();
        }
        else
        {
            image.drawFrame(0, 1, Color{ U"#292B52" });
            FontAsset(U"font")(U"No Image").drawAt(48, image.center(), Color{ U"#292B52" });
        }
        FontAsset(U"font")(Current->Title).draw(32, Vec2{ 970, 380 }, Color{ U"#292B52" });
        FontAsset(U"font")(Current->Staff).draw(24, Arg::topRight = Vec2{ 1270, 420 }, Color{ U"#292B52" });
        PlayStop.draw(Color{ U"#D9D9D9" });
        Back.draw(Color{ U"#D9D9D9" });
        Next.draw(Color{ U"#D9D9D9" });
        FontAsset(U"font")(AudioAsset(MusicName).isPlaying()?U"\U000F03E4":U"\U000F040A").drawAt(PlayStop.center, Color{ U"#292B52" });
        FontAsset(U"font")(U"\U000F04AD").drawAt(Next.center, Color{ U"#292B52" });
        FontAsset(U"font")(U"\U000F04AE").drawAt(Back.center, Color{ U"#292B52" });
        
        const int32 nowSec = AudioAsset(Current->ID).posSec();
        const int32 allSec = AudioAsset(Current->ID).lengthSec();
        FontAsset(U"font")(U"{:0>2}:{:0>2}"_fmt(nowSec / 60, nowSec % 60)).draw(12, Vec2{ 970, 500 }, Color{ U"#292B52" });
        FontAsset(U"font")(U"{:0>2}:{:0>2}"_fmt(allSec / 60, allSec % 60)).draw(12, Arg::topRight = Vec2{ 1270, 500 }, Color{ U"#292B52" });
        Line(970, 520, 1270, 520).draw(1, Color{ U"#56576B" });
        Line(970, 520, 970 + (300 * AudioAsset(Current->ID).posSample() / AudioAsset(Current->ID).samples()), 520).draw(2, Color{ U"#292B52" });
        if (Queue.size())
        {
            FontAsset(U"font")(U"次:"+Queue.front().Title).draw(24, Vec2{ 970, 450 }, Color{ U"#292B52" });
        }
    }
    else
    {
        image.draw(ColorF{ U"#D9D9D9" }).drawFrame(0, 1, Color{ U"#4F539E" });
        PlayStop.draw(Color{ U"#D9D9D9" });
        Back.draw(Color{ U"#D9D9D9" });
        Next.draw(Color{ U"#D9D9D9" });
        FontAsset(U"font")(U"\U000F040A").drawAt(PlayStop.center, Color{ U"#292B52" });
        FontAsset(U"font")(U"\U000F04AD").drawAt(Next.center, Color{ U"#292B52" });
        FontAsset(U"font")(U"\U000F04AE").drawAt(Back.center, Color{ U"#292B52" });
        Line(970, 520, 1270, 520).draw(Color{ U"#56576B" });
    }
}


void Common::Music::draw(Vec2 pos, int32 id) const
{
    const auto Region = getRegion(pos);
    if(Region.mouseOver())
    {
        Region.draw(Color{ U"#414252" });
        FontAsset(U"font")(U"\U000F040A").drawAt( 24, pos + Vec2{ 20, 35 }, Color{ U"#696ED1" });
    }
    else
    {
        FontAsset(U"font")(ToString(id)).drawAt( 16, pos + Vec2{ 20, 35 }, Color{ U"#E6E6EB" });
    }
    FontAsset(U"font")(Title).draw(32, Arg::leftCenter=Vec2{ 40, 35 }+pos, Color{ U"#E6E6EB" });
    Region.bottom().draw(Color{ U"#E6E6EB" });
    Region.top().draw(Color{ U"#E6E6EB" });
}

RectF Common::Music::getRegion(Vec2 pos) const
{
    return RectF{ pos, 550, 70 };
}
