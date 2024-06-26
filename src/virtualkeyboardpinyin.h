/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDPINYIN_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDPINYIN_H_

#include "virtualkeyboard.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeygeneral.h"

namespace fcitx {
namespace classicui {

enum class PinyinKeyboardMode {
    Text,
    Mark,
};

class PinyinKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Pinyin; };
    const char *label() const override { return "CH"; }
    const std::string languageCode() const override { return "zh_CN"; }
    void updateKeys() override;
    void switchMode();
    PinyinKeyboardMode mode() const { return mode_; }
    bool isAdditionalMarkOn() const { return isAdditionalMarkOn_; }
    void toggleMark();

private:
    void setTextKeys();
    void setMarkKeys();
    void setAdditionalMarkKeys();
    PinyinKeyboardMode mode_ = PinyinKeyboardMode::Text;
    bool isAdditionalMarkOn_ = false;
};

class PinyinSpaceKey : public SpaceKey {
public:
    PinyinSpaceKey() {
        setFontSize(18);
    }
    const char* label(VirtualKeyboard *keyboard) const override;
};

class PinyinEnterKey : public EnterKey {
public:
    PinyinEnterKey() {
        setFontSize(18);
    }
    const char* label(VirtualKeyboard *keyboard) const override;
};

class PinyinModeSwitchKey : public SwitchKey {
public:
    PinyinModeSwitchKey() : SwitchKey() {}
    const char* label(VirtualKeyboard *) const override { return "A#"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        return index == 0 ? "A" : "#";
    }
    void switchState(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

class PinyinMarkToggleKey : public ToggleKey {
public:
    PinyinMarkToggleKey() {
        setFontSize(18);
    }
    const char* label(VirtualKeyboard *) const override { return "更多"; }

protected:
    void toggle(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    bool isOn(VirtualKeyboard *keyboard) override;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDPINYIN_H_
