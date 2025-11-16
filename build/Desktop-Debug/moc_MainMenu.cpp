/****************************************************************************
** Meta object code from reading C++ file 'MainMenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MainMenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainMenu_t {
    QByteArrayData data[16];
    char stringdata0[360];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainMenu_t qt_meta_stringdata_MainMenu = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MainMenu"
QT_MOC_LITERAL(1, 9, 22), // "on_sortingCard_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 20), // "on_stackCard_clicked"
QT_MOC_LITERAL(4, 54, 20), // "on_queueCard_clicked"
QT_MOC_LITERAL(5, 75, 25), // "on_linkedListCard_clicked"
QT_MOC_LITERAL(6, 101, 19), // "on_treeCard_clicked"
QT_MOC_LITERAL(7, 121, 20), // "on_graphCard_clicked"
QT_MOC_LITERAL(8, 142, 21), // "on_stringCard_clicked"
QT_MOC_LITERAL(9, 164, 28), // "on_backButtonSorting_clicked"
QT_MOC_LITERAL(10, 193, 26), // "on_backButtonStack_clicked"
QT_MOC_LITERAL(11, 220, 26), // "on_backButtonQueue_clicked"
QT_MOC_LITERAL(12, 247, 31), // "on_backButtonLinkedList_clicked"
QT_MOC_LITERAL(13, 279, 25), // "on_backButtonTree_clicked"
QT_MOC_LITERAL(14, 305, 26), // "on_backButtonGraph_clicked"
QT_MOC_LITERAL(15, 332, 27) // "on_backButtonString_clicked"

    },
    "MainMenu\0on_sortingCard_clicked\0\0"
    "on_stackCard_clicked\0on_queueCard_clicked\0"
    "on_linkedListCard_clicked\0on_treeCard_clicked\0"
    "on_graphCard_clicked\0on_stringCard_clicked\0"
    "on_backButtonSorting_clicked\0"
    "on_backButtonStack_clicked\0"
    "on_backButtonQueue_clicked\0"
    "on_backButtonLinkedList_clicked\0"
    "on_backButtonTree_clicked\0"
    "on_backButtonGraph_clicked\0"
    "on_backButtonString_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainMenu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainMenu *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_sortingCard_clicked(); break;
        case 1: _t->on_stackCard_clicked(); break;
        case 2: _t->on_queueCard_clicked(); break;
        case 3: _t->on_linkedListCard_clicked(); break;
        case 4: _t->on_treeCard_clicked(); break;
        case 5: _t->on_graphCard_clicked(); break;
        case 6: _t->on_stringCard_clicked(); break;
        case 7: _t->on_backButtonSorting_clicked(); break;
        case 8: _t->on_backButtonStack_clicked(); break;
        case 9: _t->on_backButtonQueue_clicked(); break;
        case 10: _t->on_backButtonLinkedList_clicked(); break;
        case 11: _t->on_backButtonTree_clicked(); break;
        case 12: _t->on_backButtonGraph_clicked(); break;
        case 13: _t->on_backButtonString_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject MainMenu::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainMenu.data,
    qt_meta_data_MainMenu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainMenu.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
