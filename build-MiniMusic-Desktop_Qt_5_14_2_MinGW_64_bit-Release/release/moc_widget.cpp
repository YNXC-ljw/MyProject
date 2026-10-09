/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MiniMusic/widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[45];
    char stringdata0[596];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 16), // "startParseMusics"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "QList<QUrl>"
QT_MOC_LITERAL(4, 37, 4), // "urls"
QT_MOC_LITERAL(5, 42, 15), // "on_quit_clicked"
QT_MOC_LITERAL(6, 58, 14), // "on_min_clicked"
QT_MOC_LITERAL(7, 73, 14), // "on_max_clicked"
QT_MOC_LITERAL(8, 88, 11), // "onBtClicked"
QT_MOC_LITERAL(9, 100, 6), // "pageId"
QT_MOC_LITERAL(10, 107, 22), // "updateLikeMusicAndPage"
QT_MOC_LITERAL(11, 130, 6), // "isLike"
QT_MOC_LITERAL(12, 137, 7), // "musicId"
QT_MOC_LITERAL(13, 145, 17), // "on_volume_clicked"
QT_MOC_LITERAL(14, 163, 19), // "on_addLocal_clicked"
QT_MOC_LITERAL(15, 183, 14), // "onMusicsParsed"
QT_MOC_LITERAL(16, 198, 12), // "QList<Music>"
QT_MOC_LITERAL(17, 211, 6), // "musics"
QT_MOC_LITERAL(18, 218, 12), // "onPlayMiusic"
QT_MOC_LITERAL(19, 231, 15), // "onPlayUpClicked"
QT_MOC_LITERAL(20, 247, 17), // "onPlayDownClicked"
QT_MOC_LITERAL(21, 265, 18), // "onPlayModelClicked"
QT_MOC_LITERAL(22, 284, 14), // "setPlayerMuted"
QT_MOC_LITERAL(23, 299, 7), // "isMuted"
QT_MOC_LITERAL(24, 307, 16), // "onLrcWordClicked"
QT_MOC_LITERAL(25, 324, 15), // "setPlayerVolume"
QT_MOC_LITERAL(26, 340, 6), // "volume"
QT_MOC_LITERAL(27, 347, 9), // "onPlayAll"
QT_MOC_LITERAL(28, 357, 8), // "PageType"
QT_MOC_LITERAL(29, 366, 8), // "pageType"
QT_MOC_LITERAL(30, 375, 24), // "playAllMusicOfCommonPage"
QT_MOC_LITERAL(31, 400, 11), // "CommonPage*"
QT_MOC_LITERAL(32, 412, 4), // "page"
QT_MOC_LITERAL(33, 417, 5), // "index"
QT_MOC_LITERAL(34, 423, 21), // "onCurrentIndexChanged"
QT_MOC_LITERAL(35, 445, 17), // "onDurationChanged"
QT_MOC_LITERAL(36, 463, 8), // "duration"
QT_MOC_LITERAL(37, 472, 17), // "onPositionChanged"
QT_MOC_LITERAL(38, 490, 8), // "position"
QT_MOC_LITERAL(39, 499, 20), // "onMusicSliderChanged"
QT_MOC_LITERAL(40, 520, 5), // "ratio"
QT_MOC_LITERAL(41, 526, 26), // "onMetaDataAvailableChanged"
QT_MOC_LITERAL(42, 553, 9), // "available"
QT_MOC_LITERAL(43, 563, 16), // "playMusicByIndex"
QT_MOC_LITERAL(44, 580, 15) // "on_skin_clicked"

    },
    "Widget\0startParseMusics\0\0QList<QUrl>\0"
    "urls\0on_quit_clicked\0on_min_clicked\0"
    "on_max_clicked\0onBtClicked\0pageId\0"
    "updateLikeMusicAndPage\0isLike\0musicId\0"
    "on_volume_clicked\0on_addLocal_clicked\0"
    "onMusicsParsed\0QList<Music>\0musics\0"
    "onPlayMiusic\0onPlayUpClicked\0"
    "onPlayDownClicked\0onPlayModelClicked\0"
    "setPlayerMuted\0isMuted\0onLrcWordClicked\0"
    "setPlayerVolume\0volume\0onPlayAll\0"
    "PageType\0pageType\0playAllMusicOfCommonPage\0"
    "CommonPage*\0page\0index\0onCurrentIndexChanged\0"
    "onDurationChanged\0duration\0onPositionChanged\0"
    "position\0onMusicSliderChanged\0ratio\0"
    "onMetaDataAvailableChanged\0available\0"
    "playMusicByIndex\0on_skin_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  139,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,  142,    2, 0x08 /* Private */,
       6,    0,  143,    2, 0x08 /* Private */,
       7,    0,  144,    2, 0x08 /* Private */,
       8,    1,  145,    2, 0x08 /* Private */,
      10,    2,  148,    2, 0x08 /* Private */,
      13,    0,  153,    2, 0x08 /* Private */,
      14,    0,  154,    2, 0x08 /* Private */,
      15,    1,  155,    2, 0x08 /* Private */,
      18,    0,  158,    2, 0x08 /* Private */,
      19,    0,  159,    2, 0x08 /* Private */,
      20,    0,  160,    2, 0x08 /* Private */,
      21,    0,  161,    2, 0x08 /* Private */,
      22,    1,  162,    2, 0x08 /* Private */,
      24,    0,  165,    2, 0x08 /* Private */,
      25,    1,  166,    2, 0x08 /* Private */,
      27,    1,  169,    2, 0x08 /* Private */,
      30,    2,  172,    2, 0x08 /* Private */,
      34,    1,  177,    2, 0x08 /* Private */,
      35,    1,  180,    2, 0x08 /* Private */,
      37,    1,  183,    2, 0x08 /* Private */,
      39,    1,  186,    2, 0x08 /* Private */,
      41,    1,  189,    2, 0x08 /* Private */,
      43,    2,  192,    2, 0x08 /* Private */,
      44,    0,  197,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   26,
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, 0x80000000 | 31, QMetaType::Int,   32,   33,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::LongLong,   36,
    QMetaType::Void, QMetaType::LongLong,   38,
    QMetaType::Void, QMetaType::Float,   40,
    QMetaType::Void, QMetaType::Bool,   42,
    QMetaType::Void, 0x80000000 | 31, QMetaType::Int,   32,   33,
    QMetaType::Void,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startParseMusics((*reinterpret_cast< const QList<QUrl>(*)>(_a[1]))); break;
        case 1: _t->on_quit_clicked(); break;
        case 2: _t->on_min_clicked(); break;
        case 3: _t->on_max_clicked(); break;
        case 4: _t->onBtClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->updateLikeMusicAndPage((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->on_volume_clicked(); break;
        case 7: _t->on_addLocal_clicked(); break;
        case 8: _t->onMusicsParsed((*reinterpret_cast< const QList<Music>(*)>(_a[1]))); break;
        case 9: _t->onPlayMiusic(); break;
        case 10: _t->onPlayUpClicked(); break;
        case 11: _t->onPlayDownClicked(); break;
        case 12: _t->onPlayModelClicked(); break;
        case 13: _t->setPlayerMuted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->onLrcWordClicked(); break;
        case 15: _t->setPlayerVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->onPlayAll((*reinterpret_cast< PageType(*)>(_a[1]))); break;
        case 17: _t->playAllMusicOfCommonPage((*reinterpret_cast< CommonPage*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->onCurrentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onDurationChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 20: _t->onPositionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 21: _t->onMusicSliderChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 22: _t->onMetaDataAvailableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->playMusicByIndex((*reinterpret_cast< CommonPage*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: _t->on_skin_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QUrl> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<Music> >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CommonPage* >(); break;
            }
            break;
        case 23:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CommonPage* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Widget::*)(const QList<QUrl> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Widget::startParseMusics)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Widget.data,
    qt_meta_data_Widget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void Widget::startParseMusics(const QList<QUrl> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
