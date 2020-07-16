/****************************************************************************
** Meta object code from reading C++ file 'WatchesWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/core/Operator/OPERAT~2/WatchesWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WatchesWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WatchesWindow_t {
    QByteArrayData data[12];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WatchesWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WatchesWindow_t qt_meta_stringdata_WatchesWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "WatchesWindow"
QT_MOC_LITERAL(1, 14, 13), // "OnItemChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(4, 47, 16), // "OnValuesReceived"
QT_MOC_LITERAL(5, 64, 19), // "OnHeaderDoubleClick"
QT_MOC_LITERAL(6, 84, 12), // "AddEmptyLine"
QT_MOC_LITERAL(7, 97, 14), // "RemoveLastLine"
QT_MOC_LITERAL(8, 112, 23), // "SetHistoryLengthSeconds"
QT_MOC_LITERAL(9, 136, 11), // "SaveWatches"
QT_MOC_LITERAL(10, 148, 14), // "RestoreWatches"
QT_MOC_LITERAL(11, 163, 12) // "ClearWatches"

    },
    "WatchesWindow\0OnItemChanged\0\0"
    "QTableWidgetItem*\0OnValuesReceived\0"
    "OnHeaderDoubleClick\0AddEmptyLine\0"
    "RemoveLastLine\0SetHistoryLengthSeconds\0"
    "SaveWatches\0RestoreWatches\0ClearWatches"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WatchesWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08 /* Private */,
       4,    3,   62,    2, 0x08 /* Private */,
       5,    1,   69,    2, 0x08 /* Private */,
       6,    0,   72,    2, 0x08 /* Private */,
       7,    0,   73,    2, 0x08 /* Private */,
       8,    1,   74,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    0,   78,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::VoidStar, QMetaType::QString, QMetaType::Double,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WatchesWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WatchesWindow *_t = static_cast<WatchesWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnItemChanged((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->OnValuesReceived((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 2: _t->OnHeaderDoubleClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->AddEmptyLine(); break;
        case 4: _t->RemoveLastLine(); break;
        case 5: _t->SetHistoryLengthSeconds((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->SaveWatches(); break;
        case 7: _t->RestoreWatches(); break;
        case 8: _t->ClearWatches(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WatchesWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_WatchesWindow.data,
    qt_meta_data_WatchesWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WatchesWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WatchesWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WatchesWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int WatchesWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
