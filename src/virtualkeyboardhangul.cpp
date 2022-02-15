/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardhangul.h"

namespace fcitx::classicui {

void HangulKeyboard::updateKeys() {
    if (mode_ == HangulKeyboardMode::Text) {
        setTextKeys();
    } else {
        setMarkKeys();
    }
}

void HangulKeyboard::switchMode() {
    if (mode_ == HangulKeyboardMode::Text) {
        mode_ = HangulKeyboardMode::Mark;
    } else {
        mode_ = HangulKeyboardMode::Text;
    }

    updateKeys();
}

void HangulModeSwitchKey::switchState(VirtualKeyboard *keyboard, InputContext *) {
    keyboard->i18nKeyboard<HangulKeyboard>()->switchMode();
}

int HangulModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<HangulKeyboard>()->mode() == HangulKeyboardMode::Text) {
        return 0;
    }
    return 1;
}

void HangulKeyboard::setTextKeys() {
    keys_.clear();
    keys_.emplace_back(new NormalKey("ᄇ", 24, "ᄈ", "q", "Q"));
    keys_.emplace_back(new NormalKey("ᄌ", 25, "ᄍ", "w", "W"));
    keys_.emplace_back(new NormalKey("ᄃ", 26, "ᄄ", "e", "E"));
    keys_.emplace_back(new NormalKey("ᄀ", 27, "ᄁ", "r", "R"));
    keys_.emplace_back(new NormalKey("ᄉ", 28, "ᄊ", "t", "T"));
    keys_.emplace_back(new NormalKey("ᅭ", 29, "", "y", "Y"));
    keys_.emplace_back(new NormalKey("ᅧ", 30, "", "u", "U"));
    keys_.emplace_back(new NormalKey("ᅣ", 31, "", "i", "I"));
    keys_.emplace_back(new NormalKey("ᅢ", 32, "ᅤ", "o", "O"));
    keys_.emplace_back(new NormalKey("ᅦ", 33, "ᅨ", "p", "P"));
    keys_.emplace_back(new BackSpaceKey()); keys_.back()->setCustomLayout(1.0);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("7"));
    keys_.emplace_back(new NumberKey("8"));
    keys_.emplace_back(new NumberKey("9")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NormalKey("ᄆ", 38, "", "a", "A"));
    keys_.emplace_back(new NormalKey("ᄂ", 39, "", "s", "S"));
    keys_.emplace_back(new NormalKey("ᄋ", 40, "", "d", "D"));
    keys_.emplace_back(new NormalKey("ᄅ", 41, "", "f", "F"));
    keys_.emplace_back(new NormalKey("ᄒ", 42, "", "g", "G"));
    keys_.emplace_back(new NormalKey("ᅩ", 43, "", "h", "H"));
    keys_.emplace_back(new NormalKey("ᅥ", 44, "", "j", "J"));
    keys_.emplace_back(new NormalKey("ᅡ", 45, "", "k", "K"));
    keys_.emplace_back(new NormalKey("ᅵ", 46, "", "l", "L"));
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4"));
    keys_.emplace_back(new NumberKey("5"));
    keys_.emplace_back(new NumberKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ShiftToggleKey());
    keys_.emplace_back(new NormalKey("ᄏ", 52, "", "z", "Z"));
    keys_.emplace_back(new NormalKey("ᄐ", 53, "", "x", "X"));
    keys_.emplace_back(new NormalKey("ᄎ", 54, "", "c", "C"));
    keys_.emplace_back(new NormalKey("ᄑ", 55, "", "v", "V"));
    keys_.emplace_back(new NormalKey("ᅲ", 56, "", "b", "B"));
    keys_.emplace_back(new NormalKey("ᅮ", 57, "", "n", "N"));
    keys_.emplace_back(new NormalKey("ᅳ", 58, "", "m", "M"));
    keys_.emplace_back(new MarkKey("-", "minus"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1"));
    keys_.emplace_back(new NumberKey("2"));
    keys_.emplace_back(new NumberKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new HangulModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey(",", "comma"));
    keys_.emplace_back(new MarkKey(".", "period"));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("!", "exclam"));
    keys_.emplace_back(new MarkKey("?", "question"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

void HangulKeyboard::setMarkKeys() {
    keys_.clear();
    keys_.emplace_back(new MarkKey("["));
    keys_.emplace_back(new MarkKey("]"));
    keys_.emplace_back(new MarkKey("{"));
    keys_.emplace_back(new MarkKey("}"));
    keys_.emplace_back(new MarkKey("#"));
    keys_.emplace_back(new MarkKey("%"));
    keys_.emplace_back(new MarkKey("^"));
    keys_.emplace_back(new MarkKey("*"));
    keys_.emplace_back(new MarkKey("+"));
    keys_.emplace_back(new MarkKey("="));
    keys_.emplace_back(new BackSpaceKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("7"));
    keys_.emplace_back(new NumberKey("8"));
    keys_.emplace_back(new NumberKey("9")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new MarkKey("/"));
    keys_.emplace_back(new MarkKey("\\"));
    keys_.emplace_back(new MarkKey(":"));
    keys_.emplace_back(new MarkKey(";"));
    keys_.emplace_back(new MarkKey("("));
    keys_.emplace_back(new MarkKey(")"));
    keys_.emplace_back(new MarkKey("&"));
    keys_.emplace_back(new MarkKey("@"));
    keys_.emplace_back(new MarkKey(u8"\u20A9")); // won sign
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4"));
    keys_.emplace_back(new NumberKey("5"));
    keys_.emplace_back(new NumberKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey());
    keys_.emplace_back(new MarkKey("\""));
    keys_.emplace_back(new MarkKey("\'"));
    keys_.emplace_back(new MarkKey("_"));
    keys_.emplace_back(new MarkKey("|"));
    keys_.emplace_back(new MarkKey("~"));
    keys_.emplace_back(new MarkKey("<"));
    keys_.emplace_back(new MarkKey(">"));
    keys_.emplace_back(new MarkKey("-"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1"));
    keys_.emplace_back(new NumberKey("2"));
    keys_.emplace_back(new NumberKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new HangulModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey(","));
    keys_.emplace_back(new MarkKey("."));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("!"));
    keys_.emplace_back(new MarkKey("?"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

}
