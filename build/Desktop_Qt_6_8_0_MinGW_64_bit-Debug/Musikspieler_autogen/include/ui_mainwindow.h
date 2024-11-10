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
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
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
    QPushButton *AddTrack;
    QPushButton *RemoveTrack;
    QPushButton *choosePlaylistButton;
    QLabel *currentSongLabel;
    QSlider *LautstaerkeRegler;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Play = new QPushButton(centralwidget);
        Play->setObjectName("Play");
        Play->setGeometry(QRect(300, 420, 80, 24));
        Pause = new QPushButton(centralwidget);
        Pause->setObjectName("Pause");
        Pause->setGeometry(QRect(420, 420, 80, 24));
        Playlist = new QListWidget(centralwidget);
        Playlist->setObjectName("Playlist");
        Playlist->setGeometry(QRect(40, 30, 256, 192));
        AddTrack = new QPushButton(centralwidget);
        AddTrack->setObjectName("AddTrack");
        AddTrack->setGeometry(QRect(320, 160, 101, 24));
        RemoveTrack = new QPushButton(centralwidget);
        RemoveTrack->setObjectName("RemoveTrack");
        RemoveTrack->setGeometry(QRect(320, 200, 101, 24));
        choosePlaylistButton = new QPushButton(centralwidget);
        choosePlaylistButton->setObjectName("choosePlaylistButton");
        choosePlaylistButton->setGeometry(QRect(320, 120, 101, 24));
        currentSongLabel = new QLabel(centralwidget);
        currentSongLabel->setObjectName("currentSongLabel");
        currentSongLabel->setGeometry(QRect(280, 350, 241, 41));
        LautstaerkeRegler = new QSlider(centralwidget);
        LautstaerkeRegler->setObjectName("LautstaerkeRegler");
        LautstaerkeRegler->setGeometry(QRect(290, 470, 221, 20));
        LautstaerkeRegler->setOrientation(Qt::Orientation::Horizontal);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
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
        AddTrack->setText(QCoreApplication::translate("MainWindow", "AddTrack", nullptr));
        RemoveTrack->setText(QCoreApplication::translate("MainWindow", "RemoveTrack", nullptr));
        choosePlaylistButton->setText(QCoreApplication::translate("MainWindow", "ChoosePlaylist", nullptr));
        currentSongLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
