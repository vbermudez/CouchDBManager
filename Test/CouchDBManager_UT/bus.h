#ifndef BUS_H
#define BUS_H

#include "baseentity.h"
#include "versionableentity.h"
#include "rs485cosmos.h"
#include "variables.h"

#include <QList>

class Bus: public CouchDBManager::VersionableEntity
{
    Q_OBJECT
    Q_PROPERTY(int basic_period READ get_basic_period WRITE set_basic_period)
    Q_PROPERTY(int t_reply_max READ get_t_reply_max WRITE set_t_reply_max)
    Q_PROPERTY(Rs485Cosmos* rs485_cosmos READ get_rs485_cosmos WRITE set_rs485_cosmos)
    Q_PROPERTY(QList<Variables*> variables READ get_variables WRITE set_variables)

private:
    int basic_period;
    int t_reply_max;
    Rs485Cosmos* rs485_cosmos;
    QList<Variables*> variables;


public:
    Q_INVOKABLE Bus(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {


        this->basic_period = 0;
        this->t_reply_max = 0;
        this->collection = "bus";
        this->rs485_cosmos = NULL;
    }

public slots:
    int get_basic_period() const;
    void set_basic_period(int a_basic_period);
    int get_t_reply_max() const;
    void set_t_reply_max(int a_t_reply_max);
    Rs485Cosmos* get_rs485_cosmos();
    void set_rs485_cosmos(Rs485Cosmos* a_rs485_cosmos);
    QList<Variables*> get_variables();
    void set_variables(QList<Variables*> a_variables);
    void add_variables(CouchDBManager::BaseObject* variable);
};

Q_DECLARE_METATYPE(Bus*)

#endif // BUS_H
