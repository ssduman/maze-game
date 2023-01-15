#pragma once

#include "../event/EventData.h"

#include <string>

struct Input {
    static bool isPressed(const KeyEventData& data, KeyCode target) {
        return data.key == target && data.action == InputAction::press;
    }

    static bool isReleased(const KeyEventData& data, KeyCode target) {
        return data.key == target && data.action == InputAction::release;
    }

    static bool isRepeating(const KeyEventData& data, KeyCode target) {
        return data.key == target && data.action == InputAction::repeat;
    }

    static bool isPressing(const KeyEventData& data, KeyCode target) {
        return data.key == target && data.action != InputAction::release;
    }

    static std::string encodeUTF8(unsigned int codepoint) {
        std::string str;

        if (codepoint < 0x80) {
            str += static_cast<char>(codepoint);
        }
        else if (codepoint < 0x800) {
            str += (codepoint >> 6) | 0xc0;
            str += (codepoint & 0x3f) | 0x80;
        }
        else if (codepoint < 0x10000) {
            str += (codepoint >> 12) | 0xe0;
            str += ((codepoint >> 6) & 0x3f) | 0x80;
            str += (codepoint & 0x3f) | 0x80;
        }
        else if (codepoint < 0x110000) {
            str += (codepoint >> 18) | 0xf0;
            str += ((codepoint >> 12) & 0x3f) | 0x80;
            str += ((codepoint >> 6) & 0x3f) | 0x80;
            str += (codepoint & 0x3f) | 0x80;
        }

        return str;
    }
};
