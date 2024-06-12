/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARDLAYOUT_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARDLAYOUT_H_

#include <json-c/json.h>
#include "fcitx-utils/log.h"
#include "virtualkeyboard.h"
#include "virtualkeygeneral.h"

namespace fcitx {
namespace classicui {

FCITX_DECLARE_LOG_CATEGORY(keyboard);
#if DEBUG
#define FCITX_KEYBOARD_LAYOUT() FCITX_LOGC_IF(::keyboard, Debug, 1)
#else
#define FCITX_KEYBOARD_LAYOUT() FCITX_LOGC_IF(::keyboard, Debug, 0)
#endif

class KeyboardLayout {
public:
    KeyboardLayout(const char *jsonPath) {
        FCITX_KEYBOARD() << "path of keyboard layout file: " << jsonPath;
        jsonPath_ = jsonPath;
        metadata_ = nullptr;
        if (!fs::isreg(jsonPath_)) {
            FCITX_ERROR() << "Failed to access keyboard layout file (JSON): "
                          << jsonPath_;
            return;
        }
        json_ = json_object_from_file(jsonPath_);
        if (!json_object_is_type(json_, json_type_array)) {
            FCITX_ERROR() << "Failed to load keyboard layout file (JSON): "
                          << jsonPath_;
        } else {
            metadata_ = json_object_array_get_idx(json_, 0);
        }
    }

    // Keyboard metadata
    const char *label() { return label_; }
    const char *languageCode() { return languageCode_; }

    int numberOfLayers();
    int layerOffset(size_t offset);

    // ModeSwitchKey
    int numberOfStates();
    const char *stateLabel();

    bool validate();
    bool loadMetadata(size_t offset);
    bool loadKeyLayout(size_t offset);
    bool load(size_t offset);
    std::vector<VirtualKey *> &keys() { return keys_; }
    std::string modeLabel(int mode) { return modeLabels_[mode]; }
    std::map<std::string, int> &modeActions() { return modeActions_; }
    std::map<std::string, int> &modeOffsets() { return modeOffsets_; }
    int modeOffsetsFallback(size_t modeIndex,
                            std::map<std::string, int> conditions);

protected:
    std::vector<VirtualKey *> keys_;
    bool parseMetadata(size_t offset);
    double parseDouble(json_object *object);
    bool parseKeyboard(json_object *keyboard, size_t offset);
    bool parseModeSwitchKey(json_object *modeSwitchKey);
    bool parseNormalKey(json_object *modeSwitchKey);
    bool parseMarkKey(json_object *markKey);
    void pushShiftToggleKey(json_object *width);
    void pushKey(const char *klassName, json_object *label, json_object *code,
                 json_object *upperLabel, json_object *keyName,
                 json_object *upperName, json_object *useLabelAsKeyName,
                 json_object *width, json_object *withShift,
                 json_object *zenkakuMark, json_object *isNumberKey);
    void applyScale(json_object *scale, bool newLine);
    void applyWidth(json_object *width);
    void pushDummyKey(json_object *width);
    void pushNormalKey(json_object *label, json_object *code,
                       json_object *upperLabel, json_object *name,
                       json_object *upperName, json_object *useLabelAsKeyName,
                       json_object *width);
    void pushMarkKey(json_object *label, json_object *keyName,
                     json_object *code, json_object *withShift,
                     json_object *width);
    void pushAnthyKanaKey(json_object *label, json_object *code,
                          json_object *keyName, json_object *upperLabel,
                          json_object *upperName, json_object *isNumberKey);

    bool IsNormalKey(json_object *label, json_object *code,
                     json_object *upperLabel, json_object *keyName,
                     json_object *upperName);
    bool IsNormalKeyWithLabelAsKeyName(json_object *label, json_object *code,
                                       json_object *upperLabel,
                                       json_object *useLabelAsKeyName);
    bool IsNormalKeyWithWidth(json_object *label, json_object *code,
                              json_object *upperLabel,
                              json_object *useLabelAsKeyName,
                              json_object *width);

#if DEBUG
    void printJsonObjectType(const char *label, json_object *obj);

#endif

    bool missingKeyboard() {
        if (!label_ && !languageCode_ && startRow_ == 0 && endRow_ == 0 &&
            numberOfLayers_ == 0) {
            return true;
        } else {
            return false;
        }
    };

    bool missingModeSwitchKey() {
        if (numberOfStates_ == 0 && stateLabels_.size() == 0) {
            return true;
        } else {
            return false;
        }
    }

private:
    //
    // [
    //   { /*keyboard metadata; optional*/ },
    //   [ /*row 1*/ ],
    //   /*...*/
    //   [ /*row n*/ ]
    // ]
    //
    json_object *json_ = nullptr;     // points whole JSON tree
    json_object *metadata_ = nullptr; // points optional metadata
    const char *jsonPath_;
    const char *label_ = nullptr;
    const char *languageCode_ = nullptr;
    int numberOfLayers_ = 0;
    int numberOfStates_ = 0;
    double normalKeyDefaultWidth_ = 0.0;
    double markKeyDefaultWidth_ = 0.0;
    bool normalKeyDefaultUseLabelAsKeyName_ = false;
    // CustomModeSwitchKey
    std::vector<std::string> stateLabels_;
    bool updateInputPanel_ = false;
    size_t startRow_ = 0;
    size_t endRow_ = 0;
    size_t currentRow_ = 0;
    size_t currentElement_ = 0;
    std::vector<std::string> modeLabels_;
    std::map<std::string, int> modeActions_;
    std::map<std::string, int> modeOffsets_;
};

} // namespace classicui
} // namespace fcitx

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARDLAYOUT_H_
