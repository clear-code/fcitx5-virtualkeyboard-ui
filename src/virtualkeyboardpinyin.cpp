/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardpinyin.h"

namespace fcitx::classicui {

void PinyinKeyboard::updateKeys() {
#if USE_CUSTOM_LAYOUT
    if (mode_ == PinyinKeyboardMode::Text) {
        FCITX_KEYBOARD() << "PinyinKeyboard::mode_: Text";
        setLayerKeys(static_cast<int>(PinyinKeyboardMode::Text));
        return;
    }
    if (isAdditionalMarkOn()) {
        setLayerKeys(static_cast<int>(PinyinKeyboardMode::AdditionalMark));
    } else {
        setLayerKeys(static_cast<int>(PinyinKeyboardMode::Mark));
    }
#else
    if (mode_ == PinyinKeyboardMode::Text) {
        setTextKeys();
        return;
    }
    if (isAdditionalMarkOn()) {
        setAdditionalMarkKeys();
    } else {
        setMarkKeys();
    }
#endif
}

void PinyinKeyboard::switchMode() {
    if (mode_ == PinyinKeyboardMode::Text) {
        mode_ = PinyinKeyboardMode::Mark;
    } else {
        mode_ = PinyinKeyboardMode::Text;
    }

    updateKeys();
}

void PinyinKeyboard::toggleMark() {
    isAdditionalMarkOn_ = !isAdditionalMarkOn_;
    updateKeys();
}

const char *PinyinSpaceKey::label(VirtualKeyboard *keyboard) const {
    return keyboard->isPreediting() ? "选定" : "空格";
}

const char *PinyinEnterKey::label(VirtualKeyboard *keyboard) const {
    return keyboard->isPreediting() ? "确认" : "换行";
}

void PinyinMarkToggleKey::toggle(VirtualKeyboard *keyboard, InputContext *) {
    keyboard->i18nKeyboard<PinyinKeyboard>()->toggleMark();
}

bool PinyinMarkToggleKey::isOn(VirtualKeyboard *keyboard) {
    return keyboard->i18nKeyboard<PinyinKeyboard>()->isAdditionalMarkOn();
}

void PinyinModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                      InputContext *) {
    keyboard->i18nKeyboard<PinyinKeyboard>()->switchMode();
}

int PinyinModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<PinyinKeyboard>()->mode() ==
        PinyinKeyboardMode::Text) {
        return 0;
    }
    return 1;
}

#if USE_CUSTOM_LAYOUT
void PinyinKeyboard::setLayerKeys(size_t offset) {
    FCITX_KEYBOARD() << "setLayerKeys(): offset: " << offset;
    keys_.clear();
    loader_->load(offset);
    FCITX_KEYBOARD() << "loaded size of keys: " << loader_->keys().size();
    for (size_t i = 0; i < loader_->keys().size(); i++) {
        keys_.emplace_back(loader_->keys()[i]);
    }
}
#else
// clang-format off
void PinyinKeyboard::setTextKeys() {
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
    keys_.emplace_back(new PinyinEnterKey()); keys_.back()->setCustomLayout(1.5);
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
    keys_.emplace_back(new MarkKey("、", "backslash"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1"));
    keys_.emplace_back(new NumberKey("2"));
    keys_.emplace_back(new NumberKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new PinyinModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("，", "comma"));
    keys_.emplace_back(new MarkKey("。", "period"));
    keys_.emplace_back(new PinyinSpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("！", "exclam"));
    keys_.emplace_back(new MarkKey("？", "question"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

void PinyinKeyboard::setMarkKeys() {
    keys_.clear();
    keys_.emplace_back(new MarkKey("“"));
    keys_.emplace_back(new MarkKey("”"));
    keys_.emplace_back(new MarkKey("‘"));
    keys_.emplace_back(new MarkKey("’"));
    keys_.emplace_back(new MarkKey("（"));
    keys_.emplace_back(new MarkKey("）"));
    keys_.emplace_back(new MarkKey("「"));
    keys_.emplace_back(new MarkKey("」"));
    keys_.emplace_back(new MarkKey("『"));
    keys_.emplace_back(new MarkKey("』"));
    keys_.emplace_back(new BackSpaceKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("7"));
    keys_.emplace_back(new NumberKey("8"));
    keys_.emplace_back(new NumberKey("9")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new MarkKey("《"));
    keys_.emplace_back(new MarkKey("》"));
    keys_.emplace_back(new MarkKey("〈"));
    keys_.emplace_back(new MarkKey("〉"));
    keys_.emplace_back(new MarkKey("："));
    keys_.emplace_back(new MarkKey("；"));
    keys_.emplace_back(new MarkKey("—"));
    keys_.emplace_back(new MarkKey("…"));
    keys_.emplace_back(new MarkKey("@"));
    keys_.emplace_back(new PinyinEnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4"));
    keys_.emplace_back(new NumberKey("5"));
    keys_.emplace_back(new NumberKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new PinyinMarkToggleKey());
    keys_.emplace_back(new MarkKey("·"));
    keys_.emplace_back(new MarkKey("～"));
    keys_.emplace_back(new MarkKey("￥"));
    keys_.emplace_back(new MarkKey("$"));
    keys_.emplace_back(new MarkKey("&"));
    keys_.emplace_back(new MarkKey("|"));
    keys_.emplace_back(new MarkKey("_"));
    keys_.emplace_back(new MarkKey("、"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1"));
    keys_.emplace_back(new NumberKey("2"));
    keys_.emplace_back(new NumberKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new PinyinModeSwitchKey());  keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("，"));
    keys_.emplace_back(new MarkKey("。"));
    keys_.emplace_back(new PinyinSpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("！"));
    keys_.emplace_back(new MarkKey("？"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

void PinyinKeyboard::setAdditionalMarkKeys() {
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
    keys_.emplace_back(new MarkKey("-"));
    keys_.emplace_back(new MarkKey("~"));
    keys_.emplace_back(new MarkKey("@"));
    keys_.emplace_back(new PinyinEnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4"));
    keys_.emplace_back(new NumberKey("5"));
    keys_.emplace_back(new NumberKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new PinyinMarkToggleKey());
    keys_.emplace_back(new MarkKey("\""));
    keys_.emplace_back(new MarkKey("\'"));
    keys_.emplace_back(new MarkKey(u8"\u00A5"));
    keys_.emplace_back(new MarkKey("!"));
    keys_.emplace_back(new MarkKey("?"));
    keys_.emplace_back(new MarkKey("<"));
    keys_.emplace_back(new MarkKey(">"));
    keys_.emplace_back(new MarkKey("、"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1"));
    keys_.emplace_back(new NumberKey("2"));
    keys_.emplace_back(new NumberKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new PinyinModeSwitchKey());  keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("，"));
    keys_.emplace_back(new MarkKey("。"));
    keys_.emplace_back(new PinyinSpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("！"));
    keys_.emplace_back(new MarkKey("？"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}
#endif

}
