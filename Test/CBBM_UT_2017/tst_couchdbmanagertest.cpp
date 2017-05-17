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

#define NAME __FUNCTION__

class CouchDBManagerTest : public QObject
{
    Q_OBJECT

private:
    CouchDBManager::DBManager* db;
    QString last_id;
    QString last_rev;
    QMap<QString, bool> exec;

    void sleep(int ms);
    bool skip(const QString& name);

public:
    CouchDBManagerTest();

private Q_SLOTS:
    void initTestCase();

    void createVersionableEntity();
    void lockVersionableEntity();
    void unlockVersionableEntity();
    void lockAndUpdateVersionableEntity();
    void lockAndUpgradeVersionableEntity();
    void tryDowngradeVersionableEntity();
    void lockAndUpdateVersionableEntityAgain();
    void lockVersionableEntityUsingDifferentUser();
    void tryToLockVersionableEntityUsingDifferentUser();
    void tryToLockVersionableEntityUsingAnotherUser();
    void tryToLUnlockVersionableEntityUsingAnotherUser();
    void unlockVersionableEntityUsingDifferentUser();
    void tryToLUnlockVersionableEntityUsingDifferentUser();
    void deleteVersionableEntity();
    void tryToDeleteVersionableEntityUsingDifferentUser();
    void replicateOnce();
    void addReplicationService();
    void addFilteredReplicationService();
    void listTasks();
    void listReplications();

    void cleanupTestCase();
};

CouchDBManagerTest::CouchDBManagerTest()
{
    exec.insert("CouchDBManagerTest::createVersionableEntity", true);
    exec.insert("CouchDBManagerTest::lockVersionableEntity", false);
    exec.insert("CouchDBManagerTest::unlockVersionableEntity", false);
    exec.insert("CouchDBManagerTest::lockAndUpdateVersionableEntity", true);
    exec.insert("CouchDBManagerTest::lockAndUpgradeVersionableEntity", true);
    exec.insert("CouchDBManagerTest::tryDowngradeVersionableEntity", true);
    exec.insert("CouchDBManagerTest::lockAndUpdateVersionableEntityAgain", false);
    exec.insert("CouchDBManagerTest::lockVersionableEntityUsingDifferentUser", false);
    exec.insert("CouchDBManagerTest::tryToLockVersionableEntityUsingDifferentUser", false);
    exec.insert("CouchDBManagerTest::tryToLockVersionableEntityUsingAnotherUser", false);
    exec.insert("CouchDBManagerTest::tryToLUnlockVersionableEntityUsingAnotherUser", false);
    exec.insert("CouchDBManagerTest::unlockVersionableEntityUsingDifferentUser", false);
    exec.insert("CouchDBManagerTest::tryToLUnlockVersionableEntityUsingDifferentUser", false);
    exec.insert("CouchDBManagerTest::deleteVersionableEntity", false);
    exec.insert("CouchDBManagerTest::tryToDeleteVersionableEntityUsingDifferentUser", false);
    exec.insert("CouchDBManagerTest::replicateOnce", false);
    exec.insert("CouchDBManagerTest::addReplicationService", false);
    exec.insert("CouchDBManagerTest::addFilteredReplicationService", false);
    exec.insert("CouchDBManagerTest::listTasks", true);
    exec.insert("CouchDBManagerTest::listReplications", true);

    db = new CouchDBManager::DBManager(this);

    db->registerType<Car*>();
}

bool CouchDBManagerTest::skip(const QString &name)
{
    if (exec.contains(name))
    {
        return !exec.value(name);
    }

    return true;
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

void CouchDBManagerTest::initTestCase()
{
    db->set_server_address("http://localhost:5984");
    db->set_database_name("aunia");
    db->set_timeout(5000);
    db->set_remote_conn(true);

    QStringList roles;
    bool logged_in = db->login("root", "root", roles);

    QVERIFY2(logged_in, "Not logged in");
    QVERIFY2(roles.size() > 0, "Root has no roles");
}

void CouchDBManagerTest::createVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    car.set_name("My Car");
    car.set_description("My Own Car");
    car.set_version(1);

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
    if (skip(NAME)) QSKIP("Prueba no necesaria");

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
    if (skip(NAME)) QSKIP("Prueba no necesaria");

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
    if (skip(NAME)) QSKIP("Prueba no necesaria");

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

void CouchDBManagerTest::lockAndUpgradeVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My unlocked car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    car.set_name("My new version car");
    car.set_version(2);
    db->update<Car>(&car, true);

}

void CouchDBManagerTest::tryDowngradeVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My new version car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    car.set_name("My downgraded version car");
    car.set_version(1);
    CouchDBManager::DBManagerResponse* resp = db->update<Car>(&car, true);

    QVERIFY2(!resp->get_went_ok(), "Downgrade permitted!");
    qDebug() << resp->get_response();
}

void CouchDBManagerTest::lockAndUpdateVersionableEntityAgain()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My new version car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    car.set_name("My new version car updated");

    CouchDBManager::DBManagerResponse* resp = db->update<Car>(&car, true);

    QVERIFY2(resp->get_went_ok(), "Entity not updated");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::lockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "usuario");

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");
}

void CouchDBManagerTest::tryToLockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "usuario");

    QVERIFY2(!locked, "Entity locked");
    QVERIFY2(db->get_error_string().isEmpty(), "Error generated");
}

void CouchDBManagerTest::tryToLockVersionableEntityUsingAnotherUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "distinto");

    QVERIFY2(!locked, "Entity locked");
    QVERIFY2(!db->get_error_string().isEmpty(), "No error generated");
}

void CouchDBManagerTest::tryToLUnlockVersionableEntityUsingAnotherUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked(), "Entity not locked");
    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");

    bool unlocked = db->unlock<Car>(&car, "distinto");

    QVERIFY2(!unlocked, "Entity unlocked");
    QVERIFY2(!car.get_locked_by().isEmpty(), "User empty");
    QVERIFY2(car.get_locked(), "Entity unlocked");
    QVERIFY2(car.get_locked_by() != "distinto", "Incorrect user");
    QVERIFY2(!db->get_error_string().isEmpty(), "Error not generated");
}

void CouchDBManagerTest::unlockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked(), "Entity not locked");
    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");

    bool unlocked = db->unlock<Car>(&car, "usuario");

    QVERIFY2(unlocked, "Entity not unlocked");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::tryToLUnlockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(!car.get_locked(), "Entity locked");
    QVERIFY2(car.get_locked_by().isEmpty(), "Incorrect user");

    bool unlocked = db->unlock<Car>(&car, "usuario");

    QVERIFY2(!unlocked, "Entity unlocked");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(db->get_error_string().isEmpty(), "Error generated");
}

void CouchDBManagerTest::deleteVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    CouchDBManager::DBManagerResponse* resp = db->delete_doc<Car>(&car);

    QVERIFY2(resp->get_went_ok(), "Entity not deleted");
}

void CouchDBManagerTest::tryToDeleteVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

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
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "usuario");

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");

    CouchDBManager::DBManagerResponse* resp = db->delete_doc<Car>(&car);

    QVERIFY2(!resp->get_went_ok(), "Entity deleted");
}

void CouchDBManagerTest::replicateOnce()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    CouchDBManager::ReplicationConfig cfg;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

    source->set_url("repl_test");
    target->set_url("http://172.24.2.222:5984/repl_test_dest");
    target->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);

    bool replicated = db->replicate(&cfg);

    QVERIFY2(replicated, "Replication failed");

    CouchDBManager::DBManager* remote = new CouchDBManager::DBManager(this);
    QStringList roles;

    remote->set_server_address("http://172.24.2.222:5984");
    remote->set_database_name("repl_test_dest");
    remote->login("root", "root", roles);
    remote->delete_database("repl_test_dest");
    remote->logout();

    delete remote;
}

void CouchDBManagerTest::addReplicationService()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    CouchDBManager::ReplicationConfig cfg;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

    source->set_url("http://localhost:5984/aunia_data");
    source->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    target->set_url("http://172.24.2.222:5984/repl_test_dest");
    target->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);
    cfg.set_continuous(true);

    bool added = db->add_replication_service(&cfg);

    QVERIFY2(added, "Replication service not added");
}

void CouchDBManagerTest::addFilteredReplicationService()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    CouchDBManager::ReplicationConfig cfg;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);
    QMap<QString, QVariant> params;

    source->set_url("http://localhost:5984/aunia_data");
    source->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    target->set_url("http://localhost:5984/repl_test_dest_cars_only");
    target->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);
    cfg.set_continuous(true);
    cfg.set_filter("commons/by_query");
    params.insert("where", "collection='car'");
    cfg.set_query_params(params);

    bool added = db->add_replication_service(&cfg);

    QVERIFY2(added, "Replication service not added");
}

void CouchDBManagerTest::listTasks()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    QList<CouchDBManager::ActiveTask*> tasks = db->list_active_tasks();

    QVERIFY2(!tasks.isEmpty(), "Empty task list");
}

void CouchDBManagerTest::listReplications()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    QList<CouchDBManager::ReplicationConfig*> repls = db->list_active_replications();

    QVERIFY2(!repls.isEmpty(), "Empty replications list");
}

void CouchDBManagerTest::cleanupTestCase()
{
    bool logged_out = db->logout();

    QVERIFY2(logged_out, "Not logged oun");

    delete db;
}

QTEST_MAIN(CouchDBManagerTest)

#include "tst_couchdbmanagertest.moc"
