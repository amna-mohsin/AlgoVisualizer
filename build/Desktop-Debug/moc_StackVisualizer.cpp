/****************************************************************************
** Meta object code from reading C++ file 'StackVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../widgets/StackVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StackVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StackVisualizer_t {
    QByteArrayData data[9];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StackVisualizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StackVisualizer_t qt_meta_stringdata_StackVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "StackVisualizer"
QT_MOC_LITERAL(1, 16, 17), // "requestBackToMenu"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "handlePush"
QT_MOC_LITERAL(4, 46, 9), // "handlePop"
QT_MOC_LITERAL(5, 56, 11), // "handleClear"
QT_MOC_LITERAL(6, 68, 20), // "handleGenerateRandom"
QT_MOC_LITERAL(7, 89, 16), // "handleApplyInput"
QT_MOC_LITERAL(8, 106, 16) // "handleBackButton"

    },
    "StackVisualizer\0requestBackToMenu\0\0"
    "handlePush\0handlePop\0handleClear\0"
    "handleGenerateRandom\0handleApplyInput\0"
    "handleBackButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StackVisualizer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void StackVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StackVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestBackToMenu(); break;
        case 1: _t->handlePush(); break;
        case 2: _t->handlePop(); break;
        case 3: _t->handleClear(); break;
        case 4: _t->handleGenerateRandom(); break;
        case 5: _t->handleApplyInput(); break;
        case 6: _t->handleBackButton(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StackVisualizer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StackVisualizer::requestBackToMenu)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject StackVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_StackVisualizer.data,
    qt_meta_data_StackVisualizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StackVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StackVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StackVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int StackVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void StackVisualizer::requestBackToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
