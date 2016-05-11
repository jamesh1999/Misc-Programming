/****************************************************************************
** Meta object code from reading C++ file 'Hill.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodeBreakerGUI/Hill.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Hill.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Cipher__HillWorker_t {
    QByteArrayData data[9];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__HillWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__HillWorker_t qt_meta_stringdata_Cipher__HillWorker = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Cipher::HillWorker"
QT_MOC_LITERAL(1, 19, 8), // "finished"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "progress"
QT_MOC_LITERAL(4, 38, 12), // "setPlainText"
QT_MOC_LITERAL(5, 51, 15), // "appendToConsole"
QT_MOC_LITERAL(6, 67, 8), // "storeKey"
QT_MOC_LITERAL(7, 76, 5), // "crack"
QT_MOC_LITERAL(8, 82, 6) // "useKey"

    },
    "Cipher::HillWorker\0finished\0\0progress\0"
    "setPlainText\0appendToConsole\0storeKey\0"
    "crack\0useKey"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__HillWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,
       4,    1,   53,    2, 0x06 /* Public */,
       5,    1,   56,    2, 0x06 /* Public */,
       6,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    4,   64,    2, 0x0a /* Public */,
       8,    1,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Double,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void Cipher::HillWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HillWorker *_t = static_cast<HillWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPlainText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->appendToConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->storeKey((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->crack((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 6: _t->useKey((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HillWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HillWorker::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (HillWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HillWorker::progress)) {
                *result = 1;
            }
        }
        {
            typedef void (HillWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HillWorker::setPlainText)) {
                *result = 2;
            }
        }
        {
            typedef void (HillWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HillWorker::appendToConsole)) {
                *result = 3;
            }
        }
        {
            typedef void (HillWorker::*_t)(QString , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HillWorker::storeKey)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject Cipher::HillWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cipher__HillWorker.data,
      qt_meta_data_Cipher__HillWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::HillWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::HillWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__HillWorker.stringdata0))
        return static_cast<void*>(const_cast< HillWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< ICipherWorker*>(const_cast< HillWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< Cipher::ICipherWorker*>(const_cast< HillWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int Cipher::HillWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Cipher::HillWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Cipher::HillWorker::progress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Cipher::HillWorker::setPlainText(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Cipher::HillWorker::appendToConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Cipher::HillWorker::storeKey(QString _t1, double _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_Cipher__Hill_t {
    QByteArrayData data[4];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__Hill_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__Hill_t qt_meta_stringdata_Cipher__Hill = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Cipher::Hill"
QT_MOC_LITERAL(1, 13, 5), // "crack"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 6) // "useKey"

    },
    "Cipher::Hill\0crack\0\0useKey"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__Hill[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   24,    2, 0x06 /* Public */,
       3,    1,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void Cipher::Hill::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Hill *_t = static_cast<Hill *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->crack((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->useKey((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Hill::*_t)(bool , int , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hill::crack)) {
                *result = 0;
            }
        }
        {
            typedef void (Hill::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hill::useKey)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Cipher::Hill::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Cipher__Hill.data,
      qt_meta_data_Cipher__Hill,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::Hill::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::Hill::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__Hill.stringdata0))
        return static_cast<void*>(const_cast< Hill*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< ICipher*>(const_cast< Hill*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< Cipher::ICipher*>(const_cast< Hill*>(this));
    return QWidget::qt_metacast(_clname);
}

int Cipher::Hill::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void Cipher::Hill::crack(bool _t1, int _t2, QString _t3, QString _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Cipher::Hill::useKey(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
