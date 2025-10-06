// TitleScene class

# pragma once

# include "BaseScene.hpp"

class TitleScene : public BaseScene
{
public:

    // コンストラクタ
    TitleScene(ComPtr<ID3D11Device> device);

    // デストラクタ
    ~TitleScene();

    // 更新処理
    void update() override;

    // 終了処理
    void exit() override;

private:

    // 頂点バッファの作成処理
    HRESULT createVertexBuffer(ComPtr<ID3D11Device> device) override;

    // 描画処理
    void draw() const override;
};