# include "Detail.hpp"

Detail::Detail(const InitData& init):IScene(init)
{
    
}

void Detail::update()
{
    getData().player.update();
    if (BackButton.leftClicked())
    {
        changeScene(U"Albums", 0s);
    }
    
    Scroll -= (Mouse::Wheel() * 10);
    Scroll = Clamp(Scroll, -(getData().Detailing.m_Musics.size() * 70.0 - 70), 0.0);
    
    for (int32 idx = 0; auto& music : getData().Detailing.m_Musics)
    {
        Vec2 pos{ 400, 30 + idx * 70 + Scroll };
        const auto Region = music.getRegion(pos);
        if(Region.leftClicked())
        {
            getData().player.AddQueue(music);
        }
        ++idx;
    }
}

void Detail::draw() const
{
    BackButton.draw(Color{ U"#E6E6EB" });
    FontAsset(U"font")(U"\U000F0141").drawAt(BackButton.center, ColorF{ U"#292B52" });
    TextureAsset(getData().Detailing.Title).resized(300, 300).draw(50, 150);
    FontAsset(U"font")(getData().Detailing.Title).draw(48, 50, 480);
    FontAsset(U"font")(U"{}曲"_fmt(getData().Detailing.m_Musics.size())).draw(24, Vec2{ 50, 550 });
    FontAsset(U"font")(getData().Detailing.Auth).draw(24, Arg::topRight=Vec2{ 350, 550 });
    
    for(int32 idx = 0; const auto& music : getData().Detailing.m_Musics)
    {
        Vec2 pos{ 400, 30 + idx * 70 + Scroll };
        music.draw(pos, idx+1);
        ++idx;
    }
    
    getData().player.draw();
}
