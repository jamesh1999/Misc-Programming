/****************************************************************************
** Meta object code from reading C++ file 'CodeBreakerMain.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodeBreakerGUI/CodeBreakerMain.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CodeBreakerMain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CodeBreakerMain_t {
    QByteArrayData data[14];
    char stringdata0[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CodeBreakerMain_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CodeBreakerMain_t qt_meta_stringdata_CodeBreakerMain = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CodeBreakerMain"
QT_MOC_LITERAL(1, 16, 8), // "finished"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "progress"
QT_MOC_LITERAL(4, 35, 18), // "setPlainTextOutput"
QT_MOC_LITERAL(5, 54, 15), // "appendToConsole"
QT_MOC_LITERAL(6, 70, 17), // "guessCipherReturn"
QT_MOC_LITERAL(7, 88, 18), // "on_decrypt_clicked"
QT_MOC_LITERAL(8, 107, 23), // "on_guess_cipher_clicked"
QT_MOC_LITERAL(9, 131, 34), // "on_actionGlobal_Settings_trig..."
QT_MOC_LITERAL(10, 166, 24), // "on_actionClose_triggered"
QT_MOC_LITERAL(11, 191, 29), // "on_cipher_currentIndexChanged"
QT_MOC_LITERAL(12, 221, 31), // "on_stackedWidget_currentChanged"
QT_MOC_LITERAL(13, 253, 29) // "on_actionAdd_Spaces_triggered"

    },
    "CodeBreakerMain\0finished\0\0progress\0"
    "setPlainTextOutput\0appendToConsole\0"
    "guessCipherReturn\0on_decrypt_clicked\0"
    "on_guess_cipher_clicked\0"
    "on_actionGlobal_Settings_triggered\0"
    "on_actionClose_triggered\0"
    "on_cipher_currentIndexChanged\0"
    "on_stackedWidget_currentChanged\0"
    "on_actionAdd_Spaces_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CodeBreakerMain[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    1,   75,    2, 0x0a /* Public */,
       4,    1,   78,    2, 0x0a /* Public */,
       5,    1,   81,    2, 0x0a /* Public */,
       6,    1,   84,    2, 0x0a /* Public */,
       7,    0,   87,    2, 0x08 /* Private */,
       8,    0,   88,    2, 0x08 /* Private */,
       9,    0,   89,    2, 0x08 /* Private */,
      10,    0,   90,    2, 0x08 /* Private */,
      11,    1,   91,    2, 0x08 /* Private */,
      12,    1,   94,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void CodeBreakerMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CodeBreakerMain *_t = static_cast<CodeBreakerMain *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPlainTextOutput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->appendToConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->guessCipherReturn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_decrypt_clicked(); break;
        case 6: _t->on_guess_cipher_clicked(); break;
        case 7: _t->on_actionGlobal_Settings_triggered(); break;
        case 8: _t->on_actionClose_triggered(); break;
        case 9: _t->on_cipher_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_stackedWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_actionAdd_Spaces_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject CodeBreakerMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CodeBreakerMain.data,
      qt_meta_data_CodeBreakerMain,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CodeBreakerMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CodeBreakerMain::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CodeBreakerMain.stringdata0))
        return static_cast<void*>(const_cast< CodeBreakerMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CodeBreakerMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
