/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDCUSTOM_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDCUSTOM_H_

#if USE_CUSTOM_LAYOUT
#include <filesystem>
#include <json-c/json.h>
#include "fcitx-utils/log.h"
#include "virtualkeyboard.h"
#include "virtualkeyboardcustom.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

namespace fcitx {
namespace classicui {

class CustomKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Custom; };
    const char *label() const override { return label_; }
    const std::string languageCode() const override { return languageCode_; }
    void updateKeys() override;
    CustomKeyboard(const char *jsonPath) : I18nKeyboard() {
        FCITX_KEYBOARD_LAYOUT()
            << "given path of keyboard layout file: " << jsonPath;
        auto parentDir = stringutils::joinPath(FCITX_INSTALL_PKGDATADIR,
                                               "/virtualkeyboardui/");
        auto relativePath = stringutils::joinPath(parentDir, jsonPath);
        char fullPath[PATH_MAX];
        char *returnPath = realpath(relativePath.c_str(), fullPath);
        if (!returnPath) {
            FCITX_ERROR() << "failed to resolve path of keyboard layout file: "
                          << relativePath;
            return;
        }
        FCITX_KEYBOARD_LAYOUT()
            << "resolved full path of keyboard layout file: " << fullPath;
        loader_.reset(new KeyboardLayout(fullPath));
        loader_->loadMetadata(0);
        label_ = loader_->label();
        languageCode_ = std::string(loader_->languageCode());
    }
    int mode() { return mode_; }
    void switchMode();
    bool isAdditionalMarkOn() const { return isAdditionalMarkOn_; }
    void toggleMark();

private:
    //
    // [
    //   { /*keyboard metadata; optional*/ },
    //   [ /*row 1*/ ],
    //   /*...*/
    //   [ /*row n*/ ]
    // ]
    //
    const char *label_ = nullptr;
    std::string languageCode_;
    void setLayerKeys(size_t offset);
    std::unique_ptr<KeyboardLayout> loader_;
    int mode_ = 0;
    bool isAdditionalMarkOn_ = false;
};

class CustomModeSwitchKey : public SwitchKey {
public:
    CustomModeSwitchKey(const char *label, const int numberOfStates,
                        std::vector<std::string> stateLabels,
                        bool updateInputPanel)
        : SwitchKey(), label_(label), numberOfStates_(numberOfStates),
          stateLabels_(stateLabels), updateInputPanel_(updateInputPanel) {}
    const char *label(VirtualKeyboard *) const override { return label_; }

protected:
    int numberOfStates() const override { return numberOfStates_; }
    const char *stateLabel(int index) const override {
        return stateLabels_[index].c_str();
    }
    void switchState(VirtualKeyboard *keyboard,
                     InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;

private:
    const char *label_;
    int numberOfStates_;
    std::vector<std::string> stateLabels_;
    bool updateInputPanel_;
};

class CustomNumberKey : public NormalKey {
public:
    CustomNumberKey(const std::string &label, const std::string &number,
                    uint32_t code, bool useNumberWithSelectingCandidate)
        : NormalKey(label, code, "", number, ""), number_(number),
          useNumberWithSelectingCandidate_(useNumberWithSelectingCandidate) {}
    const char *label(VirtualKeyboard *keyboard) const override;

private:
    const std::string number_;
    bool useNumberWithSelectingCandidate_;
};

class CustomNumPadKey : public NumberKey {
public:
    CustomNumPadKey(const std::string &number,
                    bool useKeyNameWhenSelectingCandidates)
        : NumberKey(number), useKeyNameWhenSelectingCandidates_(
                                 useKeyNameWhenSelectingCandidates) {}
    void click(VirtualKeyboard *keyboard, InputContext *inputContext,
               bool isRelease) override;

private:
    typedef NumberKey super;
    bool useKeyNameWhenSelectingCandidates_;
};

class CustomEnterKey : public EnterKey {
public:
    CustomEnterKey(double fontSize, bool forceWithUpKey);
    const char *label(VirtualKeyboard *keyboard) const override;
    void click(VirtualKeyboard *keyboard, InputContext *inputContext,
               bool isRelease) override;

private:
    typedef EnterKey super;
    bool forceWithUpKey_;
    std::string preeditLabel_;
    std::string label_;
};

class CustomSpaceKey : public SpaceKey {
public:
    CustomSpaceKey(double fontSize, std::string &label,
                   std::string &preeditLabel);
    const char *label(VirtualKeyboard *keyboard) const override;

private:
    std::string preeditLabel_;
    std::string label_;
};

class CustomMarkToggleKey : public ToggleKey {
public:
    CustomMarkToggleKey(const std::string &label, double fontSize);
    const char *label(VirtualKeyboard *) const override {
        return label_.c_str();
    }

protected:
    void toggle(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    bool isOn(VirtualKeyboard *keyboard) override;
    const std::string label_;
};

} // namespace classicui
} // namespace fcitx

#endif // USE_CUSTOM_LAYOUT

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDCUSTOM_H_
