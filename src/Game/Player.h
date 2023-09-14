#pragma once

#include <System/Scene.h>

namespace FenceGame
{

class Player : public Scene::Base
{
public:
    //baseClass
    BP_CLASS_TYPE(Player, Scene::Base);

    bool Init() override;

    void Update() override;

    void Exit() override;

private:
    float speed_ = 1.0f;
    float rot_y_ = 0.0f;
    float rot_x_ = 0.0f;

    struct Material
    {
        std::shared_ptr<Texture> albedo_;
        std::shared_ptr<Texture> normal_;
        std::shared_ptr<Texture> roughness_;
        std::shared_ptr<Texture> metalness_;
    };

    std::vector<Material> materials_;
};

}   // namespace FenceGame
