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

#include "xlibtouchpad.h"

class LibinputTouchpad : public XlibTouchpad
{
    Q_OBJECT
    //
    // general
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool supportsDisableEvents READ supportsDisableEvents CONSTANT)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    //
    // advanced
    Q_PROPERTY(Qt::MouseButtons supportedButtons READ supportedButtons CONSTANT)

    Q_PROPERTY(bool supportsLeftHanded READ supportsLeftHanded CONSTANT)
    Q_PROPERTY(bool leftHandedEnabledByDefault READ leftHandedEnabledByDefault CONSTANT)
    Q_PROPERTY(bool leftHanded READ isLeftHanded WRITE setLeftHanded NOTIFY leftHandedChanged)

    Q_PROPERTY(bool supportsDisableEventsOnExternalMouse READ supportsDisableEventsOnExternalMouse CONSTANT)

    Q_PROPERTY(bool supportsDisableWhileTyping READ supportsDisableWhileTyping CONSTANT)
    Q_PROPERTY(bool disableWhileTypingEnabledByDefault READ disableWhileTypingEnabledByDefault CONSTANT)
    Q_PROPERTY(bool disableWhileTyping READ isDisableWhileTyping WRITE setDisableWhileTyping NOTIFY disableWhileTypingChanged)

    Q_PROPERTY(bool supportsMiddleEmulation READ supportsMiddleEmulation CONSTANT)
    Q_PROPERTY(bool middleEmulationEnabledByDefault READ middleEmulationEnabledByDefault CONSTANT)
    Q_PROPERTY(bool middleEmulation READ isMiddleEmulation WRITE setMiddleEmulation NOTIFY middleEmulationChanged)

    //
    // acceleration speed and profile
    Q_PROPERTY(bool supportsPointerAcceleration READ supportsPointerAcceleration CONSTANT)
    Q_PROPERTY(qreal pointerAcceleration READ pointerAcceleration WRITE setPointerAcceleration NOTIFY pointerAccelerationChanged)

    Q_PROPERTY(bool supportsPointerAccelerationProfileFlat READ supportsPointerAccelerationProfileFlat CONSTANT)
    Q_PROPERTY(bool defaultPointerAccelerationProfileFlat READ defaultPointerAccelerationProfileFlat CONSTANT)
    Q_PROPERTY(bool pointerAccelerationProfileFlat READ pointerAccelerationProfileFlat WRITE setPointerAccelerationProfileFlat NOTIFY pointerAccelerationProfileChanged)

    Q_PROPERTY(bool supportsPointerAccelerationProfileAdaptive READ supportsPointerAccelerationProfileAdaptive CONSTANT)
    Q_PROPERTY(bool defaultPointerAccelerationProfileAdaptive READ defaultPointerAccelerationProfileAdaptive CONSTANT)
    Q_PROPERTY(bool pointerAccelerationProfileAdaptive READ pointerAccelerationProfileAdaptive WRITE setPointerAccelerationProfileAdaptive NOTIFY pointerAccelerationProfileChanged)

    //
    // scrolling
    Q_PROPERTY(bool supportsNaturalScroll READ supportsNaturalScroll CONSTANT)
    Q_PROPERTY(bool naturalScrollEnabledByDefault READ naturalScrollEnabledByDefault CONSTANT)
    Q_PROPERTY(bool naturalScroll READ isNaturalScroll WRITE setNaturalScroll NOTIFY naturalScrollChanged)

    Q_PROPERTY(bool supportsScrollTwoFinger READ supportsScrollTwoFinger CONSTANT)
    Q_PROPERTY(bool scrollTwoFingerEnabledByDefault READ scrollTwoFingerEnabledByDefault CONSTANT)
    Q_PROPERTY(bool scrollTwoFinger READ isScrollTwoFinger WRITE setScrollTwoFinger NOTIFY scrollMethodChanged)

    Q_PROPERTY(bool supportsScrollEdge READ supportsScrollEdge CONSTANT)
    Q_PROPERTY(bool scrollEdgeEnabledByDefault READ scrollEdgeEnabledByDefault CONSTANT)
    Q_PROPERTY(bool scrollEdge READ isScrollEdge WRITE setScrollEdge NOTIFY scrollMethodChanged)

    Q_PROPERTY(bool supportsScrollOnButtonDown READ supportsScrollOnButtonDown CONSTANT)
    Q_PROPERTY(bool scrollOnButtonDownEnabledByDefault READ scrollOnButtonDownEnabledByDefault CONSTANT)
    Q_PROPERTY(bool scrollOnButtonDown READ isScrollOnButtonDown WRITE setScrollOnButtonDown NOTIFY scrollMethodChanged)

    Q_PROPERTY(quint32 defaultScrollButton READ defaultScrollButton CONSTANT)
    Q_PROPERTY(quint32 scrollButton READ scrollButton WRITE setScrollButton NOTIFY scrollButtonChanged)

    //
    // tapping
    Q_PROPERTY(int tapFingerCount READ tapFingerCount CONSTANT)
    Q_PROPERTY(bool tapToClickEnabledByDefault READ tapToClickEnabledByDefault CONSTANT)
    Q_PROPERTY(bool tapToClick READ isTapToClick WRITE setTapToClick NOTIFY tapToClickChanged)

    Q_PROPERTY(bool supportsLmrTapButtonMap READ supportsLmrTapButtonMap CONSTANT)
    Q_PROPERTY(bool lmrTapButtonMapEnabledByDefault READ lmrTapButtonMapEnabledByDefault CONSTANT)
    Q_PROPERTY(bool lmrTapButtonMap READ lmrTapButtonMap WRITE setLmrTapButtonMap NOTIFY lmrTapButtonMapChanged)

    Q_PROPERTY(bool tapAndDragEnabledByDefault READ tapAndDragEnabledByDefault CONSTANT)
    Q_PROPERTY(bool tapAndDrag READ isTapAndDrag WRITE setTapAndDrag NOTIFY tapAndDragChanged)

    Q_PROPERTY(bool tapDragLockEnabledByDefault READ tapDragLockEnabledByDefault CONSTANT)
    Q_PROPERTY(bool tapDragLock READ isTapDragLock WRITE setTapDragLock NOTIFY tapDragLockChanged)

    // Click Methods
    Q_PROPERTY(bool supportsClickMethodAreas READ supportsClickMethodAreas CONSTANT)
    Q_PROPERTY(bool defaultClickMethodAreas READ defaultClickMethodAreas CONSTANT)
    Q_PROPERTY(bool clickMethodAreas READ isClickMethodAreas WRITE setClickMethodAreas NOTIFY clickMethodChanged)

    Q_PROPERTY(bool supportsClickMethodClickfinger READ supportsClickMethodClickfinger CONSTANT)
    Q_PROPERTY(bool defaultClickMethodClickfinger READ defaultClickMethodClickfinger CONSTANT)
    Q_PROPERTY(bool clickMethodClickfinger READ isClickMethodClickfinger WRITE setClickMethodClickfinger NOTIFY clickMethodChanged)

public:
    LibinputTouchpad(Display *display, int deviceId);
    bool getConfig() override;
    bool applyConfig() override;
    bool getDefaultConfig() override;
    bool isChangedConfig() override;

    //
    // general
    QString name() {
        return m_name;
    }
    bool supportsDisableEvents() {
        return m_supportsDisableEvents.avail ? m_supportsDisableEvents.val : false;
    }
    bool enabled() {
        return !m_enabled.val;
    }
    void setEnabled(bool set) {
        m_enabled.set(!set);
    }
    //
    // advanced
    Qt::MouseButtons supportedButtons() const {
        return m_supportedButtons.val;
    }
    bool supportsLeftHanded() const {
        return m_leftHanded.avail;
    }
    bool leftHandedEnabledByDefault() const {
        return m_leftHandedEnabledByDefault.val;
    }
    bool isLeftHanded() const {
        return m_leftHanded.val;
    }
    void setLeftHanded(bool set) {
        m_leftHanded.set(set);
    }

    bool supportsDisableEventsOnExternalMouse() const {
        return m_supportsDisableEventsOnExternalMouse.avail ? m_supportsDisableEventsOnExternalMouse.val : false;
    }

    bool supportsDisableWhileTyping() const {
        return m_disableWhileTyping.avail;
    }
    bool disableWhileTypingEnabledByDefault() const {
        return m_disableWhileTypingEnabledByDefault.val;
    }
    bool isDisableWhileTyping() const {
        return m_disableWhileTyping.val;
    }
    void setDisableWhileTyping(bool set) {
        m_disableWhileTyping.set(set);
    }

    bool supportsMiddleEmulation() const {
        return m_middleEmulation.avail;
    }
    bool middleEmulationEnabledByDefault() const {
        return m_middleEmulationEnabledByDefault.val;
    }
    bool isMiddleEmulation() const {
        return m_middleEmulation.val;
    }
    void setMiddleEmulation(bool set) {
        m_middleEmulation.set(set);
    }

    //
    // acceleration speed and profile
    bool supportsPointerAcceleration() const {
        return m_pointerAcceleration.avail;
    }
    qreal pointerAcceleration() const {
        return m_pointerAcceleration.val;
    }
    void setPointerAcceleration(qreal acceleration) {
        m_pointerAcceleration.set(acceleration);
    }

    bool supportsPointerAccelerationProfileFlat() const {
        return m_supportsPointerAccelerationProfileFlat.avail ? m_supportsPointerAccelerationProfileFlat.val : false;
    }
    bool defaultPointerAccelerationProfileFlat() const {
        return m_defaultPointerAccelerationProfileFlat.val;
    }
    bool pointerAccelerationProfileFlat() const {
        return m_pointerAccelerationProfileFlat.val;
    }
    void setPointerAccelerationProfileFlat(bool set) {
        m_pointerAccelerationProfileFlat.set(set);
    }

    bool supportsPointerAccelerationProfileAdaptive() const {
        return m_supportsPointerAccelerationProfileAdaptive.avail ? m_supportsPointerAccelerationProfileAdaptive.val : false;
    }
    bool defaultPointerAccelerationProfileAdaptive() const {
        return m_defaultPointerAccelerationProfileAdaptive.val;
    }
    bool pointerAccelerationProfileAdaptive() const {
        return m_pointerAccelerationProfileAdaptive.val;
    }
    void setPointerAccelerationProfileAdaptive(bool set) {
        m_pointerAccelerationProfileAdaptive.set(set);
    }

    //
    // scrolling
    bool supportsNaturalScroll() const {
        return m_naturalScroll.avail;
    }
    bool naturalScrollEnabledByDefault() const {
        return m_naturalScrollEnabledByDefault.val;
    }
    bool isNaturalScroll() const {
        return m_naturalScroll.val;
    }
    void setNaturalScroll(bool set) {
        m_naturalScroll.set(set);
    }

    bool supportsScrollTwoFinger() const {
        return m_supportsScrollTwoFinger.avail ? m_supportsScrollTwoFinger.val : false;
    }
    bool scrollTwoFingerEnabledByDefault() const {
        return m_scrollTwoFingerEnabledByDefault.val;
    }
    bool isScrollTwoFinger() const {
        return m_isScrollTwoFinger.val;
    }
    void setScrollTwoFinger(bool set) {
        m_isScrollTwoFinger.set(set);
    }

    bool supportsScrollEdge() const {
        return m_supportsScrollEdge.avail ? m_supportsScrollEdge.val : false;
    }
    bool scrollEdgeEnabledByDefault() const {
        return m_scrollEdgeEnabledByDefault.val;
    }
    bool isScrollEdge() const {
        return m_isScrollEdge.val;
    }
    void setScrollEdge(bool set) {
        m_isScrollEdge.set(set);
    }

    bool supportsScrollOnButtonDown() const {
        return m_supportsScrollOnButtonDown.avail ? m_supportsScrollOnButtonDown.val : false;
    }
    bool scrollOnButtonDownEnabledByDefault() const {
        return m_scrollOnButtonDownEnabledByDefault.val;
    }
    bool isScrollOnButtonDown() const {
        return m_isScrollOnButtonDown.val;
    }
    void setScrollOnButtonDown(bool set) {
        m_isScrollOnButtonDown.set(set);
    }

    quint32 defaultScrollButton() const {
        return m_defaultScrollButton.val;
    }
    quint32 scrollButton() const {
        return m_scrollButton.val;
    }
    void setScrollButton(quint32 button) {
        m_scrollButton.set(button);
    }

    //
    // tapping
    int tapFingerCount() const {
        return m_tapFingerCount.val;
    }
    bool tapToClickEnabledByDefault() const {
        return m_tapToClickEnabledByDefault.val;
    }
    bool isTapToClick() const {
        return m_tapToClick.val;
    }
    void setTapToClick(bool set) {
        m_tapToClick.set(set);
    }

    bool supportsLmrTapButtonMap() const {
        return m_tapFingerCount.val > 1;
    }
    bool lmrTapButtonMapEnabledByDefault() const {
        return m_lmrTapButtonMapEnabledByDefault.val;
    }
    bool lmrTapButtonMap() const {
        return m_lmrTapButtonMap.val;
    }
    void setLmrTapButtonMap(bool set) {
        m_lmrTapButtonMap.set(set);
    }

    bool tapAndDragEnabledByDefault() const {
        return m_tapAndDragEnabledByDefault.val;
    }
    bool isTapAndDrag() const {
        return m_tapAndDrag.val;
    }
    void setTapAndDrag(bool set) {
        m_tapAndDrag.set(set);
    }

    bool tapDragLockEnabledByDefault() const {
        return m_tapDragLockEnabledByDefault.val;
    }
    bool isTapDragLock() const {
        return m_tapDragLock.val;
    }
    void setTapDragLock(bool set) {
        m_tapDragLock.set(set);
    }

    bool supportsClickMethodAreas() const {
        return m_supportsClickMethodAreas.avail ? m_supportsClickMethodAreas.val : false;
    }
    bool defaultClickMethodAreas() const {
        return m_defaultClickMethodAreas.val;
    }
    bool isClickMethodAreas() const {
        return m_clickMethodAreas.val;
    }
    void setClickMethodAreas(bool set) {
        m_clickMethodAreas.set(set);
    }

    bool supportsClickMethodClickfinger() const {
        return m_supportsClickMethodClickfinger.avail ? m_supportsClickMethodClickfinger.val : false;
    }
    bool defaultClickMethodClickfinger() const {
        return m_defaultClickMethodClickfinger.val;
    }
    bool isClickMethodClickfinger() const {
        return m_clickMethodClickfinger.val;
    }
    void setClickMethodClickfinger(bool set) {
        m_clickMethodClickfinger.set(set);
    }

Q_SIGNALS:
    void enabledChanged();
    // Tapping
    void tapToClickChanged();
    void lmrTapButtonMapChanged();
    void tapAndDragChanged();
    void tapDragLockChanged();
    // Advanced
    void leftHandedChanged();
    void disableWhileTypingChanged();
    void middleEmulationChanged();
    // acceleration speed and profile
    void pointerAccelerationChanged();
    void pointerAccelerationProfileChanged();
    // scrolling
    void naturalScrollChanged();
    void scrollMethodChanged();
    void scrollButtonChanged();
    // click methods
    void clickMethodChanged();

private:
    template <typename T>
    struct Prop {
        explicit Prop(const QByteArray &_name)
            : name(_name)
        {}

        void set(T newVal) {
            if (avail && val != newVal) {
                val = newVal;
            }
        }
        void set(const Prop<T> &p) {
            if (avail && val != p.val) {
                val = p.val;
            }
        }
        bool changed() const {
            return avail && (old != val);
        }

        QByteArray name;
        bool avail;
        T old;
        T val;
    };

    template<typename T>
    bool valueLoader(Prop<T> &prop);

    template<typename T>
    QString valueWriter(const Prop<T> &prop);

    //
    // general
    Prop<bool> m_supportsDisableEvents = Prop<bool>("SupportsDisableEvents");
    Prop<bool> m_enabled = Prop<bool>("Enabled");
    Prop<bool> m_enabledDefault = Prop<bool>("EnabledDefault");

    //
    // advanced
    Prop<Qt::MouseButtons> m_supportedButtons  = Prop<Qt::MouseButtons>("SupportedButtons");

    Prop<bool> m_leftHandedEnabledByDefault = Prop<bool>("LeftHandedEnabledByDefault");
    Prop<bool> m_leftHanded = Prop<bool>("LeftHanded");

    Prop<bool> m_supportsDisableEventsOnExternalMouse = Prop<bool>("SupportsDisableEventsOnExternalMouse");
    Prop<bool> m_disableEventsOnExternalMouse = Prop<bool>("DisableEventsOnExternalMouse");
    Prop<bool> m_disableEventsOnExternalMouseDefault = Prop<bool>("DisableEventsOnExternalMouseDefault");

    Prop<bool> m_disableWhileTypingEnabledByDefault = Prop<bool>("DisableWhileTypingEnabledByDefault");
    Prop<bool> m_disableWhileTyping = Prop<bool>("DisableWhileTyping");

    Prop<bool> m_middleEmulationEnabledByDefault = Prop<bool>("MiddleEmulationEnabledByDefault");
    Prop<bool> m_middleEmulation = Prop<bool>("MiddleEmulation");

    //
    // acceleration speed and profile
    Prop<qreal> m_defaultPointerAcceleration = Prop<qreal>("DefaultPointerAcceleration");
    Prop<qreal> m_pointerAcceleration = Prop<qreal>("PointerAcceleration");

    Prop<bool> m_supportsPointerAccelerationProfileFlat = Prop<bool>("SupportsPointerAccelerationProfileFlat");
    Prop<bool> m_defaultPointerAccelerationProfileFlat = Prop<bool>("DefaultPointerAccelerationProfileFlat");
    Prop<bool> m_pointerAccelerationProfileFlat = Prop<bool>("PointerAccelerationProfileFlat");

    Prop<bool> m_supportsPointerAccelerationProfileAdaptive = Prop<bool>("SupportsPointerAccelerationProfileAdaptive");
    Prop<bool> m_defaultPointerAccelerationProfileAdaptive = Prop<bool>("DefaultPointerAccelerationProfileAdaptive");
    Prop<bool> m_pointerAccelerationProfileAdaptive = Prop<bool>("PointerAccelerationProfileAdaptive");

    //
    // scrolling
    Prop<bool> m_naturalScrollEnabledByDefault = Prop<bool>("NaturalScrollEnabledByDefault");
    Prop<bool> m_naturalScroll = Prop<bool>("NaturalScroll");

    Prop<bool> m_supportsScrollTwoFinger = Prop<bool>("SupportsScrollTwoFinger");
    Prop<bool> m_scrollTwoFingerEnabledByDefault = Prop<bool>("ScrollTwoFingerEnabledByDefault");
    Prop<bool> m_isScrollTwoFinger = Prop<bool>("ScrollTwoFinger");

    Prop<bool> m_supportsScrollEdge = Prop<bool>("SupportsScrollEdge");
    Prop<bool> m_scrollEdgeEnabledByDefault = Prop<bool>("ScrollEdgeEnabledByDefault");
    Prop<bool> m_isScrollEdge = Prop<bool>("ScrollEdge");

    Prop<bool> m_supportsScrollOnButtonDown = Prop<bool>("SupportsScrollOnButtonDown");
    Prop<bool> m_scrollOnButtonDownEnabledByDefault = Prop<bool>("ScrollOnButtonDownEnabledByDefault");
    Prop<bool> m_isScrollOnButtonDown = Prop<bool>("ScrollOnButtonDown");

    Prop<quint32> m_defaultScrollButton = Prop<quint32>("DefaultScrollButton");
    Prop<quint32> m_scrollButton = Prop<quint32>("ScrollButton");

    //
    // tapping
    Prop<int> m_tapFingerCount = Prop<int>("TapFingerCount");
    Prop<bool> m_tapToClickEnabledByDefault = Prop<bool>("TapToClickEnabledByDefault");
    Prop<bool> m_tapToClick = Prop<bool>("Tapping");

    Prop<bool> m_lmrTapButtonMapEnabledByDefault = Prop<bool>("LmrTapButtonMapEnabledByDefault");
    Prop<bool> m_lmrTapButtonMap = Prop<bool>("LmrTapButtonMap");

    Prop<bool> m_tapAndDragEnabledByDefault = Prop<bool>("TapAndDragEnabledByDefault");
    Prop<bool> m_tapAndDrag = Prop<bool>("TapAndDrag");
    Prop<bool> m_tapDragLockEnabledByDefault = Prop<bool>("TapDragLockEnabledByDefault");
    Prop<bool> m_tapDragLock = Prop<bool>("TapDragLock");

    // Click Method
    Prop<bool> m_supportsClickMethodAreas = Prop<bool>("SupportsClickMethodAreas");
    Prop<bool> m_defaultClickMethodAreas = Prop<bool>("DefaultClickMethodAreas");
    Prop<bool> m_clickMethodAreas = Prop<bool>("ClickMethodAreas");

    Prop<bool> m_supportsClickMethodClickfinger = Prop<bool>("SupportsClickMethodClickfinger");
    Prop<bool> m_defaultClickMethodClickfinger = Prop<bool>("DefaultClickMethodClickfinger");
    Prop<bool> m_clickMethodClickfinger = Prop<bool>("ClickMethodClickfinger");


    QString m_name;
};

#endif // LIBINPUTTOUCHPAD_H
