/****************************************************************************
** Meta object code from reading C++ file 'columnartransposition.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodeBreakerGUI/columnartransposition.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'columnartransposition.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Cipher__ColumnarTranspositionWorker_t {
    QByteArrayData data[8];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__ColumnarTranspositionWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__ColumnarTranspositionWorker_t qt_meta_stringdata_Cipher__ColumnarTranspositionWorker = {
    {
QT_MOC_LITERAL(0, 0, 35), // "Cipher::ColumnarTransposition..."
QT_MOC_LITERAL(1, 36, 8), // "finished"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 8), // "progress"
QT_MOC_LITERAL(4, 55, 12), // "setPlainText"
QT_MOC_LITERAL(5, 68, 15), // "appendToConsole"
QT_MOC_LITERAL(6, 84, 5), // "crack"
QT_MOC_LITERAL(7, 90, 6) // "useKey"

    },
    "Cipher::ColumnarTranspositionWorker\0"
    "finished\0\0progress\0setPlainText\0"
    "appendToConsole\0crack\0useKey"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__ColumnarTranspositionWorker[] = {

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
       6,    1,   54,    2, 0x0a /* Public */,
       7,    1,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void Cipher::ColumnarTranspositionWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColumnarTranspositionWorker *_t = static_cast<ColumnarTranspositionWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPlainText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->appendToConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->crack((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->useKey((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ColumnarTranspositionWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnarTranspositionWorker::finished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ColumnarTranspositionWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnarTranspositionWorker::progress)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ColumnarTranspositionWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnarTranspositionWorker::setPlainText)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ColumnarTranspositionWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnarTranspositionWorker::appendToConsole)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject Cipher::ColumnarTranspositionWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cipher__ColumnarTranspositionWorker.data,
      qt_meta_data_Cipher__ColumnarTranspositionWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::ColumnarTranspositionWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::ColumnarTranspositionWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__ColumnarTranspositionWorker.stringdata0))
        return static_cast<void*>(const_cast< ColumnarTranspositionWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< ICipherWorker*>(const_cast< ColumnarTranspositionWorker*>(this));
    if (!strcmp(_clname, "ICipherWorker"))
        return static_cast< Cipher::ICipherWorker*>(const_cast< ColumnarTranspositionWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int Cipher::ColumnarTranspositionWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Cipher::ColumnarTranspositionWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Cipher::ColumnarTranspositionWorker::progress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Cipher::ColumnarTranspositionWorker::setPlainText(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Cipher::ColumnarTranspositionWorker::appendToConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_Cipher__ColumnarTransposition_t {
    QByteArrayData data[7];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cipher__ColumnarTransposition_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cipher__ColumnarTransposition_t qt_meta_stringdata_Cipher__ColumnarTransposition = {
    {
QT_MOC_LITERAL(0, 0, 29), // "Cipher::ColumnarTransposition"
QT_MOC_LITERAL(1, 30, 5), // "crack"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "useKey"
QT_MOC_LITERAL(4, 44, 12), // "getStoredKey"
QT_MOC_LITERAL(5, 57, 20), // "std::vector<QString>"
QT_MOC_LITERAL(6, 78, 19) // "on_edit_key_clicked"

    },
    "Cipher::ColumnarTransposition\0crack\0"
    "\0useKey\0getStoredKey\0std::vector<QString>\0"
    "on_edit_key_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cipher__ColumnarTransposition[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   40,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,

       0        // eod
};

void Cipher::ColumnarTransposition::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColumnarTransposition *_t = static_cast<ColumnarTransposition *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->crack((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->useKey((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->getStoredKey((*reinterpret_cast< std::vector<QString>(*)>(_a[1]))); break;
        case 3: _t->on_edit_key_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ColumnarTransposition::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnarTransposition::crack)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ColumnarTransposition::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnarTransposition::useKey)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Cipher::ColumnarTransposition::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Cipher__ColumnarTransposition.data,
      qt_meta_data_Cipher__ColumnarTransposition,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cipher::ColumnarTransposition::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cipher::ColumnarTransposition::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cipher__ColumnarTransposition.stringdata0))
        return static_cast<void*>(const_cast< ColumnarTransposition*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< ICipher*>(const_cast< ColumnarTransposition*>(this));
    if (!strcmp(_clname, "ICipher"))
        return static_cast< Cipher::ICipher*>(const_cast< ColumnarTransposition*>(this));
    return QWidget::qt_metacast(_clname);
}

int Cipher::ColumnarTransposition::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Cipher::ColumnarTransposition::crack(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Cipher::ColumnarTransposition::useKey(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
