/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include <fcntl.h> // O_RDONLY
#include "fcitx-utils/stringutils.h"
#include "fcitx-config/iniparser.h"
#include "virtualkeyboardi18n.h"
#include "virtualkeyboard.h"
#include "virtualkeyboardus.h"
#include "virtualkeyboardanthy.h"
#include "virtualkeyboardpinyin.h"
#include "virtualkeyboardrussian.h"
#include "virtualkeyboardhangul.h"
#include "virtualkeyboardchewing.h"

namespace fcitx::classicui {

std::tuple<I18nKeyboard *, bool> I18nKeyboardSelector::selectByType(KeyboardType type) {
    // Add case here when adding new keyboard type.
    switch (type) {
    case KeyboardType::Us:
        return {new UsKeyboard(), true};
    case KeyboardType::Anthy:
        return {new AnthyKeyboard(), true};
    case KeyboardType::Pinyin:
        return {new PinyinKeyboard(), true};
    case KeyboardType::Russian:
        return {new RussianKeyboard(), true};
    case KeyboardType::Hangul:
        return {new HangulKeyboard(), true};
    case KeyboardType::Chewing:
        return {new ChewingKeyboard(), true};
    default:
        break;
    }
    return {new NullI18nKeyboard(), false};
}

std::tuple<I18nKeyboard *, bool> I18nKeyboardSelector::select(
    std::vector<fcitx::InputMethodGroupItem> &inputMethodItems
) {
#if USE_CUSTOM_LAYOUT
    // Select custom keyboard layout when keyboard configuration
    // is matched with virtualkeyboardui.conf.
    //
    // If section name matched with current ime name (e.g. keyboard-us)
    // [keyboard-us]
    // Layout=PATH_TO_JSON (e.g. /opt/fcitx5/share/fcitx5/addon/virtualkeyboardui-us.ext.json
    auto file = StandardPath::global().open(
        StandardPath::Type::PkgData,
        stringutils::joinPath("addon", "virtualkeyboardui.conf"), O_RDONLY);
    RawConfig config;
    readFromIni(config, file.fd());
    FCITX_KEYBOARD() << "I18nKeyboardSelector::select check virtualkeyboardui.conf";
    // First, select from addon imes such as `anthy` or `pinyin`.
    for (const auto &ime : inputMethodItems) {
        auto isSimpleKeyboard = stringutils::startsWith(ime.name(), "keyboard-");
        if (isSimpleKeyboard) continue;

        if (!ime.name().empty() && ime.layout().empty()) {
            FCITX_KEYBOARD() << "I18nKeyboardSelector::select Name:"
                             << ime.name() << " Layout: " << ime.layout();
            for (auto &uniqueName : config.subItems()) {
                FCITX_KEYBOARD() << "current virtualkeyboardui.conf section name: " << uniqueName;
                const auto *layout = config.valueByPath(
                    stringutils::joinPath(uniqueName, "Layout"));

                if (uniqueName == ime.name() && !layout->empty()) {
                    FCITX_KEYBOARD() << "I18nKeyboardSelector::select matched ["
                                     << ime.name() << "] section in virtualkeyboardui.conf";
                    auto isCustomKeyboard =
                        stringutils::endsWith(*layout, ".json");
                    if (isCustomKeyboard) {
                        FCITX_KEYBOARD() << "I18nKeyboardSelector::select use " << layout;
                        CustomKeyboard *custom = new CustomKeyboard(layout->c_str());
                        custom->setCustomImeName(ime.name());
                        return {custom, true};
                    }
                }
            }
        }
    }
    // Second, select from simple keyboards such as `keyboard-us`.
    for (const auto &ime : inputMethodItems) {
        auto isSimpleKeyboard = stringutils::startsWith(ime.name(), "keyboard-");
        if (!isSimpleKeyboard) continue;

        if (!ime.name().empty() && ime.layout().empty()) {
            FCITX_KEYBOARD() << "I18nKeyboardSelector::select Name:"
                             << ime.name() << " Layout: " << ime.layout();
            for (auto &uniqueName : config.subItems()) {
                FCITX_KEYBOARD() << "current virtualkeyboardui.conf section name: " << uniqueName;
                const auto *layout = config.valueByPath(
                    stringutils::joinPath(uniqueName, "Layout"));

                if (uniqueName == ime.name() && !layout->empty()) {
                    FCITX_KEYBOARD() << "I18nKeyboardSelector::select matched ["
                                     << ime.name() << "] section in virtualkeyboardui.conf";
                    auto isCustomKeyboard =
                        stringutils::endsWith(*layout, ".json");
                    if (isCustomKeyboard) {
                        FCITX_KEYBOARD() << "I18nKeyboardSelector::select use " << layout;
                        CustomKeyboard *custom = new CustomKeyboard(layout->c_str());
                        custom->setCustomImeName(ime.name());
                        return {custom, true};
                    }
                }
            }
        }
    }
#endif

    // First, select from addon imes such as `anthy` or `pinyin`.
    for (const auto &ime : inputMethodItems)
    {
        auto isSimpleKeyboard = stringutils::startsWith(ime.name(), "keyboard-");
        if (isSimpleKeyboard) continue;

        if (canSelect(inputMethodItems, ime.name())) {
            return selectByType(getTypeByName(ime.name()));
        }
    }

    // Second, select from simple keyboards such as `keyboard-us`.
    for (const auto &ime : inputMethodItems)
    {
        auto isSimpleKeyboard = stringutils::startsWith(ime.name(), "keyboard-");
        if (!isSimpleKeyboard) continue;

        if (canSelect(inputMethodItems, ime.name())) {
            return selectByType(getTypeByName(ime.name()));
        }
    }

    return selectByType(KeyboardType::Unknown);
}

KeyboardType I18nKeyboardSelector::getTypeByName(const std::string &inputMethodName) {
    for (const auto &[type, name] : imeNames)
    {
        if (inputMethodName == name) {
            return type;
        }
    }
    return KeyboardType::Unknown;
}

bool I18nKeyboardSelector::canSelect(
    std::vector<fcitx::InputMethodGroupItem> &allItems,
    const std::string &inputMethodNameToSelect
) {
    auto type = getTypeByName(inputMethodNameToSelect);
    if (type == KeyboardType::Unknown) return false;

    auto [keyboard, hasFound] = selectByType(type);
    if (!hasFound) return false;
    return keyboard->checkOtherNecessaryImesExist(allItems);
}

bool I18nKeyboard::checkOtherNecessaryImesExist(
    std::vector<fcitx::InputMethodGroupItem> &allItems
) {
    for (const auto &anotherIme : otherNecessaryImeList())
    {
        if (!containInputMethod(allItems, anotherIme)) {
            return false;
        }
    }
    return true;
}

bool I18nKeyboard::containInputMethod(
    std::vector<fcitx::InputMethodGroupItem> &items,
    const std::string &name
) {
    auto iter = std::find_if(items.begin(), items.end(),
        [&name](const InputMethodGroupItem &item) {
            return item.name() == name;
        });
    return iter != items.end();
}

void I18nKeyboard::syncState(VirtualKeyboard *, const std::string &) {
    // Do nothing.
    // Override this if need to do some processes depending on the launguage.
}

}
