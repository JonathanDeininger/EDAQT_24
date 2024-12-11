/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *Play;
    QPushButton *Pause;
    QListWidget *Playlist;
    QPushButton *AddTrackButton;
    QPushButton *RemoveTrackButton;
    QPushButton *AddPlaylistButton;
    QLabel *currentSongLabel;
    QSlider *LautstaerkeRegler;
    QListWidget *PlaylistSammlung;
    QPushButton *previousPushButton;
    QPushButton *nextPushButton;
    QSlider *Fortschrittslider;
    QLabel *currentSongDurationLabel;
    QPushButton *Random;
    QPushButton *Repeat;
    QLineEdit *searchBar;
    QPushButton *RemovePlaylistButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1144, 607);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Play = new QPushButton(centralwidget);
        Play->setObjectName("Play");
        Play->setGeometry(QRect(480, 520, 80, 24));
        Pause = new QPushButton(centralwidget);
        Pause->setObjectName("Pause");
        Pause->setGeometry(QRect(570, 520, 80, 24));
        Playlist = new QListWidget(centralwidget);
        Playlist->setObjectName("Playlist");
        Playlist->setGeometry(QRect(320, 60, 521, 361));
        AddTrackButton = new QPushButton(centralwidget);
        AddTrackButton->setObjectName("AddTrackButton");
        AddTrackButton->setGeometry(QRect(960, 240, 101, 24));
        RemoveTrackButton = new QPushButton(centralwidget);
        RemoveTrackButton->setObjectName("RemoveTrackButton");
        RemoveTrackButton->setGeometry(QRect(960, 280, 101, 24));
        AddPlaylistButton = new QPushButton(centralwidget);
        AddPlaylistButton->setObjectName("AddPlaylistButton");
        AddPlaylistButton->setGeometry(QRect(960, 110, 101, 24));
        currentSongLabel = new QLabel(centralwidget);
        currentSongLabel->setObjectName("currentSongLabel");
        currentSongLabel->setGeometry(QRect(440, 440, 241, 41));
        LautstaerkeRegler = new QSlider(centralwidget);
        LautstaerkeRegler->setObjectName("LautstaerkeRegler");
        LautstaerkeRegler->setGeometry(QRect(840, 520, 221, 20));
        LautstaerkeRegler->setOrientation(Qt::Orientation::Horizontal);
        PlaylistSammlung = new QListWidget(centralwidget);
        PlaylistSammlung->setObjectName("PlaylistSammlung");
        PlaylistSammlung->setGeometry(QRect(40, 60, 221, 361));
        previousPushButton = new QPushButton(centralwidget);
        previousPushButton->setObjectName("previousPushButton");
        previousPushButton->setGeometry(QRect(380, 520, 80, 24));
        nextPushButton = new QPushButton(centralwidget);
        nextPushButton->setObjectName("nextPushButton");
        nextPushButton->setGeometry(QRect(670, 520, 80, 24));
        Fortschrittslider = new QSlider(centralwidget);
        Fortschrittslider->setObjectName("Fortschrittslider");
        Fortschrittslider->setGeometry(QRect(379, 490, 371, 20));
        Fortschrittslider->setOrientation(Qt::Orientation::Horizontal);
        currentSongDurationLabel = new QLabel(centralwidget);
        currentSongDurationLabel->setObjectName("currentSongDurationLabel");
        currentSongDurationLabel->setGeometry(QRect(760, 490, 71, 21));
        Random = new QPushButton(centralwidget);
        Random->setObjectName("Random");
        Random->setGeometry(QRect(280, 450, 80, 24));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistShuffle));
        Random->setIcon(icon);
        Repeat = new QPushButton(centralwidget);
        Repeat->setObjectName("Repeat");
        Repeat->setGeometry(QRect(770, 450, 80, 24));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistRepeat));
        Repeat->setIcon(icon1);
        searchBar = new QLineEdit(centralwidget);
        searchBar->setObjectName("searchBar");
        searchBar->setGeometry(QRect(320, 20, 521, 24));
        RemovePlaylistButton = new QPushButton(centralwidget);
        RemovePlaylistButton->setObjectName("RemovePlaylistButton");
        RemovePlaylistButton->setGeometry(QRect(960, 150, 101, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1144, 33));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Play->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        Pause->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        AddTrackButton->setText(QCoreApplication::translate("MainWindow", "Add Track", nullptr));
        RemoveTrackButton->setText(QCoreApplication::translate("MainWindow", "RemoveTrack", nullptr));
        AddPlaylistButton->setText(QCoreApplication::translate("MainWindow", "Add Playlist", nullptr));
        currentSongLabel->setText(QString());
        previousPushButton->setText(QCoreApplication::translate("MainWindow", "previous", nullptr));
        nextPushButton->setText(QCoreApplication::translate("MainWindow", "next", nullptr));
        currentSongDurationLabel->setText(QString());
        Random->setText(QString());
        Repeat->setText(QString());
        RemovePlaylistButton->setText(QCoreApplication::translate("MainWindow", "RemovePlaylist", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
