/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"
#include "virtualkeyboard.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

int main() {

    KeyboardTester *tester = new KeyboardTester();

    // DummyKey
    std::cout << "TEST DummyKey" << std::endl;
    std::vector<fcitx::classicui::VirtualKey *> keys;
    keys.emplace_back(new fcitx::classicui::DummyKey());
    keys.back()->setCustomLayout(0.5);
    tester->testLoadKey(keys, "{\"class\":\"DummyKey\",\"scale\":0.5}");

    // NormalKey
    std::cout << "TEST NormalKey" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::NormalKey("й", 24, "Й", false, 50.0));
    tester->setNormalKey("\"useLabelAsKeyName\":false,\"width\":50.0");
    tester->testLoadKey(
        keys,
        "{\"class\":\"NormalKey\",\"code\":24,\"upperLabel\":\"Й\"},\"й\"");
    std::cout << "TEST NormalKey width(int)" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::NormalKey("й", 24, "Й", false, 60.0));
    tester->setNormalKey("\"useLabelAsKeyName\":false,\"width\":50.0");
    tester->testLoadKey(keys,
                        "{\"class\":\"NormalKey\",\"code\":24,\"upperLabel\":"
                        "\"Й\",\"width\": 60},\"й\"");
    std::cout << "TEST NormalKey width(double)" << std::endl;
    keys.clear();
    keys.emplace_back(
        new fcitx::classicui::NormalKey("й", 24, "Й", false, 60.0));
    tester->setNormalKey("\"useLabelAsKeyName\":false,\"width\":50.0");
    tester->testLoadKey(keys,
                        "{\"class\":\"NormalKey\",\"code\":24,\"upperLabel\":"
                        "\"Й\",\"width\": 60.0},\"й\"");

    // ModeSwitchKey
    std::cout << "TEST ModeSwitchKey" << std::endl;
    keys.clear();
    std::vector<std::string> labels;
    labels.push_back("A");
    keys.emplace_back(new fcitx::classicui::ModeSwitchKey("A#", 2, labels));
    keys.back()->setCustomLayout(2.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ModeSwitchKey\",\"scale\":2.0},\"A#\"");
    std::cout << "TEST ModeSwitchKey width" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ModeSwitchKey("A#", 2, labels));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"ModeSwitchKey\",\"width\":100.0},\"A#\"");

    // ShiftToggleKey
    std::cout << "TEST ShiftToogleKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ShiftToggleKey());
    // FIXME: don't ignore label
    tester->testLoadKey(keys, "{\"class\":\"ShiftToggleKey\"},\"⇧\"");
    std::cout << "TEST ShiftToogleKey(int)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ShiftToggleKey(40.0));
    tester->testLoadKey(keys,
                        "{\"class\":\"ShiftToggleKey\",\"width\":40},\"⇧\"");
    std::cout << "TEST ShiftToogleKey(double)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::ShiftToggleKey(40.0));
    tester->testLoadKey(keys,
                        "{\"class\":\"ShiftToggleKey\",\"width\":40.0},\"⇧\"");

    // LanguageSwitchKey
    std::cout << "TEST LanguageSwitchKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::LanguageSwitchKey());
    tester->testLoadKey(keys, "{\"class\":\"LanguageSwitchKey\"},\"JP\"");
    // LanguageSwitchKey (width)
    std::cout << "TEST LanguageSwitchKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::LanguageSwitchKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"LanguageSwitchKey\", \"width\": 100.0},\"JP\"");

    // NumberKey
    std::cout << "TEST NumberKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::NumberKey("1", 10));
    tester->testLoadKey(keys, "{\"class\":\"NumberKey\",\"code\":10},\"1\"");
    std::cout << "TEST NumberKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::NumberKey("1", 10));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"NumberKey\",\"code\":10,\"width\":100.0},\"1\"");

    // BackSpaceKey
    std::cout << "TEST BackSpaceKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::BackSpaceKey());
    tester->testLoadKey(keys, "{\"class\":\"BackSpaceKey\"},\"Back\"");
    std::cout << "TEST BackSpaceKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::BackSpaceKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(
        keys, "{\"class\":\"BackSpaceKey\",\"width\": 100.0},\"Back\"");

    // SpaceKey
    std::cout << "TEST SpaceKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::SpaceKey());
    keys.back()->setCustomLayout(2.0);
    tester->testLoadKey(keys, "{\"class\":\"SpaceKey\",\"scale\":2.0},\"\"");
    std::cout << "TEST SpaceKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::SpaceKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"SpaceKey\",\"width\":100.0},\"\"");

    // EnterKey
    std::cout << "TEST EnterKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::EnterKey());
    keys.back()->setCustomLayout(1.5);
    tester->testLoadKey(keys,
                        "{\"class\":\"EnterKey\",\"scale\":1.5},\"Enter\"");
    std::cout << "TEST EnterKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::EnterKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys,
                        "{\"class\":\"EnterKey\",\"width\":100.0},\"Enter\"");

    // MarkKey
    std::cout << "TEST MarkKey (label only)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::MarkKey("."));
    tester->setMarkKey("\"width\":60.0");
    tester->testLoadKey(keys, "{\"class\":\"MarkKey\"},\".\"");
    std::cout << "TEST MarkKey (label, keyName)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::MarkKey(".", "period"));
    tester->testLoadKey(keys,
                        "{\"class\":\"MarkKey\",\"keyName\":\"period\"},\".\"");
    std::cout << "TEST MarkKey (label, keyName, code, withShift)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::MarkKey(",", "comma", 61, true));
    tester->testLoadKey(keys,
                        "{\"class\":\"MarkKey\",\"keyName\":\"comma\",\"code\":"
                        "61, \"withShift\":true},\",\"");
    std::cout << "TEST MarkKey rouble sign (label)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::MarkKey(u8"\u20BD"));
    tester->testLoadKey(keys, "{\"class\":\"MarkKey\"},\"\u20BD,\"");
    std::cout << "TEST MarkKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::MarkKey("."));
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"MarkKey\",\"width\":100.0},\".\"");
    std::cout << "TEST MarkKey (default width=50.0)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::MarkKey("."));
    keys.back()->setCustomWidth(50.0);
    tester->setMarkKey("\"width\":50.0");
    tester->testLoadKey(keys, "{\"class\":\"MarkKey\"},\".\"");
    tester->reset();

    // UpKey
    std::cout << "TEST UpKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::UpKey());
    tester->testLoadKey(keys, "{\"class\":\"UpKey\"},\"↑\"");
    std::cout << "TEST UpKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::UpKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"UpKey\",\"width\":100.0},\"↑\"");

    // DownKey
    std::cout << "TEST DownKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::DownKey());
    tester->testLoadKey(keys, "{\"class\":\"DownKey\"},\"↓\"");
    std::cout << "TEST DownKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::DownKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"DownKey\",\"width\":100.0},\"↓\"");

    // RightKey
    std::cout << "TEST RightKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::RightKey());
    tester->testLoadKey(keys, "{\"class\":\"RightKey\"},\"→\"");
    std::cout << "TEST RightKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::RightKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"RightKey\",\"width\":100.0},\"→\"");

    // LeftKey
    std::cout << "TEST LeftKey" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::LeftKey());
    tester->testLoadKey(keys, "{\"class\":\"LeftKey\"},\"←\"");
    std::cout << "TEST LeftKey (width)" << std::endl;
    keys.clear();
    keys.emplace_back(new fcitx::classicui::LeftKey());
    keys.back()->setCustomWidth(100.0);
    tester->testLoadKey(keys, "{\"class\":\"LeftKey\",\"width\":100.0},\"←\"");

    return 0;
}
