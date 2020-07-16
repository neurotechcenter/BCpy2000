/****************************************************************************
** Meta object code from reading C++ file 'AnalysisGui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/core/Tools/BC4825~1/BCI200~1/AnalysisGui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnalysisGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnalysisGui_t {
    QByteArrayData data[15];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnalysisGui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnalysisGui_t qt_meta_stringdata_AnalysisGui = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AnalysisGui"
QT_MOC_LITERAL(1, 12, 21), // "onRequirementsChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 18), // "onRunButtonClicked"
QT_MOC_LITERAL(4, 54, 19), // "onSaveButtonClicked"
QT_MOC_LITERAL(5, 74, 27), // "onSetOutputDirButtonClicked"
QT_MOC_LITERAL(6, 102, 18), // "onProgressCanceled"
QT_MOC_LITERAL(7, 121, 15), // "onAnalyzerBegin"
QT_MOC_LITERAL(8, 137, 14), // "onFileProgress"
QT_MOC_LITERAL(9, 152, 3), // "cur"
QT_MOC_LITERAL(10, 156, 5), // "total"
QT_MOC_LITERAL(11, 162, 4), // "info"
QT_MOC_LITERAL(12, 167, 14), // "onDataProgress"
QT_MOC_LITERAL(13, 182, 14), // "onAnalyzerDone"
QT_MOC_LITERAL(14, 197, 20) // "onResultsContextMenu"

    },
    "AnalysisGui\0onRequirementsChanged\0\0"
    "onRunButtonClicked\0onSaveButtonClicked\0"
    "onSetOutputDirButtonClicked\0"
    "onProgressCanceled\0onAnalyzerBegin\0"
    "onFileProgress\0cur\0total\0info\0"
    "onDataProgress\0onAnalyzerDone\0"
    "onResultsContextMenu"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnalysisGui[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    3,   70,    2, 0x08 /* Private */,
      12,    2,   77,    2, 0x08 /* Private */,
      13,    0,   82,    2, 0x08 /* Private */,
      14,    1,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,    9,   10,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,

       0        // eod
};

void AnalysisGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnalysisGui *_t = static_cast<AnalysisGui *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onRequirementsChanged(); break;
        case 1: _t->onRunButtonClicked(); break;
        case 2: _t->onSaveButtonClicked(); break;
        case 3: _t->onSetOutputDirButtonClicked(); break;
        case 4: _t->onProgressCanceled(); break;
        case 5: _t->onAnalyzerBegin(); break;
        case 6: _t->onFileProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->onDataProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->onAnalyzerDone(); break;
        case 9: _t->onResultsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AnalysisGui::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_AnalysisGui.data,
    qt_meta_data_AnalysisGui,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AnalysisGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnalysisGui::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnalysisGui.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AnalysisGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
