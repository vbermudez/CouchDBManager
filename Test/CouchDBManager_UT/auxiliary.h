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
// PROJECT: ......... AD37
//
// NAME: ............ $RCSFile: $
//
// AUTHOR: .......... $Author: jgurrutxaga $
//
// REVISION: ........ $Revision: 1.2 $ ($Date: 2016/10/03 07:11:59 $)
//
// REFERENCE: ....... AD.37.91.3057
//
// CHANGES:
//

#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <QList>
#include <QVariant>
#include "variables.h"
#include "bus.h"
#include "basereference.h"

class CosmosRef : public CouchDBManager::BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ get_name WRITE set_name)

private:

    QString name;

public:

    Q_INVOKABLE CosmosRef(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

    }

public slots:

    QString get_name() const { return this->name; }
    void set_name(const QString &value) { this->name = value; }

};

Q_DECLARE_METATYPE(CosmosRef*)

class Rule : public CouchDBManager::BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QString prefix READ get_prefix WRITE set_prefix)
    Q_PROPERTY(QString suffix READ get_suffix WRITE set_suffix)
    Q_PROPERTY(QString replace READ get_replace WRITE set_replace)
    Q_PROPERTY(QString with READ get_with WRITE set_with)


private:

    QString prefix;
    QString suffix;
    QString replace;
    QString with;

public:

    Q_INVOKABLE Rule(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {
        qRegisterMetaType<Rule*>();
    }

public slots:

    QString get_prefix() const;
    void set_prefix(const QString &value);

    QString get_suffix() const;
    void set_suffix(const QString &value);

    QString get_replace() const;
    void set_replace(const QString &value);

    QString get_with() const;
    void set_with(const QString &value);

};

Q_DECLARE_METATYPE(Rule*)

class Linked : public CouchDBManager::BaseObject
{
    Q_OBJECT
    Q_PROPERTY(CouchDBManager::BaseReference *parent READ get_parent WRITE set_parent)
    Q_PROPERTY(Rule *rule READ get_rule WRITE set_rule)


private:

    CouchDBManager::BaseReference *parent;
    Rule *rule;


public:
    Q_INVOKABLE Linked(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {
        qRegisterMetaType<Linked*>();

        this->parent = NULL;
        this->rule = NULL;
    }

public slots:

    CouchDBManager::BaseReference *get_parent() const;
    void set_parent(CouchDBManager::BaseReference *value);

    Rule *get_rule() const;
    void set_rule(Rule *value);

};

Q_DECLARE_METATYPE(Linked*)





#endif // AUXILIARY_H

