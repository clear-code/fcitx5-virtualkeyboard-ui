# カスタムキーボードレイアウト仕様 v1.0

## 概要

従来C++のコードとしてハードコーディングされていた仮想キーボードのUIを、ソースコードを修正することなくカスタマイズできるようにするための仕様を定義したものである。

従来の方式では、さまざまな仮想キーボードを実装するにあたって言語ごとにC++で実装する必要があった。

本仕様では、仮想キーボードのレイアウトを外部の設定ファイルとして定義する方法を示す。

## はじめに

### 本文書の対象範囲

本カスタムキーボードレイアウト仕様は、[Keyboard Layout Editor](https://www.keyboard-layout-editor.com)
で採用されている[Serialized Data Format](https://github.com/ijprest/keyboard-layout-editor/wiki/Serialized-Data-Format)を拡張したものある。

fcitx5-virtualkeyboard-ui向けに本文書ではSerialized Data Formatをどのように拡張したかを示す。

### 用語の定義

* KLE:
  * Keyboard Layout Editorの略。KLEはウェブサービスとして運用されているが、ソースコード自体が公開されているので自前で動作させることも可能である。
* Serialized Data Format
  * https://github.com/ijprest/keyboard-layout-editor/wiki/Serialized-Data-Format

## カスタムキーボードレイアウト仕様

本カスタムキーボードレイアウト仕様では、KLE向けのSerialized Data Formatを拡張した。

KLEのフォーマットは次のように、汎用的に利用可能なメタデータ定義部分と、行単位のキー定義部分から構成されるJSON形式となっている。


```
[
  { /*keyboard metadata*/ },
  [ /*row 1*/ ],
  /*...*/
  [ /*row n*/ ]
]
```

カスタムキーボードレイアウト仕様では、次の2点について拡張した。

* キーボードメタデータ
* キーキャッププロパティ

### キーボードメタデータ

キーボードメタデータはKLEフォーマットの先頭で定義される、汎用のメタデータ定義領域である。

カスタムキーボードレイアウト仕様では、後続のキー定義をどのように利用するかを定義するために必須である。

このメタデータないし行単位のキー定義に不備があると仮想キーボードのキーが表示されない。

キーボードメタデータは必須の項目と、オプションである次の項目から構成される。

* `Keyboard` (必須)
  * `label` (必須): キーボードのインスタンス特有のラベルを指定する。(例: `US`)
  * `languageCode` (必須): キーボードの言語コードを指定する。(例: `us`)
  * `numberOfLayers`(必須): キーボードで利用可能なレイヤーの数を指定する。レイヤーとは打鍵可能なキーセットを意味する。(例: 英数字を打鍵するためのレイヤー、記号を打鍵するためのレイヤーなど)
  * `layerOffset`(必須): 各レイヤーの行単位のキー定義部分が開始される位置を示すオフセット値の配列。値は1から始まる。(0はキーボードのメタデータを示す) これに対応する行単位のキー定義が存在しなければならない。
* `ModeSwitchKey` (必須)
  * `numberOfStates`(必須): キーボードが扱える入力モード数を指定する。(例: 英数字を入力するモード、記号を入力するモードが扱えるなら値は2となる。)
  * `stateLabel`(必須): モード切替キーの状態を示すラベルを指定する。値は文字列の配列。
  * `mode`(オプション): モードの切替時の挙動を定義する。各モードを示す`label`,`next`,`layer`をキーにもつハッシュの配列。
     * `label`: モードを区別するためのラベルを指定する。
     * `next`; キーをクリックしたとき次にどのモードに切り替えるかを指定する。
     * `layer`: モードに対応した`layerOffset`のオフセット値を指定する。
* `NormalKey` (オプション)
  * `width`: キーの既定幅を指定する。後述する個別のキーのプロパティとして指定することで上書き設定可能である。
  * `useLabelAsKeyName`: ラベルをキー名として利用するか否かを指定する。既定値は`false`。
英語キーボードのメタ情報の指定例は次のとおりである。

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

慣例として、カスタムレイアウト定義にコメントを埋め込む際は、`"// ....": "",`形式にしている。
サポートしているキーワードに該当しない場合は無視するようになっているためである。

### キーキャッププロパティ

キーキャッププロパティは、個別のキーの属性を定義するものである。
KLEでは、物理キーとその配置に関する属性は仕様として定義されているが、各キーの挙動という概念はないためそれらを定義できない。

そこで本仕様では、各キーの挙動を定義できるように拡張した。
ただし、拡張したプロパティをKLEで読み込ませると失われることに注意が必要である。

キーの定義はキーキャッププロパティとラベルから構成される。

書式としては、`{PROPERTIES},"LABEL"` もしくは `{PROPERTIES}`の形式である。

例えば、`{"class": "NormalKey"},"a"`は`{"class": "NormalKey"}`というキーキャッププロパティとラベル"a"から構成されるキーを示す。

拡張したキーキャッププロパティと利用できる値は次のとおりである。

* `class` (必須): 各キーの種別を指定する。次のキーが利用可能である。
  * `DummyKey`: スペーサーとして利用するキー。クリックイベントには応答しない。
  * `LanguageSwitchKey`: キーボードの言語を切り替えるためのキーとして指定する。
  * `NormalKey`: 英字等を入力するためのキーとして指定する。
  * `NumberKey`: 数字を入力するためのキーとして指定する。
  * `ModeSwitchKey`: モードを切り替えるためのキーとして指定する。
  * `MarkKey`: 記号類を入力するためのキーとして指定する。
  * `BackSpaceKey`: バックスペースを入力するためのキーとして指定する。
  * `SpaceKey`: スペースを入力するためのキーとして指定する。
  * `EnterKey`: エンターキーを入力するためのキーとして指定する。
  * `UpKey`: 上矢印のカーソルキーとして指定する。
  * `LeftKey`: 左矢印のカーソルキーとして指定する。
  * `DownKey`: 下矢印のカーソルキーとして指定する。
  * `RightKey`: 右矢印のカーソルキーとして指定する。
  * `ShiftToggleKey`: シフト機能を提供するキーとして指定する。
  * `ChewingNumberKey`: 繁体字専用の数字を入力するためのキーとして指定する。
  * `ChewingNumPadKey`: 繁体字専用の数字パッドで入力するためのキーとして指定する。
  * `ChewingEnterKey`: 繁体字専用のエンターを入力するためのキーとして指定する。
  * `ChewingSpaceKey`: 繁体字専用のスペースを入力するためのキーとして指定する。
  * `ChewingMarkToggleKey`: 繁体字専用の記号の入力モードの切り替えキーとして指定する。
  * `PinyinModeSwitchKey`: 簡体字専用の入力モード切り替えキーとして指定する。
  * `PinyinSpaceKey`: 簡体字専用のスペースキーとして指定する。
  * `PinyinMarkToggleKey`: 簡体字専用の記号入力モード切り替えキーとして指定する。
  * `AnthyInputModeSwitchKey`: Anthyの入力モード切り替えキーとして指定する。
  * `AnthyTypingMethodSpaceKey`: Anthyのタイプ切り替えキーとして指定する。
  * `AnthyModeSwitch`: Anthyのモード切り替えキーとして指定する。
  * `AnthyNumpadModeSwitchKey`: Anthyの数字パッド切り替えキーとして指定する。
  * `ZenkakuHankakuKey`: Anthyの全角半角キーとして指定する。
  * `AnthyKanaKey`: Anthyのかなキーとして指定する。
  * `AnthyKanaNumPadKey`: Anthyのかなパッドキーとして指定する。
  * `AnthyMarkKey`: Anthyの記号キーとして指定する。
  * `CustomModeSwitchKey`: モードを切り替えるための汎用キーとして指定する。C++で実装された特定の言語のキーボード向けのロジックを必要としない場合にのみ適用できる。(対応している英語、ロシア語、ハングルでのみ利用できる)
* `scale`: キーキャップの幅の表示倍率を指定する。実際の幅はキー固有の既定幅に依存する。(`width`と併用可能で、その場合は`width`の`scale`倍が適用される。)
* `width`: キーキャップの幅を指定する。
* `code`: キーコードを指定する。
* `upperLabel`: シフトキーを押した状態で入力される文字を指定する。
* `keyName`: キー名を指定する。

#### キーとプロパティの詳細

すでに述べたようにキー定義は `{PROPERTIES},"LABEL"` もしくは `{PROPERTIES}`から構成される。

後続のラベルを必要とするものと、必要しないものとがあるが、プロパティのみで完結する場合は、後続のラベルが不要となる。
例えば、レイアウト調整のために配置される`DummyKey`はキートップに何も表示する必要がないスペーサーとして機能するため、後続のラベルは不要である。

各キーと、利用可能な拡張プロパティの例を示す。

* `DummyKey` (後続ラベル不要)
  * `width` (オプション)
    * 例: `{"x": 0.5, "class": "DummyKey", "width": 20}`
  * `scale` (オプション)

* `LanguageSwitchKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "LanguageSwitchKey"},"JP"`

* `NumberKey`: 
  * `code` (オプション):
    * 例: `{"class": "NumberKey", "code": 10},"1"`
  * `width` (オプション)
  * `scale` (オプション)
  * `zenkakuMark` (オプション)
  * `keyName` (オプション)

* `ModeSwitchKey`
  * `width` (オプション)
  * `scale` (オプション)

* `MarkKey`
  * `width` (オプション)
  * `scale` (オプション)
  * `keyname` (オプション)
  * `zenkakuMark` (オプション)
  * `code` (オプション)
  * `withShift` (オプション)

  使用例は次のとおり。

  * プロパティなし 例: `{"class": "MarkKey"},"."`
  * キー名とキーコード指定 例: `{"class": "MarkKey", "keyname": "", "code": 61},","`
  * キー名、キーコード、keyname, code, withShift. e.g. `{"class": "MarkKey", "keyname": "", "code": 61, "withShift": true},","`

* `BackSpaceKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "BackSpaceKey"},"Back"`

* `SpaceKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "SpaceKey", "scale": 2.0},""`

* `EnterKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "EnterKey", "scale": 1.5},"Enter"`

* `UpKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "UpKey"},"↑"`

* `LeftKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "LeftKey"},"←"`

* `DownKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "DownKey"},"↓"`

* `RightKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "RightKey"},"→"`

* `ShiftToggleKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ShiftToggleKey"},"⇧"`

* `ChewingNumberKey` (繁体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * `code` (必須)
  * `keyName` (必須)
  * 例: `{"class": "ChewingNumberKey"},"1"`

* `ChewingNumPadKey` (繁体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ChewingNumPadKey"},"1"`

* `ChewingEnterKey` (繁体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ChewingEnterKey"},"換行"`

* `ChewingSpaceKey` (繁体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ChewingSpaceKey"},"空格"`

* `ChewingMarkToggleKey` (繁体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ChewingSpaceKey"},"空格"`

* `PinyinModeSwitchKey` (簡体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ChewingMarkToggleKey"},"更多"`

* `PinyinSpaceKey` (簡体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "PinyinSpaceKey"},"空格"`

* `PinyinMarkToggleKey` (簡体字専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "PinyinMarkToggleKey"},"更多"`

* `AnthyInputModeSwitchKey` (Anthy専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyInputModeSwitchKey"},"あアｱ"`

* `AnthyTypingMethodSpaceKey` (Anthy専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyTypingMethodSpaceKey"},"ローマ字かな"`

* `AnthyModeSwitch` (Anthy専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyModeSwitchKey"},"A#"`

* `AnthyNumpadModeSwitchKey` (Anthy専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyNumpadModeSwitchKey"},"#S"`

* `ZenkakuHankakuKey` (Anthy専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "ZenkakuHankakuKey"},"全角"`

* `AnthyKanaKey` (Anthy専用)
  * `code` (必須)
  * `upperLabel` (必須)
  * `keyName` (必須)
  * `upperName` (必須)
  * `isNumberKey` (必須)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyKanaKey","code":10,"upperLabel":"","keyName:"1",upperName:"","isNumberKey":true},"ぬ"`

* `AnthyKanaNumPadKey` (Anthy専用)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyKanaNumPadKey"},"1"`

* `AnthyMarkKey` (Anthy専用)
  * `keyName` (必須)
  * `zenkakuMark` (必須)
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "AnthyMarkKey","keyName":"1","zenkakuMark":"１"},"1"`

* `CustomModeSwitchKey`
  * `width` (オプション)
  * `scale` (オプション)
  * 例: `{"class": "CustomModeSwitchKey"},"A#"`

## 付録A: fcitx5-virtualkeyboard-uiへの適用

fcitx5-virtualkeyboard-uiへカスタムレイアウト機能を適用するにあたっては
以下の手順の適用が必要である。

* カスタムレイアウト機能を有効にしたバイナリをビルドする
* 仮想キーボードの設定を有効にする

### カスタムレイアウト機能を有効にしたバイナリをビルドする

カスタムレイアウト機能を有効にするためには、次のように`ENABLE_CUSTOM_LAYOUT`を有効にしてインストールする必要がある。

```bash
sudo apt build-dep fcitx5
git clone https://github.com/clear-code/fcitx5-virtualkeyboard-ui.git --branch fcitx-5.0.8
cd fcitx5-virtualkeyboard-ui
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_CUSTOM_LAYOUT=ON -DENABLE_WAYLAND=ON
sudo cmake --install build
```

* `ENABLE_CUSTOM_LAYOUT`が無効な場合、従来どおりC++で実装された言語ごとのキーボード(A)が有効となる。
* `ENABLE_CUSTOM_LAYOUT`が有効であっても、後述する仮想キーボードの設定がない場合、レイアウト情報を一部設定ファイル(パスは固定)から読み込むようにしたキーボード(B)が言語ごとに有効となる。(A)と(B)はレイアウト情報を一部設定ファイルとして抽出したのみであり、対応言語が固定(us,jp,ru,hangul,zh_CN,zh_TW)である点を除いて仮想キーボードとしての機能としては同等である。

### 仮想キーボードの設定を有効にする

仮想キーボードのカスタムレイアウト機能を有効にするには、特定の言語のキーボードに対応するレイアウトを関連付けるための設定が必要である。

仮想キーボードの設定ファイルは `/usr/share/fcitx5/virtualkeyboardui/virtualkeyboardui.conf` である。
(異なるパスにインストールしている場合は適宜読み替えること)

カスタムレイアウトを関連付けるための書式は次のとおりである。

```
[グループ名]
Name=(キーボード名もしくは入力メソッド名)
Layout=(カスタムレイアウト定義ファイル名)
```

グループ名は、`~/.config/fcitx5/profile`に定義されているセクション名と一致しなければならない。
また、キーボード名もしくは入力メソッド名も、プロファイルに定義されている`Name`と一致する必要がある。

カスタムレイアウト定義ファイルのパスは `${datadir}/fcitx5/virtualkeyboardui` 配下にあるものとする。
(例: `/usr/share/fcitx5/virtualkeyboardui/`)

仮想キーボードのカスタムレイアウト機能に対応している英語、ロシア語、ハングルと入力メソッドを
関連付けるための`virtualkeyboardui.conf`の設定例を次に示す。

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

上記は、プロファイルに対応する次のような定義があることを想定している。

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

## 付録B: レイアウト設定定義ファイルの種別

既定では/usr/share/fcitx5/virtualkeyboardui/配下にインストールされるレイアウト定義ファイルは次のとおりである。

* virtualkeyboardui-ja.json
  * Anthy向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * ファイル名を変更するとレイアウト定義を適用できなくなる。
* virtualkeyboardui-ko.ext.json
  * hangul向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * 仮想キーボードで対応するLayout=の設定あると適用される。
* virtualkeyboardui-ko.json
  * hangul向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * 仮想キーボードの設定をしていない場合にフォールバック先として用いられる。
  * ファイル名を変更するとレイアウト定義を適用できなくなる。
* virtualkeyboardui-ru.ext.json
  * ロシア語向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * 仮想キーボードで対応するLayout=の設定あると適用される。
* virtualkeyboardui-ru.json
  * ロシア語向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * 仮想キーボードの設定をしていない場合にフォールバック先として用いられる。
  * ファイル名を変更するとレイアウト定義を適用できなくなる。
* virtualkeyboardui-us.ext.json
  * 英語向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * 仮想キーボードで対応するLayout=の設定あると適用される。
* virtualkeyboardui-us.json
  * 英語向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * 仮想キーボードの設定をしていない場合にフォールバック先として用いられる。
  * ファイル名を変更するとレイアウト定義を適用できなくなる。
* virtualkeyboardui-zh_CN.ext.json
  * 簡体字(pinyin)向けのレイアウト定義ファイル。
  * 将来的に仮想キーボードの設定ファイルとして利用できるようにすることを想定した実験的なファイル。
* virtualkeyboardui-zh_CN.json
  * 簡体字(pinyin)向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * ファイル名を変更するとレイアウト定義を適用できなくなる。
* virtualkeyboardui-zh_TW.json
  * 繁体字(chewing)向けのレイアウト定義ファイル。ENABLE_CUSTOM_LAYOUTを定義している場合に適用される。
  * ファイル名を変更するとレイアウト定義を適用できなくなる。


## 変更履歴

* 2024/06/20 v1.0 初版
