#ifndef VARIABLES_H
#define VARIABLES_H

#include <QObject>
#include "baseobject.h"

class Variables : public CouchDBManager::BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QString full_name READ get_full_name WRITE set_full_name)

private:
    QString full_name;

public:
    Q_INVOKABLE Variables(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

    }

public slots:
    QString get_full_name() const;
    void set_full_name(const QString& a_full_name);

};

Q_DECLARE_METATYPE(Variables*)

#endif // VARIABLES_H
