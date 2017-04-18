#include "relationentity.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>

bool CouchDBManager::RelationEntity::is_relation_entity(QObject *object) const
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::RelationEntity* e = qobject_cast<CouchDBManager::RelationEntity*>(object);
    bool is_re = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en RelationEntity";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->methodCount(); i++)
        {
            if (meta->method(i).name() == "is_relation_entity")
            {
                is_re = true;
                qDebug() << "# Es un objeto RelationEntity.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_re;
}
