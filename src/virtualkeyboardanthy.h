/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDANTHY_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDANTHY_H_

#include "virtualkeyboard.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeygeneral.h"
#if USE_CUSTOM_LAYOUT
#include "virtualkeyboardlayout.h"
#endif

namespace fcitx {
namespace classicui {

enum class AnthyKeyboardMode {
    Text,
    Mark,
};

enum class AnthyNumpadMode {
    Number,
    Setting,
};

enum class AnthyTypingMethod {
    Romaji,
    Kana,
};

enum class AnthyInputMode {
    Hiragana,
    Katakana,
    HalfKatakana,
};

static const std::string hankakuImeName = "keyboard-us";
static const std::string actionNameOfAnthyRomaji = "anthy-typing-method-romaji";
static const std::string actionNameOfAnthyKana = "anthy-typing-method-kana";
static const std::string actionNameOfAnthyHiragana =
    "anthy-input-mode-hiragana";
static const std::string actionNameOfAnthyKatakana =
    "anthy-input-mode-katakana";
static const std::string actionNameOfAnthyHalfKatakana =
    "anthy-input-mode-half-katakana";
static const std::string actionNameOfAnthyTypingMethod = "anthy-typing-method";
static const std::string actionNameOfAnthyInputMode = "anthy-input-mode";
static const std::string anthyGetTextDomainName = "fcitx5-anthy";

class AnthyKeyboard : public I18nKeyboard {
public:
    AnthyKeyboard();
    KeyboardType type() const override { return KeyboardType::Anthy; };
    const char *label() const override { return "JP"; }
    const std::string languageCode() const override { return "ja"; }
    void updateKeys() override;
    void syncState(VirtualKeyboard *keyboard,
                   const std::string &currentInputMethodName) override;

    void switchMode(VirtualKeyboard *keyboard);
    AnthyKeyboardMode mode() const { return mode_; }

    void switchNumpadMode();
    AnthyNumpadMode numpadMode() const { return numpadMode_; }

    void switchTypingMethod(VirtualKeyboard *keyboard);
    AnthyTypingMethod typingMethod() const { return typingMethod_; }

    void switchInputMode(VirtualKeyboard *keyboard);
    AnthyInputMode inputMode() const { return inputMode_; }

    bool isZenkakuOn() const { return isZenkakuOn_; }
    void toggleZenkakuHankaku(VirtualKeyboard *keyboard);

protected:
    std::vector<std::string> otherNecessaryImeList() override {
        return {hankakuImeName};
    }

private:
#if USE_CUSTOM_LAYOUT
    void setLayerKeys(size_t offset);
    std::unique_ptr<KeyboardLayout> loader_;
#else
    void setTextRomajiKeys();
    void setMarkKeys();
    void setTextJisKanaKeys();
#endif
    void syncTypingMethod(VirtualKeyboard *keyboard);
    void syncInputMode(VirtualKeyboard *keyboard);
    AnthyKeyboardMode mode_ = AnthyKeyboardMode::Text;
    AnthyNumpadMode numpadMode_ = AnthyNumpadMode::Number;
    AnthyTypingMethod typingMethod_ = AnthyTypingMethod::Romaji;
    AnthyInputMode inputMode_ = AnthyInputMode::Hiragana;
    bool isZenkakuOn_ = true;
    const std::string typingMethodNameOfKana_;
    const std::string inputModeNameOfKatakana_;
    const std::string inputModeNameOfHalfKatakana_;
};

class AnthyMarkKey : public MarkKey {
public:
    AnthyMarkKey(const std::string &name, const std::string &hankakuMark,
                 const std::string &zenkakuMark)
        : MarkKey("", name), hankakuMark_(hankakuMark),
          zenkakuMark_(zenkakuMark) {}
    const char *label(VirtualKeyboard *keyboard) const override;

private:
    const std::string hankakuMark_;
    const std::string zenkakuMark_;
};

class AnthyKanaKey : public NormalKey {
public:
    AnthyKanaKey(const std::string &label, uint32_t code,
                 const std::string &upperLabel, const std::string &name,
                 const std::string &upperName, bool isNumberKey = false)
        : NormalKey(label, code, upperLabel, name, upperName),
          isNumberKey_(isNumberKey) {
        setCustomLayout(0.8);
    }

    const char *label(VirtualKeyboard *keyboard) const override;

private:
    typedef NormalKey super;
    bool isNumberKey_;
};

class AnthyKanaNumPadKey : public NumberKey {
public:
    AnthyKanaNumPadKey(const std::string &number) : NumberKey(number) {}
    void click(VirtualKeyboard *keyboard, InputContext *inputContext,
               bool isRelease) override;
};

class ZenkakuHankakuKey : public ToggleKey {
public:
    ZenkakuHankakuKey() { setFontSize(18); }
    const char *label(VirtualKeyboard *) const override { return "全角"; }

protected:
    void toggle(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    bool isOn(VirtualKeyboard *keyboard) override;
};

class AnthyModeSwitchKey : public SwitchKey {
public:
    AnthyModeSwitchKey() : SwitchKey() {}
    const char *label(VirtualKeyboard *) const override { return "A#"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        switch (index) {
        case 0:
            return "A";
        case 1:
            return "#";
        default:
            break;
        }
        return "";
    }
    void switchState(VirtualKeyboard *keyboard,
                     InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

class AnthyNumpadModeSwitchKey : public SwitchKey {
public:
    AnthyNumpadModeSwitchKey() : SwitchKey() {}
    const char *label(VirtualKeyboard *) const override { return "#S"; }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        switch (index) {
        case 0:
            return "#";
        case 1:
            return "S";
        default:
            break;
        }
        return "";
    }
    void switchState(VirtualKeyboard *keyboard,
                     InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

class AnthyTypingMethodSwitchKey : public SwitchKey {
public:
    AnthyTypingMethodSwitchKey() : SwitchKey() {}
    const char *label(VirtualKeyboard *) const override {
        return "ローマ字 かな";
    }

protected:
    int numberOfStates() const override { return 2; }
    const char *stateLabel(int index) const override {
        switch (index) {
        case 0:
            return "ローマ字 ";
        case 1:
            return "かな";
        default:
            break;
        }
        return "";
    }
    void switchState(VirtualKeyboard *keyboard,
                     InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

class AnthyInputModeSwitchKey : public SwitchKey {
public:
    AnthyInputModeSwitchKey() : SwitchKey() {}
    const char *label(VirtualKeyboard *) const override { return "あ ア ｱ"; }

protected:
    int numberOfStates() const override { return 3; }
    const char *stateLabel(int index) const override {
        switch (index) {
        case 0:
            return "あ ";
        case 1:
            return "ア ";
        case 2:
            return "ｱ";
        default:
            break;
        }
        return "";
    }
    void switchState(VirtualKeyboard *keyboard,
                     InputContext *inputContext) override;
    int currentIndex(VirtualKeyboard *keyboard) override;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDANTHY_H_
