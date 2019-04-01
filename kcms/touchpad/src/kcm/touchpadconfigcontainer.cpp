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

#include "touchpadconfigcontainer.h"
#include "touchpadconfigplugin.h"
#include "kcm/libinput/touchpadconfiglibinput.h"
#include "kcm/xlib/touchpadconfigxlib.h"
#include "touchpadbackend.h"

#include <KWindowSystem/kwindowsystem.h>

extern "C"
{
    Q_DECL_EXPORT void kcminit_touchpad()
    {
        if (KWindowSystem::isPlatformX11()) {
            TouchpadConfigXlib::kcmInit();
        }
    }
}

TouchpadInputBackendMode findX11Backend() {
    TouchpadBackend *choice = TouchpadBackend::implementation();
    return choice->m_mode;
}

TouchpadConfigContainer::TouchpadConfigContainer(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
{
    if (KWindowSystem::isPlatformX11()) {
        TouchpadInputBackendMode mode = findX11Backend();

        if (mode == TouchpadInputBackendMode::XLibinput){
            m_plugin = new TouchpadConfigLibinput(this);
        }
        else if (mode == TouchpadInputBackendMode::XSynaptics)
            m_plugin = new TouchpadConfigXlib(this);

    } else if (KWindowSystem::isPlatformWayland()) {
        m_plugin = new TouchpadConfigLibinput(this);
    }
}

QSize TouchpadConfigContainer::minimumSizeHint() const
{
    return m_plugin->minimumSizeHint();
}
QSize TouchpadConfigContainer::sizeHint() const
{
    return m_plugin->sizeHint();
}
void TouchpadConfigContainer::resizeEvent(QResizeEvent *event)
{
    Q_EMIT changed(false);
    m_plugin->resize(this->size());
}

void TouchpadConfigContainer::load()
{
    m_plugin->load();
}

void TouchpadConfigContainer::save()
{
    m_plugin->save();
}

void TouchpadConfigContainer::defaults()
{
    m_plugin->defaults();
}

void TouchpadConfigContainer::hideEvent(QHideEvent *e)
{
    m_plugin->hideEvent(e);
    KCModule::hideEvent(e);
}
