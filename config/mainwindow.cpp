/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QtCore/QProcess>
#include <QMessageBox>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#ifndef NOLXQT
    #include <LXQt/Globals>
    #include <LXQt/ConfigDialog>
    using Settings = LXQt::Settings;
#else
    #include <QDebug>
    #include <QSettings>
    #include <QTabWidget>
    #include <QDialogButtonBox>
    #include <QAbstractButton>
    using Settings = QSettings;
    #define QSL(s) QStringLiteral(s)
    #define QL1S(s) QLatin1Literal(s)
    #define QL1C(s) QLatin1Char(s)
#endif

#include "mainwindow.h"
#include "basicsettings.h"
#include "advancedsettings.h"

#ifdef NOLXQT
ConfigDialog::ConfigDialog(const QString &title, QSettings *settings, QWidget *parent)
    : QDialog(parent)
    , mSettings(settings)
{
    setWindowTitle(title);
    tabWidget = new QTabWidget(this);

    // no point in providing a reset button because AFAICT
    // the settings widgets do not cache previous values that could be restored.
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel
                                     /*| QDialogButtonBox::Reset*/);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::clicked, this, &ConfigDialog::buttonClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

void ConfigDialog::buttonClicked(QAbstractButton *button)
{
    const auto standardButton = buttonBox->standardButton(button);
    if (standardButton == QDialogButtonBox::Reset) {
        emit reset();
    }
}

void ConfigDialog::addPage(QWidget *tab, const QString &title, const QString &iconName)
{
    tabWidget->addTab(tab, QIcon::fromTheme(iconName), title);
}
#endif

MainWindow::MainWindow(QWidget *parent) :
    ConfigDialog(tr("Desktop Notifications"), new Settings(QSL("notifications")), parent)
{
    BasicSettings* basic = new BasicSettings(mSettings, this);
    addPage(basic, tr("Basic Settings"), QSL("preferences-desktop-notification"));
    connect(this, SIGNAL(reset()), basic, SLOT(restoreSettings()));

    AdvancedSettings* menu = new AdvancedSettings(mSettings, this);
    addPage(menu, tr("Advanced Settings"), QSL("preferences-desktop-notification-bell"));
    connect(this, SIGNAL(reset()), menu, SLOT(restoreSettings()));
}

MainWindow::~MainWindow()
{
    delete mSettings;
}

#ifdef NOLXQT
#include "moc_mainwindow.cpp"
#endif
