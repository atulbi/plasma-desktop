/*
 * Copyright 2017 Roman Gilg <subdiff@gmail.com>
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

#ifndef KWINWAYLANDTOUCHPAD_H
#define KWINWAYLANDTOUCHPAD_H

#include <backends/libinputcommon.h>
#include <QString>

class QDBusInterface;

class KWinWaylandTouchpad : public QObject, public LibinputCommon
{
    Q_OBJECT

public:
    KWinWaylandTouchpad(QString dbusName);
    ~KWinWaylandTouchpad() override;

    bool init();

    bool getConfig();
    bool getDefaultConfig();
    bool applyConfig();
    bool isChangedConfig() const;

    //
    // general
    QString name() const override {
        return m_name.val;
    }
    QString sysName() const {
        return m_sysName.val;
    }
    bool supportsDisableEvents() const override {
        return m_supportsDisableEvents.val;
    }
    void setEnabled(bool enabled) override {
        m_enabled.set(enabled);
    }
    bool isEnabled() const override {
        return m_enabled.val;
    }
    //
    // advanced
    bool supportsLeftHanded() const override {
        return m_supportsLeftHanded.val;
    }
    bool supportsDisableEventsOnExternalMouse() const override {
        return m_supportsDisableEventsOnExternalMouse.val;
    }
    bool supportsDisableWhileTyping() const override {
        return m_supportsDisableWhileTyping.val;
    }
    bool supportsMiddleEmulation() const override {
        return m_supportsMiddleEmulation.val;
    }
    //
    // acceleration speed and profile
    bool supportsPointerAcceleration() const override {
        return m_supportsPointerAcceleration.val;
    }
    bool supportsPointerAccelerationProfileFlat() const override {
        return m_supportsPointerAccelerationProfileFlat.val;
    }
    bool supportsPointerAccelerationProfileAdaptive() const override {
        return m_supportsPointerAccelerationProfileAdaptive.val;
    }
    //
    // scrolling
    bool supportsNaturalScroll() const override {
        return m_supportsNaturalScroll.val;
    }
    bool supportsScrollTwoFinger() const override {
        return m_supportsScrollTwoFinger.val;
    }
    bool supportsScrollEdge() const override {
        return m_supportsScrollEdge.val;
    }
    bool supportsScrollOnButtonDown() const override {
        return m_supportsScrollOnButtonDown.val;
    }
    //
    // Click method
    bool supportsClickMethodAreas() const override {
        return m_supportsClickMethodAreas.val;
    }
    bool supportsClickMethodClickfinger() const override {
        return m_supportsClickMethodClickfinger.val;
    }

private:

    template<typename T>
    bool valueLoader(Prop<T> &prop);

    template<typename T>
    QString valueWriter(const Prop<T> &prop);
    //
    // general
    Prop<QString> m_name = Prop<QString>("name");
    Prop<QString> m_sysName = Prop<QString>("sysName");
//    Prop<bool> m_supportsDisableEvents = Prop<bool>("supportsDisableEvents");
//    Prop<bool> m_enabled = Prop<bool>("enabled");

    //
    // advanced
//    Prop<Qt::MouseButtons> m_supportedButtons  = Prop<Qt::MouseButtons>("supportedButtons");

    Prop<bool> m_supportsLeftHanded = Prop<bool>("supportsLeftHanded");
//    Prop<bool> m_leftHandedEnabledByDefault = Prop<bool>("leftHandedEnabledByDefault");
//    Prop<bool> m_leftHanded = Prop<bool>("leftHanded");

//    Prop<bool> m_supportsDisableEventsOnExternalMouse = Prop<bool>("supportsDisableEventsOnExternalMouse");

    Prop<bool> m_supportsDisableWhileTyping = Prop<bool>("supportsDisableWhileTyping");
//    Prop<bool> m_disableWhileTypingEnabledByDefault = Prop<bool>("disableWhileTypingEnabledByDefault");
//    Prop<bool> m_disableWhileTyping = Prop<bool>("disableWhileTyping");

    Prop<bool> m_supportsMiddleEmulation = Prop<bool>("supportsMiddleEmulation");
//    Prop<bool> m_middleEmulationEnabledByDefault = Prop<bool>("middleEmulationEnabledByDefault");
//    Prop<bool> m_middleEmulation = Prop<bool>("middleEmulation");

    //
    // acceleration speed and profile
    Prop<bool> m_supportsPointerAcceleration = Prop<bool>("supportsPointerAcceleration");
//    Prop<qreal> m_defaultPointerAcceleration = Prop<qreal>("defaultPointerAcceleration");
//    Prop<qreal> m_pointerAcceleration = Prop<qreal>("pointerAcceleration");

//    Prop<bool> m_supportsPointerAccelerationProfileFlat = Prop<bool>("supportsPointerAccelerationProfileFlat");
//    Prop<bool> m_defaultPointerAccelerationProfileFlat = Prop<bool>("defaultPointerAccelerationProfileFlat");
//    Prop<bool> m_pointerAccelerationProfileFlat = Prop<bool>("pointerAccelerationProfileFlat");

//    Prop<bool> m_supportsPointerAccelerationProfileAdaptive = Prop<bool>("supportsPointerAccelerationProfileAdaptive");
//    Prop<bool> m_defaultPointerAccelerationProfileAdaptive = Prop<bool>("defaultPointerAccelerationProfileAdaptive");
//    Prop<bool> m_pointerAccelerationProfileAdaptive = Prop<bool>("pointerAccelerationProfileAdaptive");

    //
    // tapping
//    Prop<int> m_tapFingerCount = Prop<int>("tapFingerCount");
//    Prop<bool> m_tapToClickEnabledByDefault = Prop<bool>("tapToClickEnabledByDefault");
//    Prop<bool> m_tapToClick = Prop<bool>("tapToClick");

//    Prop<bool> m_lmrTapButtonMapEnabledByDefault = Prop<bool>("lmrTapButtonMapEnabledByDefault");
//    Prop<bool> m_lmrTapButtonMap = Prop<bool>("lmrTapButtonMap");

//    Prop<bool> m_tapAndDragEnabledByDefault = Prop<bool>("tapAndDragEnabledByDefault");
//    Prop<bool> m_tapAndDrag = Prop<bool>("tapAndDrag");
//    Prop<bool> m_tapDragLockEnabledByDefault = Prop<bool>("tapDragLockEnabledByDefault");
//    Prop<bool> m_tapDragLock = Prop<bool>("tapDragLock");

    //
    // scrolling
    Prop<bool> m_supportsNaturalScroll = Prop<bool>("supportsNaturalScroll");
//    Prop<bool> m_naturalScrollEnabledByDefault = Prop<bool>("naturalScrollEnabledByDefault");
//    Prop<bool> m_naturalScroll = Prop<bool>("naturalScroll");

//    Prop<bool> m_supportsScrollTwoFinger = Prop<bool>("supportsScrollTwoFinger");
//    Prop<bool> m_scrollTwoFingerEnabledByDefault = Prop<bool>("scrollTwoFingerEnabledByDefault");
//    Prop<bool> m_isScrollTwoFinger = Prop<bool>("scrollTwoFinger");

//    Prop<bool> m_supportsScrollEdge = Prop<bool>("supportsScrollEdge");
//    Prop<bool> m_scrollEdgeEnabledByDefault = Prop<bool>("scrollEdgeEnabledByDefault");
//    Prop<bool> m_isScrollEdge = Prop<bool>("scrollEdge");

//    Prop<bool> m_supportsScrollOnButtonDown = Prop<bool>("supportsScrollOnButtonDown");
//    Prop<bool> m_scrollOnButtonDownEnabledByDefault = Prop<bool>("scrollOnButtonDownEnabledByDefault");
//    Prop<bool> m_isScrollOnButtonDown = Prop<bool>("scrollOnButtonDown");

//    Prop<quint32> m_defaultScrollButton = Prop<quint32>("defaultScrollButton");
//    Prop<quint32> m_scrollButton = Prop<quint32>("scrollButton");

//    Prop<bool> m_supportsClickMethodAreas = Prop<bool>("supportsClickMethodAreas");
//    Prop<bool> m_defaultClickMethodAreas = Prop<bool>("defaultClickMethodAreas");
//    Prop<bool> m_clickMethodAreas = Prop<bool>("clickMethodAreas");

//    Prop<bool> m_supportsClickMethodClickfinger = Prop<bool>("supportsClickMethodClickfinger");
//    Prop<bool> m_defaultClickMethodClickfinger = Prop<bool>("defaultClickMethodClickfinger");
//    Prop<bool> m_clickMethodClickfinger = Prop<bool>("clickMethodClickfinger");

    QDBusInterface *m_iface;
};

#endif
