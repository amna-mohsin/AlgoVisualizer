/****************************************************************************
** Meta object code from reading C++ file 'TreeVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "widgets/TreeVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TreeVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TreeVisualizer_t {
    QByteArrayData data[12];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TreeVisualizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TreeVisualizer_t qt_meta_stringdata_TreeVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TreeVisualizer"
QT_MOC_LITERAL(1, 15, 17), // "requestBackToMenu"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 15), // "handleBuildTree"
QT_MOC_LITERAL(4, 50, 9), // "handleRun"
QT_MOC_LITERAL(5, 60, 10), // "handleStop"
QT_MOC_LITERAL(6, 71, 11), // "handleClear"
QT_MOC_LITERAL(7, 83, 16), // "handleRandomTree"
QT_MOC_LITERAL(8, 100, 16), // "handleApplyInput"
QT_MOC_LITERAL(9, 117, 16), // "handleBackButton"
QT_MOC_LITERAL(10, 134, 25), // "handleTraversalTypeButton"
QT_MOC_LITERAL(11, 160, 2) // "id"

    },
    "TreeVisualizer\0requestBackToMenu\0\0"
    "handleBuildTree\0handleRun\0handleStop\0"
    "handleClear\0handleRandomTree\0"
    "handleApplyInput\0handleBackButton\0"
    "handleTraversalTypeButton\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TreeVisualizer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,

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
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void TreeVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TreeVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestBackToMenu(); break;
        case 1: _t->handleBuildTree(); break;
        case 2: _t->handleRun(); break;
        case 3: _t->handleStop(); break;
        case 4: _t->handleClear(); break;
        case 5: _t->handleRandomTree(); break;
        case 6: _t->handleApplyInput(); break;
        case 7: _t->handleBackButton(); break;
        case 8: _t->handleTraversalTypeButton((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TreeVisualizer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TreeVisualizer::requestBackToMenu)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TreeVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_TreeVisualizer.data,
    qt_meta_data_TreeVisualizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TreeVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TreeVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TreeVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TreeVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TreeVisualizer::requestBackToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
