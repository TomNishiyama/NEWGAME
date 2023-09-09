﻿//---------------------------------------------------------------------------
//! @file   SceneConstantBuffer.cpp
//! @brief  定数バッファサンプルシーン
//---------------------------------------------------------------------------
#include "SceneConstantBuffer.h"
#include <System/Graphics/Model.h>
#include <System/Component/ComponentCamera.h>

BP_CLASS_IMPL(SceneConstantBuffer, u8"定数バッファサンプル");

namespace
{

//==============================================================
//! [定数バッファ] モデル情報
//! HLSL側の定数バッファ定義と同じデーター構成にする必要があります
//==============================================================
struct ModelInfo
{
    float4 mesh_color_;   //!< モデルの色
};

float color_[4]{1.0f, 0.0f, 0.0f, 1.0f};   //!< モデルの色
int   cb_model_info_ = -1;                 //!< [DxLib] モデル情報定数バッファハンドル

}   // namespace

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SceneConstantBuffer::Init()
{
    auto obj = Scene::CreateObjectPtr<Object>();

    //----------------------------------------------------------
    // モデルコンポーネント
    //----------------------------------------------------------
    model_ = std::make_shared<Model>("data/Sample/Player/model.mv1");

    //----------------------------------------------------------
    // カメラコンポーネント
    //----------------------------------------------------------
    auto camera = obj->AddComponent<ComponentCamera>();
    camera->SetPerspective(60.0f);
    camera->SetPositionAndTarget({2.0f, 1.0f, -2.0f}, {0.0f, 1.0f, 0.0f});

    //----------------------------------------------------------
    // シェーダー
    // 3Dモデルの場合は第2引数にバリエーション数指定が必要です
    //----------------------------------------------------------

    // ピクセルシェーダー
    shader_ps_ = std::make_shared<ShaderPs>("data/Sample/ConstantBuffer/ps_model_cb", Model::PS_VARIANT_COUNT);

    //==========================================================
    // 定数バッファを作成
    //==========================================================
    cb_model_info_ = CreateShaderConstantBuffer(sizeof(ModelInfo));

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//! @param  [in]    delta   経過時間
//---------------------------------------------------------------------------
void SceneConstantBuffer::Update()
{
    f32 delta = GetDeltaTime();

    // Y軸中心の回転
    static f32 ry = 0.0f;
    ry += 0.5f * delta;

    matrix mat_world = mul(matrix::scale(0.01f), matrix::rotateY(ry));
    model_->setWorldMatrix(mat_world);

    //==========================================================
    // 定数バッファを更新
    //==========================================================
    // 更新に必要なワークメモリの場所を取得
    void* p = GetBufferShaderConstantBuffer(cb_model_info_);
    {
        auto* info        = reinterpret_cast<ModelInfo*>(p);
        info->mesh_color_ = float4(color_[0], color_[1], color_[2], color_[3]);
    }

    // 定数バッファワークメモリをGPU側へ転送
    UpdateShaderConstantBuffer(cb_model_info_);

    //----------------------------------------------------------
    // モデル更新
    //----------------------------------------------------------
    model_->update(delta);
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void SceneConstantBuffer::Draw()
{
    //==========================================================
    // 定数バッファを設定
    //==========================================================
    // b4 = ModelInfo
    // HLSL側で指定されたレジスタ番号に設定します。
    // DXライブラリで0～3を使用しているため予約になっています。
    //SetShaderConstantBuffer(cb_model_info_, DX_SHADERTYPE_VERTEX, 4);
    SetShaderConstantBuffer(cb_model_info_, DX_SHADERTYPE_PIXEL, 4);

    //----------------------------------------------------------
    // モデル描画
    //----------------------------------------------------------
    // カスタムシェーダーを設定して描画することができます。
    // ピクセルシェーダーをカスタムします
    ShaderVs* vs = nullptr;            // 頂点シェーダーはカスタムしない
    ShaderPs* ps = shader_ps_.get();   //
    model_->render(vs, ps);
}

//---------------------------------------------------------------------------
//! 終了
//---------------------------------------------------------------------------
void SceneConstantBuffer::Exit()
{
    //==========================================================
    // 定数バッファを解放
    //==========================================================
    DeleteShaderConstantBuffer(cb_model_info_);
}

//---------------------------------------------------------------------------
//! GUI表示
//---------------------------------------------------------------------------
void SceneConstantBuffer::GUI()
{
    ImGui::Separator();
    ImGui::ColorPicker4(u8"モデルの色", color_);
}
