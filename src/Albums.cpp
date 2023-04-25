#include "Albums.hpp"

Albums::Albums(const InitData& init):IScene(init)
{
    Scroll = 0;
}

void Albums::update()
{
    Scroll = Clamp(Scroll + Mouse::Wheel() * 10, 0.0, ((getData().Albums.size()-1)/4) * 320.0);
    getData().player.update();
    for (auto idx = 0; auto& album : getData().Albums)
    {
        Vec2 pos = Vec2{ 50, 50 } + Vec2{ (idx % 4) * 220, (idx / 4) * 320 - Scroll };
        album.update(pos);
        
        if (album.getRegion(pos).leftClicked())
        {
            getData().Detailing = album;
            changeScene(U"Detail", 0s);
        }
        
        ++idx;
    }
}

void Albums::draw() const
{
    getData().player.draw();
    for (auto idx = 0; const auto& album : getData().Albums)
    {
        Vec2 pos = Vec2{ 50, 50 } + Vec2{ (idx % 4) * 220, (idx / 4) * 320 - Scroll };
        album.draw(pos);
        ++idx;
    }
}
