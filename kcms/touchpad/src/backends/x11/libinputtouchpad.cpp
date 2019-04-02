/*
 * Copyright (C) 2015 Red Hat, Inc.
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

#include <QDebug>
#include <X11/extensions/XInput2.h>

#include "libinputtouchpad.h"

#include <stddef.h>
#include <limits.h>

#include <xserver-properties.h>
#include <libinput-properties.h>
#include "logging.h"

namespace {
template<typename T>
T valueLoaderPart(QVariant const &reply) { Q_UNUSED(reply); return T(); }

template<>
bool valueLoaderPart(QVariant const &reply) { return reply.toBool(); }

template<>
int valueLoaderPart(QVariant const &reply) { return reply.toInt(); }

template<>
quint32 valueLoaderPart(QVariant const &reply) { return reply.toInt(); }

template<>
qreal valueLoaderPart(QVariant const &reply) { return reply.toReal(); }

template<>
QString valueLoaderPart(QVariant const &reply) { return reply.toString(); }

template<>
Qt::MouseButtons valueLoaderPart(QVariant const &reply) { return static_cast<Qt::MouseButtons>(reply.toInt()); }
}

const struct Parameter libinputProperties[] = {

    /* libinput disable supports property */
    {"SupportsDisableEvents",               PT_INT, 0, 1, LIBINPUT_PROP_SENDEVENTS_AVAILABLE, 8, 0},
    {"Enabled",                             PT_INT, 0, 1, LIBINPUT_PROP_SENDEVENTS_ENABLED, 8, 0},
    {"EnabledDefault",                      PT_INT, 0, 1, LIBINPUT_PROP_SENDEVENTS_ENABLED_DEFAULT, 8, 0},

    /* LeftHandSupport */
    {"LeftHandedEnabledByDefault",          PT_INT, 0, 1, LIBINPUT_PROP_LEFT_HANDED_DEFAULT, 8, 0},
    {"LeftHanded",                          PT_INT, 0, 1, LIBINPUT_PROP_LEFT_HANDED, 8, 0},

    /* Disable on external mouse */
    {"SupportsDisableEventsOnExternalMouse",PT_INT, 0, 1, LIBINPUT_PROP_SENDEVENTS_AVAILABLE, 8, 1},
    {"DisableEventsOnExternalMouse",        PT_INT, 0, 1, LIBINPUT_PROP_SENDEVENTS_ENABLED, 8, 1},
    {"DisableEventsOnExternalMouseDefault", PT_INT, 0, 1, LIBINPUT_PROP_SENDEVENTS_ENABLED_DEFAULT, 8, 1},

    /* Disable while typing */
    {"DisableWhileTypingEnabledByDefault",  PT_INT, 0, 1, LIBINPUT_PROP_DISABLE_WHILE_TYPING_DEFAULT, 8, 0},
    {"DisableWhileTyping",                  PT_INT, 0, 1, LIBINPUT_PROP_DISABLE_WHILE_TYPING, 8, 0},

    /* Middle Emulation */
    {"MiddleEmulationEnabledByDefault",     PT_INT, 0, 1, LIBINPUT_PROP_MIDDLE_EMULATION_ENABLED_DEFAULT, 8, 0},
    {"MiddleEmulation",                     PT_INT, 0, 1, LIBINPUT_PROP_MIDDLE_EMULATION_ENABLED, 8, 0},

    /* This is a boolean for all three fingers, no per-finger config */
    {"Tapping",                             PT_INT, 0, 1, LIBINPUT_PROP_TAP,     8,     0},
    {"TapToClickEnabledByDefault",          PT_INT, 0, 1, LIBINPUT_PROP_TAP_DEFAULT, 8, 0},

    /* LMR */
    {"LmrTapButtonMapEnabledByDefault",     PT_INT, 0, 1, LIBINPUT_PROP_TAP_BUTTONMAP_DEFAULT, 8, 1},
    {"LmrTapButtonMap",                     PT_INT, 0, 1, LIBINPUT_PROP_TAP_BUTTONMAP, 8, 1},

    /* Tap and Drag Enabled */
    {"TapAndDragEnabledByDefault",          PT_INT, 0, 1, LIBINPUT_PROP_TAP_DRAG_DEFAULT, 8, 0},
    {"TapAndDrag",                          PT_INT, 0, 1, LIBINPUT_PROP_TAP_DRAG, 8, 0},

    /* Tap and Drag Lock Enabled */
    {"TapDragLockEnabledByDefault",         PT_INT, 0, 1, LIBINPUT_PROP_TAP_DRAG_LOCK_DEFAULT, 8, 0},
    {"TapDragLock",                         PT_INT, 0, 1, LIBINPUT_PROP_TAP_DRAG_LOCK, 8, 0},

    /* libinput normalizes the accel to -1/1 */
    {"DefaultPointerAcceleration",          PT_DOUBLE, -1.0, 1.0,   LIBINPUT_PROP_ACCEL_DEFAULT,  0 /*float */, 0},
    {"PointerAcceleration",                 PT_DOUBLE, -1.0, 1.0,   LIBINPUT_PROP_ACCEL,  0 /*float */, 0},

    /* Libinput Accel Profile */
    {"SupportsPointerAccelerationProfileAdaptive",  PT_BOOL, 0, 1, LIBINPUT_PROP_ACCEL_PROFILES_AVAILABLE, 8, 0},
    {"DefaultPointerAccelerationProfileAdaptive",   PT_BOOL, 0, 1, LIBINPUT_PROP_ACCEL_PROFILE_ENABLED_DEFAULT, 8, 0},
    {"PointerAccelerationProfileAdaptive",          PT_BOOL, 0, 1, LIBINPUT_PROP_ACCEL_PROFILE_ENABLED, 8, 0},
    {"SupportsPointerAccelerationProfileFlat",      PT_BOOL, 0, 1, LIBINPUT_PROP_ACCEL_PROFILES_AVAILABLE, 8, 1},
    {"DefaultPointerAccelerationProfileFlat",       PT_BOOL, 0, 1, LIBINPUT_PROP_ACCEL_PROFILE_ENABLED_DEFAULT, 8, 1},
    {"PointerAccelerationProfileFlat",              PT_BOOL, 0, 1, LIBINPUT_PROP_ACCEL_PROFILE_ENABLED, 8, 1},

    /* Natural Scrolling */
    {"NaturalScrollEnabledByDefault",       PT_INT, 0, 1, LIBINPUT_PROP_NATURAL_SCROLL_DEFAULT, 8, 0},
    {"NaturalScroll",                       PT_INT, 0, 1, LIBINPUT_PROP_NATURAL_SCROLL, 8, 0},

    /* Two-Finger Scrolling */
    {"SupportsScrollTwoFinger",             PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHODS_AVAILABLE, 8, 0},
    {"ScrollTwoFingerEnabledByDefault",     PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHOD_ENABLED_DEFAULT, 8, 0},
    {"ScrollTwoFinger",                     PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHOD_ENABLED, 8, 0},

    /* Edge Scrolling */
    {"SupportsScrollEdge",                  PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHODS_AVAILABLE, 8, 1},
    {"ScrollEdgeEnabledByDefault",          PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHOD_ENABLED_DEFAULT, 8, 1},
    {"ScrollEdge",                          PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHOD_ENABLED, 8, 1},

    /* scroll on button */
    {"SupportsScrollOnButtonDown",          PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHODS_AVAILABLE, 8, 2},
    {"ScrollOnButtonDownEnabledByDefault",  PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHOD_ENABLED_DEFAULT, 8, 2},
    {"ScrollOnButtonDown",                  PT_INT, 0, 1, LIBINPUT_PROP_SCROLL_METHOD_ENABLED, 8, 2},

    /* Scroll Button for scroll on button Down */
    {"DefaultScrollButton",                 PT_INT, 0, INT_MAX, LIBINPUT_PROP_SCROLL_BUTTON_DEFAULT, 32, 0},
    {"ScrollButton",                        PT_INT, 0, INT_MAX, LIBINPUT_PROP_SCROLL_BUTTON, 32, 0},

    /* Click Methods */
    {"SupportsClickMethodAreas",            PT_INT, 0, 1, LIBINPUT_PROP_CLICK_METHODS_AVAILABLE, 8, 0},
    {"DefaultClickMethodAreas",             PT_INT, 0, 1, LIBINPUT_PROP_CLICK_METHOD_ENABLED_DEFAULT, 8, 0},
    {"ClickMethodAreas",                    PT_INT, 0, 1, LIBINPUT_PROP_CLICK_METHOD_ENABLED, 8, 0},

    {"SupportsClickMethodClickfinger",      PT_INT, 0, 1, LIBINPUT_PROP_CLICK_METHODS_AVAILABLE, 8, 1},
    {"DefaultClickMethodClickfinger",       PT_INT, 0, 1, LIBINPUT_PROP_CLICK_METHOD_ENABLED_DEFAULT, 8, 1},
    {"ClickMethodClickfinger",              PT_INT, 0, 1, LIBINPUT_PROP_CLICK_METHOD_ENABLED, 8, 1},

    /* libinput doesn't have a separate toggle for horiz scrolling */
    { NULL, PT_INT, 0, 0, 0, 0, 0 }
};

Qt::MouseButtons maskBtns(Display *display, XIButtonClassInfo *buttonInfo)
{
    Qt::MouseButtons buttons = Qt::NoButton;
    for (int i = 0; i < buttonInfo->num_buttons; ++i) {
        QByteArray reply = XGetAtomName(display, buttonInfo->labels[i]);

        if (reply == BTN_LABEL_PROP_BTN_LEFT) {
            buttons |= Qt::LeftButton;
        }
        if (reply == BTN_LABEL_PROP_BTN_RIGHT) {
            buttons |= Qt::RightButton;
        }
        if (reply == BTN_LABEL_PROP_BTN_MIDDLE) {
            buttons |= Qt::MiddleButton;
        }
        if (reply == BTN_LABEL_PROP_BTN_SIDE) {
            buttons |= Qt::ExtraButton1;
        }
        if (reply == BTN_LABEL_PROP_BTN_EXTRA) {
            buttons |= Qt::ExtraButton2;
        }
        if (reply == BTN_LABEL_PROP_BTN_FORWARD) {
            buttons |= Qt::ForwardButton;
        }
        if (reply == BTN_LABEL_PROP_BTN_BACK) {
            buttons |= Qt::BackButton;
        }
        if (reply == BTN_LABEL_PROP_BTN_TASK) {
            buttons |= Qt::TaskButton;
        }
    }
    return buttons;
}

LibinputTouchpad::LibinputTouchpad(Display *display, int deviceId):
    XlibTouchpad(nullptr, display, deviceId)
{
    loadSupportedProperties(libinputProperties);

    int nDevices = 0;
    XIDeviceInfo *deviceInfo = XIQueryDevice(m_display, m_deviceId, &nDevices);
    m_name = deviceInfo->name;

    for (int i = 0; i < deviceInfo->num_classes; ++i) {
        XIAnyClassInfo *classInfo = deviceInfo->classes[i];

        if (classInfo->type == XIButtonClass) {
            XIButtonClassInfo *btnInfo = (XIButtonClassInfo*) classInfo;
            m_supportedButtons.avail = true;
            m_supportedButtons.set(maskBtns(m_display, btnInfo));
        }
        if (classInfo->type == XITouchClass) {
            XITouchClassInfo *touchInfo = (XITouchClassInfo*) classInfo;
            m_tapFingerCount.avail = true;
            m_tapFingerCount.set(touchInfo->num_touches);
        }
    }
    XIFreeDeviceInfo(deviceInfo);

    /* FingerCount cannot be zero */
    if (!m_tapFingerCount.val) {
        m_tapFingerCount.avail = true;
        m_tapFingerCount.set(1);
    }
}

bool LibinputTouchpad::getConfig()
{
    bool success = true;

    success &= valueLoader(m_supportsDisableEvents);
    success &= valueLoader(m_enabled);
    success &= valueLoader(m_enabledDefault);

    success &= valueLoader(m_tapToClickEnabledByDefault);
    success &= valueLoader(m_tapToClick);
    success &= valueLoader(m_lmrTapButtonMapEnabledByDefault);
    success &= valueLoader(m_lmrTapButtonMap);
    success &= valueLoader(m_tapAndDragEnabledByDefault);
    success &= valueLoader(m_tapAndDrag);
    success &= valueLoader(m_tapDragLockEnabledByDefault);
    success &= valueLoader(m_tapDragLock);

    success &= valueLoader(m_leftHandedEnabledByDefault);
    success &= valueLoader(m_leftHanded);

    success &= valueLoader(m_supportsDisableEventsOnExternalMouse);
    success &= valueLoader(m_disableEventsOnExternalMouse);
    success &= valueLoader(m_disableEventsOnExternalMouseDefault);

    success &= valueLoader(m_disableWhileTypingEnabledByDefault);
    success &= valueLoader(m_disableWhileTyping);

    success &= valueLoader(m_middleEmulationEnabledByDefault);
    success &= valueLoader(m_middleEmulation);

    success &= valueLoader(m_defaultPointerAcceleration);
    success &= valueLoader(m_pointerAcceleration);

    success &= valueLoader(m_supportsPointerAccelerationProfileFlat);
    success &= valueLoader(m_defaultPointerAccelerationProfileFlat);
    success &= valueLoader(m_pointerAccelerationProfileFlat);
    success &= valueLoader(m_supportsPointerAccelerationProfileAdaptive);
    success &= valueLoader(m_defaultPointerAccelerationProfileAdaptive);
    success &= valueLoader(m_pointerAccelerationProfileAdaptive);

    success &= valueLoader(m_naturalScrollEnabledByDefault);
    success &= valueLoader(m_naturalScroll);

    success &= valueLoader(m_supportsScrollTwoFinger);
    success &= valueLoader(m_scrollTwoFingerEnabledByDefault);
    success &= valueLoader(m_isScrollTwoFinger);

    success &= valueLoader(m_supportsScrollEdge);
    success &= valueLoader(m_scrollEdgeEnabledByDefault);
    success &= valueLoader(m_isScrollEdge);

    success &= valueLoader(m_supportsScrollOnButtonDown);
    success &= valueLoader(m_scrollOnButtonDownEnabledByDefault);
    success &= valueLoader(m_isScrollOnButtonDown);

    success &= valueLoader(m_defaultScrollButton);
    success &= valueLoader(m_scrollButton);

    // click methods
    success &= valueLoader(m_supportsClickMethodAreas);
    success &= valueLoader(m_supportsClickMethodClickfinger);
    success &= valueLoader(m_defaultClickMethodAreas);
    success &= valueLoader(m_defaultClickMethodClickfinger);
    success &= valueLoader(m_clickMethodAreas);
    success &= valueLoader(m_clickMethodClickfinger);

    return success;
}

bool LibinputTouchpad::applyConfig()
{
    QVector<QString> msgs;

    msgs << valueWriter(m_enabled)
         << valueWriter(m_tapToClick)
         << valueWriter(m_lmrTapButtonMap)
         << valueWriter(m_tapAndDrag)
         << valueWriter(m_tapDragLock)
         << valueWriter(m_leftHanded)
         << valueWriter(m_disableWhileTyping)
         << valueWriter(m_middleEmulation)
         << valueWriter(m_pointerAcceleration)
         << valueWriter(m_pointerAccelerationProfileFlat)
         << valueWriter(m_pointerAccelerationProfileAdaptive)
         << valueWriter(m_naturalScroll)
         << valueWriter(m_isScrollTwoFinger)
         << valueWriter(m_isScrollEdge)
         << valueWriter(m_isScrollOnButtonDown)
         << valueWriter(m_scrollButton)
         << valueWriter(m_clickMethodAreas)
         << valueWriter(m_clickMethodClickfinger);

    bool success = true;
    QString error_msg;

    for (QString m : msgs) {
        if (!m.isNull()) {
            qCCritical(KCM_TOUCHPAD) << "in error:" << m;
            if (!success) {
                error_msg.append("\n");
            }
            error_msg.append(m);
            success = false;
        }
    }

    if (!success) {
        qCCritical(KCM_TOUCHPAD) <<  error_msg;
    }

    flush();
    return success;
}

bool LibinputTouchpad::getDefaultConfig()
{
    m_enabled.set(m_enabledDefault);
    m_tapToClick.set(m_tapToClickEnabledByDefault);
    m_lmrTapButtonMap.set(m_lmrTapButtonMapEnabledByDefault);
    m_tapAndDrag.set(m_tapAndDragEnabledByDefault);
    m_tapDragLock.set(m_tapDragLockEnabledByDefault);
    m_leftHanded.set(m_leftHandedEnabledByDefault);
    m_disableEventsOnExternalMouse.set(m_disableEventsOnExternalMouseDefault);
    m_disableWhileTyping.set(m_disableWhileTypingEnabledByDefault);
    m_middleEmulation.set(m_middleEmulationEnabledByDefault);
    m_pointerAcceleration.set(m_defaultPointerAcceleration);
    m_pointerAccelerationProfileFlat.set(m_defaultPointerAccelerationProfileFlat);
    m_pointerAccelerationProfileAdaptive.set(m_defaultPointerAccelerationProfileAdaptive);
    m_naturalScroll.set(m_naturalScrollEnabledByDefault);
    m_isScrollTwoFinger.set(m_scrollTwoFingerEnabledByDefault);
    m_isScrollEdge.set(m_scrollEdgeEnabledByDefault);
    m_isScrollOnButtonDown.set(m_scrollOnButtonDownEnabledByDefault);
    m_scrollButton.set(m_defaultScrollButton);
    m_clickMethodAreas.set(m_defaultClickMethodAreas);
    m_clickMethodClickfinger.set(m_defaultClickMethodClickfinger);

    return true;
}

bool LibinputTouchpad::isChangedConfig()
{

    bool changed = m_enabled.changed() ||
            m_tapToClick.changed() ||
            m_lmrTapButtonMap.changed() ||
            m_tapAndDrag.changed() ||
            m_tapDragLock.changed() ||
            m_leftHanded.changed() ||
            m_disableEventsOnExternalMouse.changed() ||
            m_disableWhileTyping.changed() ||
            m_middleEmulation.changed() ||
            m_pointerAcceleration.changed() ||
            m_pointerAccelerationProfileFlat.changed() ||
            m_pointerAccelerationProfileAdaptive.changed() ||
            m_naturalScroll.changed() ||
            m_isScrollTwoFinger.changed() ||
            m_isScrollEdge.changed() ||
            m_isScrollOnButtonDown.changed() ||
            m_scrollButton.changed() ||
            m_clickMethodAreas.changed() ||
            m_clickMethodClickfinger.changed();

    return changed;
}

template<typename T>
bool LibinputTouchpad::valueLoader(Prop<T> &prop)
{
    const Parameter *p = findParameter(QString::fromAscii(prop.name));

    if (!p) {
        qCCritical(KCM_TOUCHPAD) << "Error on read of " << QString::fromAscii(prop.name);
    }

    QVariant reply = getParameter(p);
    if (!reply.isValid()) {
        qCDebug(KCM_TOUCHPAD) << "Property not available : " << QString::fromAscii(prop.name);
        prop.avail = false;
        return true;
    }
    prop.avail = true;

    T replyValue = valueLoaderPart<T>(reply);

    prop.old = replyValue;
    prop.val = replyValue;
    return true;
}

template<typename T>
QString LibinputTouchpad::valueWriter(const Prop<T> &prop)
{
    const Parameter *p = findParameter(QString::fromAscii(prop.name));

    if (!p || !prop.changed()) {
        return QString();
    }

    bool error = !setParameter( p, prop.val);
    if (error) {
        qCCritical(KCM_TOUCHPAD) << "Cannot set property " + QString::fromAscii(prop.name);
        return QStringLiteral("Cannot set property ") + QString::fromAscii(prop.name);
    }
    return QString();
}
