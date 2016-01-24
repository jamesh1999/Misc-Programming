/****************************************************************************
** Meta object code from reading C++ file 'BinaryConvert.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodeBreakerGUI/BinaryConvert.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BinaryConvert.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Cipher__BinaryWorker_t {
    QByteArrayData data[7];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__BinaryWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__BinaryWorker_t qt_meta_stringdata_Cipher__BinaryWorker = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Cipher::BinaryWorker"
QT_MOC_LITERAL(1, 21, 8), // "finished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 8), // "progress"
QT_MOC_LITERAL(4, 40, 12), // "setPlainText"
QT_MOC_LITERAL(5, 53, 15), // "appendToConsole"
QT_MOC_LITERAL(6, 69, 6) // "decode"

    },
    "Cipher::BinaryWorker\0finished\0\0progress\0"
    "setPlainText\0appendToConsole\0decode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__BinaryWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       4,    1,   43,    2, 0x06 /* Public */,
       5,    1,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    3,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString,    2,    2,    2,

       0        // eod
};

void Cipher::BinaryWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BinaryWorker *_t = static_cast<BinaryWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPlainText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->appendToConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->decode((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BinaryWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BinaryWorker::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (BinaryWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BinaryWorker::progress)) {
                *result = 1;
            }
        }
        {
            typedef void (BinaryWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BinaryWorker::setPlainText)) {
                *result = 2;
            }
        }
        {
            typedef void (BinaryWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BinaryWorker::appendToConsole)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject Cipher::BinaryWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cipher__BinaryWorker.data,
      qt_meta_data_Cipher__BinaryWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::BinaryWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::BinaryWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__BinaryWorker.stringdata0))
        return static_cast<void*>(const_cast< BinaryWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< ICipherWorker*>(const_cast< BinaryWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< Cipher::ICipherWorker*>(const_cast< BinaryWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int Cipher::BinaryWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Cipher::BinaryWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Cipher::BinaryWorker::progress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Cipher::BinaryWorker::setPlainText(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Cipher::BinaryWorker::appendToConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_Cipher__BinaryConvert_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__BinaryConvert_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__BinaryConvert_t qt_meta_stringdata_Cipher__BinaryConvert = {
    {
QT_MOC_LITERAL(0, 0, 21), // "Cipher::BinaryConvert"
QT_MOC_LITERAL(1, 22, 6), // "decode"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "Cipher::BinaryConvert\0decode\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__BinaryConvert[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString,    2,    2,    2,

       0        // eod
};

void Cipher::BinaryConvert::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BinaryConvert *_t = static_cast<BinaryConvert *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->decode((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BinaryConvert::*_t)(unsigned  , unsigned  , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BinaryConvert::decode)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Cipher::BinaryConvert::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Cipher__BinaryConvert.data,
      qt_meta_data_Cipher__BinaryConvert,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::BinaryConvert::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::BinaryConvert::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__BinaryConvert.stringdata0))
        return static_cast<void*>(const_cast< BinaryConvert*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< ICipher*>(const_cast< BinaryConvert*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< Cipher::ICipher*>(const_cast< BinaryConvert*>(this));
    return QWidget::qt_metacast(_clname);
}

int Cipher::BinaryConvert::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Cipher::BinaryConvert::decode(unsigned  _t1, unsigned  _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
