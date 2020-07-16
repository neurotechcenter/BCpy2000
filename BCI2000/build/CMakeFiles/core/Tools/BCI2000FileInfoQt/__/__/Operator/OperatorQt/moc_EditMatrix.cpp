/****************************************************************************
** Meta object code from reading C++ file 'EditMatrix.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../../src/core/Operator/OPERAT~2/EditMatrix.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditMatrix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EditMatrix_t {
    QByteArrayData data[11];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EditMatrix_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EditMatrix_t qt_meta_stringdata_EditMatrix = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EditMatrix"
QT_MOC_LITERAL(1, 11, 23), // "OnChangeMatrixSizeClick"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 7), // "OnClose"
QT_MOC_LITERAL(4, 44, 15), // "ShowContextMenu"
QT_MOC_LITERAL(5, 60, 20), // "PerformContextAction"
QT_MOC_LITERAL(6, 81, 8), // "QAction*"
QT_MOC_LITERAL(7, 90, 10), // "EditHLabel"
QT_MOC_LITERAL(8, 101, 10), // "EditVLabel"
QT_MOC_LITERAL(9, 112, 8), // "EditItem"
QT_MOC_LITERAL(10, 121, 17) // "QTableWidgetItem*"

    },
    "EditMatrix\0OnChangeMatrixSizeClick\0\0"
    "OnClose\0ShowContextMenu\0PerformContextAction\0"
    "QAction*\0EditHLabel\0EditVLabel\0EditItem\0"
    "QTableWidgetItem*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EditMatrix[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    1,   51,    2, 0x08 /* Private */,
       5,    1,   54,    2, 0x08 /* Private */,
       7,    1,   57,    2, 0x08 /* Private */,
       8,    1,   60,    2, 0x08 /* Private */,
       9,    1,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 10,    2,

       0        // eod
};

void EditMatrix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditMatrix *_t = static_cast<EditMatrix *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnChangeMatrixSizeClick(); break;
        case 1: _t->OnClose(); break;
        case 2: _t->ShowContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: _t->PerformContextAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 4: _t->EditHLabel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->EditVLabel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->EditItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EditMatrix::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_EditMatrix.data,
    qt_meta_data_EditMatrix,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EditMatrix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditMatrix::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EditMatrix.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int EditMatrix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
