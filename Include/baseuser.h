//! @addtogroup CouchDBManager
//! @{
//! @file "baseuser.h"
//! @brief Definición de la clase usuario básica.
//
// COMPANY: ......... Construcciones y Auxiliar de Ferrocarriles S.A.
//
// COPYRIGHT:
//   This document is the property of CAF. Under the copyright laws, the
//   documentation may not be copied, photocopied, reproduced, translated, or
//   reduced to any electronic medium or machine-readable form, in whole or
//   in part, without the prior written consent of CAF.
//   Copyright 2014 (c) CAF, S.A
//
// PROJECT: ......... AUNIA/DBMANAGER
//
// NAME: ............ $RCSFile: $
//
// AUTHOR: .......... $Author: jgurrutxaga $
//
// REVISION: ........ $Revision: 1.4 $ ($Date: 2016/10/03 07:11:59 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef BASEUSER_H
#define BASEUSER_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>

#include "baseobject.h"

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase BaseUser define un usuario básico.
     * @author vbermudez@xi-tek.com
     * @date 2016/07/13
     */
    class COUCHDBMANAGERSHARED_EXPORT BaseUser : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString _id READ get_id WRITE set_id)
        Q_PROPERTY(QString _rev READ get_rev WRITE set_rev)
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString password READ get_password WRITE set_password)
        Q_PROPERTY(QString type READ get_type WRITE set_type)
        Q_PROPERTY(QStringList roles READ get_roles WRITE set_roles)

    private:
        QString _id;
        QString _rev;
        QString name;
        QString password;
        QString type;
        QStringList roles;

    public:
        /**
          * @brief BaseUser Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE BaseUser(QObject *parent = 0) : CouchDBManager::BaseObject(parent)
        {
            this->type = "user";
            this->roles.append("users");
        }

    public slots:
        /**
         * @brief get_id Recupera el identificador único del usuario.
         * @return QString con el identificador único del usuario.
         */
        QString get_id() const { return this->_id; }
        /**
         * @brief set_id Establece el identificador único del usuario.
         * @param a_id Identificador único del usuario.
         */
        void set_id(const QString& a_id) { this->_id = a_id; }
        /**
         * @brief get_rev Recupera la revisión del usuario.
         * @return QString con la revisión del usuario.
         */
        QString get_rev() const { return this->_rev; }
        /**
         * @brief set_rev Establece la revisión del usuario.
         * @param a_rev Revisión del usuario.
         */
        void set_rev(const QString& a_rev) { this->_rev = a_rev; }
        /**
         * @brief get_name Recupera nombre del usuario.
         * @return QString con el nombre del usuario.
         */
        QString get_name() const { return this->name; }
        /**
         * @brief set_name Establece el nombre del usuario.
         * @param a_name Nombre del usuario.
         */
        void set_name(const QString& a_name) { this->name = a_name; }
        /**
         * @brief get_password Getter por defecto. Siempre devuelve vacío.
         * @return QString vacío.
         */
        QString get_password() const { return this->password; }
        /**
         * @brief set_password Establece la clave del usuario.
         * @param a_password Clave del usuario.
         */
        void set_password(const QString& a_password) { this->password = a_password; }
        /**
         * @brief get_type Recupera el tipo de usuario.
         * @return QString con el tipo de usuario.
         */
        QString get_type() const { return this->type; }
        /**
         * @brief set_type Setter por defecto. No modifica el valor del tipo.
         * @param a_type Tipo de usuario.
         */
        void set_type(const QString& a_type) { qDebug() << "@ Trying to set type" << a_type; return; }
        /**
         * @brief get_roles Recupera los roles del usuario.
         * @return QStringList con los roles del usuario.
         */
        QStringList get_roles() const { return this->roles; }
        /**
         * @brief set_name Establece los roles del usuario. Si no lo tiene, añade "users" por defecto.
         * @param a_roles Roles del usuario.
         */
        void set_roles(QStringList a_roles)
        {
            this->roles = a_roles;

            if (!this->roles.contains("users"))
            {
                this->roles.append("users");
            }
        }
        /**
         * @brief add_roles Añade un rol al usuario.
         * @param a_role Rol del usuario.
         */
        void add_roles(const QString& a_role) { this->roles.append(a_role); }

        /**
         * @brief is_base_user Determina si el objeto extiende a BaseUser.
         * @return true si el objeto deriva de BaseUser.
         */
        bool is_base_user() const { return true; }

        /**
         * @brief is_valid Determina si el objeto es válido.
         * @return true si el objeto ha sido inicializado.
         */
        bool is_valid() const { return (!_id.isNull() && !_id.isEmpty()); }

        void clear()
        {
            this->_id.clear();
            this->_rev.clear();
            this->name.clear();
            this->password.clear();
            this->type.clear();
            this->roles.clear();
        }
    };
}

#endif // BASEUSER_H
//! @}
