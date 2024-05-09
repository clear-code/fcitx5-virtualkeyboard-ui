/*
 * SPDX-FileCopyrightText: 2021-2021 daipom <fukuda@clear-code.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UI_CLASSIC_VIRTUALKEYBOARD_H_
#define _FCITX_UI_CLASSIC_VIRTUALKEYBOARD_H_

#include "common.h"
#include "virtualkeyboardi18n.h"
#include <cairo/cairo.h>
#include <pango/pango.h>
#include "fcitx/instance.h"
#include "fcitx/inputcontext.h"
#include "fcitx/inputmethodmanager.h"
#include "fcitx/inputmethodentry.h"
#include "fcitx/userinterfacemanager.h"
#include "fcitx/action.h"
#include "fcitx/inputpanel.h"
#include <fcitx-utils/event.h>
#include "fcitx-utils/log.h"

namespace fcitx {
namespace classicui {

class VirtualKeyboard;
class VirtualKey {
public:
    using PangoAttrListUniquePtr = UniqueCPtr<PangoAttrList, pango_attr_list_unref>;
    static constexpr double DefaultFontSize = 22;
    static void addForegroundAttr(PangoAttrList *attrList,
                                  double r, double g, double b,
                                  guint start_index = 0,
                                  guint end_index = G_MAXUINT) {
        const auto scale = std::numeric_limits<uint16_t>::max();
        auto attr = pango_attr_foreground_new(r * scale, g * scale, b * scale);
        attr->start_index = start_index;
        attr->end_index = end_index;
        pango_attr_list_insert(attrList, attr);
    }

public:
    virtual const char* label(VirtualKeyboard *keyboard) const = 0;
    virtual void click(VirtualKeyboard *keyboard, InputContext *inputContext, bool isRelease) = 0;
    virtual void paintLabel(VirtualKeyboard *keyboard, cairo_t *cr, PangoLayout *layout);
    void paintBackground(cairo_t *cr, bool highlight);

    void setRegion(int x, int y) {
        region_
            .setPosition(x, y)
            .setSize(width_, height_);
    }
    bool contains(int x, int y) const { return region_.contains(x, y); }

    void setCustomWidth(double width) {
        width_ = width;
    }

    void setCustomLayout(double scale, bool newLine = false) {
        newLine_ = newLine;
        width_ *= scale;
    }

    void setFontColor(std::tuple<double, double, double> colorRgb) {
        fontColorRgb_ = colorRgb;
    }

    void setFontSize(double size) {
        fontSize_ = size;
    }

    void setCustomBackgroundColor(std::tuple<double, double, double> colorRgb) {
        useCustomBackgroundColor_ = true;
        customBackgroundColorRgb_ = colorRgb;
    }

    double width() { return width_; }
    double height() { return height_; }
    bool newLine() { return newLine_; }
    bool visible() { return visible_; }

protected:
    virtual void applyFont(
        VirtualKeyboard *keyboard,
        PangoLayout *layout,
        PangoAttrList *attrList
    );
    virtual void fillLayout(
        VirtualKeyboard *keyboard,
        PangoLayout *layout,
        PangoAttrList *attrList
    );

    Rect region_;
    double fontSize_ = DefaultFontSize;
    std::tuple<double, double, double> fontColorRgb_ = {0.3, 0.35, 0.4};
    bool useCustomBackgroundColor_ = false;
    std::tuple<double, double, double> customBackgroundColorRgb_ = {0, 0, 0};
    double width_ = 60;
    double height_ = 50;
    bool newLine_ = false;
    bool visible_ = true;
};

class VirtualKeyboard {
public:
    VirtualKeyboard(Instance *instance, PangoContext *pangoContext);
    void paint(cairo_t *cr, unsigned int offsetX, unsigned int offsetY);
    bool click(InputContext *inputContext, int x, int y, bool isRelease);
    bool syncState();
    void setInputContext(InputContext *inputContext) {
        lastInputContext_ = inputContext->watch();
    }
    void switchLanguage();
    void setCurrentInputMethod(const std::string &name);
    void enumerateGroup();
    std::tuple<const std::string, bool> getIMActionText(
        const std::string &name,
        bool getShort
    );
    bool activateIMAction(const std::string &name);

    /// Some IMs needs to handle the single Shift key event,
    /// not only the states of the main key.
    /// Ex. In IMs using customXkbState such as keyboard-ru, some states of modifiers are not
    /// propagated.
    /// So it is not enough to process states of the main key in `WaylandIMInputContextV1`
    /// in these IMs.
    /// (See event implementation of
    /// `EventWatcherPhase::ReservedFirst` of `EventType::InputContextKeyEvent` in `Instance`.)
    void sendShiftModifierToIM(InputContext *inputContext, bool isRelease);

    bool isPreediting();
    bool isSeletingCandidates();
    void updateInputPanel();

    std::string languageCode() const { return i18nKeyboard_->languageCode(); }
    std::vector<std::unique_ptr<VirtualKey>> &keys() const { return i18nKeyboard_->keys(); }
    I18nKeyboard *i18nKeyboard() const { return i18nKeyboard_.get(); }
    template<class T>
    T *i18nKeyboard() const {
        static_assert(std::is_base_of<I18nKeyboard, T>::value,
            "type parameter of this function must derive from I18nKeyboard");
        return static_cast<T *>(i18nKeyboard_.get());
    }

    std::pair<unsigned int, unsigned int> size();
    unsigned int marginX() { return 15; }
    unsigned int marginY() { return 6; }
    VirtualKey *pushingKey() { return pushingKey_; }
    bool isShiftOn() { return isShiftOn_; }
    void toggleShift() { isShiftOn_ = !isShiftOn_; }

    const PangoFontDescription *getFontDesc(double fontSize) {
        int size = fontSize * PANGO_SCALE;
        auto context = pango_layout_get_context(pangoLayout_.get());
        if (fontDescMap_.find(size) == fontDescMap_.end()) {
            auto origFontDesc = pango_context_get_font_description(context);
            auto fontDesc = pango_font_description_copy(origFontDesc);
            pango_font_description_set_absolute_size(fontDesc, size);
            fontDescMap_[size].reset(fontDesc);
        }
        return fontDescMap_[size].get();
    }

private:
    Instance *instance_;
    GObjectUniquePtr<PangoLayout> pangoLayout_;
    std::map<int, UniqueCPtr<PangoFontDescription, pango_font_description_free>> fontDescMap_;
    TrackableObjectReference<InputContext> lastInputContext_;

    std::tuple<VirtualKey *, bool> findClickedKey(int x, int y);
    void paintBackground(cairo_t *cr);
    void setI18nKeyboard(I18nKeyboard *i18nKeyboard);

    VirtualKey *pushingKey_ = nullptr;
    bool isShiftOn_ = false;
    std::unique_ptr<I18nKeyboard> i18nKeyboard_;
    I18nKeyboardSelector i18nKeyboardSelector_;
};

} // namespace classicui
} // namespace fcitx

FCITX_DECLARE_LOG_CATEGORY(keyboard);

#define FCITX_KEYBOARD() FCITX_LOGC(::keyboard, Debug)

#endif // _FCITX_UI_CLASSIC_VIRTUALKEYBOARD_H_
