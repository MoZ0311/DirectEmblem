// InputState

# pragma once

# include "../core/Config.hpp"

struct InputState
{
    // マウスのワールド座標
    static inline Util::Vec2 mouseWorldPosition{ 0.0f, 0.0f };

    // キーの押下状況
    static inline bool keyState[256]{ false };

    // 直前のキーの押下状況
    static inline bool prevKeyState[256]{ false };

    // マウス座標の更新処理
    static inline void UpdateMouseState(HWND hWnd)
    {
        // マウスのスクリーン座標を取得
        POINT mouseScreenPosition{};
        GetCursorPos(&mouseScreenPosition);

        // スクリーン座標をクライアント座標に変換
        POINT mouseClientPosition{ mouseScreenPosition };
        ScreenToClient(hWnd, &mouseClientPosition);

        // クライアント座標をワールド座標に変換
        const float worldX = (2.0f * mouseClientPosition.x / Config::WindowWidth) - 1.0f;
        const float worldY = 1.0f - (2.0f * mouseClientPosition.y / Config::WindowHeight);

        mouseWorldPosition = { worldX, worldY };
    }

    // 全てのキーの更新処理
    static inline void UpdateKeyState()
    {
        for (int i{ 0 }; i < 256; ++i)
        {
            prevKeyState[i] = keyState[i];
        }

        for (int i{ 0 }; i < 256; ++i)
        {
            keyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
        }
    }

    // キーが押された瞬間
    static inline bool KeyPressed(int keyCode)
    {
        return keyState[keyCode] && !prevKeyState[keyCode];
    }

    // キーが離された
    static inline bool KeyUp(int keyCode)
    {
        return !keyState[keyCode] && prevKeyState[keyCode];
    }

    // キーが押下されている
    static inline bool KeyDown(int keyCode)
    {
        return keyState[keyCode];
    }
};