//! @addtogroup CouchDBManager
//! @{
//! @file "lockableentity.h"
//! @brief Definición de la clase básica que define una entidad bloqueable.
#ifndef LOCKABLEENTITY_H
#define LOCKABLEENTITY_H

#include "couchdbmanager_global.h"
#include "baseentity.h"

#include <QObject>

namespace CouchDBManager
{
    class COUCHDBMANAGERSHARED_EXPORT LockableEntity : public CouchDBManager::BaseEntity
    {
        Q_OBJECT
        Q_PROPERTY(bool locked READ get_locked WRITE set_locked)
        Q_PROPERTY(QString locked_by READ get_locked_by WRITE set_locked_by)

    protected:
        bool locked;
        QString locked_by;

    public:
        Q_INVOKABLE LockableEntity(QObject *parent = 0) : CouchDBManager::BaseEntity(parent) {
            this->locked = false;
        }

    public slots:

        void set_locked(bool a_locked) { this->locked = a_locked; }
        bool get_locked() const { return this->locked; }

        void set_locked_by(const QString& a_locked_by) { this->locked_by = a_locked_by; }
        QString get_locked_by() const { return this->locked_by; }

        bool is_lockable_entity() const { return true; }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::LockableEntity*)

#endif // LOCKABLEENTITY_H
//! @}
