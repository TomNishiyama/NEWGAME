#include "Player.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentAttachModel.h>

namespace FenceGame
{

BP_CLASS_IMPL(Player, u8"PlayerClass");

bool Player::Init()
{
    __super::Init();
    //--------------------------------------------------
    //createGround
    //--------------------------------------------------
    {
        auto obj = Scene::CreateObjectPtr<Object>();

        obj->AddComponent<ComponentModel>("data/Sample/SwordBout/Stage/Stage00.mv1");

        obj->AddComponent<ComponentCollisionModel>()->AttachToModel();

        obj->SetTranslate({0, 0, 0});
    }

    //--------------------------------------------------
    //createCamera
    //--------------------------------------------------
    {
        Scene::CreateObjectPtr<Object>()
            ->SetName("Camera")
            ->AddComponent<ComponentCamera>()
            ->SetPerspective(45)
            ->SetPositionAndTarget({0, 15, 50}, {0, 10, 0});
    }
    //--------------------------------------------------
    // player
    //--------------------------------------------------
    {
        auto obj = Scene::CreateObjectPtr<Object>()->SetName("Player");

        auto model = obj->AddComponent<ComponentModel>("data/Player/model.mv1");
        //obj->AddComponent<ComponentModel>("data/Player/model.mv1");

        //Animation
        model->SetAnimation({
            {"idle",            "data/Player/Anim/playerIdle.mv1", 1, 1.0f},
            {"walk",      "data/Player/Anim/Crouched Walking.mv1", 1, 1.0f},
            {"jump", "data/Player/Anim/Injured Standing Jump.mv1", 1, 1.0f},
        });

        obj->AddComponent<ComponentCollisionCapsule>()->SetRadius(2.5f)->SetHeight(13.0f)->UseGravity();

        auto ctrl = obj->AddComponent<ComponentObjectController>();
        ctrl->SetMoveSpeed(0.3f);      //playerMoveSpeed
        ctrl->SetRotateSpeed(10.0f);   //playerRotateSpeed

        ctrl->SetKeys(KEY_INPUT_W,   //up
                      KEY_INPUT_S,   //down
                      KEY_INPUT_A,   //left
                      KEY_INPUT_D    //right
        );

        obj->SetTranslate({0, 5, 0});
    }
    return true;
}

void Player::Update()
{
    __super::Update();
}

void Player::Exit()
{
    __super::Exit();
}

}   // namespace FenceGame
