/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDI18N_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDI18N_H_

#include "common.h"
#include "fcitx/inputmethodmanager.h"

namespace fcitx {
namespace classicui {

enum class KeyboardType {
    Unknown,
    Us,
    Anthy,
    Pinyin,
    Russian,
    Hangul,
    Chewing,
    Custom,
};

static std::map<KeyboardType, std::string> imeNames = {
    {KeyboardType::Unknown, ""},
    {KeyboardType::Us, "keyboard-us"},
    {KeyboardType::Anthy, "anthy"},
    {KeyboardType::Pinyin, "pinyin"},
    {KeyboardType::Russian, "keyboard-ru"},
    {KeyboardType::Hangul, "hangul"},
    {KeyboardType::Chewing, "chewing"},
};

class VirtualKey;
class VirtualKeyboard;
class I18nKeyboard {
public:
    virtual KeyboardType type() const = 0;
    virtual const char *label() const = 0;
    virtual void updateKeys() = 0;
    virtual const std::string languageCode() const { return ""; }
    virtual void syncState(
        VirtualKeyboard *keyboard,
        const std::string &currentInputMethodName
    );
    std::vector<std::unique_ptr<VirtualKey>> &keys() { return keys_; }
    bool checkOtherNecessaryImesExist(std::vector<fcitx::InputMethodGroupItem> &allItems);
#if USE_CUSTOM_LAYOUT
    std::string customImeName() {
        return customImeName_;
    }
    std::string setCustomImeName(std::string name) {
        customImeName_ = name;
    }
#endif
protected:
    virtual std::vector<std::string> otherNecessaryImeList() { return {}; }
    std::vector<std::unique_ptr<VirtualKey>> keys_;
private:
    bool containInputMethod(std::vector<fcitx::InputMethodGroupItem> &items,
                            const std::string &name);
#if USE_CUSTOM_LAYOUT
    std::string customImeName_;
#endif
};

class I18nKeyboardSelector {
public:
#if USE_CUSTOM_LAYOUT
    std::tuple<I18nKeyboard *, bool> select(InputMethodGroup &group, std::vector<fcitx::InputMethodGroupItem> &inputMethodItems);
#else
    std::tuple<I18nKeyboard *, bool> select(std::vector<fcitx::InputMethodGroupItem> &inputMethodItems);
#endif
private:
    KeyboardType getTypeByName(const std::string &inputMethodName);
    std::tuple<I18nKeyboard *, bool> selectByType(KeyboardType type);
    bool canSelect(
        std::vector<fcitx::InputMethodGroupItem> &allItems,
        const std::string &inputMethodNameToSelect
    );
};

class NullI18nKeyboard : public I18nKeyboard {
public:
    KeyboardType type() const override { return KeyboardType::Unknown; }
    const char *label() const override { return "X"; }
    void updateKeys() override {
        // do nothing
    }
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDI18N_H_
