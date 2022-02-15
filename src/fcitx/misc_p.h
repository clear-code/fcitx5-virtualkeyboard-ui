/*
 *
 * SPDX-FileCopyrightText: 2016-2020 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_UTILS_MISC_P_H_
#define _FCITX_UTILS_MISC_P_H_

#include <string>
#include "../fcitx-utils/misc_p.h"
#include <fcitx/candidatelist.h>
#include <fcitx/inputmethodentry.h>
#include <fcitx/inputmethodmanager.h>
#include <fcitx/instance.h>

namespace fcitx {

static inline const fcitx::CandidateWord *
nthCandidateIgnorePlaceholder(const CandidateList &candidateList, int idx) {
    int total = 0;
    if (idx < 0 || idx >= candidateList.size()) {
        return nullptr;
    }
    for (int i = 0, e = candidateList.size(); i < e; i++) {
        const auto &candidate = candidateList.candidate(i);
        if (candidate.isPlaceHolder()) {
            continue;
        }
        if (idx == total) {
            return &candidate;
        }
        ++total;
    }
    return nullptr;
}

static inline bool hasTwoKeyboardInCurrentGroup(Instance *instance) {
    size_t numOfKeyboard = 0;
    for (const auto &item :
         instance->inputMethodManager().currentGroup().inputMethodList()) {
        if (auto entry = instance->inputMethodManager().entry(item.name());
            entry && entry->isKeyboard()) {
            ++numOfKeyboard;
        }
        if (numOfKeyboard >= 2) {
            return true;
        }
    }

    std::unordered_set<std::string> groupLayouts;
    for (const auto &groupName : instance->inputMethodManager().groups()) {
        if (auto group = instance->inputMethodManager().group(groupName)) {
            groupLayouts.insert(group->defaultLayout());
        }
        if (groupLayouts.size() >= 2) {
            return true;
        }
    }
    return false;
}

}

#endif // _FCITX_UTILS_MISC_P_H_
