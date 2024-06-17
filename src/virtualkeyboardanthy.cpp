/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardanthy.h"
#include "fcitx-utils/i18n.h"

namespace fcitx::classicui {

#if USE_CUSTOM_LAYOUT
AnthyKeyboard::AnthyKeyboard()
    : typingMethodNameOfKana_(
          translateDomain(anthyGetTextDomainName.c_str(), "Kana")),
      inputModeNameOfKatakana_(
          translateDomain(anthyGetTextDomainName.c_str(), "Katakana")),
      inputModeNameOfHalfKatakana_(translateDomain(
          anthyGetTextDomainName.c_str(), "Half width katakana")) {

    const char *jsonPath =
        FCITX_INSTALL_PKGDATADIR "/virtualkeyboardui/virtualkeyboardui-ja.json";
    FCITX_KEYBOARD_LAYOUT()
        << "path of Japanese keyboard layout file: " << jsonPath;
    loader_.reset(new KeyboardLayout(jsonPath));
}
#else
AnthyKeyboard::AnthyKeyboard()
    : typingMethodNameOfKana_(
          translateDomain(anthyGetTextDomainName.c_str(), "Kana")),
      inputModeNameOfKatakana_(
          translateDomain(anthyGetTextDomainName.c_str(), "Katakana")),
      inputModeNameOfHalfKatakana_(translateDomain(
          anthyGetTextDomainName.c_str(), "Half width katakana")) {}
#endif

void AnthyKeyboard::updateKeys() {
#if USE_CUSTOM_LAYOUT
    FCITX_KEYBOARD_LAYOUT() << "updateKeys()";
    if (mode_ == AnthyKeyboardMode::Text) {
        if (typingMethod_ == AnthyTypingMethod::Romaji || !isZenkakuOn_) {
            // Romaji
            if (numpadMode_ == AnthyNumpadMode::Number) {
                setLayerKeys(static_cast<int>(AnthyKeyboardMode::Text));
            } else {
                setLayerKeys(static_cast<int>(AnthyKeyboardMode::Text) + 1);
            }
        } else {
            // JisKana
            if (numpadMode_ == AnthyNumpadMode::Number) {
                setLayerKeys(4 * static_cast<int>(AnthyTypingMethod::Kana));
            } else {
                setLayerKeys(4 * static_cast<int>(AnthyTypingMethod::Kana) + 1);
            }
        }
    } else {
        // Mark
        if (numpadMode_ == AnthyNumpadMode::Number) {
            setLayerKeys(2 * static_cast<int>(AnthyKeyboardMode::Mark));
        } else {
            setLayerKeys(2 * static_cast<int>(AnthyKeyboardMode::Mark) + 1);
        }
    }
#else
    if (mode_ == AnthyKeyboardMode::Text) {
        if (typingMethod_ == AnthyTypingMethod::Romaji || !isZenkakuOn_) {
            setTextRomajiKeys();
        } else {
            setTextJisKanaKeys();
        }
    } else {
        setMarkKeys();
    }
#endif
}

#if USE_CUSTOM_LAYOUT
void AnthyKeyboard::setLayerKeys(size_t offset) {
    FCITX_KEYBOARD_LAYOUT() << "setLayerKeys(): offset: " << offset;
    keys_.clear();
    loader_->load(offset);
    FCITX_KEYBOARD_LAYOUT()
        << "loaded size of keys: " << loader_->keys().size();
    for (size_t i = 0; i < loader_->keys().size(); i++) {
        keys_.emplace_back(std::move(loader_->keys()[i]));
    }
}
#endif

void AnthyKeyboard::syncState(VirtualKeyboard *keyboard,
                              const std::string &currentInputMethodName) {
    isZenkakuOn_ = currentInputMethodName != hankakuImeName;

    // In Mark-mode, TypingMethod of IME must be fixed to Romaji, without
    // depending on this VirtualKeyboard's state. This is because most of
    // Zenkaku-marks can't be handled in Kana-mode.
    if (mode_ != AnthyKeyboardMode::Mark)
        syncTypingMethod(keyboard);

    syncInputMode(keyboard);
}

void AnthyKeyboard::syncTypingMethod(VirtualKeyboard *keyboard) {
    auto [curName, hasFound] =
        keyboard->getIMActionText(actionNameOfAnthyTypingMethod, false);
    if (!hasFound)
        return;

    if (curName == typingMethodNameOfKana_) {
        if (typingMethod_ != AnthyTypingMethod::Kana) {
            typingMethod_ = AnthyTypingMethod::Kana;
            updateKeys();
        }
    } else {
        if (typingMethod_ != AnthyTypingMethod::Romaji) {
            typingMethod_ = AnthyTypingMethod::Romaji;
            updateKeys();
        }
    }
}

void AnthyKeyboard::syncInputMode(VirtualKeyboard *keyboard) {
    auto [curName, hasFound] =
        keyboard->getIMActionText(actionNameOfAnthyInputMode, false);
    if (!hasFound)
        return;

    if (curName == inputModeNameOfKatakana_) {
        inputMode_ = AnthyInputMode::Katakana;
    } else if (curName == inputModeNameOfHalfKatakana_) {
        inputMode_ = AnthyInputMode::HalfKatakana;
    } else {
        inputMode_ = AnthyInputMode::Hiragana;
    }
}

void AnthyKeyboard::switchMode(VirtualKeyboard *keyboard) {
    if (mode_ == AnthyKeyboardMode::Text) {
        mode_ = AnthyKeyboardMode::Mark;
        if (typingMethod_ == AnthyTypingMethod::Kana) {
            // Most of Zenkaku-marks can't be handled in Kana-mode,
            // so fix IME's state to Romaji without changing this keyboard's
            // state.
            keyboard->activateIMAction(actionNameOfAnthyRomaji);
        }
    } else {
        mode_ = AnthyKeyboardMode::Text;
        if (typingMethod_ == AnthyTypingMethod::Kana) {
            // IME's state is fixed to Romaji in Mark-mode, so we have to put it
            // back.
            keyboard->activateIMAction(actionNameOfAnthyKana);
        }
    }

    updateKeys();
}

void AnthyKeyboard::switchNumpadMode() {
    if (numpadMode_ == AnthyNumpadMode::Number) {
        numpadMode_ = AnthyNumpadMode::Setting;
    } else {
        numpadMode_ = AnthyNumpadMode::Number;
    }

    updateKeys();
}

void AnthyKeyboard::switchTypingMethod(VirtualKeyboard *keyboard) {
    if (typingMethod_ == AnthyTypingMethod::Romaji) {
        typingMethod_ = AnthyTypingMethod::Kana;
        // Most of Zenkaku-marks can't be handled in Kana-mode,
        // so fix IME's state to Romaji.
        if (mode_ != AnthyKeyboardMode::Mark) {
            keyboard->activateIMAction(actionNameOfAnthyKana);
        }
    } else {
        typingMethod_ = AnthyTypingMethod::Romaji;
        keyboard->activateIMAction(actionNameOfAnthyRomaji);
    }

    updateKeys();
}

void AnthyKeyboard::switchInputMode(VirtualKeyboard *keyboard) {
    if (inputMode_ == AnthyInputMode::Hiragana) {
        inputMode_ = AnthyInputMode::Katakana;
        keyboard->activateIMAction(actionNameOfAnthyKatakana);
    } else if (inputMode_ == AnthyInputMode::Katakana) {
        inputMode_ = AnthyInputMode::HalfKatakana;
        keyboard->activateIMAction(actionNameOfAnthyHalfKatakana);
    } else {
        inputMode_ = AnthyInputMode::Hiragana;
        keyboard->activateIMAction(actionNameOfAnthyHiragana);
    }
}

void AnthyKeyboard::toggleZenkakuHankaku(VirtualKeyboard *keyboard) {
    isZenkakuOn_ = !isZenkakuOn_;
    if (isZenkakuOn_) {
        keyboard->setCurrentInputMethod(imeNames[type()]);
        if (typingMethod_ == AnthyTypingMethod::Kana)
            updateKeys();
    } else {
        keyboard->setCurrentInputMethod(hankakuImeName);
        if (typingMethod_ == AnthyTypingMethod::Kana)
            updateKeys();
    }
}

const char *AnthyMarkKey::label(VirtualKeyboard *keyboard) const {
    if (keyboard->i18nKeyboard<AnthyKeyboard>()->isZenkakuOn()) {
        return zenkakuMark_.c_str();
    }
    return hankakuMark_.c_str();
}

const char *AnthyKanaKey::label(VirtualKeyboard *keyboard) const {
    if (isNumberKey_ && keyboard->isSelectingCandidates()) {
        return name_.c_str();
    }

    return super::label(keyboard);
}

void AnthyKanaNumPadKey::click(VirtualKeyboard *keyboard,
                               InputContext *inputContext, bool isRelease) {
    FCITX_KEYBOARD_LAYOUT() << "AnthyKanaNumPadKey pushed";

    // In JIS-kana-mode of fcitx5-anthy, number keys are used for inputting
    // KANAs, not numbers. So limit sending the event to IME into the case
    // selecting candidates.
    if (keyboard->isSelectingCandidates()) {
        auto event = KeyEvent(inputContext, fcitx::Key(name_), isRelease);
        inputContext->keyEvent(event);
        return;
    }

    if (isRelease)
        return;

    inputContext->commitString(label(keyboard));
}

void ZenkakuHankakuKey::toggle(VirtualKeyboard *keyboard, InputContext *) {
    keyboard->i18nKeyboard<AnthyKeyboard>()->toggleZenkakuHankaku(keyboard);
    // Because this switching changes the size of the keyboard.
    keyboard->updateInputPanel();
}

bool ZenkakuHankakuKey::isOn(VirtualKeyboard *keyboard) {
    return keyboard->i18nKeyboard<AnthyKeyboard>()->isZenkakuOn();
}

void AnthyModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                     InputContext *) {
    keyboard->i18nKeyboard<AnthyKeyboard>()->switchMode(keyboard);
    // Because this switching changes the size of the keyboard.
    keyboard->updateInputPanel();
}

int AnthyModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<AnthyKeyboard>()->mode() ==
        AnthyKeyboardMode::Text) {
        return 0;
    }
    return 1;
}

void AnthyNumpadModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                           InputContext *) {
    keyboard->i18nKeyboard<AnthyKeyboard>()->switchNumpadMode();
}

int AnthyNumpadModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<AnthyKeyboard>()->numpadMode() ==
        AnthyNumpadMode::Number) {
        return 0;
    }
    return 1;
}

void AnthyTypingMethodSwitchKey::switchState(VirtualKeyboard *keyboard,
                                             InputContext *) {
    keyboard->i18nKeyboard<AnthyKeyboard>()->switchTypingMethod(keyboard);
    keyboard->updateInputPanel();
}

int AnthyTypingMethodSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<AnthyKeyboard>()->typingMethod() ==
        AnthyTypingMethod::Romaji) {
        return 0;
    }
    return 1;
}

void AnthyInputModeSwitchKey::switchState(VirtualKeyboard *keyboard,
                                          InputContext *) {
    keyboard->i18nKeyboard<AnthyKeyboard>()->switchInputMode(keyboard);
}

int AnthyInputModeSwitchKey::currentIndex(VirtualKeyboard *keyboard) {
    if (keyboard->i18nKeyboard<AnthyKeyboard>()->inputMode() ==
        AnthyInputMode::Hiragana) {
        return 0;
    } else if (keyboard->i18nKeyboard<AnthyKeyboard>()->inputMode() ==
               AnthyInputMode::Katakana) {
        return 1;
    }
    return 2;
}

#if !USE_CUSTOM_LAYOUT
// clang-format off
void AnthyKeyboard::setTextRomajiKeys() {
    keys_.clear();
    keys_.emplace_back(new NormalKey("q", 24, "Q", true));
    keys_.emplace_back(new NormalKey("w", 25, "W", true));
    keys_.emplace_back(new NormalKey("e", 26, "E", true));
    keys_.emplace_back(new NormalKey("r", 27, "R", true));
    keys_.emplace_back(new NormalKey("t", 28, "T", true));
    keys_.emplace_back(new NormalKey("y", 29, "Y", true));
    keys_.emplace_back(new NormalKey("u", 30, "U", true));
    keys_.emplace_back(new NormalKey("i", 31, "I", true));
    keys_.emplace_back(new NormalKey("o", 32, "O", true));
    keys_.emplace_back(new NormalKey("p", 33, "P", true));
    keys_.emplace_back(new BackSpaceKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("7", "7", "７"));
        keys_.emplace_back(new AnthyMarkKey("8", "8", "８"));
        keys_.emplace_back(new AnthyMarkKey("9", "9", "９")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new AnthyTypingMethodSwitchKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new NormalKey("a", 38, "A", true));
    keys_.emplace_back(new NormalKey("s", 39, "S", true));
    keys_.emplace_back(new NormalKey("d", 40, "D", true));
    keys_.emplace_back(new NormalKey("f", 41, "F", true));
    keys_.emplace_back(new NormalKey("g", 42, "G", true));
    keys_.emplace_back(new NormalKey("h", 43, "H", true));
    keys_.emplace_back(new NormalKey("j", 44, "J", true));
    keys_.emplace_back(new NormalKey("k", 45, "K", true));
    keys_.emplace_back(new NormalKey("l", 46, "L", true));
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("4", "4", "４"));
        keys_.emplace_back(new AnthyMarkKey("5", "5", "５"));
        keys_.emplace_back(new AnthyMarkKey("6", "6", "６")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new AnthyInputModeSwitchKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new ShiftToggleKey());
    keys_.emplace_back(new NormalKey("z", 52, "Z", true));
    keys_.emplace_back(new NormalKey("x", 53, "X", true));
    keys_.emplace_back(new NormalKey("c", 54, "C", true));
    keys_.emplace_back(new NormalKey("v", 55, "V", true));
    keys_.emplace_back(new NormalKey("b", 56, "B", true));
    keys_.emplace_back(new NormalKey("n", 57, "N", true));
    keys_.emplace_back(new NormalKey("m", 58, "M", true));
    keys_.emplace_back(new AnthyMarkKey("minus", "-", "ー"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("1", "1", "１"));
        keys_.emplace_back(new AnthyMarkKey("2", "2", "２"));
        keys_.emplace_back(new AnthyMarkKey("3", "3", "３")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new EmptyKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new AnthyModeSwitchKey());
    keys_.emplace_back(new ZenkakuHankakuKey());
    keys_.emplace_back(new AnthyMarkKey("comma", ",", "、"));
    keys_.emplace_back(new AnthyMarkKey("period", ".", "。"));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new AnthyMarkKey("exclam", "!", "！"));
    keys_.emplace_back(new AnthyMarkKey("question", "?", "？"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("0", "0", "０"));
        keys_.emplace_back(new AnthyMarkKey("period", ".", "。"));
    } else {
        keys_.emplace_back(new EmptyKey()); keys_.back()->setCustomLayout(2.0);
    }
    keys_.emplace_back(new AnthyNumpadModeSwitchKey());
}

void AnthyKeyboard::setMarkKeys() {
    keys_.clear();
    keys_.emplace_back(new AnthyMarkKey("bracketleft", "[", "「"));
    keys_.emplace_back(new AnthyMarkKey("bracketright", "]", "」"));
    keys_.emplace_back(new AnthyMarkKey("braceleft", "{", "｛"));
    keys_.emplace_back(new AnthyMarkKey("braceright", "}", "｝"));
    keys_.emplace_back(new AnthyMarkKey("numbersign", "#", "＃"));
    keys_.emplace_back(new AnthyMarkKey("percent", "%", "％"));
    keys_.emplace_back(new AnthyMarkKey("asciicircum", "^", "＾"));
    keys_.emplace_back(new AnthyMarkKey("asterisk", "*", "＊"));
    keys_.emplace_back(new AnthyMarkKey("plus", "+", "＋"));
    keys_.emplace_back(new AnthyMarkKey("equal", "=", "＝"));
    keys_.emplace_back(new BackSpaceKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("7", "7", "７"));
        keys_.emplace_back(new AnthyMarkKey("8", "8", "８"));
        keys_.emplace_back(new AnthyMarkKey("9", "9", "９")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new AnthyTypingMethodSwitchKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    keys_.emplace_back(new AnthyMarkKey("slash", "/", "・"));
    keys_.emplace_back(new AnthyMarkKey("backslash", "\\", "＼"));
    keys_.emplace_back(new AnthyMarkKey("colon", ":", "："));
    keys_.emplace_back(new AnthyMarkKey("semicolon", ";", "；"));
    keys_.emplace_back(new AnthyMarkKey("parenleft", "(", "（"));
    keys_.emplace_back(new AnthyMarkKey("parenright", ")", "）"));
    keys_.emplace_back(new AnthyMarkKey("ampersand", "&", "＆"));
    keys_.emplace_back(new AnthyMarkKey("at", "@", "＠"));
    // `FcitxKey_yen: 0x00a5` does not work in fcitx5-anthy,
    // so use `FcitxKey_backslash` for Zenkaku.
    keys_.emplace_back(new AnthyMarkKey("backslash", u8"\u00A5", "＼"));
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.5);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("4", "4", "４"));
        keys_.emplace_back(new AnthyMarkKey("5", "5", "５"));
        keys_.emplace_back(new AnthyMarkKey("6", "6", "６")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new AnthyInputModeSwitchKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new ShiftToggleKey());
    keys_.emplace_back(new AnthyMarkKey("quotedbl", "\"", "”"));
    keys_.emplace_back(new AnthyMarkKey("apostrophe", "\'", "’"));
    keys_.emplace_back(new AnthyMarkKey("underscore", "_", "＿"));
    keys_.emplace_back(new AnthyMarkKey("bar", "|", "｜"));
    keys_.emplace_back(new AnthyMarkKey("asciitilde", "~", "〜"));
    keys_.emplace_back(new AnthyMarkKey("less", "<", "＜"));
    keys_.emplace_back(new AnthyMarkKey("greater", ">", "＞"));
    keys_.emplace_back(new AnthyMarkKey("minus", "-", "ー"));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("1", "1", "１"));
        keys_.emplace_back(new AnthyMarkKey("2", "2", "２"));
        keys_.emplace_back(new AnthyMarkKey("3", "3", "３")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new EmptyKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new AnthyModeSwitchKey());
    keys_.emplace_back(new ZenkakuHankakuKey());
    keys_.emplace_back(new AnthyMarkKey("comma", ",", "、"));
    keys_.emplace_back(new AnthyMarkKey("period", ".", "。"));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(2.0);
    keys_.emplace_back(new AnthyMarkKey("exclam", "!", "！"));
    keys_.emplace_back(new AnthyMarkKey("question", "?", "？"));
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyMarkKey("0", "0", "０"));
        keys_.emplace_back(new AnthyMarkKey("period", ".", "。"));
    } else {
        keys_.emplace_back(new EmptyKey()); keys_.back()->setCustomLayout(2.0);
    }
    keys_.emplace_back(new AnthyNumpadModeSwitchKey());
}

void AnthyKeyboard::setTextJisKanaKeys() {
    keys_.clear();
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.4);
    keys_.emplace_back(new AnthyKanaKey("ぬ", 10, "", "1", "", true));
    keys_.emplace_back(new AnthyKanaKey("ふ", 11, "", "2", "", true));
    keys_.emplace_back(new AnthyKanaKey("あ", 12, "ぁ", "3", "numbersign", true));
    keys_.emplace_back(new AnthyKanaKey("う", 13, "ぅ", "4", "dollar", true));
    keys_.emplace_back(new AnthyKanaKey("え", 14, "ぇ", "5", "percent", true));
    keys_.emplace_back(new AnthyKanaKey("お", 15, "ぉ", "6", "ampersand", true));
    keys_.emplace_back(new AnthyKanaKey("や", 16, "ゃ", "7", "apostrophe", true));
    keys_.emplace_back(new AnthyKanaKey("ゆ", 17, "ゅ", "8", "parenleft", true));
    keys_.emplace_back(new AnthyKanaKey("よ", 18, "ょ", "9", "parenright", true));
    keys_.emplace_back(new AnthyKanaKey("わ", 19, "を", "0", "asciitilde", true));
    keys_.emplace_back(new AnthyKanaKey("ほ", 19, "", "minus", ""));
    keys_.emplace_back(new AnthyKanaKey("へ", 19, "", "asciicircum", ""));
    keys_.emplace_back(new AnthyKanaKey("ー", 20, "", "backslash", "")); keys_.back()->setCustomLayout(1.0, true);

    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.4);
    keys_.emplace_back(new AnthyKanaKey("た", 24, "", "q", "Q"));
    keys_.emplace_back(new AnthyKanaKey("て", 25, "", "w", "W"));
    keys_.emplace_back(new AnthyKanaKey("い", 26, "ぃ", "e", "E"));
    keys_.emplace_back(new AnthyKanaKey("す", 27, "", "r", "R"));
    keys_.emplace_back(new AnthyKanaKey("か", 28, "ヵ", "t", "T"));
    keys_.emplace_back(new AnthyKanaKey("ん", 29, "", "y", "Y"));
    keys_.emplace_back(new AnthyKanaKey("な", 30, "", "u", "U"));
    keys_.emplace_back(new AnthyKanaKey("に", 31, "", "i", "I"));
    keys_.emplace_back(new AnthyKanaKey("ら", 32, "", "o", "O"));
    keys_.emplace_back(new AnthyKanaKey("せ", 33, "", "p", "P"));
    keys_.emplace_back(new AnthyKanaKey("゛", 34, "", "at", ""));
    keys_.emplace_back(new AnthyKanaKey("゜", 35, "「", "bracketleft", "braceleft"));
    keys_.emplace_back(new BackSpaceKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyKanaNumPadKey("7"));
        keys_.emplace_back(new AnthyKanaNumPadKey("8"));
        keys_.emplace_back(new AnthyKanaNumPadKey("9")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new AnthyTypingMethodSwitchKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new AnthyKanaKey("ち", 38, "", "a", "A"));
    keys_.emplace_back(new AnthyKanaKey("と", 39, "", "s", "S"));
    keys_.emplace_back(new AnthyKanaKey("し", 40, "", "d", "D"));
    keys_.emplace_back(new AnthyKanaKey("は", 41, "ゎ", "f", "F"));
    keys_.emplace_back(new AnthyKanaKey("き", 42, "", "g", "G"));
    keys_.emplace_back(new AnthyKanaKey("く", 43, "", "h", "H"));
    keys_.emplace_back(new AnthyKanaKey("ま", 44, "", "j", "J"));
    keys_.emplace_back(new AnthyKanaKey("の", 45, "", "k", "K"));
    keys_.emplace_back(new AnthyKanaKey("り", 46, "", "l", "L"));
    keys_.emplace_back(new AnthyKanaKey("れ", 47, "", "semicolon", ""));
    keys_.emplace_back(new AnthyKanaKey("け", 48, "", "colon", ""));
    keys_.emplace_back(new AnthyKanaKey("む", 49, "」", "bracketright", "braceright"));
    keys_.emplace_back(new EnterKey()); keys_.back()->setCustomLayout(1.4);
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyKanaNumPadKey("4"));
        keys_.emplace_back(new AnthyKanaNumPadKey("5"));
        keys_.emplace_back(new AnthyKanaNumPadKey("6")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new AnthyInputModeSwitchKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new ShiftToggleKey()); keys_.back()->setCustomLayout(1.0);
    keys_.emplace_back(new AnthyKanaKey("つ", 52, "っ", "z", "Z"));
    keys_.emplace_back(new AnthyKanaKey("さ", 53, "", "x", "X"));
    keys_.emplace_back(new AnthyKanaKey("そ", 54, "", "c", "C"));
    keys_.emplace_back(new AnthyKanaKey("ひ", 55, "ゐ", "v", "V"));
    keys_.emplace_back(new AnthyKanaKey("こ", 56, "", "b", "B"));
    keys_.emplace_back(new AnthyKanaKey("み", 57, "", "n", "N"));
    keys_.emplace_back(new AnthyKanaKey("も", 58, "", "m", "M"));
    keys_.emplace_back(new AnthyKanaKey("ね", 59, "", "comma", ""));
    keys_.emplace_back(new AnthyKanaKey("る", 60, "", "period", ""));
    keys_.emplace_back(new AnthyKanaKey("め", 61, "", "slash", ""));
    keys_.emplace_back(new UpKey());
    keys_.emplace_back(new LanguageSwitchKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyKanaNumPadKey("1"));
        keys_.emplace_back(new AnthyKanaNumPadKey("2"));
        keys_.emplace_back(new AnthyKanaNumPadKey("3")); keys_.back()->setCustomLayout(1.0, true);
    } else {
        keys_.emplace_back(new EmptyKey()); keys_.back()->setCustomLayout(3.0, true);
    }

    keys_.emplace_back(new AnthyModeSwitchKey());
    keys_.emplace_back(new ZenkakuHankakuKey());
    keys_.emplace_back(new MarkKey("、", "less", 0, true));
    keys_.emplace_back(new SpaceKey()); keys_.back()->setCustomLayout(3.0);
    keys_.emplace_back(new MarkKey("。", "greater", 0, true));
    keys_.emplace_back(new AnthyKanaKey("ろ", 62, "", "underscore", "")); keys_.back()->setCustomLayout(1.25);
    keys_.emplace_back(new LeftKey());
    keys_.emplace_back(new DownKey());
    keys_.emplace_back(new RightKey());
    keys_.emplace_back(new DummyKey()); keys_.back()->setCustomLayout(0.5);
    if (numpadMode_ == AnthyNumpadMode::Number) {
        keys_.emplace_back(new AnthyKanaNumPadKey("0"));
        keys_.emplace_back(new MarkKey("."));
    } else {
        keys_.emplace_back(new EmptyKey()); keys_.back()->setCustomLayout(2.0);
    }
    keys_.emplace_back(new AnthyNumpadModeSwitchKey());
}
#endif

}
