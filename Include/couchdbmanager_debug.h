#ifndef COUCHDBMANAGER_DEBUG_H
#define COUCHDBMANAGER_DEBUG_H

#include <QDebug>

// define CouchDBManagerDebug()
#ifdef COUCHDBMANAGER_VERBOSE_DEBUG_OUTPUT
  inline QDebug CouchDBManagerDebug() { return QDebug(QtDebugMsg); }
#else
  #define CouchDBManagerDebug() if(false) QDebug(QtDebugMsg)
#endif

#endif
