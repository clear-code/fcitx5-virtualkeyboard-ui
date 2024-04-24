/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeygeneral.h"
#include <pango/pangocairo.h>

#if USE_CUSTOM_LAYOUT
#include "virtualkeyboardrussian.h"
#endif

namespace fcitx::classicui {

const char* NormalKey::label(VirtualKeyboard *keyboard) const {
    if (!keyboard->isShiftOn() || upperLabel_.empty()) {
        return label_.c_str();
    }
    return upperLabel_.c_str();
}

void NormalKey::click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "NormalKey pushed: " << label(keyboard);

    if (keyboard->isShiftOn()) {
        keyboard->sendShiftModifierToIM(inputContext, false);
    }

    auto event = KeyEvent(inputContext, convert(keyboard->isShiftOn()), isRelease);
    inputContext->virtualKeyEvent(event);

    if (keyboard->isShiftOn()) {
        keyboard->sendShiftModifierToIM(inputContext, true);
    }
}

const char* MarkKey::label(VirtualKeyboard *) const {
    return label_.c_str();
}

void MarkKey::click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "MarkKey pushed: " << label(keyboard);

    if (sendKeyEventFirst()) {
        if (withShift_) {
            keyboard->sendShiftModifierToIM(inputContext, false);
        }

        auto event = KeyEvent(inputContext, convert(withShift_), isRelease);
        const auto hasProcessedInIME = inputContext->keyEvent(event);

        if (withShift_) {
            keyboard->sendShiftModifierToIM(inputContext, true);
        }

        if (hasProcessedInIME) return;
    }

    if (isRelease) {
        return;
    }

    inputContext->commitString(label(keyboard));
}

const char* NumberKey::label(VirtualKeyboard *) const {
    return name_.c_str();
}

void NumberKey::click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "NumberKey pushed: " << label(keyboard);

    auto event = fcitx::KeyEvent(inputContext, convert(false), isRelease);
    const auto hasProcessedInIME = inputContext->keyEvent(event);
    if (hasProcessedInIME || isRelease) return;

    inputContext->commitString(label(keyboard));
}

void ToggleKey::click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "ToggleKey pushed: " << label(keyboard);
    // This may be used for changing key layouts.
    // Changin key layouts must be executed on key-release,
    // because VirtualKeyboard has `pushingKey_` pointer.
    if (!isRelease) {
        return;
    }
    toggle(keyboard, inputContext);
}

void ToggleKey::fillLayout(
    VirtualKeyboard *keyboard,
    PangoLayout *layout,
    PangoAttrList *attrList
) {
    if (isOn(keyboard)) {
        addForegroundAttr(attrList, 0.2, 0.7, 0.6);
    } else {
        addForegroundAttr(attrList, 0.8, 0.8, 0.8);
    }
    pango_layout_set_text(layout, label(keyboard), -1);
}

void ShiftToggleKey::toggle(VirtualKeyboard *keyboard, InputContext *) {
    keyboard->toggleShift();
}

bool ShiftToggleKey::isOn(VirtualKeyboard *keyboard) {
    return keyboard->isShiftOn();
}

void SwitchKey::click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD() << "SwitchKey pushed: " << label(keyboard);

    // This may be used for changing key layouts.
    // Changin key layouts must be executed on key-release,
    // because VirtualKeyboard has `pushingKey_` pointer.
    if (!isRelease) {
        return;
    }

    switchState(keyboard, inputContext);
}

void SwitchKey::fillLayout(
    VirtualKeyboard *keyboard,
    PangoLayout *layout,
    PangoAttrList *attrList
) {
    std::string label;

    for (int i = 0; i < numberOfStates(); i++)
    {
        int end_index = label.size() + strlen(stateLabel(i));
        if (i == currentIndex(keyboard)) {
            addForegroundAttr(attrList, 0.2, 0.7, 0.6, label.size(), end_index);
        } else {
            addForegroundAttr(attrList, 0.8, 0.8, 0.8, label.size(), end_index);
        }
        label.append(stateLabel(i));
    }

    pango_layout_set_text(layout, label.c_str(), -1);
}

#if USE_CUSTOM_LAYOUT
void ModeSwitchKey::switchState(VirtualKeyboard *keyboard, InputContext *) {
    FCITX_KEYBOARD() << "ModeSwitchKey::switchState()";
    if (keyboard->languageCode() == "ru") {
        FCITX_KEYBOARD() << "i18Keyboard switchMode()";
        keyboard->i18nKeyboard<RussianKeyboard>()->switchMode();
    }
}

int ModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    FCITX_KEYBOARD() << "ModeSwitchKey::currentIndex()";
    if (keyboard->languageCode() == "ru") {
        if (keyboard->i18nKeyboard<RussianKeyboard>()->mode() ==
            RussianKeyboardMode::Text) {
            FCITX_KEYBOARD() << "ModeSwitchKey::currentIndex() RussianKeyboardMode::Text";
            return 0;
        }
    }
    return 1;
}
#endif

const char *LanguageSwitchKey::label(VirtualKeyboard *keyboard) const {
    return keyboard->i18nKeyboard()->label();
}

void LanguageSwitchKey::click(VirtualKeyboard *keyboard, InputContext *, bool isRelease) {
    FCITX_KEYBOARD() << "LanguageSwitchKey pushed";
    if (!isRelease) {
        return;
    }

    // check this key has been pushed and released in order to prevent miss click this key.
    if (dynamic_cast<LanguageSwitchKey *>(keyboard->pushingKey())) {
        keyboard->switchLanguage();
    }

    keyboard->updateInputPanel();
}

}
