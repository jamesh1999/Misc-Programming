/****************************************************************************
** Meta object code from reading C++ file 'Trifid.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodeBreakerGUI/Trifid.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Trifid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Cipher__TrifidWorker_t {
    QByteArrayData data[8];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__TrifidWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__TrifidWorker_t qt_meta_stringdata_Cipher__TrifidWorker = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Cipher::TrifidWorker"
QT_MOC_LITERAL(1, 21, 8), // "finished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 8), // "progress"
QT_MOC_LITERAL(4, 40, 12), // "setPlainText"
QT_MOC_LITERAL(5, 53, 15), // "appendToConsole"
QT_MOC_LITERAL(6, 69, 5), // "crack"
QT_MOC_LITERAL(7, 75, 6) // "useKey"

    },
    "Cipher::TrifidWorker\0finished\0\0progress\0"
    "setPlainText\0appendToConsole\0crack\0"
    "useKey"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__TrifidWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,
       4,    1,   48,    2, 0x06 /* Public */,
       5,    1,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   54,    2, 0x0a /* Public */,
       7,    3,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    2,    2,    2,

       0        // eod
};

void Cipher::TrifidWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrifidWorker *_t = static_cast<TrifidWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPlainText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->appendToConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->crack((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->useKey((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TrifidWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrifidWorker::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (TrifidWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrifidWorker::progress)) {
                *result = 1;
            }
        }
        {
            typedef void (TrifidWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrifidWorker::setPlainText)) {
                *result = 2;
            }
        }
        {
            typedef void (TrifidWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrifidWorker::appendToConsole)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject Cipher::TrifidWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cipher__TrifidWorker.data,
      qt_meta_data_Cipher__TrifidWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::TrifidWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::TrifidWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__TrifidWorker.stringdata0))
        return static_cast<void*>(const_cast< TrifidWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< ICipherWorker*>(const_cast< TrifidWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< Cipher::ICipherWorker*>(const_cast< TrifidWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int Cipher::TrifidWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void Cipher::TrifidWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Cipher::TrifidWorker::progress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Cipher::TrifidWorker::setPlainText(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Cipher::TrifidWorker::appendToConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_Cipher__Trifid_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__Trifid_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__Trifid_t qt_meta_stringdata_Cipher__Trifid = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Cipher::Trifid"
QT_MOC_LITERAL(1, 15, 5), // "crack"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 6), // "useKey"
QT_MOC_LITERAL(4, 29, 22) // "on_key_editingFinished"

    },
    "Cipher::Trifid\0crack\0\0useKey\0"
    "on_key_editingFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__Trifid[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       3,    3,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Cipher::Trifid::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Trifid *_t = static_cast<Trifid *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->crack((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->useKey((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->on_key_editingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Trifid::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trifid::crack)) {
                *result = 0;
            }
        }
        {
            typedef void (Trifid::*_t)(int , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trifid::useKey)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Cipher::Trifid::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Cipher__Trifid.data,
      qt_meta_data_Cipher__Trifid,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::Trifid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::Trifid::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__Trifid.stringdata0))
        return static_cast<void*>(const_cast< Trifid*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< ICipher*>(const_cast< Trifid*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< Cipher::ICipher*>(const_cast< Trifid*>(this));
    return QWidget::qt_metacast(_clname);
}

int Cipher::Trifid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void Cipher::Trifid::crack(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Cipher::Trifid::useKey(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
