/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDUS_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDUS_H_

#include "virtualkeyboard.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeygeneral.h"

namespace fcitx {
namespace classicui {

enum class UsKeyboardMode {
    Text,
    Mark,
};

class UsKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Us; };
    const char *label() const override { return "US"; }
    const std::string languageCode() const override { return "us"; }
    void updateKeys() override;
    void switchMode();
    UsKeyboardMode mode() const { return mode_; }

private:
    void setTextKeys();
    void setMarkKeys();
    UsKeyboardMode mode_ = UsKeyboardMode::Text;
};

class UsModeSwitchKey : public SwitchKey {
public:
    UsModeSwitchKey() : SwitchKey() {}
    const char* label(VirtualKeyboard *) const override { return "A#"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        return index == 0 ? "A" : "#";
    }
    void switchState(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDUS_H_
