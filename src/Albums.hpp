# pragma once
# include "Common.hpp"

class Albums : public App::Scene
{
public:
    Albums(const InitData& init);
    void update() override;
    void draw() const override;
    
private:
    double Scroll;
};
