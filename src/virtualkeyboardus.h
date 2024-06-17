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
#if USE_CUSTOM_LAYOUT
#include "virtualkeyboardlayout.h"
#endif

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
#if USE_CUSTOM_LAYOUT
    UsKeyboard() : I18nKeyboard() {
        const char *jsonPath = FCITX_INSTALL_PKGDATADIR
            "/virtualkeyboardui/virtualkeyboardui-us.json";
        FCITX_KEYBOARD() << "path of English keyboard layout file: "
                         << jsonPath;
        loader_.reset(new KeyboardLayout(jsonPath));
    }
#endif

private:
#if USE_CUSTOM_LAYOUT
    void setLayerKeys(size_t offset);
    std::unique_ptr<KeyboardLayout> loader_;
#else
    void setTextKeys();
    void setMarkKeys();
#endif
    UsKeyboardMode mode_ = UsKeyboardMode::Text;
};

#if !USE_CUSTOM_LAYOUT
class UsModeSwitchKey : public SwitchKey {
public:
    UsModeSwitchKey() : SwitchKey() {}
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

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDUS_H_
