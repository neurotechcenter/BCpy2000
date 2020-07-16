/****************************************************************************
** Meta object code from reading C++ file 'ParamDisplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/core/Operator/OPERAT~2/ParamDisplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParamDisplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ParamLabel_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParamLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParamLabel_t qt_meta_stringdata_ParamLabel = {
    {
QT_MOC_LITERAL(0, 0, 10) // "ParamLabel"

    },
    "ParamLabel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParamLabel[] = {

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

void ParamLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ParamLabel::staticMetaObject = { {
    &QLabel::staticMetaObject,
    qt_meta_stringdata_ParamLabel.data,
    qt_meta_data_ParamLabel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ParamLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParamLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParamLabel.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int ParamLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_DisplayBase_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DisplayBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DisplayBase_t qt_meta_stringdata_DisplayBase = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DisplayBase"
QT_MOC_LITERAL(1, 12, 15), // "OnContentChange"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "DisplayBase\0OnContentChange\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DisplayBase[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void DisplayBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DisplayBase *_t = static_cast<DisplayBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnContentChange(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DisplayBase::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DisplayBase.data,
    qt_meta_data_DisplayBase,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DisplayBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DisplayBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayBase.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DisplayBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_SeparateComment_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeparateComment_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeparateComment_t qt_meta_stringdata_SeparateComment = {
    {
QT_MOC_LITERAL(0, 0, 15) // "SeparateComment"

    },
    "SeparateComment"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeparateComment[] = {

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

void SeparateComment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SeparateComment::staticMetaObject = { {
    &DisplayBase::staticMetaObject,
    qt_meta_stringdata_SeparateComment.data,
    qt_meta_data_SeparateComment,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SeparateComment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeparateComment::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SeparateComment.stringdata0))
        return static_cast<void*>(this);
    return DisplayBase::qt_metacast(_clname);
}

int SeparateComment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DisplayBase::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SingleEntryEdit_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryEdit_t qt_meta_stringdata_SingleEntryEdit = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SingleEntryEdit"
QT_MOC_LITERAL(1, 16, 12), // "OnEditChange"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "SingleEntryEdit\0OnEditChange\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void SingleEntryEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SingleEntryEdit *_t = static_cast<SingleEntryEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnEditChange(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryEdit::staticMetaObject = { {
    &SeparateComment::staticMetaObject,
    qt_meta_stringdata_SingleEntryEdit.data,
    qt_meta_data_SingleEntryEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryEdit.stringdata0))
        return static_cast<void*>(this);
    return SeparateComment::qt_metacast(_clname);
}

int SingleEntryEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SeparateComment::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_SingleEntryButton_t {
    QByteArrayData data[3];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryButton_t qt_meta_stringdata_SingleEntryButton = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SingleEntryButton"
QT_MOC_LITERAL(1, 18, 13), // "OnButtonClick"
QT_MOC_LITERAL(2, 32, 0) // ""

    },
    "SingleEntryButton\0OnButtonClick\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryButton[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void SingleEntryButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SingleEntryButton *_t = static_cast<SingleEntryButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnButtonClick(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryButton::staticMetaObject = { {
    &SingleEntryEdit::staticMetaObject,
    qt_meta_stringdata_SingleEntryButton.data,
    qt_meta_data_SingleEntryButton,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryButton.stringdata0))
        return static_cast<void*>(this);
    return SingleEntryEdit::qt_metacast(_clname);
}

int SingleEntryButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SingleEntryEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_SingleEntryInputFile_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryInputFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryInputFile_t qt_meta_stringdata_SingleEntryInputFile = {
    {
QT_MOC_LITERAL(0, 0, 20) // "SingleEntryInputFile"

    },
    "SingleEntryInputFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryInputFile[] = {

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

void SingleEntryInputFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryInputFile::staticMetaObject = { {
    &SingleEntryButton::staticMetaObject,
    qt_meta_stringdata_SingleEntryInputFile.data,
    qt_meta_data_SingleEntryInputFile,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryInputFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryInputFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryInputFile.stringdata0))
        return static_cast<void*>(this);
    return SingleEntryButton::qt_metacast(_clname);
}

int SingleEntryInputFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SingleEntryButton::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SingleEntryOutputFile_t {
    QByteArrayData data[1];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryOutputFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryOutputFile_t qt_meta_stringdata_SingleEntryOutputFile = {
    {
QT_MOC_LITERAL(0, 0, 21) // "SingleEntryOutputFile"

    },
    "SingleEntryOutputFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryOutputFile[] = {

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

void SingleEntryOutputFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryOutputFile::staticMetaObject = { {
    &SingleEntryButton::staticMetaObject,
    qt_meta_stringdata_SingleEntryOutputFile.data,
    qt_meta_data_SingleEntryOutputFile,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryOutputFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryOutputFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryOutputFile.stringdata0))
        return static_cast<void*>(this);
    return SingleEntryButton::qt_metacast(_clname);
}

int SingleEntryOutputFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SingleEntryButton::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SingleEntryDirectory_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryDirectory_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryDirectory_t qt_meta_stringdata_SingleEntryDirectory = {
    {
QT_MOC_LITERAL(0, 0, 20) // "SingleEntryDirectory"

    },
    "SingleEntryDirectory"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryDirectory[] = {

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

void SingleEntryDirectory::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryDirectory::staticMetaObject = { {
    &SingleEntryButton::staticMetaObject,
    qt_meta_stringdata_SingleEntryDirectory.data,
    qt_meta_data_SingleEntryDirectory,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryDirectory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryDirectory::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryDirectory.stringdata0))
        return static_cast<void*>(this);
    return SingleEntryButton::qt_metacast(_clname);
}

int SingleEntryDirectory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SingleEntryButton::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SingleEntryColor_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryColor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryColor_t qt_meta_stringdata_SingleEntryColor = {
    {
QT_MOC_LITERAL(0, 0, 16) // "SingleEntryColor"

    },
    "SingleEntryColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryColor[] = {

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

void SingleEntryColor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryColor::staticMetaObject = { {
    &SingleEntryButton::staticMetaObject,
    qt_meta_stringdata_SingleEntryColor.data,
    qt_meta_data_SingleEntryColor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryColor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryColor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryColor.stringdata0))
        return static_cast<void*>(this);
    return SingleEntryButton::qt_metacast(_clname);
}

int SingleEntryColor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SingleEntryButton::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SingleEntryBlob_t {
    QByteArrayData data[5];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryBlob_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryBlob_t qt_meta_stringdata_SingleEntryBlob = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SingleEntryBlob"
QT_MOC_LITERAL(1, 16, 17), // "OnViewButtonClick"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 17), // "OnLoadButtonClick"
QT_MOC_LITERAL(4, 53, 17) // "OnSaveButtonClick"

    },
    "SingleEntryBlob\0OnViewButtonClick\0\0"
    "OnLoadButtonClick\0OnSaveButtonClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryBlob[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SingleEntryBlob::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SingleEntryBlob *_t = static_cast<SingleEntryBlob *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnViewButtonClick(); break;
        case 1: _t->OnLoadButtonClick(); break;
        case 2: _t->OnSaveButtonClick(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryBlob::staticMetaObject = { {
    &SeparateComment::staticMetaObject,
    qt_meta_stringdata_SingleEntryBlob.data,
    qt_meta_data_SingleEntryBlob,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryBlob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryBlob::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryBlob.stringdata0))
        return static_cast<void*>(this);
    return SeparateComment::qt_metacast(_clname);
}

int SingleEntryBlob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SeparateComment::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_List_t {
    QByteArrayData data[1];
    char stringdata0[5];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_List_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_List_t qt_meta_stringdata_List = {
    {
QT_MOC_LITERAL(0, 0, 4) // "List"

    },
    "List"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_List[] = {

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

void List::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject List::staticMetaObject = { {
    &SingleEntryButton::staticMetaObject,
    qt_meta_stringdata_List.data,
    qt_meta_data_List,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *List::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *List::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_List.stringdata0))
        return static_cast<void*>(this);
    return SingleEntryButton::qt_metacast(_clname);
}

int List::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SingleEntryButton::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_Matrix_t {
    QByteArrayData data[5];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Matrix_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Matrix_t qt_meta_stringdata_Matrix = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Matrix"
QT_MOC_LITERAL(1, 7, 17), // "OnEditButtonClick"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 17), // "OnLoadButtonClick"
QT_MOC_LITERAL(4, 44, 17) // "OnSaveButtonClick"

    },
    "Matrix\0OnEditButtonClick\0\0OnLoadButtonClick\0"
    "OnSaveButtonClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Matrix[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Matrix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Matrix *_t = static_cast<Matrix *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnEditButtonClick(); break;
        case 1: _t->OnLoadButtonClick(); break;
        case 2: _t->OnSaveButtonClick(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Matrix::staticMetaObject = { {
    &SeparateComment::staticMetaObject,
    qt_meta_stringdata_Matrix.data,
    qt_meta_data_Matrix,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Matrix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Matrix::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Matrix.stringdata0))
        return static_cast<void*>(this);
    return SeparateComment::qt_metacast(_clname);
}

int Matrix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SeparateComment::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_SingleEntryEnum_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryEnum_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryEnum_t qt_meta_stringdata_SingleEntryEnum = {
    {
QT_MOC_LITERAL(0, 0, 15) // "SingleEntryEnum"

    },
    "SingleEntryEnum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryEnum[] = {

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

void SingleEntryEnum::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryEnum::staticMetaObject = { {
    &SeparateComment::staticMetaObject,
    qt_meta_stringdata_SingleEntryEnum.data,
    qt_meta_data_SingleEntryEnum,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryEnum::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryEnum::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryEnum.stringdata0))
        return static_cast<void*>(this);
    return SeparateComment::qt_metacast(_clname);
}

int SingleEntryEnum::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SeparateComment::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SingleEntryBoolean_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEntryBoolean_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEntryBoolean_t qt_meta_stringdata_SingleEntryBoolean = {
    {
QT_MOC_LITERAL(0, 0, 18) // "SingleEntryBoolean"

    },
    "SingleEntryBoolean"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEntryBoolean[] = {

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

void SingleEntryBoolean::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SingleEntryBoolean::staticMetaObject = { {
    &DisplayBase::staticMetaObject,
    qt_meta_stringdata_SingleEntryBoolean.data,
    qt_meta_data_SingleEntryBoolean,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEntryBoolean::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEntryBoolean::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEntryBoolean.stringdata0))
        return static_cast<void*>(this);
    return DisplayBase::qt_metacast(_clname);
}

int SingleEntryBoolean::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DisplayBase::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
