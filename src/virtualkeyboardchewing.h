/*
 * SPDX-FileCopyrightText: 2022-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDCHEWING_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDCHEWING_H_

#include "virtualkeyboard.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeygeneral.h"

namespace fcitx {
namespace classicui {

enum class ChewingKeyboardMode {
    Text,
    Mark,
};

class ChewingKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Chewing; };
    const char *label() const override { return "TW"; }
    const std::string languageCode() const override { return "zh_TW"; }
    void updateKeys() override;
    void switchMode();
    ChewingKeyboardMode mode() const { return mode_; }
    bool isAdditionalMarkOn() const { return isAdditionalMarkOn_; }
    void toggleMark();

private:
    void setTextKeys();
    void setMarkKeys();
    void setAdditionalMarkKeys();
    ChewingKeyboardMode mode_ = ChewingKeyboardMode::Text;
    bool isAdditionalMarkOn_ = false;
};

class ChewingNumberKey : public NormalKey {
public:
    ChewingNumberKey(
        const std::string &label,
        const std::string &number,
        uint32_t code
    ) : NormalKey(
            label,
            code,
            "",
            number,
            ""
        ),
        number_(number) {}
    const char* label(VirtualKeyboard *keyboard) const override;

private:
    const std::string number_;
};

class ChewingNumPadKey : public NumberKey {
public:
    ChewingNumPadKey(const std::string &number) : NumberKey(number) {}
    void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;
};

class ChewingEnterKey : public EnterKey {
public:
    ChewingEnterKey() {
        setFontSize(18);
    }
    const char* label(VirtualKeyboard *keyboard) const override;
    void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;

private:
    typedef EnterKey super;
};

class ChewingSpaceKey : public SpaceKey {
public:
    ChewingSpaceKey() {
        setFontSize(18);
    }
    const char* label(VirtualKeyboard *keyboard) const override;
};

class ChewingModeSwitchKey : public SwitchKey {
public:
    ChewingModeSwitchKey() : SwitchKey() {}
    const char* label(VirtualKeyboard *) const override { return "注#"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        return index == 0 ? "注" : "#";
    }
    void switchState(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

class ChewingMarkToggleKey : public ToggleKey {
public:
    ChewingMarkToggleKey() {
        setFontSize(18);
    }
    const char* label(VirtualKeyboard *) const override { return "更多"; }

protected:
    void toggle(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    bool isOn(VirtualKeyboard *keyboard) override;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDCHEWING_H_
