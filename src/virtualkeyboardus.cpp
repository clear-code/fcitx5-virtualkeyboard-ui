/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardus.h"

namespace fcitx::classicui {

void UsKeyboard::updateKeys() {
    if (mode_ == UsKeyboardMode::Mark) {
        setMarkKeys();
        return;
    }

    setTextKeys();
}

void UsKeyboard::switchMode() {
    if (mode_ == UsKeyboardMode::Text) {
        mode_ = UsKeyboardMode::Mark;
    } else {
        mode_ = UsKeyboardMode::Text;
    }

    updateKeys();
}

void UsModeSwitchKey::switchState(VirtualKeyboard *keyboard, InputContext *) {
    keyboard->i18nKeyboard<UsKeyboard>()->switchMode();
}

int UsModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<UsKeyboard>()->mode() == UsKeyboardMode::Text) {
        return 0;
    }
    return 1;
}

void UsKeyboard::setTextKeys() {
    keys_.clear();
    keys_.emplace_back(new NormalKey("q", 24, "Q", true));
    keys_.emplace_back(new NormalKey("w", 25, "W", true));
    keys_.emplace_back(new NormalKey("e", 26, "E", true));
    keys_.emplace_back(new NormalKey("r", 27, "R", true));
    keys_.emplace_back(new NormalKey("t", 28, "T", true));
    keys_.emplace_back(new NormalKey("y", 29, "Y", true));
    keys_.emplace_back(new NormalKey("u", 30, "U", true));
    keys_.emplace_back(new NormalKey("i", 31, "I", true));
    keys_.emplace_back(new NormalKey("o", 32, "O", true));
    keys_.emplace_back(new NormalKey("p", 33, "P", true));
    keys_.emplace_back(new BackSpaceKey()); keys_.back()->setCustomLayout(1.0);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("7"));
    keys_.emplace_back(new NumberKey("8"));
    keys_.emplace_back(new NumberKey("9")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NormalKey("a", 38, "A", true));
    keys_.emplace_back(new NormalKey("s", 39, "S", true));
    keys_.emplace_back(new NormalKey("d", 40, "D", true));
    keys_.emplace_back(new NormalKey("f", 41, "F", true));
    keys_.emplace_back(new NormalKey("g", 42, "G", true));
    keys_.emplace_back(new NormalKey("h", 43, "H", true));
    keys_.emplace_back(new NormalKey("j", 44, "J", true));
    keys_.emplace_back(new NormalKey("k", 45, "K", true));
    keys_.emplace_back(new NormalKey("l", 46, "L", true));
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4"));
    keys_.emplace_back(new NumberKey("5"));
    keys_.emplace_back(new NumberKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ShiftToggleKey());
    keys_.emplace_back(new NormalKey("z", 52, "Z", true));
    keys_.emplace_back(new NormalKey("x", 53, "X", true));
    keys_.emplace_back(new NormalKey("c", 54, "C", true));
    keys_.emplace_back(new NormalKey("v", 55, "V", true));
    keys_.emplace_back(new NormalKey("b", 56, "B", true));
    keys_.emplace_back(new NormalKey("n", 57, "N", true));
    keys_.emplace_back(new NormalKey("m", 58, "M", true));
    keys_.emplace_back(new MarkKey("-", "minus"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1"));
    keys_.emplace_back(new NumberKey("2"));
    keys_.emplace_back(new NumberKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new UsModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
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

void UsKeyboard::setMarkKeys() {
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
    keys_.emplace_back(new MarkKey("$"));
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

    keys_.emplace_back(new UsModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
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
