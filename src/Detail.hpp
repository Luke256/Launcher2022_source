# pragma once
# include "Common.hpp"

class Detail : public App::Scene
{
public:
    Detail(const InitData& init);
    void update() override;
    void draw() const override;
private:
    const Circle BackButton = Circle{ 70, 75, 20 };
    double Scroll = 0;
};
