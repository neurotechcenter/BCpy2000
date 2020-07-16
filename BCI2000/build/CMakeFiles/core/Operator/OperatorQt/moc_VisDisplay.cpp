/****************************************************************************
** Meta object code from reading C++ file 'VisDisplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/core/Operator/OPERAT~2/VisDisplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VisDisplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VisDisplayProxy_t {
    QByteArrayData data[14];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VisDisplayProxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VisDisplayProxy_t qt_meta_stringdata_VisDisplayProxy = {
    {
QT_MOC_LITERAL(0, 0, 15), // "VisDisplayProxy"
QT_MOC_LITERAL(1, 16, 6), // "Create"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "std::string"
QT_MOC_LITERAL(4, 36, 5), // "visID"
QT_MOC_LITERAL(5, 42, 4), // "kind"
QT_MOC_LITERAL(6, 47, 12), // "HandleSignal"
QT_MOC_LITERAL(7, 60, 8), // "channels"
QT_MOC_LITERAL(8, 69, 18), // "std::vector<float>"
QT_MOC_LITERAL(9, 88, 10), // "HandleMemo"
QT_MOC_LITERAL(10, 99, 12), // "HandleBitmap"
QT_MOC_LITERAL(11, 112, 11), // "BitmapImage"
QT_MOC_LITERAL(12, 124, 14), // "HandleProperty"
QT_MOC_LITERAL(13, 139, 5) // "Clear"

    },
    "VisDisplayProxy\0Create\0\0std::string\0"
    "visID\0kind\0HandleSignal\0channels\0"
    "std::vector<float>\0HandleMemo\0"
    "HandleBitmap\0BitmapImage\0HandleProperty\0"
    "Clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VisDisplayProxy[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x08 /* Private */,
       6,    3,   49,    2, 0x08 /* Private */,
       9,    2,   56,    2, 0x08 /* Private */,
      10,    2,   61,    2, 0x08 /* Private */,
      12,    4,   66,    2, 0x08 /* Private */,
      13,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, 0x80000000 | 8,    4,    7,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 11,    4,    2,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, 0x80000000 | 3, QMetaType::Int,    4,    2,    2,    2,
    QMetaType::Void,

       0        // eod
};

void VisDisplayProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VisDisplayProxy *_t = static_cast<VisDisplayProxy *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Create((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 1: _t->HandleSignal((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const std::vector<float>(*)>(_a[3]))); break;
        case 2: _t->HandleMemo((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 3: _t->HandleBitmap((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const BitmapImage(*)>(_a[2]))); break;
        case 4: _t->HandleProperty((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const std::string(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: _t->Clear(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VisDisplayProxy::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_VisDisplayProxy.data,
    qt_meta_data_VisDisplayProxy,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VisDisplayProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VisDisplayProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VisDisplayProxy.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VisDisplayProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
