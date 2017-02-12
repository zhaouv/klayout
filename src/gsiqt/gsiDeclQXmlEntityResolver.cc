
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2016 Matthias Koefferlein

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

/**
*  @file gsiDeclQXmlEntityResolver.cc 
*
*  DO NOT EDIT THIS FILE. 
*  This file has been created automatically
*/

#include <QXmlEntityResolver>
#include "gsiQt.h"
#include "gsiQtCommon.h"
#include "gsiDeclQtTypeTraits.h"
#include <memory>

// -----------------------------------------------------------------------
// class QXmlEntityResolver

// QString QXmlEntityResolver::errorString()


static void _init_f_errorString_c0 (qt_gsi::GenericMethod *decl)
{
  decl->set_return<QString > ();
}

static void _call_f_errorString_c0 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  ret.write<QString > ((QString)((QXmlEntityResolver *)cls)->errorString ());
}



namespace gsi
{

static gsi::Methods methods_QXmlEntityResolver () {
  gsi::Methods methods;
  methods += new qt_gsi::GenericMethod ("errorString", "@brief Method QString QXmlEntityResolver::errorString()\n", true, &_init_f_errorString_c0, &_call_f_errorString_c0);
  return methods;
}

gsi::Class<QXmlEntityResolver> decl_QXmlEntityResolver ("QXmlEntityResolver",
  methods_QXmlEntityResolver (),
  "@qt\n@brief Binding of QXmlEntityResolver");


GSIQT_PUBLIC gsi::Class<QXmlEntityResolver> &qtdecl_QXmlEntityResolver () { return decl_QXmlEntityResolver; }

}
