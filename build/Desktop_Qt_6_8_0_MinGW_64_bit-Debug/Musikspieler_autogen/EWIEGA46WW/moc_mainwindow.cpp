/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onChoosePlaylistButtonClicked",
    "",
    "onPlaylistItemClicked",
    "QListWidgetItem*",
    "item",
    "onPlayButtonPressed",
    "onVolumeChanged",
    "value",
    "onPauseButtonPressed",
    "onNextButtonPressed",
    "onSliderPressed",
    "onPreviousButtonPressed",
    "updateProgressBar",
    "currentSongPosition",
    "onSliderReleased",
    "loadTracksFromDatabase",
    "setProgressBarAndSongDurationLabel",
    "setCurrentSongDuration",
    "Track",
    "currentTrack",
    "updateCurrentTrackInfo",
    "index",
    "title",
    "onRandomButtonPressed",
    "onRepeatButtonPressed",
    "randomizePlaylist",
    "onSearchTextChanged",
    "text"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  122,    2, 0x08,    1 /* Private */,
       3,    1,  123,    2, 0x08,    2 /* Private */,
       6,    0,  126,    2, 0x08,    4 /* Private */,
       7,    1,  127,    2, 0x08,    5 /* Private */,
       9,    0,  130,    2, 0x08,    7 /* Private */,
      10,    0,  131,    2, 0x08,    8 /* Private */,
      11,    0,  132,    2, 0x08,    9 /* Private */,
      12,    0,  133,    2, 0x08,   10 /* Private */,
      13,    1,  134,    2, 0x08,   11 /* Private */,
      15,    0,  137,    2, 0x08,   13 /* Private */,
      16,    0,  138,    2, 0x08,   14 /* Private */,
      17,    0,  139,    2, 0x08,   15 /* Private */,
      18,    1,  140,    2, 0x08,   16 /* Private */,
      21,    2,  143,    2, 0x08,   18 /* Private */,
      24,    0,  148,    2, 0x08,   21 /* Private */,
      25,    0,  149,    2, 0x08,   22 /* Private */,
      26,    0,  150,    2, 0x08,   23 /* Private */,
      27,    1,  151,    2, 0x08,   24 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   28,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onChoosePlaylistButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPlaylistItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onPlayButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onVolumeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'onPauseButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNextButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSliderPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPreviousButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateProgressBar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'onSliderReleased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadTracksFromDatabase'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setProgressBarAndSongDurationLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setCurrentSongDuration'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Track, std::false_type>,
        // method 'updateCurrentTrackInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onRandomButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRepeatButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'randomizePlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onChoosePlaylistButtonClicked(); break;
        case 1: _t->onPlaylistItemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 2: _t->onPlayButtonPressed(); break;
        case 3: _t->onVolumeChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->onPauseButtonPressed(); break;
        case 5: _t->onNextButtonPressed(); break;
        case 6: _t->onSliderPressed(); break;
        case 7: _t->onPreviousButtonPressed(); break;
        case 8: _t->updateProgressBar((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 9: _t->onSliderReleased(); break;
        case 10: _t->loadTracksFromDatabase(); break;
        case 11: _t->setProgressBarAndSongDurationLabel(); break;
        case 12: _t->setCurrentSongDuration((*reinterpret_cast< std::add_pointer_t<Track>>(_a[1]))); break;
        case 13: _t->updateCurrentTrackInfo((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->onRandomButtonPressed(); break;
        case 15: _t->onRepeatButtonPressed(); break;
        case 16: _t->randomizePlaylist(); break;
        case 17: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
