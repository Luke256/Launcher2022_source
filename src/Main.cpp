# include "Common.hpp"
# include "Albums.hpp"
# include "Detail.hpp"

void Main()
{
    Window::Resize(1280, 720);
    Window::SetTitle(U"MusicRoom");
    Scene::SetBackground(Color{ U"#292B52" });
    
    App manager;
    manager.add<Albums>(U"Albums");
    manager.add<Detail>(U"Detail");
    
    Font iconFont{ FontMethod::MSDF, 32, Typeface::Icon_MaterialDesign };
    FontAsset::Register(U"font", FontMethod::MSDF, 32);
    FontAsset(U"font").addFallback(iconFont);

	while (System::Update())
	{
        if(!manager.update()) break;
	}
}
