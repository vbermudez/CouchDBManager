#ifndef RS485COSMOS_H
#define RS485COSMOS_H

#include <QObject>
#include <QString>

#include "baseobject.h"

class Rs485Cosmos : public CouchDBManager::BaseObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ get_name WRITE set_name)
    Q_PROPERTY(int baudrate READ get_baudrate WRITE set_baudrate)
    Q_PROPERTY(QString parity READ get_parity WRITE set_parity)
    Q_PROPERTY(int bit_stop READ get_bit_stop WRITE set_bit_stop)
    Q_PROPERTY(QString error_detection READ get_error_detection WRITE set_error_detection)

private:
    QString name;
    int baudrate;
    QString parity;
    int bit_stop;
    QString error_detection;

public:
    Q_INVOKABLE Rs485Cosmos(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {


        this->baudrate = 0;
        this->bit_stop = 0;
    }

public slots:
    QString get_name() const { return this->name; }
    void set_name(const QString& value) { this->name = value; }
    int get_baudrate() const { return this->baudrate; }
    void set_baudrate(int value) { this->baudrate = value; }
    QString get_parity() const { return this->parity; }
    void set_parity(const QString& value) { this->parity = value; }
    int get_bit_stop() const { return this->bit_stop; }
    void set_bit_stop(int value) { this->bit_stop = value; }
    QString get_error_detection() const { return this->error_detection; }
    void set_error_detection(const QString& value) { this->error_detection = value; }

};

Q_DECLARE_METATYPE(Rs485Cosmos*)

#endif // RS485COSMOS_H
