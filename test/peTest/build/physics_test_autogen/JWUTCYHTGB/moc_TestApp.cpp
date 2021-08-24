/****************************************************************************
** Meta object code from reading C++ file 'TestApp.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "apps/TestApp.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TestApp.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TestApp_t {
    QByteArrayData data[10];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestApp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestApp_t qt_meta_stringdata_TestApp = {
    {
QT_MOC_LITERAL(0, 0, 7), // "TestApp"
QT_MOC_LITERAL(1, 8, 13), // "requestUpdate"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "Vector2"
QT_MOC_LITERAL(4, 31, 3), // "Run"
QT_MOC_LITERAL(5, 35, 10), // "SingleStep"
QT_MOC_LITERAL(6, 46, 5), // "Pause"
QT_MOC_LITERAL(7, 52, 8), // "setReady"
QT_MOC_LITERAL(8, 61, 9), // "SetPreset"
QT_MOC_LITERAL(9, 71, 4) // "name"

    },
    "TestApp\0requestUpdate\0\0Vector2\0Run\0"
    "SingleStep\0Pause\0setReady\0SetPreset\0"
    "name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestApp[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   47,    2, 0x0a /* Public */,
       5,    0,   48,    2, 0x0a /* Public */,
       6,    0,   49,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x0a /* Public */,
       8,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

void TestApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TestApp *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestUpdate((*reinterpret_cast< Vector2(*)>(_a[1]))); break;
        case 1: _t->Run(); break;
        case 2: _t->SingleStep(); break;
        case 3: _t->Pause(); break;
        case 4: _t->setReady(); break;
        case 5: _t->SetPreset((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Vector2 >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TestApp::*)(Vector2 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TestApp::requestUpdate)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TestApp::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TestApp.data,
    qt_meta_data_TestApp,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TestApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestApp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestApp.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TestApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TestApp::requestUpdate(Vector2 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
