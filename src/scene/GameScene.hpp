// GameScene class

# pragma once

# include "BaseScene.hpp"

class GameScene : public BaseScene
{
public:

    // コンストラクタ
    GameScene();

    // デストラクタ
    ~GameScene();

    // 更新処理
    void update() override;

    // 終了処理
    void exit() override;

private:

    // 頂点バッファの作成処理
    HRESULT createVertexBuffer(const ComPtr<ID3D11Device>& device) override;

    // 描画処理
    void draw() const override;
};