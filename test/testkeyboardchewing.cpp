/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"
#include "virtualkeyboardchewing.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

int main() {

    KeyboardTester *tester = new KeyboardTester();

    // ChewingEnterKey
    std::cout << "test ChewingEnterKey" << std::endl;
    std::vector<fcitx::classicui::VirtualKey *> keys;
    keys.emplace_back(new fcitx::classicui::ChewingEnterKey());
    tester->testLoadKey(keys, "{\"class\":\"ChewingEnterKey\"},\"換行\"");
    std::cout << "test ChewingEnterKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingEnterKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingEnterKey\",\"width\":100.0},\"換行\"");
    std::cout << "test ChewingEnterKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingEnterKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingEnterKey\",\"scale\":100.0},\"換行\"");
    std::cout << "test ChewingEnterKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingEnterKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys, "{\"class\":\"ChewingEnterKey\",\"width\":100.0,"
                              "\"scale\":10},\"換行\"");

    // ChewingMarkToggleKey
    std::cout << "test ChewingMarkToggleKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingMarkToggleKey());
    tester->testLoadKey(keys, "{\"class\":\"ChewingMarkToggleKey\"},\"更多\"");
    std::cout << "test ChewingMarkToggleKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingMarkToggleKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingMarkToggleKey\",\"width\":100.0},\"更多\"");
    std::cout << "test ChewingMarkToggleKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingMarkToggleKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingMarkToggleKey\",\"scale\":100.0},\"更多\"");
    std::cout << "test ChewingMarkToggleKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingMarkToggleKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ChewingMarkToggleKey\",\"width\":100.0,"
                        "\"scale\":10},\"更多\"");

    // ChewingModeSwitchKey
    std::cout << "test ChewingModeSwitchKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingModeSwitchKey());
    tester->testLoadKey(keys, "{\"class\":\"ChewingModeSwitchKey\"},\"注#\"");
    std::cout << "test ChewingModeSwitchKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingModeSwitchKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingModeSwitchKey\",\"width\":100.0},\"注#\"");
    std::cout << "test ChewingModeSwitchKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingModeSwitchKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingModeSwitchKey\",\"scale\":100.0},\"注#\"");
    std::cout << "test ChewingModeSwitchKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingModeSwitchKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ChewingModeSwitchKey\",\"width\":100.0,"
                        "\"scale\":10},\"注#\"");

    // ChewingNumPadKey
    std::cout << "test ChewingNumPadKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumPadKey("0"));
    tester->testLoadKey(keys, "{\"class\":\"ChewingNumPadKey\"},\"0\"");
    std::cout << "test ChewingNumPadKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumPadKey("0"));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingNumPadKey\",\"width\":100.0},\"0\"");
    std::cout << "test ChewingNumPadKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumPadKey("0"));
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingNumPadKey\",\"scale\":100.0},\"0\"");
    std::cout << "test ChewingNumPadKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumPadKey("0"));
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys,
        "{\"class\":\"ChewingNumPadKey\",\"width\":100.0,\"scale\":10},\"0\"");

    // ChewingNumberKey
    std::cout << "test ChewingNumberKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumberKey("1", "1", 10));
    tester->testLoadKey(
        keys,
        "{\"class\":\"ChewingNumberKey\",\"keyName\":\"1\",\"code\":10},\"1\"");
    std::cout << "test ChewingNumberKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumberKey("1", "1", 10));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ChewingNumberKey\",\"keyName\":\"1\","
                        "\"code\":10,\"width\":100.0},\"1\"");
    std::cout << "test ChewingNumberKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumberKey("1", "1", 10));
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ChewingNumberKey\",\"keyName\":\"1\","
                        "\"code\":10,\"scale\":100.0},\"1\"");
    std::cout << "test ChewingNumberKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingNumberKey("1", "1", 10));
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ChewingNumberKey\",\"keyName\":\"1\","
                        "\"code\":10,\"width\":100.0,\"scale\":10},\"1\"");

    // ChewingSpaceKey
    std::cout << "test ChewingSpaceKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingSpaceKey());
    tester->testLoadKey(keys, "{\"class\":\"ChewingSpaceKey\"},\"空格\"");
    std::cout << "test ChewingSpaceKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingSpaceKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingSpaceKey\",\"width\":100.0},\"空格\"");
    std::cout << "test ChewingSpaceKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingSpaceKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"ChewingSpaceKey\",\"scale\":100.0},\"空格\"");
    std::cout << "test ChewingSpaceKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ChewingSpaceKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys, "{\"class\":\"ChewingSpaceKey\",\"width\":100.0,"
                              "\"scale\":10},\"空格\"");

    return 0;
}
