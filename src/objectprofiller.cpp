/**************************************************************************
**
** This file is part of Noron.
** https://github.com/HamedMasafi/Noron
**
** Noron is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Noron is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Noron.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include <QtGui/QGuiApplication>
#include <QtCore/QDebug>

#include "Logger/objectprofiller.h"

ObjectProfiller::ObjectProfiller(QObject *parent) : QObject(parent)
{
    connect(qApp, &QCoreApplication::aboutToQuit, this, &ObjectProfiller::printReport);
}

void ObjectProfiller::printReport()
{
    qDebug() << "=== ObjectProfiller Report ===";
    for (auto &data : _dataList) {
        if(data->obj == nullptr)
            data->isDestroyed = true;

//        if(!isValidPointer(data->obj) && !data->isDestroyed)
//            qDebug() << "Invalid pointer!!!";

        if(!data->isDestroyed)
            qDebug() << QString("QObject (%1), Is Destroyed=%2, Location=%3:%4 (%5)")
                        .arg(data->code)
                        .arg(data->isDestroyed)
                        .arg(data->fileName)
                        .arg(data->lineNumber)
                        .arg(data->function);
    }
}

ObjectProfiller::~ObjectProfiller()
{
    printReport();
}

ObjectProfiller *ObjectProfiller::instance()
{
    static ObjectProfiller *instance = 0;

    if(!instance)
        instance = new ObjectProfiller;

    return instance;
}

void ObjectProfiller::addData(const char *code, QObject *obj, const char *fileName, const char *function, int lineNumber)
{
    ObjectData *data = new ObjectData;
    data->obj = obj;
    data->fileName = fileName;
    data->function = function;
    data->lineNumber = lineNumber;
    data->isDestroyed = false;
    data->code = code;

    connect(obj, &QObject::destroyed, this, &ObjectProfiller::obj_destroyed);
    _dataList.append(data);
}

void ObjectProfiller::obj_destroyed(QObject *o)
{    
    if(o == Q_NULLPTR)
        return;

//    qDebug() << "ObjectProfiller::obj_destroyed";
    for(int i = 0; i < _dataList.count(); i++)
//    foreach (ObjectData *data, _dataList)
        if(_dataList[i]->obj != Q_NULLPTR && _dataList[i]->obj == o){
            _dataList[i]->isDestroyed = true;
            qDebug() << "ObjectProfiller: Object" << _dataList[i]->code << "destroyed";
            return;
        }

    qDebug() << "ObjectProfiller: Object not found" << o << sender();
}

void ObjectProfiller::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    printReport();
}

bool ObjectProfiller::isValidPointer(void *ptr)
{
#ifndef Q_OS_WINDOWS
    if ((((ulong)ptr)&7) == 7)
        return false; //Not valid address at all (Maybe random pointer?)
    char _prefix;
    try {
        _prefix=*(((char*)ptr)-1); //Get the prefix of this data
    } catch(...)  { //Catch all unique exceptions (Windows exceptions)
        return false; //Can't reach this memory
    }
    switch (_prefix) {
    case 0:    //Running release mode with debugger
    case -128: //Running release mode without debugger
    case -2:   //Running debug mode with debugger
    case -35:  //Running debug mode without debugger
        return false; //Deleted :(
        break;
    }
#endif
    return true; //Still alive!
}
