/****************************************************************************
** Meta object code from reading C++ file 'GraphVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../widgets/GraphVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GraphVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GraphVisualizer_t {
    QByteArrayData data[11];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphVisualizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphVisualizer_t qt_meta_stringdata_GraphVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "GraphVisualizer"
QT_MOC_LITERAL(1, 16, 17), // "requestBackToMenu"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 16), // "handleBuildGraph"
QT_MOC_LITERAL(4, 52, 13), // "handleAddEdge"
QT_MOC_LITERAL(5, 66, 18), // "handleRunAlgorithm"
QT_MOC_LITERAL(6, 85, 11), // "handleClear"
QT_MOC_LITERAL(7, 97, 17), // "handleRandomGraph"
QT_MOC_LITERAL(8, 115, 16), // "handleBackButton"
QT_MOC_LITERAL(9, 132, 21), // "handleAlgorithmButton"
QT_MOC_LITERAL(10, 154, 2) // "id"

    },
    "GraphVisualizer\0requestBackToMenu\0\0"
    "handleBuildGraph\0handleAddEdge\0"
    "handleRunAlgorithm\0handleClear\0"
    "handleRandomGraph\0handleBackButton\0"
    "handleAlgorithmButton\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphVisualizer[] = {

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

void GraphVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GraphVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestBackToMenu(); break;
        case 1: _t->handleBuildGraph(); break;
        case 2: _t->handleAddEdge(); break;
        case 3: _t->handleRunAlgorithm(); break;
        case 4: _t->handleClear(); break;
        case 5: _t->handleRandomGraph(); break;
        case 6: _t->handleBackButton(); break;
        case 7: _t->handleAlgorithmButton((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GraphVisualizer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphVisualizer::requestBackToMenu)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GraphVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_GraphVisualizer.data,
    qt_meta_data_GraphVisualizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GraphVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GraphVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void GraphVisualizer::requestBackToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
