#include "bus.h"

int Bus::get_basic_period() const
{
    return this->basic_period;
}

void Bus::set_basic_period(int a_basic_period)
{
    this->basic_period = a_basic_period;
}

int Bus::get_t_reply_max() const
{
    return this->t_reply_max;
}

void Bus::set_t_reply_max(int a_t_reply_max)
{
    this->t_reply_max = a_t_reply_max;
}

Rs485Cosmos* Bus::get_rs485_cosmos()
{
    return this->rs485_cosmos;
}

void Bus::set_rs485_cosmos(Rs485Cosmos* a_rs485_cosmos)
{
    this->rs485_cosmos = a_rs485_cosmos;
}

QList<Variables*> Bus::get_variables()
{
    return this->variables;
}

void Bus::set_variables(QList<Variables *> a_variables)
{
    this->variables = a_variables;
}

void Bus::add_variables(CouchDBManager::BaseObject *variable)
{
    this->variables.append( qobject_cast<Variables*>(variable) );
}
