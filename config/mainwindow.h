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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef NOLXQT
    #include <LXQt/ConfigDialog>
    using ConfigDialog = LXQt::ConfigDialog;
#else
    #include <QDialog>
    #include <QString>
    class QTabWidget;
    class QDialogButtonBox;
    class QAbstractButton;
    class QSettings;

    class ConfigDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit ConfigDialog(const QString &title, QSettings *settings, QWidget *parent = 0);
        void addPage(QWidget *tab, const QString &title, const QString &iconName);

        QSettings *mSettings;
    signals:
        void reset();
    private slots:
        void buttonClicked(QAbstractButton *button);
    private:
        QTabWidget *tabWidget;
        QDialogButtonBox *buttonBox;
    };
#endif


class MainWindow : public ConfigDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

};

#endif
