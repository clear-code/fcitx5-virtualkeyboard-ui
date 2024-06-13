# Custom Keyboard Layout Specification v1.0

## Abstract

The UI of virtual keyboard is previously implemented in fully
hard-coding.  It means that if you want to change keyboard layout, you
need to change the source code and recompile them.

This is not suitable way to implement various software keyboard.
In this specification, it describes the custom keyboard layout specification
which defines keyboard layout in external configuration file as JSON.

## Introduction

### Scope

The custom keyboard layout specification is subset of Serialized Data Format
which is known for keyboard-layout-editor.com.

This document defines extended part of above serialized data format
for fcitx5-virtualkeyboard-ui.

### Definitions

* KLE: 
  * Keyboard Layout Editor. The software is served as a web service.
* Serialized Data Format
  * https://github.com/ijprest/keyboard-layout-editor/wiki/Serialized-Data-Format

## Custom Keyboard Layout Specification

In custom keyboard layout specification, it extends the serialized data format
for KLE in the following section.

* Keyboard Metadata
* Keycap properties

### Keyboard Metadata

This is mandatory.

In metadata field, fcitx5-virtualkeyboard-ui specific metadata must be embedded.

```
[
  { /*keyboard metadata*/ },
  [ /*row 1*/ ],
  /*...*/
  [ /*row n*/ ]
]
```

There are some mandatory and optional properties.

* `Keyboard` (mandatory)
  * `label` (reserved): the label which is used for language specific keyboard instance.
  * `languageCode` (reserved): the label which is used as language code for specific keyboard instance.
  * `numberOfLayers`: the number of supported keyboard layers (text, mark and so on)
  * `layerOffset`: the offset of each layer in keyboard rows (1 origin). It must be an array.
* `ModeSwitchKey` (mandatory)
  * `numberOfStates`: the number of mode for language specific keyboard. If there are text and mark mode, it must be 2.
  * `stateLabel`: the label of each state. It must be an array of string.
  * `mode`(optional): Define actions when switching mode. It must be array of hash which contains `label`,`next` and `layer`.
     * `label`: the label which defines mode.
     * `next`; the next mode which switch to when clicking virtual key.
     * `layer`: the offset value of `layerOffset`.
* `NormalKey` (optional)
  * `width`: the default width of NormalKey. you can override
    it in keycap properties which is described later.

e.g. metadata of English keyboard layout

```
[
  {
    "// The layout definition for English keyboard": "Designed for fcitx5-virtualkeyboard-ui",
    "Keyboard": {
      "label": "US",
      "languageCode": "us",
      "numberOfLayers": 2,
      "layerOffset": [
        1,
        5
      ]
    },
    "ModeSwitchKey": {
      "numberOfStates": 2,
      "stateLabel": [
        "A",
        "#"
      ]
    },
    "NormalKey": {
      "width": 60.0
    }
  },

```

You can embed comment like `"// ...": ""` syntax.
The unknown keyword is just ignored.

### Keycap properties

This is incompatible with KLE Serialized Data Format.

The following extra properties were introduced.
Note that these extended properties were lost in KLE when uploading JSON.

* `class` (mandatory): pre-defined class of each key.
  the following keys are available:
  * `DummyKey`: The key which is used as a spacer. It does not respond to click event.
  * `LanguageSwitchKey`: The key which is used to switch virtual keyboard language.
  * `NumberKey`: The key which is used to input number.
  * `ModeSwitchKey`: The key which is used to switch mode in current virtual keyboard.
  * `MarkKey`: The key which is used to input mark key.
  * `BackSpaceKey`: The key which is used to input back space key.
  * `SpaceKey`: The key which is used to input space key.
  * `EnterKey`: The key which is used to input enter key.
  * `UpKey`: The key which is used to input up arrow key.
  * `LeftKey`: The key which is used to input left arrow key.
  * `DownKey`: The key which is used to input down arrow key.
  * `RightKey`: The key which is used to input right arrow key.
  * `ShiftToggleKey`: The key which is used to provide shift functionality.
  * `ChewingNumberKey`: The key which is used to input number in chewing.
  * `ChewingNumPadKey`: The key which is used to input numpad in chewing.
  * `ChewingEnterKey`: The key which is used to input enter in chewing.
  * `ChewingSpaceKey`: The key which is used to input space in chewing.
  * `ChewingMarkToggleKey`: The key which is used to switch mark in chewing.
  * `PinyinModeSwitchKey`: The key which is used to switch mode in pinyin.
  * `PinyinSpaceKey`: The key which is used to input space in pinyin.
  * `PinyinMarkToggleKey`: The key which is used to toggle mark mode in pinyin.
  * `AnthyInputModeSwitchKey`: The key which is used to switch input mode in anthy.
  * `AnthyTypingMethodSpaceKey`: The key which is used to switch typing mode in anthy.
  * `AnthyModeSwitch`: The key which is used to switch mode in anthy.
  * `AnthyNumpadModeSwitchKey`: The key which is used to switch numpad mode in anthy.
  * `ZenkakuHankakuKey`: The key which is used to switch Zenkaku/Hankaku mode in anthy.
  * `AnthyKanaKey`: The key which is used to input kana in anthy.
  * `AnthyKanaNumPadKey`: The key which is used to input kana numpad in anthy.
  * `AnthyMarkKey`: The key which is used to input mark in anthy.
  * `CustomModeSwitchKey`: The key which is used to switch mode in custom keyboard.
* `scale`: the scale factor of keycap width. It depends on each key's default width.
* `width`: the width of keycap.
* `code`: the key code. Specify the value when you switched the target language keyboard layout and collect the corresponding physical key code.
* `upperLabel`: the label which is pressed with shift key.
* `keyName`: the key name.

#### Key and properties

There are two kind of keys. One requires the successor label, the other is not.

It means that key and properties are constructed with `{PROPERTIES},"LABEL"` or `{PROPERTIES}`.

* `DummyKey` (no successor label)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"x": 0.5, "class": "DummyKey", "width": 20}`

* `LanguageSwitchKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "LanguageSwitchKey"},"US"`

* `NumberKey`
  * `code` (optional)
  * `width` (optional)
  * `scale` (optional)
  * `zenkakuMark` (optional)
  * `keyName` (optional)

* `ModeSwitchKey`
  * `width` (optional)
  * `scale` (optional)

* `MarkKey`
  * `width` (optional)
  * `scale` (optional)
  * `keyname` (optional)
  * `zenkakuMark` (optional)
  * `code` (optional)
  * `withShift` (optional)

  Note that above properties are optional, but currently only the following combination was supported.

  * no properties.  e.g. `{"class": "MarkKey"},"."`
  * keyname and code. e.g. `{"class": "MarkKey", "keyname": "", "code": 61},","`
  * keyname, code, withShift. e.g. `{"class": "MarkKey", "keyname": "", "code": 61, "withShift": true},","`

* `BackSpaceKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "BackSpaceKey"},"Back"`

* `SpaceKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "SpaceKey", "scale": 2.0},""`

* `EnterKey`
  * `width` (optional)
  * `scale` (optional)
  *  e.g. `{"class": "EnterKey", "scale": 1.5},"Enter"`

* `UpKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "UpKey"},"↑"`

* `LeftKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "LeftKey"},"←"`

* `DownKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "DownKey"},"↓"`

* `RightKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "RightKey"},"→"`

* `ShiftToggleKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ShiftToggleKey"},"⇧"`

* `ChewingNumberKey` (chewing)
  * `width` (optional)
  * `scale` (optional)
  * `code` (mandatory)
  * `keyName` (mandatory)
  * e.g. `{"class": "ChewingNumberKey"},"1"`

* `ChewingNumPadKey` (chewing)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ChewingNumPadKey"},"1"`

* `ChewingEnterKey` (chewing)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ChewingEnterKey"},"換行"`

* `ChewingSpaceKey` (chewing)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ChewingSpaceKey"},"空格"`

* `ChewingMarkToggleKey` (chewing)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ChewingSpaceKey"},"空格"`

* `PinyinModeSwitchKey` (pinyin)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ChewingMarkToggleKey"},"更多"`

* `PinyinSpaceKey` (pinyin)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "PinyinSpaceKey"},"空格"`

* `PinyinMarkToggleKey` (pinyin)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "PinyinMarkToggleKey"},"更多"`

* `AnthyInputModeSwitchKey` (anthy)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyInputModeSwitchKey"},"あアｱ"`

* `AnthyTypingMethodSpaceKey` (anthy)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyTypingMethodSpaceKey"},"ローマ字かな"`

* `AnthyModeSwitch` (anthy)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyModeSwitchKey"},"A#"`

* `AnthyNumpadModeSwitchKey` (anthy)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyNumpadModeSwitchKey"},"#S"`

* `ZenkakuHankakuKey` (anthy)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "ZenkakuHankakuKey"},"全角"`

* `AnthyKanaKey` (anthy)
  * `code` (mandatory)
  * `upperLabel` (mandatory)
  * `keyName` (mandatory)
  * `upperName` (mandatory)
  * `isNumberKey` (mandatory)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyKanaKey","code":10,"upperLabel":"","keyName:"1",upperName:"","isNumberKey":true},"ぬ"`

* `AnthyKanaNumPadKey` (anthy)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyKanaNumPadKey"},"1"`

* `AnthyMarkKey` (anthy)
  * `keyName` (mandatory)
  * `zenkakuMark` (mandatory)
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "AnthyMarkKey","keyName":"1","zenkakuMark":"１"},"1"`

* `CustomModeSwitchKey`
  * `width` (optional)
  * `scale` (optional)
  * e.g. `{"class": "CustomModeSwitchKey"},"A#"`

## Appendix: A How to enable custom layout in fcitx5-virtualkeyboard-ui

To enable custom layout functionality in fcitx5-virtualkeyboard-ui, the following process is required.

* Enable custom layout functionality and rebuild binary
* Set configuration for custom layout feature in virtualkeyboardui.conf

### Enable custom layout functionality and rebuild binary

To enable this feature, `ENABLE_CUSTOM_LAYOUT` must be enable.

```bash
sudo apt build-dep fcitx5
git clone https://github.com/clear-code/fcitx5-virtualkeyboard-ui.git --branch fcitx-5.0.8
cd fcitx5-virtualkeyboard-ui
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_CUSTOM_LAYOUT=ON -DENABLE_WAYLAND=ON
sudo cmake --install build
```

* when `ENABLE_CUSTOM_LAYOUT` is disabled, the traditional keyboard implementation is used. (A)
* even though `ENABLE_CUSTOM_LAYOUT` is enabled, if there is no
  correct configuration for custom keyboard layout in virtualkeyboardui.conf, as a fallback, only specific path of layout
  definition file is read.

### Set configuration for custom layout feature in virtualkeyboardui.conf

To enable fully custom layout functionality, the configuration for custom keyboard layout and profile must be set.

The virtual keyboard configuration file is installed at `/usr/share/fcitx5/virtualkeyboardui/virtualkeyboardui.conf`.

Here is the syntax of custom layout configuration.

```
[GROUP_NAME]
Name=(Keyboard Name or Input method name)
Layout=(Custom layout definition file name)
```

GROUP_NAME must be matched with section name in `~/.config/fcitx5/profile`.
Then keyboard name or input method name must be also matched with `Name` in profile.

The custom layout definition file must be installed at `${datadir}/fcitx5/virtualkeyboardui`.

Here is the example configuration for us, ru and hangul in `virtualkeyboardui.conf`.

```
[US]
Name=keyboard-us
Layout=virtualkeyboardui-us.ext.json

[RU]
Name=keyboard-ru
Layout=virtualkeyboardui-ru.ext.json

[KR]
Name=hangul
Layout=virtualkeyboardui-ko.ext.json
```

Above example assumes the configuration in profile like this.

```
[Groups/0]
Name=US
Default Layout=us
DefaultIM=keyboard-us

[Groups/0/Items/0]
Name=keyboard-us
Layout=

[Groups/1]
Name=RU
Default Layout=us
DefaultIM=keyboard-ru

[Groups/1/Items/0]
Name=keyboard-ru
Layout=

[Groups/2]
Name=KR
Default Layout=us
DefaultIM=hangul

[Groups/2/Items/0]
Name=hangul
Layout=

[GroupOrder]
0=US
1=RU
2=KR
```

## Appendix: B Kind of custom layout definition files

By default, custom layout definition files are installed under /usr/share/fcitx5/virtualkeyboardui/.

* virtualkeyboardui-ja.json
  * Custom layout file for Anthy. `ENABLE_CUSTOM_LAYOUT` must be enabled.
  * This file must not be renamed.
* virtualkeyboardui-ko.ext.json
  * Custom layout file for hangul. `ENABLE_CUSTOM_LAYOUT` must be enabled.
  * The Layout= configuration in virtualkeyboardui.conf must be set.
* virtualkeyboardui-ko.json
  * Custom layout file for hangul as a fallback when `Layout=` entry in virtualkeyboardui.conf is not set. `ENABLE_CUSTOM_LAYOUT` must be enabled. 
  * This file must not be renamed.
* virtualkeyboardui-ru.ext.json
  * Custom layout file for Russian. `ENABLE_CUSTOM_LAYOUT` must be enabled.
  * The Layout= configuration in virtualkeyboardui.conf must be set.
* virtualkeyboardui-ru.json
  * Custom layout file for Russian as a fallback when `Layout=` entry in virtualkeyboardui.conf is not set. `ENABLE_CUSTOM_LAYOUT` must be enabled. 
  * This file must not be renamed.
* virtualkeyboardui-us.ext.json
  * Custom layout file for English. `ENABLE_CUSTOM_LAYOUT` must be enabled.
  * The Layout= configuration in virtualkeyboardui.conf must be set.
* virtualkeyboardui-us.json
  * Custom layout file for English as a fallback when `Layout=` entry in virtualkeyboardui.conf is not set. `ENABLE_CUSTOM_LAYOUT` must be enabled. 
  * This file must not be renamed.
* virtualkeyboardui-zh_CN.ext.json
  * Custom layout file for Pinyin. `ENABLE_CUSTOM_LAYOUT` must be enabled. 
  * This is experimental custom layout file.
* virtualkeyboardui-zh_CN.json
  * Custom layout file for Pinyin. `ENABLE_CUSTOM_LAYOUT` must be enabled. 
  * This file must not be renamed.
* virtualkeyboardui-zh_TW.json
  * Custom layout file for Chewing. `ENABLE_CUSTOM_LAYOUT` must be enabled. 
  * This file must not be renamed.

## ChangeLog

* 2024/06/20 v1.0
