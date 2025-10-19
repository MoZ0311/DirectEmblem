// InputState

# pragma once

# include <windows.h>

struct InputState
{
    static inline bool keyState[256]{ false };
    static inline bool prevKeyState[256]{ false };

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

    // キーが押下されている
    static inline bool KeyUp(int keyCode)
    {
        return !keyState[keyCode] && prevKeyState[keyCode];
    }

    // キーが離された
    static inline bool KeyDown(int keyCode)
    {
        return keyState[keyCode];
    }
};