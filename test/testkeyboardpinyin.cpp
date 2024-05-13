/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeyboardpinyin.h"
#include "virtualkeygeneral.h"

int main() {

    KeyboardTester *tester = new KeyboardTester();

    // PinyinEnterKey
    std::cout << "test PinyinEnterKey" << std::endl;
    std::vector<fcitx::classicui::VirtualKey *> keys;
    keys.emplace_back(new fcitx::classicui::PinyinEnterKey());
    tester->testLoadKey(keys, "{\"class\":\"PinyinEnterKey\"},\"Enter\"");
    std::cout << "test PinyinEnterKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinEnterKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinEnterKey\",\"width\":100.0},\"Enter\"");
    std::cout << "test PinyinEnterKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinEnterKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinEnterKey\",\"scale\":100.0},\"Enter\"");
    std::cout << "test PinyinEnterKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinEnterKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinEnterKey\",\"width\":100.0,\"scale\":"
              "10},\"Enter\"");

    // PinyinMarkTogleKey
    std::cout << "test PinyinMarkToggleKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinMarkToggleKey());
    tester->testLoadKey(keys, "{\"class\":\"PinyinMarkToggleKey\"},\"更多\"");
    std::cout << "test PinyinMarkToggleKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinMarkToggleKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinMarkToggleKey\",\"width\":100.0},\"更多\"");
    std::cout << "test PinyinMarkToggleKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinMarkToggleKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinMarkToggleKey\",\"scale\":100.0},\"更多\"");
    std::cout << "test PinyinMarkToggleKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinMarkToggleKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"PinyinMarkToggleKey\",\"width\":100.0,"
                        "\"scale\":10},\"更多\"");

    // PinyinModeSwitchKey
    std::cout << "test PinyinModeSwitchKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinModeSwitchKey());
    tester->testLoadKey(keys, "{\"class\":\"PinyinModeSwitchKey\"},\"A#\"");
    std::cout << "test PinyinModeSwitchKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinModeSwitchKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinModeSwitchKey\",\"width\":100.0},\"A#\"");
    std::cout << "test PinyinModeSwitchKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinModeSwitchKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinModeSwitchKey\",\"scale\":100.0},\"A#\"");
    std::cout << "test PinyinModeSwitchKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinModeSwitchKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"PinyinModeSwitchKey\",\"width\":100.0,"
                        "\"scale\":10},\"A#\"");

    // PinyinSpaceKey
    std::cout << "test PinyinSpaceKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinSpaceKey());
    tester->testLoadKey(keys, "{\"class\":\"PinyinSpaceKey\"},\"CH\"");
    std::cout << "test PinyinSpaceKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinSpaceKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinSpaceKey\",\"width\":100.0},\"CH\"");
    std::cout << "test PinyinSpaceKey (scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinSpaceKey());
    keys.back()->setCustomLayout(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"PinyinSpaceKey\",\"scale\":100.0},\"CH\"");
    std::cout << "test PinyinSpaceKey (width+scale)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::PinyinSpaceKey());
    keys.back()->setCustomWidth(1000.0);
    tester->testLoadKey(
        keys,
        "{\"class\":\"PinyinSpaceKey\",\"width\":100.0,\"scale\":10},\"CH\"");

    return 0;
}
