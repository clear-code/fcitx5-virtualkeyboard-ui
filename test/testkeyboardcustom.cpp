/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"
#include "virtualkeyboardcustom.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

int main() {

    KeyboardTester *tester = new KeyboardTester();

    // CustomModeSwitchKey
    std::cout << "test CustomModeSwitchKey" << std::endl;
    std::vector<fcitx::classicui::VirtualKey *> keys;
    std::vector<std::string> labels;
    labels.push_back("A");
    keys.emplace_back(
        new fcitx::classicui::CustomModeSwitchKey("", 1, labels, false));
    tester->testLoadKey(keys, "{\"class\":\"CustomModeSwitchKey\"},\"MODE\"");

    return 0;
}
