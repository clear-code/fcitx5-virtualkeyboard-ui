/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

int main() {

    KeyboardTester *tester = new KeyboardTester();

    // Metadata
    tester->testMatchLog(
        "nonexistent.json",
        "Failed to access keyboard layout file (JSON): nonexistent.json");

    tester->testMatchLog("empty.json",
                         "Failed to load keyboard layout file (JSON):");

    std::vector<std::string> expected;
    expected.push_back("metadata of keyboard layout must not be null");
    expected.push_back("Failed to parse metadata of keyboard layout file "
                       "(JSON) with offset: 0");
    tester->testMatchLogs("empty-array.json", expected, 0, "*=2");

    tester->testLoadError("malformed.json",
                          "Failed to parse metadata of keyboard "
                          "layout file (JSON) with offset: 0");
    tester->testLoadError("metadata-non-object.json",
                          "metadata of keyboard layout must be object");
    tester->testLoadError("metadata-no-keyboard.json", "Keyboard must be set");
    tester->testLoadError("metadata-no-modeswitchkey.json",
                          "ModeSwitchKey must be set");
    tester->testLoadWarning("metadata-unknown-key.json",
                            "unknown key of metadata: Unknown");

    // Keyboard
    tester->testLoadError("keyboard-null.json", "Keyboard must not be null");
    tester->testLoadError("keyboard-no-label.json",
                          "Keyboard.label must be set");
    tester->testLoadError("keyboard-label-null.json",
                          "Keyboard.label must be set");
    tester->testLoadError("keyboard-label-non-string.json",
                          "Keyboard.label must be string");

    tester->testLoadError("keyboard-no-languagecode.json",
                          "Keyboard.languageCode must be set");
    tester->testLoadError("keyboard-languagecode-null.json",
                          "Keyboard.languageCode must be set");
    tester->testLoadError("keyboard-languagecode-non-string.json",
                          "Keyboard.languageCode must be string");

    tester->testLoadError("keyboard-no-numberoflayers.json",
                          "Keyboard.numberOfLayers must be set");
    tester->testLoadError("keyboard-numberoflayers-null.json",
                          "Keyboard.numberOfLayers must be set");
    tester->testLoadError("keyboard-numberoflayers-non-integer.json",
                          "Keyboard.numberOfLayers must be integer");

    tester->testLoadError("keyboard-no-layeroffset.json",
                          "Keyboard.layerOffset must be set");
    tester->testLoadError("keyboard-layeroffset-non-array.json",
                          "Keyboard.layerOffset must be array");
    tester->testLoadError("keyboard-layeroffset-non-integer.json",
                          "Keyboard.layerOffset[0] must be integer");
    tester->testLoadError(
        "keyboard-layeroffset-inverted.json",
        "Keyboard.layerOffset must be set in ascending order: 2 < 1");

    // ModeSwitchKey
    tester->testLoadError("modeswitchkey-null.json",
                          "ModeSwitchKey must not be null");
    tester->testLoadError("modeswitchkey-no-numberofstates.json",
                          "ModeSwitchKey.numberOfStates must be set");
    tester->testLoadError("modeswitchkey-numberofstates-non-integer.json",
                          "ModeSwitchKey.numberOfStates must be integer");
    tester->testLoadError("modeswitchkey-no-statelabel.json",
                          "ModeSwitchKey.stateLabel must be set");
    tester->testLoadError("modeswitchkey-statelabel-non-array.json",
                          "ModeSwitchKey.stateLabel must be array");
    tester->testLoadError("modeswitchkey-statelabel-non-string.json",
                          "ModeSwitchKey.stateLabel[0] must be string");
    tester->testLoadError(
        "modeswitchkey-states-mismatch.json",
        "The number of stateLabel and numberOfStates must be same. "
        "numberOfStates: 2 stateLabel: 1");

    // NormalKey
    tester->testLoadError("normalkey-null.json", "NormalKey must not be null");
    tester->testLoadError("normalkey-width-null.json",
                          "NormalKey.width must be set");
    tester->testLoadError("normalkey-width-string.json",
                          "NormalKey.width must be double");

    // MarkKey
    tester->testLoadError("markkey-null.json", "MarkKey must not be null");
    tester->testLoadError("markkey-width-null.json",
                          "MarkKey.width must be set");
    tester->testLoadError("markkey-width-string.json",
                          "MarkKey.width must be double");

    return 0;
}
