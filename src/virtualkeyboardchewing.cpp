/*
 * SPDX-FileCopyrightText: 2022-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardchewing.h"

namespace fcitx::classicui {

void ChewingKeyboard::updateKeys() {
#if USE_CUSTOM_LAYOUT
    if (mode_ == ChewingKeyboardMode::Text) {
        FCITX_KEYBOARD_LAYOUT() << "ChewingKeyboard::mode_: Text";
        setLayerKeys(static_cast<int>(ChewingKeyboardMode::Text));
        return;
    }
    if (isAdditionalMarkOn()) {
        FCITX_KEYBOARD_LAYOUT() << "HangulKeyboard::mode_: Text";
        setLayerKeys(2);
    } else {
        setLayerKeys(static_cast<int>(ChewingKeyboardMode::Mark));
    }
#else
    if (mode_ == ChewingKeyboardMode::Text) {
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

#if USE_CUSTOM_LAYOUT
void ChewingKeyboard::setLayerKeys(size_t offset) {
    FCITX_KEYBOARD_LAYOUT() << "setLayerKeys(): offset: " << offset;
    keys_.clear();
    loader_->load(offset);
    FCITX_KEYBOARD_LAYOUT()
        << "loaded size of keys: " << loader_->keys().size();
    for (size_t i = 0; i < loader_->keys().size(); i++) {
        keys_.emplace_back(loader_->keys()[i]);
    }
}
#endif

void ChewingKeyboard::switchMode() {
    if (mode_ == ChewingKeyboardMode::Text) {
        mode_ = ChewingKeyboardMode::Mark;
    } else {
        mode_ = ChewingKeyboardMode::Text;
    }

    updateKeys();
}

void ChewingKeyboard::toggleMark() {
    isAdditionalMarkOn_ = !isAdditionalMarkOn_;
    updateKeys();
}

const char *ChewingNumberKey::label(VirtualKeyboard *keyboard) const {
    // `ㄅ` is NumberKey of `1`, and `1` is used for selecting a candidate while
    // selecting candidates. So need to change the label while selecting
    // candidates.
    if (keyboard->isSelectingCandidates()) {
        return number_.c_str();
    }

    return label_.c_str();
}

void ChewingNumPadKey::click(VirtualKeyboard *keyboard,
                             InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD_LAYOUT() << "ChewingNumPadKey pushed";

    // In order to numpad-keys can select candidates too.
    // Number sym keys input chewings such as `ㄅ`, so we need `commitString` to
    // input number chars.
    if (keyboard->isSelectingCandidates()) {
        auto event = KeyEvent(inputContext, fcitx::Key(name_), isRelease);
        inputContext->keyEvent(event);
        return;
    }

    if (isRelease)
        return;

    inputContext->commitString(label(keyboard));
}

const char *ChewingEnterKey::label(VirtualKeyboard *keyboard) const {
    return keyboard->isPreediting() ? "確認" : "換行";
}

void ChewingEnterKey::click(VirtualKeyboard *keyboard,
                            InputContext *inputContext, bool isRelease) {
    // In fcitx5-chewing, EnterKey does nothing while selecting candidates.
    // This is weird because EnterKey displays `確認`.
    // So send `Up` key in order to cancel selecting.
    if (keyboard->isSelectingCandidates()) {
        auto event = KeyEvent(inputContext, fcitx::Key("Up"), isRelease);
        inputContext->keyEvent(event);
        return;
    }

    super::click(keyboard, inputContext, isRelease);
}

const char *ChewingSpaceKey::label(VirtualKeyboard *keyboard) const {
    return keyboard->isPreediting() ? "一聲/變換" : "空格";
}

void ChewingModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                       InputContext *) {
    keyboard->i18nKeyboard<ChewingKeyboard>()->switchMode();
    // Because this switching changes the size of the keyboard.
    keyboard->updateInputPanel();
}

int ChewingModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<ChewingKeyboard>()->mode() ==
        ChewingKeyboardMode::Text) {
        return 0;
    }
    return 1;
}

void ChewingMarkToggleKey::toggle(VirtualKeyboard *keyboard, InputContext *) {
    keyboard->i18nKeyboard<ChewingKeyboard>()->toggleMark();
}

bool ChewingMarkToggleKey::isOn(VirtualKeyboard *keyboard) {
    return keyboard->i18nKeyboard<ChewingKeyboard>()->isAdditionalMarkOn();
}

#if !USE_CUSTOM_LAYOUT
// clang-format off
void ChewingKeyboard::setTextKeys() {
    keys_.clear();
    keys_.emplace_back(new ChewingNumberKey("ㄅ", "1", 10));
    keys_.emplace_back(new ChewingNumberKey("ㄉ", "2", 11));
    // want to set Top-align to these labels: ˇ, `, ´, ˙,
    // but it is difficult because the number muse be Center-align...
    keys_.emplace_back(new ChewingNumberKey("ˇ", "3", 12));
    keys_.emplace_back(new ChewingNumberKey("`", "4", 13));
    keys_.emplace_back(new ChewingNumberKey("ㄓ", "5", 14));
    keys_.emplace_back(new ChewingNumberKey("´", "6", 15));
    keys_.emplace_back(new ChewingNumberKey("˙", "7", 16));
    keys_.emplace_back(new ChewingNumberKey("ㄚ", "8", 17));
    keys_.emplace_back(new ChewingNumberKey("ㄞ", "9", 18));
    keys_.emplace_back(new ChewingNumberKey("ㄢ", "0", 19));
    keys_.emplace_back(new NormalKey("ㄦ", 20, "", "minus", "")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new NormalKey("ㄆ", 24, "", "q", ""));
    keys_.emplace_back(new NormalKey("ㄊ", 25, "", "w", ""));
    keys_.emplace_back(new NormalKey("ㄍ", 26, "", "e", ""));
    keys_.emplace_back(new NormalKey("ㄐ", 27, "", "r", ""));
    keys_.emplace_back(new NormalKey("ㄔ", 28, "", "t", ""));
    keys_.emplace_back(new NormalKey("ㄗ", 29, "", "y", ""));
    keys_.emplace_back(new NormalKey("ㄧ", 30, "", "u", ""));
    keys_.emplace_back(new NormalKey("ㄛ", 31, "", "i", ""));
    keys_.emplace_back(new NormalKey("ㄟ", 32, "", "o", ""));
    keys_.emplace_back(new NormalKey("ㄣ", 33, "", "p", ""));
    keys_.emplace_back(new BackSpaceKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("7"));
    keys_.emplace_back(new ChewingNumPadKey("8"));
    keys_.emplace_back(new ChewingNumPadKey("9")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new NormalKey("ㄇ", 38, "", "a", ""));
    keys_.emplace_back(new NormalKey("ㄋ", 39, "", "s", ""));
    keys_.emplace_back(new NormalKey("ㄎ", 40, "", "d", ""));
    keys_.emplace_back(new NormalKey("ㄑ", 41, "", "f", ""));
    keys_.emplace_back(new NormalKey("ㄕ", 42, "", "g", ""));
    keys_.emplace_back(new NormalKey("ㄘ", 43, "", "h", ""));
    keys_.emplace_back(new NormalKey("ㄨ", 44, "", "j", ""));
    keys_.emplace_back(new NormalKey("ㄜ", 45, "", "k", ""));
    keys_.emplace_back(new NormalKey("ㄠ", 46, "", "l", ""));
    keys_.emplace_back(new NormalKey("ㄤ", 47, "", "semicolon", ""));
    keys_.emplace_back(new ChewingEnterKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("4"));
    keys_.emplace_back(new ChewingNumPadKey("5"));
    keys_.emplace_back(new ChewingNumPadKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new NormalKey("ㄈ", 52, "", "z", ""));
    keys_.emplace_back(new NormalKey("ㄌ", 53, "", "x", ""));
    keys_.emplace_back(new NormalKey("ㄏ", 54, "", "c", ""));
    keys_.emplace_back(new NormalKey("ㄒ", 55, "", "v", ""));
    keys_.emplace_back(new NormalKey("ㄖ", 56, "", "b", ""));
    keys_.emplace_back(new NormalKey("ㄙ", 57, "", "n", ""));
    keys_.emplace_back(new NormalKey("ㄩ", 58, "", "m", ""));
    keys_.emplace_back(new NormalKey("ㄝ", 59, "", "comma", ""));
    keys_.emplace_back(new NormalKey("ㄡ", 60, "", "period", ""));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("1"));
    keys_.emplace_back(new ChewingNumPadKey("2"));
    keys_.emplace_back(new ChewingNumPadKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ChewingModeSwitchKey());
    keys_.emplace_back(new MarkKey("，", "less"));
    keys_.emplace_back(new MarkKey("。", "greater"));
    keys_.emplace_back(new ChewingSpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("！", "exclam"));
    keys_.emplace_back(new MarkKey("？", "question"));
    keys_.emplace_back(new NormalKey("ㄥ", 61, "", "slash", ""));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

void ChewingKeyboard::setMarkKeys() {
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
    keys_.emplace_back(new ChewingNumPadKey("7"));
    keys_.emplace_back(new ChewingNumPadKey("8"));
    keys_.emplace_back(new ChewingNumPadKey("9")); keys_.back()->setCustomLayout(1.0, true);

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
    keys_.emplace_back(new ChewingEnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("4"));
    keys_.emplace_back(new ChewingNumPadKey("5"));
    keys_.emplace_back(new ChewingNumPadKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ChewingMarkToggleKey());
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
    keys_.emplace_back(new ChewingNumPadKey("1"));
    keys_.emplace_back(new ChewingNumPadKey("2"));
    keys_.emplace_back(new ChewingNumPadKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ChewingModeSwitchKey());
    keys_.emplace_back(new MarkKey("，"));
    keys_.emplace_back(new MarkKey("。"));
    keys_.emplace_back(new ChewingSpaceKey()); keys_.back()->setCustomLayout(3.0);
    keys_.emplace_back(new MarkKey("！"));
    keys_.emplace_back(new MarkKey("？"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

void ChewingKeyboard::setAdditionalMarkKeys() {
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
    keys_.emplace_back(new ChewingNumPadKey("7"));
    keys_.emplace_back(new ChewingNumPadKey("8"));
    keys_.emplace_back(new ChewingNumPadKey("9")); keys_.back()->setCustomLayout(1.0, true);

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
    keys_.emplace_back(new ChewingEnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("4"));
    keys_.emplace_back(new ChewingNumPadKey("5"));
    keys_.emplace_back(new ChewingNumPadKey("6")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ChewingMarkToggleKey());
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
    keys_.emplace_back(new ChewingNumPadKey("1"));
    keys_.emplace_back(new ChewingNumPadKey("2"));
    keys_.emplace_back(new ChewingNumPadKey("3")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new ChewingModeSwitchKey());
    keys_.emplace_back(new MarkKey("，"));
    keys_.emplace_back(new MarkKey("。"));
    keys_.emplace_back(new ChewingSpaceKey()); keys_.back()->setCustomLayout(3.0);
    keys_.emplace_back(new MarkKey("！"));
    keys_.emplace_back(new MarkKey("？"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new ChewingNumPadKey("0")); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}
#endif

}
