/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDRUSSIAN_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDRUSSIAN_H_

#include "virtualkeyboard.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeygeneral.h"
#if USE_CUSTOM_LAYOUT
#include "virtualkeyboardlayout.h"
#endif

namespace fcitx {
namespace classicui {

enum class RussianKeyboardMode {
    Text,
    Mark,
};

class RussianKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Russian; };
    const char *label() const override { return "RU"; }
    const std::string languageCode() const override { return "ru"; }
    void updateKeys() override;
    void switchMode();
    RussianKeyboardMode mode() const { return mode_; }
#if USE_CUSTOM_LAYOUT
    RussianKeyboard() : I18nKeyboard() {
        const char *jsonPath = FCITX_INSTALL_PKGDATADIR
            "/virtualkeyboardui/virtualkeyboardui-ru.json";
        FCITX_KEYBOARD() << "path of Russian keyboard layout file: "
                         << jsonPath;
        loader_ = new KeyboardLayout(jsonPath);
    }
#endif

private:
#if USE_CUSTOM_LAYOUT
    void setLayerKeys(size_t offset);
    KeyboardLayout *loader_;
#else
    void setCyrillicTextKeys();
    void setMarkKeys();
#endif
    RussianKeyboardMode mode_ = RussianKeyboardMode::Text;
};

#if !USE_CUSTOM_LAYOUT
class RussianDummyKey : public DummyKey {
public:
    RussianDummyKey(double width) : DummyKey() { width_ = width; }
};

class RussianShiftToggleKey : public ShiftToggleKey {
public:
    RussianShiftToggleKey() : ShiftToggleKey() { width_ = 40; }
};

class RussianNormalKey : public NormalKey {
public:
    RussianNormalKey(const std::string &label, uint32_t code,
                     const std::string &upperLabel)
        : NormalKey(label, code, upperLabel, false) {
        width_ = 50;
    }
};

class RussianModeSwitchKey : public SwitchKey {
public:
    RussianModeSwitchKey() : SwitchKey() {}
    const char *label(VirtualKeyboard *) const override { return "A#"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        return index == 0 ? "A" : "#";
    }
    void switchState(VirtualKeyboard *keyboard,
                     InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};
#endif

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDRUSSIAN_H_
