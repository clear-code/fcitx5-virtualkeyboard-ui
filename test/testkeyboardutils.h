/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef _FCITX_UI_CLASSIC_TESTKEYBOARDUTILS_H_
#define _FCITX_UI_CLASSIC_TESTKEYBOARDUTILS_H_

#include <stdlib.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <fcitx-utils/log.h>
#include <fcitx-utils/metastring.h>
#include "fcitx-utils/stringutils.h"
#include "virtualkeyboard.h"
#include "virtualkeyboardlayout.h"
#include "virtualkeygeneral.h"

class KeyboardTester {
public:
    void setLabel(std::string label) { label_ = label; }
    void setLanguageCode(std::string value) { languageCode_ = value; }
    void setNumberOfLayers(std::string value) { numberOfLayers_ = value; }
    void setLayerOffset(std::string value) { layerOffset_ = value; }
    void setModeSwitchKey(std::string value) { modeSwitchKey_ = value; }
    void setNormalKey(std::string value) { normalKey_ = value; }
    void setMarkKey(std::string value) { markKey_ = value; }
    void testLoadKey(std::vector<fcitx::classicui::VirtualKey *> expected,
                     std::string layout_json, size_t offset = 0);

    void testLoadInvalidKey(std::string json_body, std::string message);
    void testLoadError(std::string json, std::string message,
                       size_t offset = 0);
    void testLoadWarning(std::string json, std::string message,
                         size_t offset = 0);
    void testMatchLogs(std::string json, std::vector<std::string> expected,
                       size_t offset, std::string rule);
    void testMatchLog(std::string json, std::string expected);
    void reset();

private:
    std::string generateTemporaryJSON(std::string layout_json);
    void saveTemporaryJSON(std::string json);
    std::string normalizeClassName(std::string klass);
    FILE *fp_;
    char path_[PATH_MAX];
    std::string json_;
    std::string label_ = "JP";
    std::string languageCode_ = "jp";
    std::string numberOfLayers_ = "1";
    std::string layerOffset_ = "1";
    std::string modeSwitchKey_ = "\"numberOfStates\": 1,\"stateLabel\":[\"A\"]";
    std::string normalKey_ = "\"width\":50.0";
    std::string markKey_ = "\"width\":50.0";
};

#endif
