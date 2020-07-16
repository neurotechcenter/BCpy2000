/****************************************************************************
** Meta object code from reading C++ file 'CertLauncherGui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/core/Tools/BC4825~1/BCI200~2/CertLauncherGui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CertLauncherGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LabeledLineEdit_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabeledLineEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabeledLineEdit_t qt_meta_stringdata_LabeledLineEdit = {
    {
QT_MOC_LITERAL(0, 0, 15) // "LabeledLineEdit"

    },
    "LabeledLineEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabeledLineEdit[] = {

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

void LabeledLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LabeledLineEdit::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_LabeledLineEdit.data,
    qt_meta_data_LabeledLineEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LabeledLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabeledLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabeledLineEdit.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "LineEditWrapper"))
        return static_cast< LineEditWrapper*>(this);
    return QWidget::qt_metacast(_clname);
}

int LabeledLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_GetFileWidget_t {
    QByteArrayData data[6];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GetFileWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GetFileWidget_t qt_meta_stringdata_GetFileWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GetFileWidget"
QT_MOC_LITERAL(1, 14, 7), // "getFile"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "setFile"
QT_MOC_LITERAL(4, 31, 4), // "file"
QT_MOC_LITERAL(5, 36, 14) // "getCurrentFile"

    },
    "GetFileWidget\0getFile\0\0setFile\0file\0"
    "getCurrentFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GetFileWidget[] = {

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
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::QString,

       0        // eod
};

void GetFileWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GetFileWidget *_t = static_cast<GetFileWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getFile(); break;
        case 1: _t->setFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: { QString _r = _t->getCurrentFile();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GetFileWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_GetFileWidget.data,
    qt_meta_data_GetFileWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GetFileWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GetFileWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GetFileWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "LineEditWrapper"))
        return static_cast< LineEditWrapper*>(this);
    return QWidget::qt_metacast(_clname);
}

int GetFileWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_GetDirWidget_t {
    QByteArrayData data[5];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GetDirWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GetDirWidget_t qt_meta_stringdata_GetDirWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "GetDirWidget"
QT_MOC_LITERAL(1, 13, 7), // "getFile"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "setFile"
QT_MOC_LITERAL(4, 30, 4) // "file"

    },
    "GetDirWidget\0getFile\0\0setFile\0file"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GetDirWidget[] = {

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
       3,    1,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void GetDirWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GetDirWidget *_t = static_cast<GetDirWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getFile(); break;
        case 1: _t->setFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GetDirWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_GetDirWidget.data,
    qt_meta_data_GetDirWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GetDirWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GetDirWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GetDirWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "LineEditWrapper"))
        return static_cast< LineEditWrapper*>(this);
    return QWidget::qt_metacast(_clname);
}

int GetDirWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_LabeledComboBox_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabeledComboBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabeledComboBox_t qt_meta_stringdata_LabeledComboBox = {
    {
QT_MOC_LITERAL(0, 0, 15) // "LabeledComboBox"

    },
    "LabeledComboBox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabeledComboBox[] = {

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

void LabeledComboBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LabeledComboBox::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_LabeledComboBox.data,
    qt_meta_data_LabeledComboBox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LabeledComboBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabeledComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabeledComboBox.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LabeledComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_AutoEditMenu_t {
    QByteArrayData data[6];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutoEditMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutoEditMenu_t qt_meta_stringdata_AutoEditMenu = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AutoEditMenu"
QT_MOC_LITERAL(1, 13, 8), // "populate"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "QWidget*"
QT_MOC_LITERAL(4, 32, 16), // "QList<QAction*>&"
QT_MOC_LITERAL(5, 49, 6) // "onShow"

    },
    "AutoEditMenu\0populate\0\0QWidget*\0"
    "QList<QAction*>&\0onShow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoEditMenu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void AutoEditMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutoEditMenu *_t = static_cast<AutoEditMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->populate((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QList<QAction*>(*)>(_a[2]))); break;
        case 1: _t->onShow(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AutoEditMenu::*)(QWidget * , QList<QAction*> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AutoEditMenu::populate)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AutoEditMenu::staticMetaObject = { {
    &QMenu::staticMetaObject,
    qt_meta_stringdata_AutoEditMenu.data,
    qt_meta_data_AutoEditMenu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AutoEditMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AutoEditMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AutoEditMenu.stringdata0))
        return static_cast<void*>(this);
    return QMenu::qt_metacast(_clname);
}

int AutoEditMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AutoEditMenu::populate(QWidget * _t1, QList<QAction*> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CertLauncherGui_t {
    QByteArrayData data[29];
    char stringdata0[469];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CertLauncherGui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CertLauncherGui_t qt_meta_stringdata_CertLauncherGui = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CertLauncherGui"
QT_MOC_LITERAL(1, 16, 10), // "onFileOpen"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "onFileSave"
QT_MOC_LITERAL(4, 39, 12), // "onFileSaveAs"
QT_MOC_LITERAL(5, 52, 11), // "onFileClose"
QT_MOC_LITERAL(6, 64, 10), // "onHelpHelp"
QT_MOC_LITERAL(7, 75, 11), // "onHelpAbout"
QT_MOC_LITERAL(8, 87, 18), // "onEditMenuPopulate"
QT_MOC_LITERAL(9, 106, 8), // "QWidget*"
QT_MOC_LITERAL(10, 115, 16), // "QList<QAction*>&"
QT_MOC_LITERAL(11, 132, 11), // "addParmsBtn"
QT_MOC_LITERAL(12, 144, 11), // "delParmsBtn"
QT_MOC_LITERAL(13, 156, 21), // "onTaskListItemChanged"
QT_MOC_LITERAL(14, 178, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(15, 195, 30), // "onTaskListItemSelectionChanged"
QT_MOC_LITERAL(16, 226, 16), // "onTaskBtnClicked"
QT_MOC_LITERAL(17, 243, 21), // "onTaskNameTextChanged"
QT_MOC_LITERAL(18, 265, 19), // "onWidgetTextChanged"
QT_MOC_LITERAL(19, 285, 20), // "onDataDirTextChanged"
QT_MOC_LITERAL(20, 306, 22), // "onCheckBoxStateChanged"
QT_MOC_LITERAL(21, 329, 17), // "updateTaskDetails"
QT_MOC_LITERAL(22, 347, 12), // "onTabChanged"
QT_MOC_LITERAL(23, 360, 17), // "onStartBtnClicked"
QT_MOC_LITERAL(24, 378, 24), // "onProgressDialogCanceled"
QT_MOC_LITERAL(25, 403, 15), // "onLauncherEvent"
QT_MOC_LITERAL(26, 419, 13), // "const Config*"
QT_MOC_LITERAL(27, 433, 15), // "const TaskType*"
QT_MOC_LITERAL(28, 449, 19) // "onAnalyzeBtnClicked"

    },
    "CertLauncherGui\0onFileOpen\0\0onFileSave\0"
    "onFileSaveAs\0onFileClose\0onHelpHelp\0"
    "onHelpAbout\0onEditMenuPopulate\0QWidget*\0"
    "QList<QAction*>&\0addParmsBtn\0delParmsBtn\0"
    "onTaskListItemChanged\0QListWidgetItem*\0"
    "onTaskListItemSelectionChanged\0"
    "onTaskBtnClicked\0onTaskNameTextChanged\0"
    "onWidgetTextChanged\0onDataDirTextChanged\0"
    "onCheckBoxStateChanged\0updateTaskDetails\0"
    "onTabChanged\0onStartBtnClicked\0"
    "onProgressDialogCanceled\0onLauncherEvent\0"
    "const Config*\0const TaskType*\0"
    "onAnalyzeBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CertLauncherGui[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x08 /* Private */,
       3,    0,  125,    2, 0x08 /* Private */,
       4,    0,  126,    2, 0x08 /* Private */,
       5,    0,  127,    2, 0x08 /* Private */,
       6,    0,  128,    2, 0x08 /* Private */,
       7,    0,  129,    2, 0x08 /* Private */,
       8,    2,  130,    2, 0x08 /* Private */,
      11,    0,  135,    2, 0x08 /* Private */,
      12,    0,  136,    2, 0x08 /* Private */,
      13,    1,  137,    2, 0x08 /* Private */,
      15,    0,  140,    2, 0x08 /* Private */,
      16,    0,  141,    2, 0x08 /* Private */,
      17,    1,  142,    2, 0x08 /* Private */,
      18,    1,  145,    2, 0x08 /* Private */,
      19,    1,  148,    2, 0x08 /* Private */,
      20,    1,  151,    2, 0x08 /* Private */,
      21,    0,  154,    2, 0x08 /* Private */,
      22,    0,  155,    2, 0x08 /* Private */,
      23,    0,  156,    2, 0x08 /* Private */,
      24,    0,  157,    2, 0x08 /* Private */,
      25,    4,  158,    2, 0x08 /* Private */,
      28,    0,  167,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 10,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 27, QMetaType::Int, QMetaType::QString,    2,    2,    2,    2,
    QMetaType::Void,

       0        // eod
};

void CertLauncherGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CertLauncherGui *_t = static_cast<CertLauncherGui *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onFileOpen(); break;
        case 1: _t->onFileSave(); break;
        case 2: _t->onFileSaveAs(); break;
        case 3: _t->onFileClose(); break;
        case 4: _t->onHelpHelp(); break;
        case 5: _t->onHelpAbout(); break;
        case 6: _t->onEditMenuPopulate((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QList<QAction*>(*)>(_a[2]))); break;
        case 7: _t->addParmsBtn(); break;
        case 8: _t->delParmsBtn(); break;
        case 9: _t->onTaskListItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->onTaskListItemSelectionChanged(); break;
        case 11: _t->onTaskBtnClicked(); break;
        case 12: _t->onTaskNameTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->onWidgetTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->onDataDirTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->onCheckBoxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->updateTaskDetails(); break;
        case 17: _t->onTabChanged(); break;
        case 18: _t->onStartBtnClicked(); break;
        case 19: _t->onProgressDialogCanceled(); break;
        case 20: _t->onLauncherEvent((*reinterpret_cast< const Config*(*)>(_a[1])),(*reinterpret_cast< const TaskType*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 21: _t->onAnalyzeBtnClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CertLauncherGui::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_CertLauncherGui.data,
    qt_meta_data_CertLauncherGui,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CertLauncherGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CertLauncherGui::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CertLauncherGui.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CertLauncherGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
