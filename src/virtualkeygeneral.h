/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYGENERAL_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYGENERAL_H_

#include "virtualkeyboard.h"

namespace fcitx {
namespace classicui {

/*
 * For making empty space in keyboard layout.
 */
class DummyKey : public VirtualKey {
public:
    DummyKey() {
        visible_ = false;
    }
    DummyKey(double width) {
        visible_ = false;
        width_ = width;
    }
    const char* label(VirtualKeyboard *) const override { return ""; }
    void click(VirtualKeyboard *, InputContext *, bool) override {}
};

/*
 * Key that is visible, but do not work.
 */
class EmptyKey : public VirtualKey {
public:
    const char* label(VirtualKeyboard *) const override { return ""; }
    void click(VirtualKeyboard *, InputContext *, bool) override {}
};

/*
 * Base class that provides function to convert to fcitx::key.
 * The name of key is in keynametable.h and is used for taking the corresponding sym.
 */
class KeyByNameAndCode : public VirtualKey {
protected:
    KeyByNameAndCode(
        const std::string &name,
        uint32_t code,
        const std::string &upperName = ""
    ) : name_(name), upperName_(upperName), code_(code) {}

    std::string keyName(bool withShift = false) const {
        if (withShift) {
            auto baseSymName = upperName_;
            if (baseSymName.empty()) baseSymName = name_;
            if (baseSymName.empty()) baseSymName = "None";

            return "SHIFT_" + baseSymName;
        }
        return !name_.empty() ? name_: "None";
    };

    fcitx::Key convert(bool withShift = false) const {
        const auto keyFromName = fcitx::Key(keyName(withShift));
        return fcitx::Key(keyFromName.sym(), keyFromName.states(), code_);
    }

    /// Be used in converting to Fcitx::Key.
    /// Corresponding to keyNameList in keynametable.h.
    const std::string name_;

    /// Be used in converting to Fcitx::Key with shift on.
    /// Corresponding to keyNameList in keynametable.h.
    /// If this is empty, `name_` with `SHIFT_` prefix is used for the key name,
    /// which is converted to the state of the shift modifier.
    const std::string upperName_;

    const uint32_t code_;
};

/*
 * Normal key, which can act the same way as physical keys.
 */
class NormalKey : public KeyByNameAndCode {
public:
    /// `label` value is used for display.
    /// `name` value is used for taking the keysym.
    /// `upperLabel` and `upperName` are used with Shift on,
    /// and these values can be emtpy to use `label` and `name` instead.
    NormalKey(
        const std::string &label,
        uint32_t code,
        const std::string &upperLabel,
        const std::string &name,
        const std::string &upperName
    ) : KeyByNameAndCode(
            name,
            code,
            upperName
        ),
        label_(label),
        upperLabel_(upperLabel) {}

    /// If `useLabelAsKeyName` is true,
    /// then `label` value is also used for taking the keysym, not only for display.
    /// If `useLabelAsKeyName` is false,
    /// then this key works only with keycode, without keysym.
    NormalKey(
        const std::string &label,
        uint32_t code,
        const std::string &upperLabel,
        bool useLabelAsKeyName
    ) : KeyByNameAndCode(
            useLabelAsKeyName ? label : "",
            code,
            useLabelAsKeyName ? upperLabel : ""
        ),
        label_(label),
        upperLabel_(upperLabel) {}

    virtual const char* label(VirtualKeyboard *keyboard) const override;
    virtual void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;

protected:
    /// Text for display.
    const std::string label_;
    const std::string upperLabel_;
};

/*
 * Key for inputting marks.
 * Without `name` or `code` value, this simply inputs the label texts directly.
 * With them, this sends the event to IME first.
 */
class MarkKey : public KeyByNameAndCode {
public:
    MarkKey(
        const std::string &label,
        const std::string &name = "",
        uint32_t code = 0,
        bool withShift = false
    ) : KeyByNameAndCode(name, code),
        label_(label),
        withShift_(withShift) {}
    virtual const char* label(VirtualKeyboard *keyboard) const override;
    virtual void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;

private:
    bool sendKeyEventFirst() const { return !name_.empty() || code_ != 0; }

    /// Text for display, and commit-string.
    const std::string label_;

    /// This key doesn't depend on the Shift state of the keyboard.
    /// If needing Shift modifier in KeyEvent for IME, then use this value as `true`.
    bool withShift_;
};

/*
 * Key for inputting numbers. This is similar to MarkKey, but this sends the number to IME first.
 * If there are selectable candidates in IME, the number may be used for selecting them.
 */
class NumberKey : public KeyByNameAndCode {
public:
    NumberKey(
        const std::string &number,
        uint32_t code = 0
    ) : KeyByNameAndCode(number, code) {}
    virtual const char* label(VirtualKeyboard *keyboard) const override;
    virtual void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;
};

class SpaceKey : public NormalKey {
public:
    SpaceKey() : NormalKey("", 65, "", "space", "") {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class EnterKey : public NormalKey {
public:
    EnterKey() : NormalKey("Enter", 36, "", "Return", "") {
        setCustomBackgroundColor({0.2, 0.7, 0.6});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class BackSpaceKey : public NormalKey {
public:
    BackSpaceKey() : NormalKey("Back", 22, "", "BackSpace", "") {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class UpKey : public NormalKey {
public:
    UpKey() : NormalKey(u8"\u2191", 111, "", "Up", "") {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class LeftKey : public NormalKey {
public:
    LeftKey() : NormalKey(u8"\u2190", 113, "", "Left", "") {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class DownKey : public NormalKey {
public:
    DownKey() : NormalKey(u8"\u2193", 116, "", "Down", "") {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class RightKey : public NormalKey {
public:
    RightKey() : NormalKey(u8"\u2192", 114, "", "Right", "") {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    };
};

class ToggleKey : public VirtualKey {
public:
    ToggleKey() {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
    }
    virtual const char* label(VirtualKeyboard *) const = 0;
    void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;
    void fillLayout(
        VirtualKeyboard *keyboard,
        PangoLayout *layout,
        PangoAttrList *attrList
    ) override;

protected:
    virtual void toggle(VirtualKeyboard *keyboard, InputContext *inputContext) = 0;
    virtual bool isOn(VirtualKeyboard *keyboard) = 0;
};

class ShiftToggleKey : public ToggleKey {
public:
    ShiftToggleKey() {}
    const char* label(VirtualKeyboard *) const override { return u8"\u21E7"; }

protected:
    void toggle(VirtualKeyboard *keyboard, InputContext *inputContext) override;
    bool isOn(VirtualKeyboard *keyboard) override;
};

class SwitchKey : public VirtualKey {
public:
    SwitchKey() {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
    }
    virtual const char *label(VirtualKeyboard *) const = 0;
    void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;
    void fillLayout(
        VirtualKeyboard *keyboard,
        PangoLayout *layout,
        PangoAttrList *attrList
    ) override;

protected:
    virtual int numberOfStates() const = 0;
    virtual const char *stateLabel(int index) const = 0;
    virtual void switchState(VirtualKeyboard *keyboard, InputContext *inputContext) = 0;
    virtual int currentIndex(VirtualKeyboard *keyboard) = 0;
};

class LanguageSwitchKey : public VirtualKey {
public:
    LanguageSwitchKey() {
        setCustomBackgroundColor({0.3, 0.3, 0.3});
        setFontColor({1.0, 1.0, 1.0});
    }
    const char* label(VirtualKeyboard *keyboard) const override;
    void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) override;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYGENERAL_H_
