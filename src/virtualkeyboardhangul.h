/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDHANGUL_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDHANGUL_H_

#include "virtualkeyboard.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeygeneral.h"

namespace fcitx {
namespace classicui {

enum class HangulKeyboardMode {
    Text,
    Mark,
};

class HangulKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Hangul; };
    const char *label() const override { return "KR"; }
    const std::string languageCode() const override { return "ko"; }
    void updateKeys() override;
    void switchMode();
    HangulKeyboardMode mode() const { return mode_; }

private:
    void setTextKeys();
    void setMarkKeys();
    HangulKeyboardMode mode_ = HangulKeyboardMode::Text;
};

class HangulModeSwitchKey : public SwitchKey {
public:
    HangulModeSwitchKey() : SwitchKey() {}
    const char* label(VirtualKeyboard *) const override { return "한#"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        return index == 0 ? "한" : "#";
    }
    void switchState(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDHANGUL_H_
