/****************************************************************************
** Meta object code from reading C++ file 'LinkedListVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../widgets/LinkedListVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinkedListVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LinkedListVisualizer_t {
    QByteArrayData data[15];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LinkedListVisualizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LinkedListVisualizer_t qt_meta_stringdata_LinkedListVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 20), // "LinkedListVisualizer"
QT_MOC_LITERAL(1, 21, 17), // "requestBackToMenu"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 21), // "handleInsertBeginning"
QT_MOC_LITERAL(4, 62, 15), // "handleInsertEnd"
QT_MOC_LITERAL(5, 78, 20), // "handleInsertPosition"
QT_MOC_LITERAL(6, 99, 21), // "handleDeleteBeginning"
QT_MOC_LITERAL(7, 121, 15), // "handleDeleteEnd"
QT_MOC_LITERAL(8, 137, 20), // "handleDeletePosition"
QT_MOC_LITERAL(9, 158, 11), // "handleClear"
QT_MOC_LITERAL(10, 170, 20), // "handleGenerateRandom"
QT_MOC_LITERAL(11, 191, 16), // "handleApplyInput"
QT_MOC_LITERAL(12, 208, 16), // "handleBackButton"
QT_MOC_LITERAL(13, 225, 26), // "handleLinkedListTypeButton"
QT_MOC_LITERAL(14, 252, 2) // "id"

    },
    "LinkedListVisualizer\0requestBackToMenu\0"
    "\0handleInsertBeginning\0handleInsertEnd\0"
    "handleInsertPosition\0handleDeleteBeginning\0"
    "handleDeleteEnd\0handleDeletePosition\0"
    "handleClear\0handleGenerateRandom\0"
    "handleApplyInput\0handleBackButton\0"
    "handleLinkedListTypeButton\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LinkedListVisualizer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    0,   81,    2, 0x08 /* Private */,
      10,    0,   82,    2, 0x08 /* Private */,
      11,    0,   83,    2, 0x08 /* Private */,
      12,    0,   84,    2, 0x08 /* Private */,
      13,    1,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

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
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void LinkedListVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LinkedListVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestBackToMenu(); break;
        case 1: _t->handleInsertBeginning(); break;
        case 2: _t->handleInsertEnd(); break;
        case 3: _t->handleInsertPosition(); break;
        case 4: _t->handleDeleteBeginning(); break;
        case 5: _t->handleDeleteEnd(); break;
        case 6: _t->handleDeletePosition(); break;
        case 7: _t->handleClear(); break;
        case 8: _t->handleGenerateRandom(); break;
        case 9: _t->handleApplyInput(); break;
        case 10: _t->handleBackButton(); break;
        case 11: _t->handleLinkedListTypeButton((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LinkedListVisualizer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinkedListVisualizer::requestBackToMenu)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LinkedListVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LinkedListVisualizer.data,
    qt_meta_data_LinkedListVisualizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LinkedListVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinkedListVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LinkedListVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LinkedListVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void LinkedListVisualizer::requestBackToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
