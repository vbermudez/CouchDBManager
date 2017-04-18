//! @addtogroup CouchDBManager
//! @{
//! @file "activetask.h"
//! @brief Definición de la clase contenedora de una tarea de CouchDB activa.
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
// AUTHOR: .......... $Author: jmonasterio $
//
// REVISION: ........ $Revision: 1.2 $ ($Date: 2016/12/05 13:15:26 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef ACTIVETASK_H
#define ACTIVETASK_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QVariant>

#include "baseobject.h"

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase ActiveTask define una tarea activa en CouchDB.
     * @author vbermudez@xi-tek.com
     * @date 2016/11/16
     */
    class COUCHDBMANAGERSHARED_EXPORT ActiveTask : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString pid READ get_pid WRITE set_pid)
        Q_PROPERTY(QString replication_id READ get_replication_id WRITE set_replication_id)
        Q_PROPERTY(long checkpoint_interval READ get_checkpoint_interval WRITE set_checkpoint_interval)
        Q_PROPERTY(long checkpointed_source_seq READ get_checkpointed_source_seq WRITE set_checkpointed_source_seq)
        Q_PROPERTY(bool continuous READ get_continuous WRITE set_continuous)
        Q_PROPERTY(QString doc_id READ get_doc_id WRITE set_doc_id)
        Q_PROPERTY(long doc_write_failures READ get_doc_write_failures WRITE set_doc_write_failures)
        Q_PROPERTY(long docs_read READ get_docs_read WRITE set_docs_read)
        Q_PROPERTY(long docs_written READ get_docs_written WRITE set_docs_written)
        Q_PROPERTY(long missing_revisions_found READ get_missing_revisions_found WRITE set_missing_revisions_found)
        Q_PROPERTY(int progress READ get_progress WRITE set_progress)
        Q_PROPERTY(long revisions_checked READ get_revisions_checked WRITE set_revisions_checked)
        Q_PROPERTY(QString source READ get_source WRITE set_source)
        Q_PROPERTY(long source_seq READ get_source_seq WRITE set_source_seq)
        Q_PROPERTY(long started_on READ get_started_on WRITE set_started_on)
        Q_PROPERTY(QString target READ get_target WRITE set_target)
        Q_PROPERTY(QString type READ get_type WRITE set_type)
        Q_PROPERTY(long updated_on READ get_updated_on WRITE set_updated_on)

    private:
        QString pid;
        QString replication_id;
        long checkpoint_interval;
        long checkpointed_source_seq;
        bool continuous;
        QString doc_id;
        long doc_write_failures;
        long docs_read;
        long docs_written;
        long missing_revisions_found;
        int progress;
        long revisions_checked;
        QString source;
        long source_seq;
        long started_on;
        QString target;
        QString type;
        long updated_on;

    public:
        /**
          * @brief ActiveTask Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE ActiveTask(QObject *parent = 0) : CouchDBManager::BaseObject(parent) { }

    public slots:
        QString get_pid() const { return this->pid; }
        void set_pid(const QString& value) { this->pid = value; }

        QString get_replication_id() const { return this->replication_id; }
        void set_replication_id(const QString& value) { this->replication_id = value; }

        long get_checkpoint_interval() const { return this->checkpoint_interval; }
        void set_checkpoint_interval(long value) { this->checkpoint_interval = value; }

        long get_checkpointed_source_seq() const { return this->checkpointed_source_seq; }
        void set_checkpointed_source_seq(long value) { this->checkpointed_source_seq = value; }

        bool get_continuous() const { return this->continuous; }
        void set_continuous(bool value) { this->continuous = value; }

        QString get_doc_id() const { return this->doc_id; }
        void set_doc_id(const QString& value) { this->doc_id = value; }

        long get_doc_write_failures() const { return this->doc_write_failures; }
        void set_doc_write_failures(long value) { this->doc_write_failures = value; }

        long get_docs_read() const { return this->docs_read; }
        void set_docs_read(long value) { this->docs_read = value; }

        long get_docs_written() const { return this->docs_written; }
        void set_docs_written(long value) { this->docs_written = value; }

        long get_missing_revisions_found() const { return this->missing_revisions_found; }
        void set_missing_revisions_found(long value) { this->missing_revisions_found = value; }

        int get_progress() const { return this->progress; }
        void set_progress(int value) { this->progress = value; }

        long get_revisions_checked() const { return this->revisions_checked; }
        void set_revisions_checked(long value) { this->revisions_checked = value; }

        QString get_source() const { return this->source; }
        void set_source(const QString& value) { this->source = value; }

        long get_source_seq() const { return this->source_seq; }
        void set_source_seq(long value) { this->source_seq = value; }

        long get_started_on() const { return this->started_on; }
        void set_started_on(long value) { this->started_on = value; }

        QString get_target() const { return this->target; }
        void set_target(const QString& value) { this->target = value; }

        QString get_type() const { return this->type; }
        void set_type(const QString& value) { this->type = value; }

        long get_updated_on() const { return this->updated_on; }
        void set_updated_on(long value) { this->updated_on = value; }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::ActiveTask*)

#endif // ACTIVETASK_H
//! @}
