//! @addtogroup CouchDBManager
//! @{
//! @file "couchdbmanager_global.h"
//! @brief Macros de exportación/compilación de CouchDBManager.
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
// REVISION: ........ $Revision: 1.3 $ ($Date: 2016/06/17 12:01:37 $)
//
// REFERENCE: .......
//
// CHANGES:
//
// $Log: couchdbmanager_global.h,v $
// Revision 1.3  2016/06/17 12:01:37  jmonasterio
// New architecture
//
// Revision 0.0.4   2016/05/31 12:31:00 vbermudez@xi-tek.com
// Cambios realizados:
// - Añadidos los comentarios
//
#ifndef COUCHDBMANAGER_GLOBAL_H
#define COUCHDBMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COUCHDBMANAGER_LIBRARY)
#  define COUCHDBMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COUCHDBMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COUCHDBMANAGER_GLOBAL_H
//! @}
