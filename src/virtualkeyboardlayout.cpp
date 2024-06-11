/*
 * SPDX-FileCopyrightText: 2024 Kentaro Hayashi <hayashi@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "virtualkeyboardlayout.h"
#include "virtualkeyboardanthy.h"
#include "virtualkeyboardchewing.h"
#include "virtualkeyboardcustom.h"
#include "virtualkeyboardpinyin.h"

namespace fcitx::classicui {

#if DEBUG
void KeyboardLayout::printJsonObjectType(const char *label, json_object *obj) {
    json_type type = json_object_get_type(obj);
    switch (type) {
    case json_type_null:
        FCITX_KEYBOARD_LAYOUT() << label << ": (null)";
        break;
    case json_type_boolean:
        FCITX_KEYBOARD_LAYOUT() << label << ": (boolean)";
        break;
    case json_type_double:
        FCITX_KEYBOARD_LAYOUT() << label << ": (double)";
        break;
    case json_type_int:
        FCITX_KEYBOARD_LAYOUT() << label << ": (int)";
        break;
    case json_type_object:
        FCITX_KEYBOARD_LAYOUT() << label << ": (object)";
        break;
    case json_type_array:
        FCITX_KEYBOARD_LAYOUT() << label << ": (array)";
        break;
    case json_type_string:
        FCITX_KEYBOARD_LAYOUT() << label << ": (string)";
        break;
    }
}
#endif

bool KeyboardLayout::parseKeyboard(json_object *keyboard, size_t offset) {
    FCITX_KEYBOARD_LAYOUT()
        << "KeyboardLayout::parseKeyboard(" << offset << ")";
    if (json_object_is_type(keyboard, json_type_null)) {
        FCITX_ERROR() << "Keyboard must not be null";
        return false;
    }
    json_object *layers = json_object_object_get(keyboard, "layerOffset");
    json_object *label = json_object_object_get(keyboard, "label");
    json_object *languageCode =
        json_object_object_get(keyboard, "languageCode");
    json_object *numberOfLayers =
        json_object_object_get(keyboard, "numberOfLayers");
    if (json_object_is_type(layers, json_type_null)) {
        FCITX_ERROR() << "Keyboard.layerOffset must be set";
    } else if (!json_object_is_type(layers, json_type_array)) {
        FCITX_ERROR() << "Keyboard.layerOffset must be array";
    } else {
        if (offset >= json_object_array_length(layers)) {
            FCITX_ERROR() << "offset: " << offset
                          << " must be less than size of Keyboard.layerOffset: "
                          << json_object_array_length(layers);
        } else {
            json_object *value = json_object_array_get_idx(layers, offset);
            if (!json_object_is_type(value, json_type_int)) {
                FCITX_ERROR()
                    << "Keyboard.layerOffset[" << offset << "] must be integer";
            } else {
                startRow_ = json_object_get_int64(value);
                FCITX_KEYBOARD_LAYOUT()
                    << "The value of starting row: " << startRow_
                    << " is fetched from offset: " << offset;
                if (offset + 1 < json_object_array_length(layers)) {
                    value = json_object_array_get_idx(layers, offset + 1);
                    endRow_ = json_object_get_int64(value);
                } else {
                    endRow_ = json_object_array_length(json_);
                }
                FCITX_KEYBOARD_LAYOUT()
                    << "The value of end row: " << endRow_
                    << " is fetched from offset: " << offset;
            }
        }
    }
    if (json_object_is_type(label, json_type_null)) {
        FCITX_ERROR() << "Keyboard.label must be set";
    } else if (!json_object_is_type(label, json_type_string)) {
        FCITX_ERROR() << "Keyboard.label must be string";
    } else {
        label_ = json_object_get_string(label);
    }
    if (json_object_is_type(languageCode, json_type_null)) {
        FCITX_ERROR() << "Keyboard.languageCode must be set";
    } else if (!json_object_is_type(languageCode, json_type_string)) {
        FCITX_ERROR() << "Keyboard.languageCode must be string";
    } else {
        languageCode_ = json_object_get_string(languageCode);
    }
    if (json_object_is_type(numberOfLayers, json_type_null)) {
        FCITX_ERROR() << "Keyboard.numberOfLayers must be set";
    } else if (!json_object_is_type(numberOfLayers, json_type_int)) {
        FCITX_ERROR() << "Keyboard.numberOfLayers must be integer";
    } else {
        numberOfLayers_ = json_object_get_int64(numberOfLayers);
    }
    return true;
}

bool KeyboardLayout::parseModeSwitchKey(json_object *modeSwitchKey) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::parseModeSwitchKey()";
    if (json_object_is_type(modeSwitchKey, json_type_null)) {
        FCITX_ERROR() << "ModeSwitchKey must not be null";
        return false;
    }
    json_object *numberOfStates =
        json_object_object_get(modeSwitchKey, "numberOfStates");
    if (json_object_is_type(numberOfStates, json_type_null)) {
        FCITX_ERROR() << "ModeSwitchKey.numberOfStates must be set";
    } else if (!json_object_is_type(numberOfStates, json_type_int)) {
        FCITX_ERROR() << "ModeSwitchKey.numberOfStates must be integer";
    } else {
        numberOfStates_ = json_object_get_int64(numberOfStates);
    }
    FCITX_KEYBOARD_LAYOUT() << "numberOfStates: " << numberOfStates_;
    json_object *stateLabel =
        json_object_object_get(modeSwitchKey, "stateLabel");
    if (json_object_is_type(stateLabel, json_type_null)) {
        FCITX_ERROR() << "ModeSwitchKey.stateLabel must be set";
    } else if (!json_object_is_type(stateLabel, json_type_array)) {
        FCITX_ERROR() << "ModeSwitchKey.stateLabel must be array";
    } else {
        for (size_t i = 0; i < json_object_array_length(stateLabel); i++) {
            json_object *value = json_object_array_get_idx(stateLabel, i);
            if (!json_object_is_type(value, json_type_string)) {
                FCITX_ERROR()
                    << "ModeSwitchKey.stateLabel[" << i << "] must be string";
            } else {
                stateLabels_.push_back(json_object_get_string(value));
            }
        }
    }
    if (numberOfStates_ != (int)stateLabels_.size()) {
        FCITX_ERROR() << "The number of stateLabel and numberOfStates must "
                         "be same. numberOfStates: "
                      << numberOfStates_
                      << " stateLabel: " << stateLabels_.size();
    }
    json_object *updateInputPanel =
        json_object_object_get(modeSwitchKey, "updateInputPanel");
    if (json_object_is_type(updateInputPanel, json_type_null)) {
    } else if (!json_object_is_type(updateInputPanel, json_type_boolean)) {
        FCITX_ERROR() << "ModeSwitchKey.updateInputPanel must be boolean";
    } else {
        updateInputPanel_ = json_object_get_boolean(updateInputPanel);
    }

    json_object *mode = json_object_object_get(modeSwitchKey, "mode");
    if (json_object_is_type(mode, json_type_null)) {
        FCITX_ERROR() << "ModeSwitchKey.mode must be set";
    } else if (!json_object_is_type(mode, json_type_array)) {
        FCITX_ERROR() << "ModeSwitchKey.mode must be array";
    } else {
        // collect model labels
        for (size_t i = 0; i < json_object_array_length(mode); i++) {
            json_object *element = json_object_array_get_idx(mode, i);
            if (!json_object_is_type(element, json_type_object)) {
                FCITX_ERROR()
                    << "ModeSwitchKey.mode[" << i << "] must be object";
            } else {
                json_object_object_foreach(element, key, val) {
                    if (!strcmp(key, "label")) {
                        modeLabels_.push_back(json_object_get_string(val));
                    }
                }
            }
        }
        for (size_t i = 0; i < json_object_array_length(mode); i++) {
            json_object *element = json_object_array_get_idx(mode, i);
            if (!json_object_is_type(element, json_type_object)) {
                FCITX_ERROR()
                    << "ModeSwitchKey.mode[" << i << "] must be object";
            } else {
                const char *label = nullptr;
                int layer = -1;
                int next = -1;
                json_object_object_foreach(element, key, val) {
                    if (!strcmp(key, "label")) {
                        label = json_object_get_string(val);
                    } else if (!strcmp(key, "next")) {
                        std::string target =
                            std::string(json_object_get_string(val));
                        for (size_t j = 0; j < modeLabels_.size(); j++) {
                            if (modeLabels_[j] == target) {
                                next = j;
                            }
                        }
                    } else if (!strcmp(key, "layer")) {
                        layer = json_object_get_int64(val);
                    }
                }
                if (label) {
                    if (layer < 0) {
                        FCITX_ERROR() << "no matched ModeSwitchKey.mode[" << i
                                      << "].label and layer";
                    } else {
                        modeOffsets_.insert(
                            std::make_pair(std::string(label), layer));
                    }
                    if (next < 0) {
                        FCITX_ERROR() << "no matched ModeSwitchKey.mode[" << i
                                      << "].label and next action";
                    } else {
                        modeActions_.insert(
                            std::make_pair(std::string(label), next));
                    }
                } else {
                    FCITX_KEYBOARD_LAYOUT() << "no matched ModeSwitchKey.mode["
                                            << i << "].label and layer";
                }
            }
        }
    }

    return true;
}

int KeyboardLayout::modeOffsetsFallback(size_t modeIndex,
                                        std::map<std::string, int> conditions) {
    // get current label
    std::string label = modeLabel(modeIndex);
    int defaultOffset = -1;
    int defaultGroup = -1;
    // parse ModeSwitchKey
    json_object *modeSwitchKey =
        json_object_object_get(metadata_, "ModeSwitchKey");
    json_object *mode = json_object_object_get(modeSwitchKey, "mode");

    if (!json_object_is_type(mode, json_type_array)) {
        FCITX_ERROR() << "ModeSwitchKey.mode must be array";
    } else {
        for (size_t i = modeIndex; i < json_object_array_length(mode); i++) {
            json_object *element = json_object_array_get_idx(mode, i);
            if (json_object_is_type(element, json_type_null)) {
                FCITX_KEYBOARD_LAYOUT()
                    << "ModeSwitchKey.mode[" << i << "] is missing";
            } else if (!json_object_is_type(element, json_type_object)) {
                FCITX_ERROR() << "ModeSwitchKey.mode must be array";
            } else {
                // layer
                json_object *layer = json_object_object_get(element, "layer");
                int layerValue = -1;
                if (json_object_is_type(layer, json_type_null)) {
                    FCITX_KEYBOARD_LAYOUT()
                        << "ModeSwitchKey.mode[" << i << "].layer is missing";
                } else if (!json_object_is_type(layer, json_type_int)) {
                    FCITX_ERROR() << "ModeSwitchKey.mode[" << i
                                  << "].layer must be integer";
                } else {
                    layerValue = json_object_get_int64(layer);
                }

                // group
                json_object *group = json_object_object_get(element, "group");
                int groupValue = -1;
                if (json_object_is_type(group, json_type_null)) {
                    FCITX_KEYBOARD_LAYOUT()
                        << "ModeSwitchKey.mode[" << i << "].group is missing";
                } else if (!json_object_is_type(group, json_type_int)) {
                    FCITX_ERROR() << "ModeSwitchKey.mode[" << i
                                  << "].group must be integer";
                } else {
                    groupValue = json_object_get_int64(group);
                }

                if (i == modeIndex) {
                    defaultGroup = groupValue;
                    defaultOffset = layerValue;
                } else {
                    if (groupValue != defaultGroup) {
                        // return last matched offset
                        return defaultOffset;
                    }
                }
                json_object *cond =
                    json_object_object_get(element, "conditions");
                if (json_object_is_type(cond, json_type_null)) {
                    // no conditions,
                    return defaultOffset;
                } else if (!json_object_is_type(cond, json_type_object)) {
                    FCITX_ERROR() << "ModeSwitchKey.mode[" << i
                                  << "].conditions must be object";
                } else {
                    // search whether keyboard internal conditions match to
                    // keyboard layout definition
                    bool found = true;
                    json_object_object_foreach(cond, key, value) {
                        if (json_object_is_type(value, json_type_null)) {
                        } else if (!json_object_is_type(value, json_type_int)) {
                        } else {
                            int condValue = json_object_get_int64(value);
                            if (conditions.count(key) &&
                                conditions[key] == condValue) {
                            } else {
                                found = false;
                            }
                        }
                    }
                    if (found) {
                        defaultOffset = layerValue;
                    }
                }
            }
        }
    }
    return defaultOffset;
}

bool KeyboardLayout::parseNormalKey(json_object *normalKey) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::parseNormalKey()";
    if (json_object_is_type(normalKey, json_type_null)) {
        FCITX_ERROR() << "NormalKey must not be null";
        return false;
    }
    json_object *width = json_object_object_get(normalKey, "width");
    if (json_object_is_type(width, json_type_null)) {
        FCITX_ERROR() << "NormalKey.width must be set";
    } else if (json_object_is_type(width, json_type_double)) {
        normalKeyDefaultWidth_ = json_object_get_double(width);
    } else if (json_object_is_type(width, json_type_int)) {
        normalKeyDefaultWidth_ = json_object_get_int64(width);
        FCITX_KEYBOARD_LAYOUT() << "NormalKey.width(fallback to integer): "
                                << normalKeyDefaultWidth_;
    } else {
        FCITX_ERROR() << "NormalKey.width must be double";
    }
    json_object *useLabelAsKeyName =
        json_object_object_get(normalKey, "useLabelAsKeyName");
    if (json_object_is_type(useLabelAsKeyName, json_type_null)) {
        FCITX_ERROR() << "NormalKey.useLabelAsKeyName must be set";
    } else if (json_object_is_type(useLabelAsKeyName, json_type_boolean)) {
        normalKeyDefaultUseLabelAsKeyName_ =
            json_object_get_boolean(useLabelAsKeyName);
    } else {
        FCITX_ERROR() << "NormalKey.useLabelAsKeyName must be boolean";
    }
    return true;
}

bool KeyboardLayout::parseMarkKey(json_object *markKey) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::parseMarkKey()";
    if (json_object_is_type(markKey, json_type_null)) {
        FCITX_ERROR() << "MarkKey must not be null";
        return false;
    }
    json_object *width = json_object_object_get(markKey, "width");
    if (json_object_is_type(width, json_type_null)) {
        FCITX_ERROR() << "MarkKey.width must be set";
    } else if (json_object_is_type(width, json_type_double)) {
        markKeyDefaultWidth_ = json_object_get_double(width);
    } else if (json_object_is_type(width, json_type_int)) {
        markKeyDefaultWidth_ = json_object_get_int64(width);
        FCITX_KEYBOARD_LAYOUT()
            << "MarkKey.width(fallback to integer): " << markKeyDefaultWidth_;
    } else {
        FCITX_ERROR() << "MarkKey.width must be double";
    }
    return true;
}

bool KeyboardLayout::parseMetadata(size_t offset) {
    FCITX_KEYBOARD_LAYOUT()
        << "KeyboardLayout::parseMetadata(offset = " << offset << ")";
    if (json_object_is_type(metadata_, json_type_null)) {
        FCITX_ERROR() << "metadata of keyboard layout must not be null";
        return false;
    }

    if (!json_object_is_type(metadata_, json_type_object)) {
        FCITX_ERROR() << "metadata of keyboard layout must be object";
        return false;
    }

    FCITX_KEYBOARD_LAYOUT() << "parse keyboard metadata";
    json_object_object_foreach(metadata_, key, val) {
        if (!strcmp(key, "Keyboard")) {
            parseKeyboard(val, offset);
        } else if (!strcmp(key, "ModeSwitchKey")) {
            parseModeSwitchKey(val);
        } else if (!strcmp(key, "NormalKey")) {
            parseNormalKey(val);
        } else if (!strcmp(key, "MarkKey")) {
            parseMarkKey(val);
        } else {
            FCITX_WARN() << "unknown key of metadata: " << key;
        }
    }
    if (missingKeyboard()) {
        FCITX_ERROR() << "Keyboard must be set";
        return false;
    } else if (startRow_ >= endRow_) {
        FCITX_ERROR() << "Keyboard.layerOffset must be "
                         "set in ascending order: "
                      << startRow_ << " < " << endRow_;
        return false;
    }
    if (languageCode_ && strcmp(languageCode_, "ja")) {
        // except ja, require ModeSwitchKey
        if (missingModeSwitchKey()) {
            FCITX_ERROR() << "ModeSwitchKey must be set";
            return false;
        }
    }
    return true;
}

double KeyboardLayout::parseDouble(json_object *object) {
    double value = 0.0;
    if (json_object_is_type(object, json_type_null)) {
    } else if (json_object_is_type(object, json_type_double)) {
        value = json_object_get_double(object);
    } else if (json_object_is_type(object, json_type_int)) {
        value = json_object_get_int64(object);
    }
    return value;
}

void KeyboardLayout::applyScale(json_object *scale, bool newLine) {
    double scaleValue = parseDouble(scale);
    if (scaleValue > 0.0) {
        FCITX_KEYBOARD_LAYOUT() << "apply tweak scale: " << scaleValue;
        keys_.back()->setCustomLayout(scaleValue, newLine);
    }
}

void KeyboardLayout::applyWidth(json_object *width) {
    double widthValue = parseDouble(width);
    if (widthValue > 0.0) {
        FCITX_KEYBOARD_LAYOUT() << "apply tweak width: " << widthValue;
        keys_.back()->setCustomWidth(widthValue);
    }
}

void KeyboardLayout::pushDummyKey(json_object *width) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::pushDummyKey()";
    double value = parseDouble(width);
    if (value > 0.0) {
        FCITX_KEYBOARD_LAYOUT() << "class name: DummyKey"
                                << " width: " << value;
        keys_.emplace_back(new DummyKey(value));
    } else {
        FCITX_KEYBOARD_LAYOUT() << "class name: DummyKey";
        keys_.emplace_back(new DummyKey());
    }
}

void KeyboardLayout::pushShiftToggleKey(json_object *width) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::pushShiftToggleKey()";
    double value = parseDouble(width);
    if (value > 0.0) {
        FCITX_KEYBOARD_LAYOUT() << "class name: ShiftToggleKey"
                                << " width: " << value;
        keys_.emplace_back(new ShiftToggleKey(value));
    } else {
        FCITX_KEYBOARD_LAYOUT() << "class name: ShiftToggleKey";
        keys_.emplace_back(new ShiftToggleKey());
    }
}

bool KeyboardLayout::IsNormalKey(json_object *label, json_object *code,
                                 json_object *upperLabel, json_object *keyName,
                                 json_object *upperName) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::IsNormalKey()";
    if (!json_object_is_type(label, json_type_null) &&
        !json_object_is_type(code, json_type_null) &&
        !json_object_is_type(upperLabel, json_type_null) &&
        !json_object_is_type(keyName, json_type_null) &&
        !json_object_is_type(upperName, json_type_null)) {
        FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::IsNormalKey(): true";
        return true;
    } else {
        return false;
    }
}

bool KeyboardLayout::IsNormalKeyWithLabelAsKeyName(
    json_object *label, json_object *code, json_object *upperLabel,
    json_object *useLabelAsKeyName) {
    FCITX_KEYBOARD_LAYOUT()
        << "KeyboardLayout::IsNormalKeyWithLabelAsKeyName()";
#if DEBUG
    printJsonObjectType("label", label);
    printJsonObjectType("code", code);
    printJsonObjectType("upperLabel", upperLabel);
    printJsonObjectType("useLabelAsKeyName", useLabelAsKeyName);
#endif
    if (!json_object_is_type(label, json_type_null) &&
        !json_object_is_type(code, json_type_null) &&
        !json_object_is_type(upperLabel, json_type_null) &&
        !json_object_is_type(useLabelAsKeyName, json_type_null)) {
        FCITX_KEYBOARD_LAYOUT()
            << "KeyboardLayout::IsNormalKeyWithLabelAsKeyName(): true";
        return true;
    } else {
        if (normalKeyDefaultUseLabelAsKeyName_) {
            // fallback to default useLabelAsKeyName
            if (!json_object_is_type(label, json_type_null) &&
                !json_object_is_type(code, json_type_null) &&
                !json_object_is_type(upperLabel, json_type_null)) {
                FCITX_KEYBOARD_LAYOUT()
                    << "KeyboardLayout::IsNormalKeyWithLabelAsKeyName(): "
                       "true "
                       "(default useLabelAsKeyName)";
                return true;
            }
        }
    }
    return false;
}

bool KeyboardLayout::IsNormalKeyWithWidth(json_object *label, json_object *code,
                                          json_object *upperLabel,
                                          json_object *useLabelAsKeyName,
                                          json_object *width) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::IsNormalKeyWithWidth()";
    if (!json_object_is_type(label, json_type_null) &&
        !json_object_is_type(code, json_type_null) &&
        !json_object_is_type(upperLabel, json_type_null) &&
        !json_object_is_type(useLabelAsKeyName, json_type_null) &&
        !json_object_is_type(width, json_type_null)) {
        FCITX_KEYBOARD_LAYOUT()
            << "KeyboardLayout::IsNormalKeyWithWidth(): true";
        return true;
    } else {
        if (normalKeyDefaultWidth_) {
            // fallback to default useLabelAsKeyName
            if (!json_object_is_type(label, json_type_null) &&
                !json_object_is_type(code, json_type_null) &&
                !json_object_is_type(upperLabel, json_type_null)) {
                FCITX_KEYBOARD_LAYOUT()
                    << "KeyboardLayout::IsNormalKeyWithWidth(): true "
                       "(default "
                       "width)";
                return true;
            }
        }
    }
    return false;
}

void KeyboardLayout::pushNormalKey(json_object *label, json_object *code,
                                   json_object *upperLabel,
                                   json_object *keyName, json_object *upperName,
                                   json_object *useLabelAsKeyName,
                                   json_object *width) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::pushNormalKey()";
    int keyCode = -1;
    std::string labelValue;
    std::string upperLabelValue;
    std::string keyNameValue;
    std::string upperNameValue;
    double widthValue = 0.0;
    bool useLabelAsKeyNameValue = false;

    // label
    if (json_object_is_type(label, json_type_null)) {
    } else if (json_object_is_type(label, json_type_string)) {
        labelValue = std::string(json_object_get_string(label));
    }

    // keyCode
    if (json_object_is_type(code, json_type_null)) {
    } else if (json_object_is_type(code, json_type_int)) {
        keyCode = json_object_get_int64(code);
    }

    // upperLabel
    if (json_object_is_type(upperLabel, json_type_null)) {
    } else if (json_object_is_type(upperLabel, json_type_string)) {
        upperLabelValue = std::string(json_object_get_string(upperLabel));
    }

    // useLabelAsKeyName: (optional)
    if (json_object_is_type(useLabelAsKeyName, json_type_null)) {
    } else if (json_object_is_type(useLabelAsKeyName, json_type_boolean)) {
        useLabelAsKeyNameValue = json_object_get_boolean(useLabelAsKeyName);
    }

    // keyName
    if (json_object_is_type(keyName, json_type_null)) {
    } else if (json_object_is_type(keyName, json_type_string)) {
        keyNameValue = std::string(json_object_get_string(keyName));
    }

    // upperName
    if (json_object_is_type(upperName, json_type_null)) {
    } else if (json_object_is_type(upperName, json_type_string)) {
        upperNameValue = std::string(json_object_get_string(upperName));
    }

    // width: (optional)
    widthValue = parseDouble(width);

    if (IsNormalKey(label, code, upperLabel, keyName, upperName)) {
        // hangul or chewing
        keys_.emplace_back(new NormalKey(labelValue, keyCode, upperLabelValue,
                                         keyNameValue, upperNameValue));
    } else if (IsNormalKeyWithWidth(label, code, upperLabel, useLabelAsKeyName,
                                    width)) {
        // russian (with each width)
        FCITX_KEYBOARD_LAYOUT()
            << "row[" << currentRow_ << "][" << currentElement_
            << "] NormalKey(width) label: " << labelValue
            << " code: " << keyCode << " upperLabel: " << upperLabelValue
            << " useLabelAsKeyName: " << useLabelAsKeyName
            << " width:" << widthValue;
        if (json_object_is_type(useLabelAsKeyName, json_type_null)) {
            if (json_object_is_type(width, json_type_null)) {
                // no useLabelAsKeyName, no width
                if (normalKeyDefaultUseLabelAsKeyName_) {
                    if (normalKeyDefaultWidth_ > 0.0) {
                        keys_.emplace_back(
                            new NormalKey(labelValue, keyCode, upperLabelValue,
                                          normalKeyDefaultUseLabelAsKeyName_,
                                          normalKeyDefaultWidth_));
                    } else {
                        keys_.emplace_back(
                            new NormalKey(labelValue, keyCode, upperLabelValue,
                                          normalKeyDefaultUseLabelAsKeyName_));
                    }
                } else {
                    if (normalKeyDefaultWidth_ > 0.0) {
                        // russian
                        FCITX_KEYBOARD_LAYOUT()
                            << "row[" << currentRow_ << "][" << currentElement_
                            << "] NormalKey(width) label: " << labelValue
                            << " code: " << keyCode
                            << " upperLabel: " << upperLabelValue
                            << " useLabelAsKeyName(default): "
                            << normalKeyDefaultUseLabelAsKeyName_
                            << " width(default): " << normalKeyDefaultWidth_;
                        keys_.emplace_back(
                            new NormalKey(labelValue, keyCode, upperLabelValue,
                                          normalKeyDefaultUseLabelAsKeyName_,
                                          normalKeyDefaultWidth_));
                    } else {
                        keys_.emplace_back(
                            new NormalKey(labelValue, keyCode, upperLabelValue,
                                          normalKeyDefaultUseLabelAsKeyName_));
                    }
                }
            } else {
                // no useLabelAsKeyName, width
                keys_.emplace_back(new NormalKey(
                    labelValue, keyCode, upperLabelValue,
                    normalKeyDefaultUseLabelAsKeyName_, widthValue));
            }
        } else {
            if (json_object_is_type(width, json_type_null)) {
                // useLabelAsKeyName, no width
                if (normalKeyDefaultWidth_ > 0.0) {
                    keys_.emplace_back(new NormalKey(
                        labelValue, keyCode, upperLabelValue,
                        useLabelAsKeyNameValue, normalKeyDefaultWidth_));
                } else {
                    keys_.emplace_back(new NormalKey(labelValue, keyCode,
                                                     upperLabelValue,
                                                     useLabelAsKeyNameValue));
                }
            } else {
                // useLabelAsKeyName, width
                keys_.emplace_back(
                    new NormalKey(labelValue, keyCode, upperLabelValue,
                                  useLabelAsKeyNameValue, widthValue));
            }
        }
    } else if (IsNormalKeyWithLabelAsKeyName(label, code, upperLabel,
                                             useLabelAsKeyName)) {
        // us or pinyin
        if (json_object_is_type(useLabelAsKeyName, json_type_null) &&
            normalKeyDefaultUseLabelAsKeyName_) {
            FCITX_KEYBOARD_LAYOUT()
                << "row[" << currentRow_ << "][" << currentElement_
                << "] NormalKey(LabelAsKeyName) label: " << labelValue
                << " code: " << keyCode << " upperLabel: " << upperLabelValue
                << " useLabelAsKeyName(default): "
                << normalKeyDefaultUseLabelAsKeyName_
                << " useLabelAsKeyName: " << useLabelAsKeyNameValue;
            keys_.emplace_back(
                new NormalKey(labelValue, keyCode, upperLabelValue,
                              normalKeyDefaultUseLabelAsKeyName_));
        } else {
            FCITX_KEYBOARD_LAYOUT()
                << "row[" << currentRow_ << "][" << currentElement_
                << "] NormalKey(LabelAsKeyName) label: " << labelValue
                << " code: " << keyCode << " upperLabel: " << upperLabelValue
                << " useLabelAsKeyName: " << useLabelAsKeyNameValue;
            keys_.emplace_back(new NormalKey(
                labelValue, keyCode, upperLabelValue, useLabelAsKeyNameValue));
        }
    } else {
        FCITX_ERROR() << "row[" << currentRow_ << "][" << currentElement_
                      << "] unexpected NormalKey label: " << labelValue
                      << " code: " << keyCode;
        return;
    }

    // width: (optional)
    widthValue = parseDouble(width);
    if (widthValue > 0.0) {
        keys_.back()->setCustomWidth(widthValue);
    } else if (normalKeyDefaultWidth_ > 0.0) {
        // russian with default width
        keys_.back()->setCustomWidth(normalKeyDefaultWidth_);
    }
}

void KeyboardLayout::pushMarkKey(json_object *label, json_object *keyName,
                                 json_object *code, json_object *withShift,
                                 json_object *width) {
    const char *labelValue = json_object_get_string(label);
    if (!json_object_is_type(withShift, json_type_null)) {
        // withShift
        if (!json_object_is_type(withShift, json_type_boolean)) {
            FCITX_ERROR() << "row[" << currentRow_ << "][" << currentElement_
                          << "] MarkKey.withShift must be boolean";
        } else {
            const char *keyNameValue = json_object_get_string(keyName);
            int keyCode = json_object_get_int64(code);
            bool withShiftValue = json_object_get_boolean(withShift);
            keys_.emplace_back(
                new MarkKey(labelValue, keyNameValue, keyCode, withShiftValue));
        }
    } else if (!json_object_is_type(code, json_type_null)) {
        // code
        if (!json_object_is_type(code, json_type_int)) {
            FCITX_ERROR() << "row[" << currentRow_ << "][" << currentElement_
                          << "] MarkKey.code must be integer";
        } else {
            const char *keyNameValue = json_object_get_string(keyName);
            int keyCode = json_object_get_int64(code);
            keys_.emplace_back(new MarkKey(labelValue, keyNameValue, keyCode));
        }
    } else if (!json_object_is_type(keyName, json_type_null)) {
        // keyName
        if (!json_object_is_type(keyName, json_type_string)) {
            FCITX_ERROR() << "row[" << currentRow_ << "][" << currentElement_
                          << "] MarkKey.keyName must be string";
        } else {
            const char *keyNameValue = json_object_get_string(keyName);
            keys_.emplace_back(new MarkKey(labelValue, keyNameValue));
        }
    } else {
        // label only (us)
        keys_.emplace_back(new MarkKey(labelValue));
    }
    if (json_object_is_type(width, json_type_null) &&
        markKeyDefaultWidth_ > 0.0) {
        keys_.back()->setCustomWidth(markKeyDefaultWidth_);
    }
}

void KeyboardLayout::pushAnthyKanaKey(json_object *label, json_object *code,
                                      json_object *keyName,
                                      json_object *upperLabel,
                                      json_object *upperName,
                                      json_object *isNumberKey) {
    const char *labelValue = json_object_get_string(label);
    const char *keyNameValue = json_object_get_string(keyName);
    const char *upperLabelValue = json_object_get_string(upperLabel);
    const char *upperNameValue = json_object_get_string(upperName);
    int keyCode = json_object_get_int64(code);
    if (json_object_is_type(isNumberKey, json_type_null)) {
        FCITX_KEYBOARD_LAYOUT()
            << "AnthyKanaKey: label: " << labelValue << " code: " << keyCode
            << " keyName: " << keyNameValue << " upperLabel: " << upperLabel
            << " upperName: " << upperName << " isNumberKey: false";
        keys_.emplace_back(new AnthyKanaKey(labelValue, keyCode,
                                            upperLabelValue, keyNameValue,
                                            upperNameValue));
    } else if (!json_object_is_type(isNumberKey, json_type_boolean)) {
        FCITX_ERROR() << "row[" << currentRow_ << "][" << currentElement_
                      << "] AnthyKanaKey.isNumberKey must be boolean";
    } else {
        bool isNumberKeyValue = json_object_get_boolean(isNumberKey);
        FCITX_KEYBOARD_LAYOUT()
            << "AnthyKanaKey: label: " << labelValue
            << " keyName: " << keyNameValue << " upperLabel: " << upperLabel
            << " upperName: " << upperName;
        keys_.emplace_back(new AnthyKanaKey(labelValue, keyCode,
                                            upperLabelValue, keyNameValue,
                                            upperNameValue, isNumberKeyValue));
    }
}

std::vector<std::string> reservedKeyNames = {"ModeSwitchKey",
                                             "ShiftToggleKey",
                                             "LanguageSwitchKey",
                                             "NormalKey",
                                             "NumberKey",
                                             "BackSpaceKey",
                                             "SpaceKey",
                                             "EnterKey",
                                             "MarkKey",
                                             "UpKey",
                                             "DownKey",
                                             "RightKey",
                                             "LeftKey",
                                             "DummyKey",
                                             "EmptyKey",
                                             "ChewingNumberKey",
                                             "ChewingNumPadKey",
                                             "ChewingEnterKey",
                                             "ChewingModeSwitchKey",
                                             "ChewingSpaceKey",
                                             "ChewingMarkToggleKey",
                                             "PinyinModeSwitchKey",
                                             "PinyinSpaceKey",
                                             "PinyinMarkToggleKey",
                                             "AnthyInputModeSwitchKey",
                                             "AnthyTypingMethodSwitchKey",
                                             "AnthyModeSwitchKey",
                                             "AnthyNumpadModeSwitchKey",
                                             "ZenkakuHankakuKey",
                                             "AnthyKanaKey",
                                             "AnthyKanaNumPadKey",
                                             "AnthyMarkKey",
                                             "CustomModeSwitchKey"};

void KeyboardLayout::pushKey(const char *klassName, json_object *label,
                             json_object *code, json_object *upperLabel,
                             json_object *name, json_object *upperName,
                             json_object *useLabelAsKeyName, json_object *width,
                             json_object *withShift, json_object *zenkakuMark,
                             json_object *isNumberKey) {
    const char *labelValue = json_object_get_string(label);
    FCITX_KEYBOARD_LAYOUT()
        << "class name: " << klassName << " label: " << labelValue;
    if (!strcmp(klassName, "NormalKey")) {
        pushNormalKey(label, code, upperLabel, name, upperName,
                      useLabelAsKeyName, width);
    } else if (!strcmp(klassName, "ModeSwitchKey")) {
        FCITX_KEYBOARD_LAYOUT() << "numberOfStates: " << numberOfStates_;
        if (stateLabels_.size() > 0) {
            keys_.emplace_back(
                new ModeSwitchKey(label_, numberOfStates_, stateLabels_));
        }
    } else if (!strcmp(klassName, "ShiftToggleKey")) {
        pushShiftToggleKey(width);
    } else if (!strcmp(klassName, "LanguageSwitchKey")) {
        keys_.emplace_back(new LanguageSwitchKey());
    } else if (!strcmp(klassName, "NumberKey")) {
        int keyCode = json_object_get_int64(code);
        keys_.emplace_back(new NumberKey(labelValue, keyCode));
    } else if (!strcmp(klassName, "BackSpaceKey")) {
        keys_.emplace_back(new BackSpaceKey());
    } else if (!strcmp(klassName, "SpaceKey")) {
        keys_.emplace_back(new SpaceKey());
    } else if (!strcmp(klassName, "EnterKey")) {
        keys_.emplace_back(new EnterKey());
    } else if (!strcmp(klassName, "MarkKey")) {
        pushMarkKey(label, name, code, withShift, width);
    } else if (!strcmp(klassName, "UpKey")) {
        keys_.emplace_back(new UpKey());
    } else if (!strcmp(klassName, "DownKey")) {
        keys_.emplace_back(new DownKey());
    } else if (!strcmp(klassName, "RightKey")) {
        keys_.emplace_back(new RightKey());
    } else if (!strcmp(klassName, "LeftKey")) {
        keys_.emplace_back(new LeftKey());
    } else if (!strcmp(klassName, "ChewingNumberKey")) {
        int keyCode = json_object_get_int64(code);
        const char *keyNameValue = json_object_get_string(name);
        keys_.emplace_back(
            new ChewingNumberKey(labelValue, keyNameValue, keyCode));
    } else if (!strcmp(klassName, "ChewingNumPadKey")) {
        keys_.emplace_back(new ChewingNumPadKey(labelValue));
    } else if (!strcmp(klassName, "ChewingEnterKey")) {
        keys_.emplace_back(new ChewingEnterKey());
    } else if (!strcmp(klassName, "ChewingModeSwitchKey")) {
        keys_.emplace_back(new ChewingModeSwitchKey());
    } else if (!strcmp(klassName, "ChewingSpaceKey")) {
        keys_.emplace_back(new ChewingSpaceKey());
    } else if (!strcmp(klassName, "ChewingMarkToggleKey")) {
        keys_.emplace_back(new ChewingMarkToggleKey());
    } else if (!strcmp(klassName, "PinyinModeSwitchKey")) {
        keys_.emplace_back(new PinyinModeSwitchKey());
    } else if (!strcmp(klassName, "PinyinEnterKey")) {
        keys_.emplace_back(new PinyinEnterKey());
    } else if (!strcmp(klassName, "PinyinSpaceKey")) {
        keys_.emplace_back(new PinyinSpaceKey());
    } else if (!strcmp(klassName, "PinyinMarkToggleKey")) {
        keys_.emplace_back(new PinyinMarkToggleKey());
    } else if (!strcmp(klassName, "AnthyInputModeSwitchKey")) {
        keys_.emplace_back(new AnthyInputModeSwitchKey());
    } else if (!strcmp(klassName, "AnthyTypingMethodSwitchKey")) {
        keys_.emplace_back(new AnthyTypingMethodSwitchKey());
    } else if (!strcmp(klassName, "AnthyModeSwitchKey")) {
        keys_.emplace_back(new AnthyModeSwitchKey());
    } else if (!strcmp(klassName, "AnthyNumpadModeSwitchKey")) {
        keys_.emplace_back(new AnthyNumpadModeSwitchKey());
    } else if (!strcmp(klassName, "ZenkakuHankakuKey")) {
        keys_.emplace_back(new ZenkakuHankakuKey());
    } else if (!strcmp(klassName, "AnthyKanaKey")) {
        pushAnthyKanaKey(label, code, name, upperLabel, upperName, isNumberKey);
    } else if (!strcmp(klassName, "AnthyKanaNumPadKey")) {
        keys_.emplace_back(new AnthyKanaNumPadKey(labelValue));
    } else if (!strcmp(klassName, "AnthyMarkKey")) {
        const char *keyNameValue = json_object_get_string(name);
        const char *zenkakuMarkValue = json_object_get_string(zenkakuMark);
        FCITX_KEYBOARD_LAYOUT() << "AnthyMarkKey: keyName: " << keyNameValue
                                << " label(hankakuMark): " << labelValue
                                << " zenkakuMark: " << zenkakuMarkValue;
        keys_.emplace_back(
            new AnthyMarkKey(keyNameValue, labelValue, zenkakuMarkValue));
    } else if (!strcmp(klassName, "CustomModeSwitchKey")) {
        FCITX_KEYBOARD_LAYOUT() << "numberOfStates: " << numberOfStates_;
        if (stateLabels_.size() > 0) {
            keys_.emplace_back(new CustomModeSwitchKey(
                label_, numberOfStates_, stateLabels_, updateInputPanel_));
        }
    } else {
        bool isTypoFound = false;
        for (size_t i = 0; i < reservedKeyNames.size(); i++) {
            if (!strcasecmp(klassName, reservedKeyNames[i].c_str())) {
                FCITX_ERROR() << "class name must be " << reservedKeyNames[i]
                              << ", not <" << klassName << ">";
                isTypoFound = true;
            }
        }
        if (!isTypoFound) {
            FCITX_ERROR() << "unknown klass of key name: " << klassName;
        }
    }
    // width
    double widthValue = parseDouble(width);
    if (widthValue > 0.0) {
        FCITX_KEYBOARD_LAYOUT() << "apply tweak width: " << widthValue;
        keys_.back()->setCustomWidth(widthValue);
    }
}

bool KeyboardLayout::load(size_t offset) {
    try {
        stateLabels_.clear();
        modeLabels_.clear();
        loadMetadata(offset);
        return loadKeyLayout(offset);
    } catch (...) {
        FCITX_ERROR() << "Something weird happen in KeyboardLayout::load("
                      << offset << ")";
    }
    return false;
}

bool KeyboardLayout::loadMetadata(size_t offset) {
    FCITX_KEYBOARD_LAYOUT() << "KeyboardLayout::loadMetadata()";
    // just load metadata with dummy offset=0.
    // it mainly aimed to load label and languageCode.
    if (!parseMetadata(offset)) {
        FCITX_ERROR() << "Failed to parse metadata of keyboard layout file "
                         "(JSON) with offset: "
                      << offset;
        return false;
    }
    return true;
}

bool KeyboardLayout::loadKeyLayout(size_t offset) {
    FCITX_KEYBOARD_LAYOUT()
        << "KeyboardLayout::load(offset = " << offset << ")";
    keys_.clear();

    FCITX_KEYBOARD_LAYOUT()
        << "keyboard layout raw data range: " << startRow_ << "-" << endRow_;

    FCITX_KEYBOARD_LAYOUT() << "parse keyboard layout data";
    for (size_t i = startRow_; i < endRow_; i++) {
        currentRow_ = i;
        json_object *elements = json_object_array_get_idx(json_, i);
        json_object *klass = nullptr, *code = nullptr, *upperLabel = nullptr,
                    *useLabelAsKeyName = nullptr, *scale = nullptr,
                    *withShift = nullptr, *keyName = nullptr,
                    *upperName = nullptr, *width = nullptr,
                    *zenkakuMark = nullptr, *isNumberKey = nullptr;
        if (json_object_is_type(elements, json_type_null)) {
            FCITX_ERROR() << "Failed to layout offset: " << offset << " row["
                          << i << "] key data";
            continue;
        } else if (!json_object_is_type(elements, json_type_array)) {
            FCITX_ERROR() << "Keyboard layout offset: " << offset << " row["
                          << i << "] key data must be array";
            continue;
        } else {
            for (size_t j = 0; j < json_object_array_length(elements); j++) {
                json_object *obj = json_object_array_get_idx(elements, j);
                currentElement_ = j;
                if (json_object_is_type(obj, json_type_object)) {
                    // properties
                    klass = json_object_object_get(obj, "class");
                    code = json_object_object_get(obj, "code");
                    upperLabel = json_object_object_get(obj, "upperLabel");
                    scale = json_object_object_get(obj, "scale");
                    withShift = json_object_object_get(obj, "withShift");
                    keyName = json_object_object_get(obj, "keyName");
                    upperName = json_object_object_get(obj, "upperName");
                    width = json_object_object_get(obj, "width");
                    useLabelAsKeyName =
                        json_object_object_get(obj, "useLabelAsKeyName");
                    zenkakuMark = json_object_object_get(obj, "zenkakuMark");
                    isNumberKey = json_object_object_get(obj, "isNumberKey");
                    const char *klassName = json_object_get_string(klass);
                    bool newLine = false;
                    if (j == json_object_array_length(elements) - 1) {
                        FCITX_KEYBOARD_LAYOUT() << "last element of row: " << i;
                        if (i != (endRow_ - 1)) {
                            newLine = true;
                        }
                    }
                    if (!strcmp(klassName, "DummyKey")) {
                        pushDummyKey(width);
                        applyScale(scale, newLine);
                    } else if (!strcmp(klassName, "EmptyKey")) {
                        keys_.emplace_back(new EmptyKey());
                        applyWidth(width);
                        applyScale(scale, newLine);
                    }
                } else if (json_object_is_type(obj, json_type_string)) {
                    // key
                    const char *klassName = json_object_get_string(klass);
                    pushKey(klassName, obj, code, upperLabel, keyName,
                            upperName, useLabelAsKeyName, width, withShift,
                            zenkakuMark, isNumberKey);
                    double scaleValue = parseDouble(scale);
                    if (scaleValue == 0.0) {
                        scaleValue = 1.0;
                    }
                    bool newLine = false;
                    if (j == json_object_array_length(elements) - 1) {
                        FCITX_KEYBOARD_LAYOUT() << "last element of row: " << i;
                        if (i != (endRow_ - 1)) {
                            newLine = true;
                        }
                    }
                    const char *labelValue = json_object_get_string(obj);
                    FCITX_KEYBOARD_LAYOUT() << "class name: " << klassName
                                            << " label: " << labelValue
                                            << " tweak scale: " << scaleValue
                                            << " newLine: " << newLine;
                    if (keys_.size() > 0) {
                        keys_.back()->setCustomLayout(scaleValue, newLine);
                    } else {
                        FCITX_ERROR() << "Keyboard layout offset: " << offset
                                      << " row[" << i << "][" << j
                                      << "] failed to apply custom layout";
                    }
                } else {
                    // except {} ""
                    FCITX_ERROR()
                        << "Keyboard layout offset: " << offset << " row[" << i
                        << "][" << j << "] key data must be object or string";
                }
            }
        }
    }
    return true;
}

} // namespace fcitx::classicui
