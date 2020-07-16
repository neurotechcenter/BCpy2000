/****************************************************************************
** Meta object code from reading C++ file 'pages.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/core/Tools/P300CL~1/pages.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pages.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ApplyFeatureWeightsThread_t {
    QByteArrayData data[8];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ApplyFeatureWeightsThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ApplyFeatureWeightsThread_t qt_meta_stringdata_ApplyFeatureWeightsThread = {
    {
QT_MOC_LITERAL(0, 0, 25), // "ApplyFeatureWeightsThread"
QT_MOC_LITERAL(1, 26, 17), // "signalProgressBar"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 4), // "done"
QT_MOC_LITERAL(4, 50, 5), // "total"
QT_MOC_LITERAL(5, 56, 5), // "stage"
QT_MOC_LITERAL(6, 62, 18), // "signalProgressText"
QT_MOC_LITERAL(7, 81, 7) // "message"

    },
    "ApplyFeatureWeightsThread\0signalProgressBar\0"
    "\0done\0total\0stage\0signalProgressText\0"
    "message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ApplyFeatureWeightsThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       6,    1,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void ApplyFeatureWeightsThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ApplyFeatureWeightsThread *_t = static_cast<ApplyFeatureWeightsThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalProgressBar((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->signalProgressText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ApplyFeatureWeightsThread::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ApplyFeatureWeightsThread::signalProgressBar)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ApplyFeatureWeightsThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ApplyFeatureWeightsThread::signalProgressText)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ApplyFeatureWeightsThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_ApplyFeatureWeightsThread.data,
    qt_meta_data_ApplyFeatureWeightsThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ApplyFeatureWeightsThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApplyFeatureWeightsThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ApplyFeatureWeightsThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ApplyFeatureWeightsThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ApplyFeatureWeightsThread::signalProgressBar(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ApplyFeatureWeightsThread::signalProgressText(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_GenerateFeatureWeightsThread_t {
    QByteArrayData data[9];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GenerateFeatureWeightsThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GenerateFeatureWeightsThread_t qt_meta_stringdata_GenerateFeatureWeightsThread = {
    {
QT_MOC_LITERAL(0, 0, 28), // "GenerateFeatureWeightsThread"
QT_MOC_LITERAL(1, 29, 17), // "signalProgressBar"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 4), // "done"
QT_MOC_LITERAL(4, 53, 5), // "total"
QT_MOC_LITERAL(5, 59, 5), // "stage"
QT_MOC_LITERAL(6, 65, 18), // "signalProgressText"
QT_MOC_LITERAL(7, 84, 7), // "message"
QT_MOC_LITERAL(8, 92, 12) // "errorMessage"

    },
    "GenerateFeatureWeightsThread\0"
    "signalProgressBar\0\0done\0total\0stage\0"
    "signalProgressText\0message\0errorMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GenerateFeatureWeightsThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,
       6,    1,   36,    2, 0x06 /* Public */,
       8,    1,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void GenerateFeatureWeightsThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GenerateFeatureWeightsThread *_t = static_cast<GenerateFeatureWeightsThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalProgressBar((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->signalProgressText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->errorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GenerateFeatureWeightsThread::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GenerateFeatureWeightsThread::signalProgressBar)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GenerateFeatureWeightsThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GenerateFeatureWeightsThread::signalProgressText)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GenerateFeatureWeightsThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GenerateFeatureWeightsThread::errorMessage)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GenerateFeatureWeightsThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_GenerateFeatureWeightsThread.data,
    qt_meta_data_GenerateFeatureWeightsThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GenerateFeatureWeightsThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenerateFeatureWeightsThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GenerateFeatureWeightsThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int GenerateFeatureWeightsThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GenerateFeatureWeightsThread::signalProgressBar(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GenerateFeatureWeightsThread::signalProgressText(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GenerateFeatureWeightsThread::errorMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_DataPage_t {
    QByteArrayData data[37];
    char stringdata0[480];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataPage_t qt_meta_stringdata_DataPage = {
    {
QT_MOC_LITERAL(0, 0, 8), // "DataPage"
QT_MOC_LITERAL(1, 9, 12), // "LoadSettings"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 16), // "ValidateSettings"
QT_MOC_LITERAL(4, 40, 18), // "LoadTrainingButton"
QT_MOC_LITERAL(5, 59, 17), // "LoadTestingButton"
QT_MOC_LITERAL(6, 77, 13), // "LoadIniButton"
QT_MOC_LITERAL(7, 91, 10), // "ReadVector"
QT_MOC_LITERAL(8, 102, 11), // "inputvector"
QT_MOC_LITERAL(9, 114, 13), // "QVector<int>*"
QT_MOC_LITERAL(10, 128, 13), // "poutputvector"
QT_MOC_LITERAL(11, 142, 25), // "ValidateTrainingDataFiles"
QT_MOC_LITERAL(12, 168, 15), // "vector<string>&"
QT_MOC_LITERAL(13, 184, 8), // "fPathArr"
QT_MOC_LITERAL(14, 193, 4), // "int&"
QT_MOC_LITERAL(15, 198, 10), // "numSamples"
QT_MOC_LITERAL(16, 209, 11), // "numChannels"
QT_MOC_LITERAL(17, 221, 4), // "mode"
QT_MOC_LITERAL(18, 226, 24), // "ValidateTestingDataFiles"
QT_MOC_LITERAL(19, 251, 16), // "GenerateFeatures"
QT_MOC_LITERAL(20, 268, 13), // "ApplyFeatures"
QT_MOC_LITERAL(21, 282, 16), // "slotProgressText"
QT_MOC_LITERAL(22, 299, 7), // "message"
QT_MOC_LITERAL(23, 307, 15), // "slotProgressBar"
QT_MOC_LITERAL(24, 323, 4), // "done"
QT_MOC_LITERAL(25, 328, 5), // "total"
QT_MOC_LITERAL(26, 334, 5), // "stage"
QT_MOC_LITERAL(27, 340, 22), // "slotErrorMessageDialog"
QT_MOC_LITERAL(28, 363, 12), // "errorMessage"
QT_MOC_LITERAL(29, 376, 22), // "WriteParameterFragment"
QT_MOC_LITERAL(30, 399, 10), // "showDialog"
QT_MOC_LITERAL(31, 410, 16), // "ValidateLineEdit"
QT_MOC_LITERAL(32, 427, 10), // "QLineEdit*"
QT_MOC_LITERAL(33, 438, 8), // "lineEdit"
QT_MOC_LITERAL(34, 447, 11), // "ShowToolTip"
QT_MOC_LITERAL(35, 459, 13), // "MarkEditColor"
QT_MOC_LITERAL(36, 473, 6) // "result"

    },
    "DataPage\0LoadSettings\0\0ValidateSettings\0"
    "LoadTrainingButton\0LoadTestingButton\0"
    "LoadIniButton\0ReadVector\0inputvector\0"
    "QVector<int>*\0poutputvector\0"
    "ValidateTrainingDataFiles\0vector<string>&\0"
    "fPathArr\0int&\0numSamples\0numChannels\0"
    "mode\0ValidateTestingDataFiles\0"
    "GenerateFeatures\0ApplyFeatures\0"
    "slotProgressText\0message\0slotProgressBar\0"
    "done\0total\0stage\0slotErrorMessageDialog\0"
    "errorMessage\0WriteParameterFragment\0"
    "showDialog\0ValidateLineEdit\0QLineEdit*\0"
    "lineEdit\0ShowToolTip\0MarkEditColor\0"
    "result"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x0a /* Public */,
       3,    0,  105,    2, 0x0a /* Public */,
       4,    0,  106,    2, 0x0a /* Public */,
       5,    0,  107,    2, 0x0a /* Public */,
       6,    0,  108,    2, 0x0a /* Public */,
       7,    2,  109,    2, 0x0a /* Public */,
      11,    4,  114,    2, 0x0a /* Public */,
      18,    4,  123,    2, 0x0a /* Public */,
      19,    0,  132,    2, 0x0a /* Public */,
      20,    0,  133,    2, 0x0a /* Public */,
      21,    1,  134,    2, 0x0a /* Public */,
      23,    3,  137,    2, 0x0a /* Public */,
      27,    1,  144,    2, 0x0a /* Public */,
      29,    1,  147,    2, 0x0a /* Public */,
      29,    0,  150,    2, 0x2a /* Public | MethodCloned */,
      31,    1,  151,    2, 0x0a /* Public */,
      34,    2,  154,    2, 0x0a /* Public */,
      35,    2,  159,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 9,    8,   10,
    QMetaType::Bool, 0x80000000 | 12, 0x80000000 | 14, 0x80000000 | 14, 0x80000000 | 14,   13,   15,   16,   17,
    QMetaType::Bool, 0x80000000 | 12, 0x80000000 | 14, 0x80000000 | 14, 0x80000000 | 14,   13,   15,   16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   24,   25,   26,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::Bool,   30,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 32,   33,
    QMetaType::Void, 0x80000000 | 32, QMetaType::QString,   33,   22,
    QMetaType::Void, 0x80000000 | 32, QMetaType::Bool,   33,   36,

       0        // eod
};

void DataPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataPage *_t = static_cast<DataPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LoadSettings(); break;
        case 1: _t->ValidateSettings(); break;
        case 2: _t->LoadTrainingButton(); break;
        case 3: _t->LoadTestingButton(); break;
        case 4: _t->LoadIniButton(); break;
        case 5: _t->ReadVector((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVector<int>*(*)>(_a[2]))); break;
        case 6: { bool _r = _t->ValidateTrainingDataFiles((*reinterpret_cast< vector<string>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->ValidateTestingDataFiles((*reinterpret_cast< vector<string>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->GenerateFeatures(); break;
        case 9: _t->ApplyFeatures(); break;
        case 10: _t->slotProgressText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->slotProgressBar((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: _t->slotErrorMessageDialog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->WriteParameterFragment((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->WriteParameterFragment(); break;
        case 15: { bool _r = _t->ValidateLineEdit((*reinterpret_cast< QLineEdit*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->ShowToolTip((*reinterpret_cast< QLineEdit*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 17: _t->MarkEditColor((*reinterpret_cast< QLineEdit*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DataPage.data,
    qt_meta_data_DataPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DataPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
struct qt_meta_stringdata_ParametersPage_t {
    QByteArrayData data[4];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParametersPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParametersPage_t qt_meta_stringdata_ParametersPage = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ParametersPage"
QT_MOC_LITERAL(1, 15, 14), // "TextHasChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 19) // "ChSetTextHasChanged"

    },
    "ParametersPage\0TextHasChanged\0\0"
    "ChSetTextHasChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParametersPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ParametersPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParametersPage *_t = static_cast<ParametersPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->TextHasChanged(); break;
        case 1: _t->ChSetTextHasChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ParametersPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ParametersPage.data,
    qt_meta_data_ParametersPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ParametersPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParametersPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParametersPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ParametersPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_DetailsPage_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DetailsPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DetailsPage_t qt_meta_stringdata_DetailsPage = {
    {
QT_MOC_LITERAL(0, 0, 11) // "DetailsPage"

    },
    "DetailsPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DetailsPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void DetailsPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DetailsPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DetailsPage.data,
    qt_meta_data_DetailsPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DetailsPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DetailsPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DetailsPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DetailsPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
