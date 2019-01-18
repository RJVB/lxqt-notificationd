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

#ifndef NOLXQT
    #include <LXQt/Globals>
    #include <LXQt/SingleApplication>

    #include <XdgIcon>
    #include <LXQt/Settings>
#else
    #include <QApplication>
    #include <QIcon>
    #include <QSettings>
    #include "../notifyd_fullversion.h"
    #define QSL(s) QStringLiteral(s)
    #define QL1S(s) QLatin1Literal(s)
    #define QL1C(s) QLatin1Char(s)
#endif
#include <QCommandLineParser>

#include "mainwindow.h"


int main(int argc, char** argv)
{
#ifndef NOLXQT
    LXQt::SingleApplication a(argc, argv);
#else
    QApplication a(argc, argv);
#endif
    a.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("LXQt Config Notificationd"));
#ifndef NOLXQT
    const QString VERINFO = QStringLiteral(LXQT_NOTIFICATIOND_VERSION
                                           "\nliblxqt   " LXQT_VERSION
                                           "\nQt        " QT_VERSION_STR);
#else
    const QString VERINFO = QStringLiteral(NOTIFYD_FULL_VERSION_STRING
                                           "\nQt        " QT_VERSION_STR);
#endif
    a.setApplicationVersion(VERINFO);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(a);

    MainWindow w;
    w.setWindowIcon(QIcon::fromTheme(QSL("preferences-desktop-theme")));
#ifndef NOLXQT
    a.setActivationWindow(&w);
#endif
    w.show();

    return a.exec();
}
