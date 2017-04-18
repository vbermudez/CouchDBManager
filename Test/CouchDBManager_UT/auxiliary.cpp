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
// AUTHOR: .......... $Author: jmonasterio $
//
// REVISION: ........ $Revision: 1.1 $ ($Date: 2016/07/27 12:07:12 $)
//
// REFERENCE: ....... AD.37.91.3057
//
// CHANGES:
//
#include "auxiliary.h"

QString Rule::get_replace() const
{
    return replace;
}

void Rule::set_replace(const QString &value)
{
    replace = value;
}


QString Rule::get_prefix() const
{
    return prefix;
}

void Rule::set_prefix(const QString &value)
{
    prefix = value;
}


QString Rule::get_suffix() const
{
    return suffix;
}

void Rule::set_suffix(const QString &value)
{
    suffix = value;
}

QString Rule::get_with() const
{
    return with;
}

void Rule::set_with(const QString &value)
{
    with = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CouchDBManager::BaseReference *Linked::get_parent() const
{
    return parent;
}

void Linked::set_parent(CouchDBManager::BaseReference *value)
{
    parent = value;
}

Rule *Linked::get_rule() const
{
    return rule;
}

void Linked::set_rule(Rule *value)
{
    rule = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

