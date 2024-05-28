/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardcustom.h"
#include "virtualkeyboardlayout.h"

namespace fcitx::classicui {

void CustomKeyboard::updateKeys() {
    FCITX_KEYBOARD() << "CustomKeyboard::updateKeys()";
    FCITX_KEYBOARD() << "current mode:" << mode_;
    std::string label = loader_->modeLabel(mode_);
    // change to next mode
    std::map<std::string, int> conditions;
    conditions.insert(std::make_pair("AdditionalMarkOn", isAdditionalMarkOn_));
    size_t offset = loader_->modeOffsetsFallback(mode_, conditions);
    FCITX_KEYBOARD() << "next CustomKeyboard layout offset:" << offset;
    setLayerKeys(offset);
}

void CustomKeyboard::setLayerKeys(size_t offset) {
    FCITX_KEYBOARD() << "CustomKeyboard::setLayerKeys(): offset: " << offset;
    keys_.clear();
    loader_->load(offset);
    FCITX_KEYBOARD() << "loaded size of keys: " << loader_->keys().size();
    for (size_t i = 0; i < loader_->keys().size(); i++) {
        keys_.emplace_back(loader_->keys()[i]);
    }
    FCITX_KEYBOARD() << "CustomKeyboard::setLayerKeys(): size of keys: "
                     << keys_.size();
}

void CustomKeyboard::switchMode() {
    FCITX_KEYBOARD() << "CustomKeyboard::switchMode()";
    std::string label = loader_->modeLabel(mode_);
    // change to next mode
    mode_ = loader_->modeActions().at(label);
    FCITX_KEYBOARD() << "CustomKeyboard::switchMode() next mode: " << mode_;
    updateKeys();
}

void CustomKeyboard::toggleMark() {
    FCITX_KEYBOARD() << "CustomKeyboard::toggleMark()";
    isAdditionalMarkOn_ = !isAdditionalMarkOn_;
    updateKeys();
}

// CustomModeSwitchKey

void CustomModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                      InputContext *) {
    FCITX_KEYBOARD() << "CustomModeSwitchKey::switchState()";
    keyboard->i18nKeyboard<CustomKeyboard>()->switchMode();

    if (updateInputPanel_) {
        keyboard->updateInputPanel();
    }
}

int CustomModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    FCITX_KEYBOARD() << "CustomModeSwitchKey::currentIndex() index: "
                     << keyboard->i18nKeyboard<CustomKeyboard>()->mode();
    return keyboard->i18nKeyboard<CustomKeyboard>()->mode();
}

// CustomMarkToggleKey

bool CustomMarkToggleKey::isOn(VirtualKeyboard *keyboard) {
    FCITX_KEYBOARD() << "CustomMarkToggleKey::isOn()";
    return keyboard->i18nKeyboard<CustomKeyboard>()->isAdditionalMarkOn();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void CustomMarkToggleKey::toggle(VirtualKeyboard *keyboard,
                                 InputContext *inputContext) {
    FCITX_KEYBOARD() << "CustomMarkToggleKey::toggle()";
    keyboard->i18nKeyboard<CustomKeyboard>()->toggleMark();
}
#pragma GCC diagnostic pop

// CustomNumberKey

const char *CustomNumberKey::label(VirtualKeyboard *keyboard) const {
    FCITX_KEYBOARD() << "CustomNumberKey::label()";
    // prefer number instead of label.
    if (useNumberWithSelectingCandidate_) {
        FCITX_KEYBOARD()
            << "CustomNumber::label useNumberWithSelectingCandidate: "
            << useNumberWithSelectingCandidate_;
        if (keyboard->isSeletingCandidates()) {
            return number_.c_str();
        }
    }
    return label_.c_str();
}

// CustomNumPadKey

void CustomNumPadKey::click(VirtualKeyboard *keyboard,
                            InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "CustomNumPadKey::click()";

    if (useKeyNameWhenSelectingCandidates_) {
        if (keyboard->isSeletingCandidates()) {
            auto event = KeyEvent(inputContext, fcitx::Key(name_), isRelease);
            inputContext->keyEvent(event);
            return;
        }

        if (isRelease)
            return;

        inputContext->commitString(label(keyboard));
    } else {
        super::click(keyboard, inputContext, isRelease);
    }
}

// CustomEnterKey

CustomEnterKey::CustomEnterKey(double fontSize, bool forceWithUpKey) {
    FCITX_KEYBOARD() << "CustomEnterKey::CustomEnterKey()";
    setFontSize(fontSize);
    forceWithUpKey_ = forceWithUpKey;
}

const char *CustomEnterKey::label(VirtualKeyboard *keyboard) const {
    FCITX_KEYBOARD() << "CustomEnterKey::label()";
    return keyboard->isPreediting() ? preeditLabel_.c_str() : label_.c_str();
}

void CustomEnterKey::click(VirtualKeyboard *keyboard,
                           InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "CustomEnterKey::click()";
    if (forceWithUpKey_) {
        if (keyboard->isSeletingCandidates()) {
            auto event = KeyEvent(inputContext, fcitx::Key("Up"), isRelease);
            inputContext->keyEvent(event);
            return;
        }
    }
    super::click(keyboard, inputContext, isRelease);
}

// CustomSpaceKey

CustomSpaceKey::CustomSpaceKey(double fontSize, std::string &label,
                               std::string &preeditLabel) {
    FCITX_KEYBOARD() << "CustomSpaceKey::CustomSpaceKey()";
    setFontSize(fontSize);
    label_ = label;
    preeditLabel_ = preeditLabel;
}

const char *CustomSpaceKey::label(VirtualKeyboard *keyboard) const {
    FCITX_KEYBOARD() << "CustomSpaceKey::label()";
    return keyboard->isPreediting() ? preeditLabel_.c_str() : label_.c_str();
}

} // namespace fcitx::classicui
