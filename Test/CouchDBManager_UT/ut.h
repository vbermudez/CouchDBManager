#ifndef UT_H
#define UT_H

#include <QList>
#include <QVariant>

#include "versionableentity.h"


class Ut : public CouchDBManager::VersionableEntity
{
    Q_OBJECT

private:


public:

    Q_INVOKABLE Ut(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {

    }


public slots:

};

Q_DECLARE_METATYPE(Ut*)

#endif // UT_H
