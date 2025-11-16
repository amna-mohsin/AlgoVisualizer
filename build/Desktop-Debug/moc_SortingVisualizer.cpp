/****************************************************************************
** Meta object code from reading C++ file 'SortingVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../widgets/SortingVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SortingVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SortingVisualizer_t {
    QByteArrayData data[14];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SortingVisualizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SortingVisualizer_t qt_meta_stringdata_SortingVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SortingVisualizer"
QT_MOC_LITERAL(1, 18, 17), // "requestBackToMenu"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 10), // "handlePlay"
QT_MOC_LITERAL(4, 48, 10), // "handleStep"
QT_MOC_LITERAL(5, 59, 11), // "handleReset"
QT_MOC_LITERAL(6, 71, 17), // "handleAutoAdvance"
QT_MOC_LITERAL(7, 89, 20), // "handleGenerateRandom"
QT_MOC_LITERAL(8, 110, 16), // "handleApplyInput"
QT_MOC_LITERAL(9, 127, 18), // "handleSpeedChanged"
QT_MOC_LITERAL(10, 146, 5), // "value"
QT_MOC_LITERAL(11, 152, 21), // "handleAlgorithmButton"
QT_MOC_LITERAL(12, 174, 2), // "id"
QT_MOC_LITERAL(13, 177, 16) // "handleBackButton"

    },
    "SortingVisualizer\0requestBackToMenu\0"
    "\0handlePlay\0handleStep\0handleReset\0"
    "handleAutoAdvance\0handleGenerateRandom\0"
    "handleApplyInput\0handleSpeedChanged\0"
    "value\0handleAlgorithmButton\0id\0"
    "handleBackButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SortingVisualizer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    1,   71,    2, 0x08 /* Private */,
      11,    1,   74,    2, 0x08 /* Private */,
      13,    0,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void SortingVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SortingVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestBackToMenu(); break;
        case 1: _t->handlePlay(); break;
        case 2: _t->handleStep(); break;
        case 3: _t->handleReset(); break;
        case 4: _t->handleAutoAdvance(); break;
        case 5: _t->handleGenerateRandom(); break;
        case 6: _t->handleApplyInput(); break;
        case 7: _t->handleSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->handleAlgorithmButton((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->handleBackButton(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SortingVisualizer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SortingVisualizer::requestBackToMenu)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SortingVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SortingVisualizer.data,
    qt_meta_data_SortingVisualizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SortingVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SortingVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SortingVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SortingVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SortingVisualizer::requestBackToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
