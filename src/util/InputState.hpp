// InputState

# pragma once

# include <windows.h>

struct InputState
{
    static inline bool keyState[256]{ false };
    static inline bool prevKeyState[256]{ false };

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

    static inline bool KeyPressed(int keyCode)
    {
        return keyState[keyCode] && !prevKeyState[keyCode];
    }

    static inline bool KeyUp(int keyCode)
    {
        return !keyState[keyCode] && prevKeyState[keyCode];
    }

    static inline bool KeyDown(int keyCode)
    {
        return keyState[keyCode];
    }
};