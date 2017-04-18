#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QVariant>
#include "variables.h"
#include "bus.h"

class Device : public CouchDBManager::VersionableEntity
{
    Q_OBJECT
    Q_PROPERTY(QString type READ get_type WRITE set_type)
    Q_PROPERTY(int version READ get_version WRITE set_version)
    Q_PROPERTY(QString description READ get_description WRITE set_description)
    Q_PROPERTY(QList<Bus*> bus READ get_bus WRITE set_bus)
//    Q_PROPERTY(QList<Variables*> variables READ get_variables WRITE set_variables)

private:
    QString type;
    int version;
    QString description;
    QList<Bus*> bus;

public:
    Q_INVOKABLE Device(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {


        this->collection = "device";
    }

public slots:
    QString get_type() const;
    void set_type(const QString& a_type);
    int get_version() const;
    void set_version(int a_version);
    QString get_description() const;
    void set_description(const QString& a_description);

    QList<Bus*> get_bus() const;
    void set_bus(const QList<Bus*>& a_bus);
    // ESTE METODO ES NECESARIO PARA LA SERIALIZACION JSON->OBJETO!!!!
    void add_bus(CouchDBManager::BaseObject* a_bus);
};

Q_DECLARE_METATYPE(Device*)

#endif // DEVICE_H
