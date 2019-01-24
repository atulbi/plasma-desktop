/*
 * Copyright 2018 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0 as QtDialogs
import QtQuick.Controls 2.3 as QtControls
import org.kde.kirigami 2.4 as Kirigami
import org.kde.kconfig 1.0 // for KAuthorized
import org.kde.kcm 1.1 as KCM

KCM.GridViewKCM {
    KCM.ConfigModule.quickHelp: i18n("This module lets you choose the color scheme.")

    view.model: kcm.colorsModel
    view.currentIndex: kcm.selectedSchemeIndex

    enabled: !kcm.downloadingFile

    Component.onCompleted: {
        // The thumbnails are a bit more elaborate and need more room, especially when translated
        view.implicitCellWidth = Kirigami.Units.gridUnit * 13;
    }

    DropArea {
        anchors.fill: parent
        onEntered: {
            if (!drag.hasUrls) {
                drag.accepted = false;
            }
        }
        onDropped: {
            infoLabel.visible = false;
            kcm.installSchemeFromFile(drop.urls[0]);
        }
    }

    // putting the InlineMessage as header item causes it to show up initially despite visible false
    header: ColumnLayout {
        Kirigami.InlineMessage {
            id: notInstalledWarning
            Layout.fillWidth: true

            type: Kirigami.MessageType.Warning
            showCloseButton: true
            visible: false

            Connections {
                target: kcm
                onShowSchemeNotInstalledWarning: {
                    notInstalledWarning.text = i18n("The color scheme '%1' is not installed. Selecting the default theme instead.", schemeName)
                    notInstalledWarning.visible = true;
                }
            }
        }
    }

    view.remove: Transition {
        ParallelAnimation {
            NumberAnimation { property: "scale"; to: 0.5; duration: Kirigami.Units.longDuration }
            NumberAnimation { property: "opacity"; to: 0.0; duration: Kirigami.Units.longDuration }
        }
    }

    view.removeDisplaced: Transition {
        SequentialAnimation {
            // wait for the "remove" animation to finish
            PauseAnimation { duration: Kirigami.Units.longDuration }
            NumberAnimation { properties: "x,y"; duration: Kirigami.Units.longDuration }
        }
    }

    view.delegate: KCM.GridDelegate {
        id: delegate

        text: model.display

        thumbnailAvailable: true
        thumbnail: Rectangle {
            anchors.fill: parent

            opacity: model.pendingDeletion ? 0.3 : 1
            Behavior on opacity {
                NumberAnimation { duration: Kirigami.Units.longDuration }
            }

            color: model.palette.window

            Kirigami.Theme.highlightColor: model.palette.highlight
            Kirigami.Theme.highlightedTextColor: model.palette.highlightedText
            Kirigami.Theme.linkColor: model.palette.link
            Kirigami.Theme.textColor: model.palette.text

            ColumnLayout {
                anchors {
                    fill: parent
                    margins: Kirigami.Units.smallSpacing
                }

                RowLayout {
                    Layout.fillWidth: true

                    QtControls.Label {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        verticalAlignment: Text.AlignVCenter
                        text: i18n("Window text")
                        elide: Text.ElideRight
                        color: model.palette.windowText
                    }

                    QtControls.Button {
                        Layout.alignment: Qt.AlignBottom
                        text: i18n("Button")
                        Kirigami.Theme.backgroundColor: model.palette.button
                        Kirigami.Theme.textColor: model.palette.buttonText
                        activeFocusOnTab: false
                    }
                }

                QtControls.Frame {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Kirigami.Theme.backgroundColor: model.palette.base
                    // FIXME Make Frame still visible but without any inner padding
                    padding: 1
                    activeFocusOnTab: false

                    Column {
                        id: listPreviewColumn

                        readonly property string demoText: " <a href='#'>%2</a> <a href='#'><font color='%3'>%4</font></a>"
                            .arg(i18nc("Hyperlink", "link"))
                            .arg(model.palette.linkVisited)
                            .arg(i18nc("Visited hyperlink", "visited"))

                        width: parent.width

                        QtControls.ItemDelegate {
                            width: parent.width
                            text: i18n("Normal text") + listPreviewColumn.demoText
                            activeFocusOnTab: false
                        }

                        QtControls.ItemDelegate {
                            width: parent.width
                            highlighted: true
                            // TODO use proper highlighted link color
                            text: i18n("Highlighted text") + listPreviewColumn.demoText
                            activeFocusOnTab: false
                        }

                        QtControls.ItemDelegate {
                            width: parent.width
                            enabled: false
                            text: i18n("Disabled text") + listPreviewColumn.demoText
                            activeFocusOnTab: false
                        }
                    }
                }
            }

            // Make the preview non-clickable but still reacting to hover
            MouseArea {
                anchors.fill: parent
                onClicked: delegate.clicked()
                onDoubleClicked: delegate.doubleClicked()
            }
        }

        actions: [
            Kirigami.Action {
                iconName: "document-edit"
                tooltip: i18n("Edit Color Scheme")
                enabled: !model.pendingDeletion
                onTriggered: kcm.editScheme(model.index, parent)
            },
            Kirigami.Action {
                iconName: "edit-delete"
                tooltip: i18n("Remove Color Scheme")
                enabled: model.removable
                visible: !model.pendingDeletion
                onTriggered: kcm.setPendingDeletion(model.index, true)
            },
            Kirigami.Action {
                iconName: "edit-undo"
                tooltip: i18n("Restore Color Scheme")
                visible: model.pendingDeletion
                onTriggered: kcm.setPendingDeletion(model.index, false)
            }
        ]
        onClicked: {
            kcm.selectedScheme = model.schemeName;
            view.forceActiveFocus();
        }
        onDoubleClicked: {
            kcm.save();
        }
    }

    footer: ColumnLayout {
        Kirigami.InlineMessage {
            id: infoLabel
            Layout.fillWidth: true

            showCloseButton: true

            Connections {
                target: kcm
                onShowSuccessMessage: {
                    infoLabel.type = Kirigami.MessageType.Positive;
                    infoLabel.text = message;
                    infoLabel.visible = true;
                    // Avoid dual message widgets
                    notInstalledWarning.visible = false;
                }
                onShowErrorMessage: {
                    infoLabel.type = Kirigami.MessageType.Error;
                    infoLabel.text = message;
                    infoLabel.visible = true;
                    notInstalledWarning.visible = false;
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight

            QtControls.Button {
                text: i18n("Install from File...")
                icon.name: "document-import"
                onClicked: fileDialogLoader.active = true
            }

            QtControls.Button {
                text: i18n("Get New Color Schemes...")
                icon.name: "get-hot-new-stuff"
                onClicked: kcm.getNewStuff(this)
                visible: KAuthorized.authorize("ghns")
            }
        }
    }

    Loader {
        id: fileDialogLoader
        active: false
        sourceComponent: QtDialogs.FileDialog {
            title: i18n("Open Color Scheme")
            folder: shortcuts.home
            nameFilters: [ i18n("Color Scheme Files (*.colors)") ]
            Component.onCompleted: open()
            onAccepted: {
                infoLabel.visible = false;
                kcm.installSchemeFromFile(fileUrls[0])
                fileDialogLoader.active = false
            }
            onRejected: {
                fileDialogLoader.active = false
            }
        }
    }
}