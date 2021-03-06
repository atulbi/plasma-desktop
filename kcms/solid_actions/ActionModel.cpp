/**************************************************************************
 * Copyright (C) 2009 Ben Cooksley <ben@eclipse.endoftheinternet.org>     *
 * Copyright (C) 2007 Will Stephenson <wstephenson@kde.org>               *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 2         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA          *
 * 02110-1301, USA.                                                       *
***************************************************************************/

#include "ActionModel.h"
#include "ActionItem.h"

#include <KDesktopFileActions>

#include <QIcon>
#include <QStandardPaths>
#include <QDirIterator>

class ActionModel::Private {
public:
    Private() {}

    QList<ActionItem*> actions;
};

static bool sortAction( ActionItem * left, ActionItem * right )
{
    return left->name().localeAwareCompare(right->name()) < 0;
}

ActionModel::ActionModel( QObject *parent )
    : QAbstractTableModel( parent )
    , d( new Private() )
{
}

ActionModel::~ActionModel()
{
    qDeleteAll( d->actions );
    d->actions.clear();
    delete d;
}

int ActionModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 2;
}

int ActionModel::rowCount( const QModelIndex &parent ) const
{
    if( !parent.isValid() ) {
        return d->actions.count();
    }
    return 0;
}

QVariant ActionModel::data( const QModelIndex &index, int role ) const
{
    QVariant theData;
    if ( !index.isValid() ) {
        return QVariant();
    }

    ActionItem * mi = d->actions.at( index.row() );
    switch ( role ) {
    case Qt::DisplayRole:
        if( index.column() == 0 ) {
            theData.setValue( mi->name() );
        } else if( index.column() == 1 ) {
            theData.setValue( mi->involvedTypes() );
        }
        break;
    case Qt::DecorationRole:
        if( index.column() == 0 ) {
            theData = QIcon::fromTheme(mi->icon());
        }
        break;
    case Qt::UserRole:
        theData.setValue( mi );
        break;
    default:
        break;
    }
    return theData;
}

void ActionModel::buildActionList()
{
    beginResetModel();
    qDeleteAll( d->actions );
    d->actions.clear();
    // Prepare to search for possible actions -> we only want solid types
    const QStringList actionDirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("solid/actions"), QStandardPaths::LocateDirectory);
    // Get service objects for those actions and add them to the display
    foreach (const QString & actionDir, actionDirs) {
        QDirIterator it(actionDir, QStringList() << QStringLiteral("*.desktop"));
        while (it.hasNext()) {
            it.next();
            const QString desktop = it.filePath();
            // Get contained services list
            QList<KServiceAction> services = KDesktopFileActions::userDefinedServices(desktop, true);
            foreach( const KServiceAction &deviceAction, services ) {
                ActionItem * actionItem = new ActionItem( desktop, deviceAction.name(), this ); // Create an action
                d->actions.append( actionItem );
            }
        }
    }

    std::sort(d->actions.begin(), d->actions.end(), sortAction);
    endResetModel();
}

QList<ActionItem*> ActionModel::actionList() const
{
    return d->actions;
}

