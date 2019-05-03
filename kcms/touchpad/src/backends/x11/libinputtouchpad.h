/*
 * Copyright (C) 2015 Weng Xuetian <wengxt@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef LIBINPUTTOUCHPAD_H
#define LIBINPUTTOUCHPAD_H

#include <QObject>

#include "xlibtouchpad.h"
#include "backends/libinputcommon.h"

class LibinputTouchpad : public LibinputCommon, public XlibTouchpad
{
    Q_OBJECT

public:
    LibinputTouchpad(Display *display, int deviceId);
    ~LibinputTouchpad() override {}

    bool getConfig() override;
    bool applyConfig() override;
    bool getDefaultConfig() override;
    bool isChangedConfig() override;

private:

    template<typename T>
    bool valueLoader(Prop<T> &prop);

    template<typename T>
    QString valueWriter(const Prop<T> &prop);

    //
    // general
    QString name() const override {
        return m_name;
    }
    bool supportsDisableEvents() const override {
        return m_supportsDisableEvents.avail ? m_supportsDisableEvents.val : false;
    }
    bool isEnabled() const override {
        return !m_enabled.val;
    }
    void setEnabled(bool set) override {
        m_enabled.set(!set);
    }
    //
    // advanced
    bool supportsLeftHanded() const override {
        return m_leftHanded.avail;
    }
    bool supportsDisableEventsOnExternalMouse() const override {
        return m_supportsDisableEventsOnExternalMouse.avail ? m_supportsDisableEventsOnExternalMouse.val : false;
    }
    bool supportsDisableWhileTyping() const override {
        return m_disableWhileTyping.avail;
    }
    bool supportsMiddleEmulation() const override {
        return m_middleEmulation.avail;
    }
    //
    // acceleration speed and profile
    bool supportsPointerAcceleration() const override {
        return m_pointerAcceleration.avail;
    }
    bool supportsPointerAccelerationProfileFlat() const override {
        return m_supportsPointerAccelerationProfileFlat.avail ? m_supportsPointerAccelerationProfileFlat.val : false;
    }
    bool supportsPointerAccelerationProfileAdaptive() const override {
        return m_supportsPointerAccelerationProfileAdaptive.avail ? m_supportsPointerAccelerationProfileAdaptive.val : false;
    }
    //
    // scrolling
    bool supportsNaturalScroll() const override {
        return m_naturalScroll.avail;
    }
    bool supportsHorizontalScrolling() const override {
        return true;
    }
    bool supportsScrollTwoFinger() const override {
        return m_supportsScrollTwoFinger.avail ? m_supportsScrollTwoFinger.val : false;
    }
    bool supportsScrollEdge() const override {
        return m_supportsScrollEdge.avail ? m_supportsScrollEdge.val : false;
    }
    bool supportsScrollOnButtonDown() const override {
        return m_supportsScrollOnButtonDown.avail ? m_supportsScrollOnButtonDown.val : false;
    }
    //
    // click method
    bool supportsClickMethodAreas() const override {
        return m_supportsClickMethodAreas.avail ? m_supportsClickMethodAreas.val : false;
    }
    bool supportsClickMethodClickfinger() const override {
        return m_supportsClickMethodClickfinger.avail ? m_supportsClickMethodClickfinger.val : false;
    }

private:
    // general
    Prop<bool> m_enabledDefault = Prop<bool>("enabledDefault");

    //
    // advanced
    Prop<bool> m_disableEventsOnExternalMouse = Prop<bool>("disableEventsOnExternalMouse");
    Prop<bool> m_disableEventsOnExternalMouseDefault = Prop<bool>("disableEventsOnExternalMouseDefault");

    QString m_name;
};

#endif // LIBINPUTTOUCHPAD_H
