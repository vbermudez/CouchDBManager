#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QDebug>
#include <QList>
#include <QJsonObject>
#include <QJsonValue>
#include <QDate>
#include <QMap>
#include <QMetaType>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "couchdbmanager.h"
#include "dbmanagerresponse.h"
#include "usercontext.h"
#include "car.h"

class CouchDBManagerTest : public QObject
{
    Q_OBJECT

private:
    CouchDBManager::DBManager* db;
    QString last_id;
    QString last_rev;

    void sleep(int ms);

public:
    CouchDBManagerTest();

private Q_SLOTS:
    void configureObject();
    void userLogin();

    void createEntity();
    void lockVersionableEntity();
    void unlockVersionableEntity();
    void lockAndUpdateVersionableEntity();

    void userLogout();
    void destroyManager();
};

CouchDBManagerTest::CouchDBManagerTest()
{
    db = new CouchDBManager::DBManager(this);

    db->registerType<Car*>();
}

void CouchDBManagerTest::sleep(int ms)
{
    QTEST_ASSERT(ms > 0);

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

void CouchDBManagerTest::configureObject()
{
//    QSKIP("Prueba no necesaria");

    db->set_server_address("http://localhost:5984");
    db->set_database_name("aunia");
    db->set_timeout(5000);

    QVERIFY2(db->get_server_address() == "http://localhost:5984", "Incorrect server address");
    QVERIFY2(db->get_database_name() == "aunia", "Incorrect database name");
    QVERIFY2(QString(db->metaObject()->className()) == "CouchDBManager::DBManager", "Object is not DBManager");
    QVERIFY2(db->get_timeout() == 5000, "Incorrect timeout");
}

void CouchDBManagerTest::userLogin()
{
//    QSKIP("Prueba no necesaria");

    QStringList roles;
    bool logged_in = db->login("root", "root", roles);

    QVERIFY2(logged_in, "Not logged in");
    QVERIFY2(roles.size() > 0, "Root has no roles");
    qDebug() << "# AuthCookie" << db->get_auth_cookie();
}

void CouchDBManagerTest::createEntity()
{
//    QSKIP("Prueba no necesaria");

    Car car;

    car.set_name("My Car");
    car.set_description("My Own Car");
    car.set_version(12);

    CouchDBManager::DBManagerResponse* response = db->create<Car>(&car);

    QVERIFY2(response->get_went_ok(), "Operation failed");
    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");

    last_id = car.get_id();
    last_rev = car.get_rev();
}

void CouchDBManagerTest::lockVersionableEntity()
{
//    QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");
}

void CouchDBManagerTest::unlockVersionableEntity()
{
//    QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked(), "Entity not locked");
    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    bool unlocked = db->unlock<Car>(&car);

    QVERIFY2(unlocked, "Entity not unlocked");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::lockAndUpdateVersionableEntity()
{
//    QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    car.set_name("My unlocked car");

    CouchDBManager::DBManagerResponse* resp = db->update<Car>(&car, true);

    QVERIFY2(resp->get_went_ok(), "Entity not updated");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::userLogout()
{
//    QSKIP("Prueba no necesaria");

    bool logged_out = db->logout();

    QVERIFY2(logged_out, "Not logged oun");
}

void CouchDBManagerTest::destroyManager()
{
//    QSKIP("Prueba no necesaria");

    delete db;
}

QTEST_MAIN(CouchDBManagerTest)

#include "tst_couchdbmanagertest.moc"
