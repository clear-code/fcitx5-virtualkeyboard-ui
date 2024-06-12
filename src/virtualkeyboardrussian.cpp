/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardrussian.h"

namespace fcitx::classicui {

void RussianKeyboard::updateKeys() {
    FCITX_KEYBOARD_LAYOUT() << "RussianKeyboard::updateKeys()";
#if USE_CUSTOM_LAYOUT
    FCITX_KEYBOARD_LAYOUT()
        << "RussianKeyboard:: mode: " << static_cast<int>(mode_);
    if (mode_ == RussianKeyboardMode::Mark) {
        FCITX_KEYBOARD_LAYOUT() << "RussianKeyboard::mode_: Mark";
        setLayerKeys(static_cast<int>(RussianKeyboardMode::Mark));
        return;
    }

    FCITX_KEYBOARD_LAYOUT() << "RussianKeyboard::mode_: Text";
    setLayerKeys(static_cast<int>(RussianKeyboardMode::Text));
#else
    if (mode_ == RussianKeyboardMode::Mark) {
        setMarkKeys();
        return;
    }

    setCyrillicTextKeys();
#endif
}

void RussianKeyboard::switchMode() {
    FCITX_KEYBOARD_LAYOUT() << "RussianKeyboard::switchMode()";
    if (mode_ == RussianKeyboardMode::Text) {
        mode_ = RussianKeyboardMode::Mark;
    } else {
        mode_ = RussianKeyboardMode::Text;
    }

    updateKeys();
}

#if !USE_CUSTOM_LAYOUT
void RussianModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                       InputContext *) {
    keyboard->i18nKeyboard<RussianKeyboard>()->switchMode();
}

int RussianModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<RussianKeyboard>()->mode() ==
        RussianKeyboardMode::Text) {
        return 0;
    }
    return 1;
}
#endif

#if USE_CUSTOM_LAYOUT
void RussianKeyboard::setLayerKeys(size_t offset) {
    FCITX_KEYBOARD_LAYOUT() << "setLayerKeys(): offset: " << offset;
    keys_.clear();
    loader_->load(offset);
    FCITX_KEYBOARD_LAYOUT()
        << "loaded size of keys: " << loader_->keys().size();
    for (size_t i = 0; i < loader_->keys().size(); i++) {
        keys_.emplace_back(loader_->keys()[i]);
    }
}
#else
// clang-format off
void RussianKeyboard::setCyrillicTextKeys() {
    keys_.clear();
    keys_.emplace_back(new RussianNormalKey("й", 24, "Й"));
    keys_.emplace_back(new RussianNormalKey("ц", 25, "Ц"));
    keys_.emplace_back(new RussianNormalKey("ч", 26, "Ч"));
    keys_.emplace_back(new RussianNormalKey("к", 27, "К"));
    keys_.emplace_back(new RussianNormalKey("е", 28, "Е"));
    keys_.emplace_back(new RussianNormalKey("н", 29, "Н"));
    keys_.emplace_back(new RussianNormalKey("г", 30, "Г"));
    keys_.emplace_back(new RussianNormalKey("ш", 31, "Ш"));
    keys_.emplace_back(new RussianNormalKey("щ", 32, "Щ"));
    keys_.emplace_back(new RussianNormalKey("з", 33, "З"));
    keys_.emplace_back(new RussianNormalKey("х", 34, "Х"));
    keys_.emplace_back(new RussianNormalKey("ъ", 35, "Ъ"));
    keys_.emplace_back(new BackSpaceKey()); keys_.back()->setCustomLayout(1.0);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("7", 16));
    keys_.emplace_back(new NumberKey("8", 17));
    keys_.emplace_back(new NumberKey("9", 18)); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new RussianDummyKey(20));
    keys_.emplace_back(new RussianNormalKey("ф", 38, "Ф"));
    keys_.emplace_back(new RussianNormalKey("ы", 39, "Ы"));
    keys_.emplace_back(new RussianNormalKey("в", 40, "В"));
    keys_.emplace_back(new RussianNormalKey("а", 41, "А"));
    keys_.emplace_back(new RussianNormalKey("п", 42, "П"));
    keys_.emplace_back(new RussianNormalKey("р", 43, "Р"));
    keys_.emplace_back(new RussianNormalKey("о", 44, "О"));
    keys_.emplace_back(new RussianNormalKey("л", 45, "Л"));
    keys_.emplace_back(new RussianNormalKey("д", 46, "Д"));
    keys_.emplace_back(new RussianNormalKey("ж", 47, "Ж"));
    keys_.emplace_back(new RussianNormalKey("э", 48, "Э"));
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4", 13));
    keys_.emplace_back(new NumberKey("5", 14));
    keys_.emplace_back(new NumberKey("6", 15)); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new RussianShiftToggleKey());
    keys_.emplace_back(new RussianNormalKey("я", 52, "Я"));
    keys_.emplace_back(new RussianNormalKey("у", 53, "У"));
    keys_.emplace_back(new RussianNormalKey("с", 54, "С"));
    keys_.emplace_back(new RussianNormalKey("м", 55, "М"));
    keys_.emplace_back(new RussianNormalKey("и", 56, "И"));
    keys_.emplace_back(new RussianNormalKey("т", 57, "Т"));
    keys_.emplace_back(new RussianNormalKey("ь", 58, "Ь"));
    keys_.emplace_back(new RussianNormalKey("б", 59, "Б"));
    keys_.emplace_back(new RussianNormalKey("ю", 60, "Ю"));
    keys_.emplace_back(new RussianNormalKey("ё", 49, "Ё"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1", 10));
    keys_.emplace_back(new NumberKey("2", 11));
    keys_.emplace_back(new NumberKey("3", 12)); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new RussianModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey(",", "", 61, true));
    keys_.emplace_back(new MarkKey(".", "", 61));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("!", "", 10, true));
    keys_.emplace_back(new MarkKey("?", "", 16, true));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0", 19)); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}

void RussianKeyboard::setMarkKeys() {
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
    keys_.emplace_back(new NumberKey("7", 16));
    keys_.emplace_back(new NumberKey("8", 17));
    keys_.emplace_back(new NumberKey("9", 18)); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new MarkKey("/"));
    keys_.emplace_back(new MarkKey("\\"));
    keys_.emplace_back(new MarkKey(":"));
    keys_.emplace_back(new MarkKey(";"));
    keys_.emplace_back(new MarkKey("("));
    keys_.emplace_back(new MarkKey(")"));
    keys_.emplace_back(new MarkKey("&"));
    keys_.emplace_back(new MarkKey("@"));
    keys_.emplace_back(new MarkKey(u8"\u20BD")); // rouble sign
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("4", 13));
    keys_.emplace_back(new NumberKey("5", 14));
    keys_.emplace_back(new NumberKey("6", 15)); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey());
    keys_.emplace_back(new MarkKey("\""));
    keys_.emplace_back(new MarkKey("\'"));
    keys_.emplace_back(new MarkKey("_"));
    keys_.emplace_back(new MarkKey("|"));
    keys_.emplace_back(new MarkKey("~"));
    keys_.emplace_back(new MarkKey("<"));
    keys_.emplace_back(new MarkKey(">"));
    keys_.emplace_back(new MarkKey("-"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("1", 10));
    keys_.emplace_back(new NumberKey("2", 11));
    keys_.emplace_back(new NumberKey("3", 12)); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new RussianModeSwitchKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey(","));
    keys_.emplace_back(new MarkKey("."));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("!"));
    keys_.emplace_back(new MarkKey("?"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NumberKey("0", 19)); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new MarkKey("."));
}
#endif

}
