# CouchDBManager

> A C++/Qt API that manages entities stored in CouchDB.

## Novedades 2017

### Gestión de bloqueos

Ahora es posible gestionar los bloqueos de las entidades versionables a nivel de usuario. Además, al actualizar una entidad, se comprueba el estado del bloqueo. Si la entidad no está bloqueada, o está bloqueada por otro usuario, se lanza una excepción.

Este mecanismo entra en funcionamiento únicamente si la conexión es remota o si se fuerza dicho comportamiento.

Los siguientes ejemplos pueden servir como referencia.

#### Creación de CouchDBManager, con detección automática del tipo de conexión (remota o local):

- Contructor sin configuración
```c++
CouchDBManager::DBManager* db = new CouchDBManager::DBManager(this);

db->set_server_address("http://nombre.o.ip:5984");
db->set_database_name("base_de_datos");
qDebug() << db->get_remote_conn(); // prints true
```

- Constructor con configuración
```c++
CouchDBManager::DBManager* db = new CouchDBManager::DBManager("http://nombre.o.ip:5984", "base_de_datos", this);
qDebug() << db->get_remote_conn(); // prints true
```

#### Inicia sesión en CouchDB
```c++
QStringList roles;
bool logged_in = db->login("usuario", "clave", roles);
qDebug() << logged_in; // prints true or false
qDebug() << roles; // prints list of roles or nothing
```

#### Recupera una entidad

```c++
VersionableEntity entity;
db->read<VersionableEntity>("identificador", &entity);
qDebug() << entity.get_locked(); // prints true or false
qDebug() << entity.get_locked_by(); // prints "" or user name
```

#### Bloqueo/desbloqueo de una entidad. Actualiza la BBDD.

- Bloqueo
```c++
bool locked = db->lock<VersionableEntity>(&e);
qDebug() << locked; // prints true or false
```

- Desbloqueo
```c++
bool unlocked = db->unlock<VersionableEntity>(&e);
qDebug() << unlocked; // prints true or false
```

#### Actualiza una entidad

- Con desbloqueo en una petición
```c++
bool updated = db->update<VersionableEntity>(&e, true);
qDebug() << updated; // prints true or false
```

- Con desbloqueo en otra petición
```c++
bool updated = db->update<VersionableEntity>(&e);
qDebug() << updated; // prints true or false
bool unlocked = db->unlock<VersionableEntity>(&e);
qDebug() << unlocked; // prints true or false
```

#### Errores

- Intenta bloquear una entidad ya bloqueada por el usuario actual
```c++
bool locked = db->lock<VersionableEntity>(&e);
qDebug() << locked; // prints false
// Escribe en el stream qWarning() un aviso
qDebug() << db->get_error_string() // prints nothing
```

- Intenta desbloquear una entidad ya desbloqueada
```c++
bool unlocked = db->unlock<VersionableEntity>(&e);
qDebug() << unlocked; // prints false
// Escribe en el stream qWarning() un aviso
qDebug() << db->get_error_string() // prints nothing
```

- Intenta bloquear una entidad ya bloqueada por otro usuario
```c++
bool locked = db->lock<VersionableEntity>(&e);
qDebug() << locked; // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```

- Intenta desbloquear una entidad ya bloqueada por otro usuario
```c++
bool unlocked = db->unlock<VersionableEntity>(&e);
qDebug() << unlocked; // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```

- Intenta actualizar una entidad no bloqueada o bloqueada por otro usuario
```c++
bool updated = db->update<VersionableEntity>(&e); // Igual con la sobrecarga
qDebug() << updated; // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```