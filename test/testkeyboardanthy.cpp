/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"
#include "virtualkeyboardanthy.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

int main() {

    KeyboardTester *tester = new KeyboardTester();

    // AnthyMarkKey
    std::cout << "test AnthyMarkKey" << std::endl;
    std::vector<fcitx::classicui::VirtualKey *> keys;
    keys.emplace_back(new fcitx::classicui::AnthyMarkKey("1", "1", "１"));
    tester->testLoadKey(keys, "{\"class\":\"AnthyMarkKey\",\"keyName\":\"1\","
                              "\"zenkakuMark\":\"１\"},\"1\"");
    std::cout << "test AnthyMarkKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyMarkKey("1", "1", "１"));
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(keys, "{\"class\":\"AnthyMarkKey\",\"keyName\":\"1\","
                              "\"zenkakuMark\":\"１\",\"scale\":100.0},\"1\"");
    std::cout << "test AnthyMarkKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyMarkKey("1", "1", "１"));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"AnthyMarkKey\",\"keyName\":\"1\","
                              "\"zenkakuMark\":\"１\",\"width\":100.0},\"1\"");
    std::cout << "test AnthyMarkKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyMarkKey("1", "1", "１"));
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyMarkKey\",\"keyName\":\"1\","
              "\"zenkakuMark\":\"１\",\"width\":100.0,\"scale\":10.0},\"1\"");

    // AnthyTypingMethodSwitchKey
    std::cout << "test AnthyTypingMethodSwitchKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyTypingMethodSwitchKey());
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyTypingMethodSwitchKey\"},\"ローマ字かな\"");
    std::cout << "test AnthyTypingMethodSwitchKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyTypingMethodSwitchKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyTypingMethodSwitchKey\",\"scale\":100."
              "0},\"ローマ字かな\"");
    std::cout << "test AnthyTypingMethodSwitchKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyTypingMethodSwitchKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyTypingMethodSwitchKey\",\"width\":100."
              "0},\"ローマ字かな\"");
    std::cout << "test AnthyTypingMethodSwitchKey (scale+width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyTypingMethodSwitchKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyTypingMethodSwitchKey\",\"width\":100."
              "0,\"scale\":10},\"ローマ字かな\"");

    // AnthyInputModeSwitchKey
    std::cout << "test AnthyInputModeSwitchKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyInputModeSwitchKey());
    tester->testLoadKey(keys,
                        "{\"class\":\"AnthyInputModeSwitchKey\"},\"全角\"");
    std::cout << "test AnthyInputModeSwitchKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyInputModeSwitchKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys,
        "{\"class\":\"AnthyInputModeSwitchKey\",\"scale\":100.0},\"全角\"");
    std::cout << "test AnthyInputModeSwitchKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyInputModeSwitchKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys,
        "{\"class\":\"AnthyInputModeSwitchKey\",\"width\":100.0},\"全角\"");
    std::cout << "test AnthyInputModeSwitchKey (scale+width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyInputModeSwitchKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyInputModeSwitchKey\",\"width\":100.0,"
              "\"scale\":10},\"全角\"");

    // AnthyModeSwitchKey
    std::cout << "test AnthyModeSwitchKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyModeSwitchKey());
    tester->testLoadKey(keys, "{\"class\":\"AnthyModeSwitchKey\"},\"JP\"");
    std::cout << "test AnthyModeSwitchKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyModeSwitchKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyModeSwitchKey\",\"scale\":100.0},\"JP\"");
    std::cout << "test AnthyModeSwitchKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyModeSwitchKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyModeSwitchKey\",\"width\":100.0},\"JP\"");
    std::cout << "test AnthyModeSwitchKey (scale+width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::AnthyModeSwitchKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"AnthyModeSwitchKey\",\"width\":100.0,"
                        "\"scale\":10},\"JP\"");

    // ZenkakuHankakuKey
    std::cout << "test ZenkakuHankakuKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ZenkakuHankakuKey());
    tester->testLoadKey(keys, "{\"class\":\"ZenkakuHankakuKey\"},\"全角\"");
    std::cout << "test ZenkakuHankakuKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ZenkakuHankakuKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ZenkakuHankakuKey\",\"scale\":100.0},\"全角\"");
    std::cout << "test ZenkakuHankakuKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ZenkakuHankakuKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ZenkakuHankakuKey\",\"width\":100.0},\"全角\"");
    std::cout << "test ZenkakuHankakuKey (scale+width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ZenkakuHankakuKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ZenkakuHankakuKey\",\"width\":100.0,"
                        "\"scale\":10},\"全角\"");

    // EmptyKey
    std::cout << "test EmptyKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::EmptyKey());
    tester->testLoadKey(keys, "{\"class\":\"EmptyKey\"}");
    std::cout << "test EmptyKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::EmptyKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(keys, "{\"class\":\"EmptyKey\",\"scale\":100.0}");
    std::cout << "test EmptyKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::EmptyKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"EmptyKey\",\"width\":100.0}");
    std::cout << "test EmptyKey (scale+width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::EmptyKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys, "{\"class\":\"EmptyKey\",\"width\":100.0,\"scale\":10}");

    // AnthyKanaKey
    std::cout << "test AnthyKanaKey" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::AnthyKanaKey("ぬ", 10, "", "1", "", true));
    tester->testLoadKey(
        keys,
        "{\"class\":\"AnthyKanaKey\",\"code\":10,\"upperLabel\":\"\","
        "\"keyName\":\"1\",\"upperName\":\"\",\"isNumberKey\":true},\"ぬ\"");
    std::cout << "test AnthyKanaKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::AnthyKanaKey("ぬ", 10, "", "1", "", true));
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyKanaKey\",\"code\":10,\"upperLabel\":\"\","
              "\"keyName\":\"1\",\"upperName\":\"\",\"isNumberKey\":true,"
              "\"scale\":100.0},\"ぬ\"");
    std::cout << "test AnthyKanaKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::AnthyKanaKey("ぬ", 10, "", "1", "", true));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyKanaKey\",\"code\":10,\"upperLabel\":\"\","
              "\"keyName\":\"1\",\"upperName\":\"\",\"isNumberKey\":true,"
              "\"width\":100.0},\"ぬ\"");
    std::cout << "test AnthyKanaKey (scale+width)" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::AnthyKanaKey("ぬ", 10, "", "1", "", true));
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys, "{\"class\":\"AnthyKanaKey\",\"code\":10,\"upperLabel\":\"\","
              "\"keyName\":\"1\",\"upperName\":\"\",\"isNumberKey\":true,"
              "\"width\":100.0,\"scale\":10},\"ぬ\"");

    // Unknown Keys
    std::cout << "test Unknown key" << std::endl;
    tester->testLoadInvalidKey("{\"class\":\"AnthyUnknown\"},\"dummy\"",
                               "unknown klass of key name: AnthyUnknown");

    // Invalid Keys
    std::cout << "test Invalid keys" << std::endl;
    std::vector<std::string> keyNames = {"AnthyMarkKey",
                                         "AnthyTypingMethodSwitchKey",
                                         "AnthyInputModeSwitchKey",
                                         "AnthyModeSwitchKey",
                                         "ZenkakuHankakuKey",
                                         "EmptyKey",
                                         "AnthyKanaKey"};
    for (size_t i = 0; i < keyNames.size(); i++) {
        std::string keyName = keyNames[i];
        std::transform(keyName.begin(), keyName.end(), keyName.begin(),
                       ::tolower);
        std::string json_body("{\"class\":\"" + keyName + "\"},\"dummy\"");
        std::string message =
            "class name must be " + keyNames[i] + ", not <" + keyName + ">";
        tester->testLoadInvalidKey(json_body, message);
    }
    return 0;
}
