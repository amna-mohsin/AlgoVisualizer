/****************************************************************************
** Meta object code from reading C++ file 'QueueVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../widgets/QueueVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QueueVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QueueVisualizer_t {
    QByteArrayData data[11];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QueueVisualizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QueueVisualizer_t qt_meta_stringdata_QueueVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QueueVisualizer"
QT_MOC_LITERAL(1, 16, 17), // "requestBackToMenu"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "handleEnqueue"
QT_MOC_LITERAL(4, 49, 13), // "handleDequeue"
QT_MOC_LITERAL(5, 63, 11), // "handleClear"
QT_MOC_LITERAL(6, 75, 20), // "handleGenerateRandom"
QT_MOC_LITERAL(7, 96, 16), // "handleApplyInput"
QT_MOC_LITERAL(8, 113, 16), // "handleBackButton"
QT_MOC_LITERAL(9, 130, 21), // "handleQueueTypeButton"
QT_MOC_LITERAL(10, 152, 2) // "id"

    },
    "QueueVisualizer\0requestBackToMenu\0\0"
    "handleEnqueue\0handleDequeue\0handleClear\0"
    "handleGenerateRandom\0handleApplyInput\0"
    "handleBackButton\0handleQueueTypeButton\0"
    "id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QueueVisualizer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

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

       0        // eod
};

void QueueVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QueueVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestBackToMenu(); break;
        case 1: _t->handleEnqueue(); break;
        case 2: _t->handleDequeue(); break;
        case 3: _t->handleClear(); break;
        case 4: _t->handleGenerateRandom(); break;
        case 5: _t->handleApplyInput(); break;
        case 6: _t->handleBackButton(); break;
        case 7: _t->handleQueueTypeButton((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QueueVisualizer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QueueVisualizer::requestBackToMenu)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QueueVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_QueueVisualizer.data,
    qt_meta_data_QueueVisualizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QueueVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QueueVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QueueVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QueueVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QueueVisualizer::requestBackToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
