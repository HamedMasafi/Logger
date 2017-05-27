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

#ifndef OBJECTPROFILLER_H
#define OBJECTPROFILLER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#define K_REG_OBJECT(obj) ObjectProfiller::instance()->addData(#obj, obj, __FILE__, __FUNCTION__, __LINE__)

class ObjectProfiller : public QObject
{
    Q_OBJECT

    struct ObjectData{
        QObject *obj;
        bool isDestroyed;
        QString fileName;
        QString function;
        int lineNumber;
        const char *code;
    };
    QList<ObjectData*> _dataList;


public:
    explicit ObjectProfiller(QObject *parent = 0);
    virtual ~ObjectProfiller();

    static ObjectProfiller *instance();

    void addData(const char *code, QObject *obj, const char *fileName, const char *function, int lineNumber);

public slots:
    void printReport();

private slots:
    void obj_destroyed(QObject * = Q_NULLPTR);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);

private:
    bool isValidPointer(void *ptr);
};

#endif // OBJECTPROFILLER_H
