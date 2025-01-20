/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
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
    "onPlaylistItemClicked",
    "",
    "QListWidgetItem*",
    "item",
    "onSongTableItemClicked",
    "QModelIndex",
    "index",
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
    "setProgressBarAndSongDurationLabel",
    "setCurrentSongDuration",
    "Track",
    "currentTrack",
    "updateCurrentTrackInfo",
    "onRandomButtonPressed",
    "onRepeatButtonPressed",
    "onSearchTextChanged",
    "text",
    "onAddPlaylistButtonClicked",
    "onAddTrackButtonClicked",
    "onRemovePlaylistButtonClicked",
    "onAddFolderButtonClicked",
    "onRefreshFilesButtonClicked",
    "onPlaylistSammlungItemClicked",
    "loadPlaylistsFromDatabase",
    "updateCurrentTrackDisplay"
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
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  158,    2, 0x08,    1 /* Private */,
       5,    1,  161,    2, 0x08,    3 /* Private */,
       8,    0,  164,    2, 0x08,    5 /* Private */,
       9,    1,  165,    2, 0x08,    6 /* Private */,
      11,    0,  168,    2, 0x08,    8 /* Private */,
      12,    0,  169,    2, 0x08,    9 /* Private */,
      13,    0,  170,    2, 0x08,   10 /* Private */,
      14,    0,  171,    2, 0x08,   11 /* Private */,
      15,    1,  172,    2, 0x08,   12 /* Private */,
      17,    0,  175,    2, 0x08,   14 /* Private */,
      18,    0,  176,    2, 0x08,   15 /* Private */,
      19,    1,  177,    2, 0x08,   16 /* Private */,
      22,    0,  180,    2, 0x08,   18 /* Private */,
      23,    0,  181,    2, 0x08,   19 /* Private */,
      24,    0,  182,    2, 0x08,   20 /* Private */,
      25,    1,  183,    2, 0x08,   21 /* Private */,
      27,    0,  186,    2, 0x08,   23 /* Private */,
      28,    0,  187,    2, 0x08,   24 /* Private */,
      29,    0,  188,    2, 0x08,   25 /* Private */,
      30,    0,  189,    2, 0x08,   26 /* Private */,
      31,    0,  190,    2, 0x08,   27 /* Private */,
      32,    1,  191,    2, 0x08,   28 /* Private */,
      33,    0,  194,    2, 0x08,   30 /* Private */,
      34,    0,  195,    2, 0x08,   31 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

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
        // method 'onPlaylistItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onSongTableItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
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
        // method 'setProgressBarAndSongDurationLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setCurrentSongDuration'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Track, std::false_type>,
        // method 'updateCurrentTrackInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRandomButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRepeatButtonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onAddPlaylistButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddTrackButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRemovePlaylistButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddFolderButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRefreshFilesButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPlaylistSammlungItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'loadPlaylistsFromDatabase'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateCurrentTrackDisplay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onPlaylistItemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 1: _t->onSongTableItemClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 2: _t->onPlayButtonPressed(); break;
        case 3: _t->onVolumeChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->onPauseButtonPressed(); break;
        case 5: _t->onNextButtonPressed(); break;
        case 6: _t->onSliderPressed(); break;
        case 7: _t->onPreviousButtonPressed(); break;
        case 8: _t->updateProgressBar((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 9: _t->onSliderReleased(); break;
        case 10: _t->setProgressBarAndSongDurationLabel(); break;
        case 11: _t->setCurrentSongDuration((*reinterpret_cast< std::add_pointer_t<Track>>(_a[1]))); break;
        case 12: _t->updateCurrentTrackInfo(); break;
        case 13: _t->onRandomButtonPressed(); break;
        case 14: _t->onRepeatButtonPressed(); break;
        case 15: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->onAddPlaylistButtonClicked(); break;
        case 17: _t->onAddTrackButtonClicked(); break;
        case 18: _t->onRemovePlaylistButtonClicked(); break;
        case 19: _t->onAddFolderButtonClicked(); break;
        case 20: _t->onRefreshFilesButtonClicked(); break;
        case 21: _t->onPlaylistSammlungItemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 22: _t->loadPlaylistsFromDatabase(); break;
        case 23: _t->updateCurrentTrackDisplay(); break;
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
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    return _id;
}
QT_WARNING_POP
