/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "testkeyboardutils.h"

std::string KeyboardTester::normalizeClassName(std::string klass) {
    if (klass.find("DummyKey") != std::string::npos) {
        return std::string("DummyKey");
    } else if (klass.find("NormalKey") != std::string::npos) {
        return std::string("NormalKey");
    } else if (klass.find("ModeSwitchKey") != std::string::npos) {
        return std::string("ModeSwitchKey");
    } else if (klass.find("ShiftToggleKey") != std::string::npos) {
        return std::string("ShiftToggleKey");
    } else if (klass.find("LanguageSwitchKey") != std::string::npos) {
        return std::string("LanguageSwitchKey");
    } else if (klass.find("NumberKey") != std::string::npos) {
        return std::string("NumberKey");
    } else if (klass.find("BackSpaceKey") != std::string::npos) {
        return std::string("BackSpaceKey");
    } else if (klass.find("SpaceKey") != std::string::npos) {
        return std::string("SpaceKey");
    } else if (klass.find("EnterKey") != std::string::npos) {
        return std::string("EnterKey");
    } else if (klass.find("MarkKey") != std::string::npos) {
        return std::string("MarkKey");
    } else if (klass.find("UpKey") != std::string::npos) {
        return std::string("UpKey");
    } else if (klass.find("DownKey") != std::string::npos) {
        return std::string("DownKey");
    } else if (klass.find("RightKey") != std::string::npos) {
        return std::string("RightKey");
    } else if (klass.find("LeftKey") != std::string::npos) {
        return std::string("LeftKey");
    } else if (klass.find("CustomModeSwitchKey") != std::string::npos) {
        return std::string("CustomModeSwitchKey");
    } else if (klass.find("CustomNumberKey") != std::string::npos) {
        return std::string("CustomNumberKey");
    } else if (klass.find("CustomNumPadKey") != std::string::npos) {
        return std::string("CustomNumPadKey");
    } else if (klass.find("CustomEnterKey") != std::string::npos) {
        return std::string("CustomEnterKey");
    } else if (klass.find("CustomSpaceKey") != std::string::npos) {
        return std::string("CustomSpaceKey");
    } else if (klass.find("CustomMarkToggleKey") != std::string::npos) {
        return std::string("CustomMarkToggleKey");
    }
    return std::string("unknown");
}

void KeyboardTester::reset() {
    label_ = "JP";
    languageCode_ = "jp";
    numberOfLayers_ = "1";
    layerOffset_ = "1";
    modeSwitchKey_ = "\"numberOfStates\": 1,\"stateLabel\":[\"A\"]";
    // normalKey_ = "\"width\":60.0";
    // markKey_ = "\"width\":60.0";
    normalKey_ = "";
    markKey_ = "";
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
    if (path_[0] != '\0') {
        unlink(path_);
        path_[0] = '\0';
    }
}

void KeyboardTester::saveTemporaryJSON(std::string json) {
    strcpy(path_, "/tmp/virtualkeyboardui.XXXXXX");
    int fd = mkstemp(path_);
    fp_ = fdopen(fd, "w+");
    fprintf(fp_, "%s", json.c_str());
    rewind(fp_); // close later
}

std::string KeyboardTester::generateTemporaryJSON(std::string layout_json) {
    std::string json("[{\"Keyboard\":{\"label\":\"");
    json += label_;
    json += "\",\"languageCode\":\"";
    json += languageCode_;
    json += "\",\"numberOfLayers\":";
    json += numberOfLayers_;
    json += ",\"layerOffset\":[";
    json += layerOffset_;
    json += "]}";
    if (!modeSwitchKey_.empty()) {
        json += ",\"ModeSwitchKey\":{";
        json += modeSwitchKey_;
        json += "}";
    }
    if (!normalKey_.empty()) {
        json += ",\"NormalKey\":{";
        json += normalKey_;
        json += "}";
    }
    if (!markKey_.empty()) {
        json += ",\"MarkKey\":{";
        json += markKey_;
        json += "}";
    }
    json += "},[" + layout_json + "]]";
    return json;
}

void KeyboardTester::testLoadKey(
    std::vector<fcitx::classicui::VirtualKey *> expected,
    std::string layout_json, size_t offset) {
    std::stringstream s;
#if DEBUG
    fcitx::Log::setLogRule("*=5"); // Debug = 5
#else
    fcitx::Log::setLogRule("*=2"); // Error = 2
#endif
    fcitx::Log::setLogStream(s);
    json_ = generateTemporaryJSON(layout_json);
    saveTemporaryJSON(json_);
#if DEBUG
    std::cout << "json: " << json_ << std::endl;
    std::cout << "path: " << std::string(path_) << std::endl;
#endif
    fcitx::classicui::KeyboardLayout *loader =
        new fcitx::classicui::KeyboardLayout(path_);
    loader->load(offset);

#if DEBUG
    std::cout << "expected keys: " << expected.size() << std::endl;
    std::cout << "loaded keys: " << loader->keys().size() << std::endl;
    std::cout << "log: " << s.str() << std::endl;
#endif
    FCITX_ASSERT(expected.size() == loader->keys().size());

    for (size_t i = 0; i < loader->keys().size(); i++) {
        std::string normalized =
            normalizeClassName(typeid(*expected[i]).name());
#if DEBUG
        std::cout << "expected class: " << normalized << std::endl;
        std::cout << "actual class: "
                  << normalizeClassName(typeid(*loader->keys()[i]).name())
                  << std::endl;
#endif
        FCITX_ASSERT(normalized ==
                     normalizeClassName(typeid(*loader->keys()[i]).name()));
#if DEBUG
        std::cout << "expected width: " << expected[i]->width() << std::endl;
        std::cout << "actual width: " << loader->keys()[i]->width()
                  << std::endl;
#endif
        FCITX_ASSERT(expected[i]->width() == loader->keys()[i]->width());
#if DEBUG
        std::cout << "expected newline: " << expected[i]->newLine()
                  << std::endl;
        std::cout << "actual newline: " << loader->keys()[i]->newLine()
                  << std::endl;
#endif
        FCITX_ASSERT(expected[i]->newLine() == loader->keys()[i]->newLine());
        if (normalized != "DummyKey" && normalized != "EmptyKey") {
        } else if (normalized == "NormalKey") {
            const char *expectedLabel =
                static_cast<fcitx::classicui::NormalKey *>(expected[i])
                    ->label();
            const char *actualLabel =
                static_cast<fcitx::classicui::NormalKey *>(loader->keys()[i])
                    ->label();
            // std::cout << "expected label: " << expectedLabel << std::endl;
            // std::cout << "actual label: " << actualLabel << std::endl;
            FCITX_ASSERT(!strcmp(expectedLabel, actualLabel));
        }
    }
    reset();
}

void KeyboardTester::testLoadInvalidKey(std::string json_body,
                                        std::string message) {
    std::stringstream s;
#if DEBUG
    fcitx::Log::setLogRule("*=5"); // Debug = 5
#else
    fcitx::Log::setLogRule("*=2"); // Error = 2
#endif
    fcitx::Log::setLogStream(s);

    std::string json = generateTemporaryJSON(json_body);
    saveTemporaryJSON(json);
#if DEBUG
    std::cout << "path: " << std::string(path_) << std::endl;
#endif
    fcitx::classicui::KeyboardLayout *loader =
        new fcitx::classicui::KeyboardLayout(path_);
    loader->load(0);

#if DEBUG
    std::cout << "loaded keys: " << loader->keys().size() << std::endl;
    std::cout << "log: " << s.str() << std::endl;
#endif
    FCITX_ASSERT(0 == loader->keys().size());

    auto lines = fcitx::stringutils::split(s.str(), "\n");
    bool found = false;
    for (size_t i = 0; i < lines.size(); i++) {
        std::string log = fcitx::stringutils::replaceAll(
            lines[i], VIRTUALKEYBOARDUI_TEST_DIR, "");
        if (log.find(message) != std::string::npos) {
            found = true;
        }
    }
    FCITX_ASSERT(found == true);

    fclose(fp_);
    unlink(path_);
}

void KeyboardTester::testMatchLog(std::string json, std::string expected) {
    std::stringstream s;
    fcitx::Log::setLogRule("*=2"); // Error = 2
    fcitx::Log::setLogStream(s);
    std::string jsonPath(VIRTUALKEYBOARDUI_TEST_DIR);
    jsonPath += json;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
    fcitx::classicui::KeyboardLayout *loader =
        new fcitx::classicui::KeyboardLayout(jsonPath.c_str());
#pragma GCC diagnostic pop
    std::string log =
        fcitx::stringutils::replaceAll(s.str(), VIRTUALKEYBOARDUI_TEST_DIR, "");
    FCITX_ASSERT(log.find(expected) != std::string::npos);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void KeyboardTester::testMatchLogs(std::string json,
                                   std::vector<std::string> expected,
                                   size_t offset, std::string rule) {
    std::stringstream s;
#if DEBUG
    fcitx::Log::setLogRule("*=5"); // Debug = 5
#else
    fcitx::Log::setLogRule(rule);
#endif
    fcitx::Log::setLogStream(s);
    std::string jsonPath(VIRTUALKEYBOARDUI_TEST_DIR);
    jsonPath += json;
    fcitx::classicui::KeyboardLayout *loader =
        new fcitx::classicui::KeyboardLayout(jsonPath.c_str());
    loader->load(offset);
#if DEBUG
    std::cout << "log: " << s.str() << std::endl;
#endif
    auto lines = fcitx::stringutils::split(s.str(), "\n");
    for (std::string expect : expected) {
        bool found = false;
        for (size_t i = 0; i < lines.size(); i++) {
            std::string log = fcitx::stringutils::replaceAll(
                lines[i], VIRTUALKEYBOARDUI_TEST_DIR, "");
            if (log.find(expect) != std::string::npos) {
                found = true;
            }
        }
        FCITX_ASSERT(found == true);
    }
}
#pragma GCC diagnostic pop

void KeyboardTester::testLoadWarning(std::string json, std::string message,
                                     size_t offset) {
    std::vector<std::string> expected;
    std::cout << "test with " << json << std::endl;
    expected.push_back(message);
    testMatchLogs(json, expected, offset, "*=3"); // Warn = 3
}

void KeyboardTester::testLoadError(std::string json, std::string message,
                                   size_t offset) {
    std::vector<std::string> expected;
    std::cout << "test with " << json << std::endl;
    expected.push_back(message);
    testMatchLogs(json, expected, offset, "*=2"); // Error = 2
}
