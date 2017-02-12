
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
*  @file gsiDeclQStyleOptionViewItemV3.cc 
*
*  DO NOT EDIT THIS FILE. 
*  This file has been created automatically
*/

#include <QStyleOptionViewItemV3>
#include <QStyleOptionViewItem>
#include <QStyleOptionViewItemV2>
#include <QWidget>
#include "gsiQt.h"
#include "gsiQtCommon.h"
#include "gsiDeclQtTypeTraits.h"
#include <memory>

// -----------------------------------------------------------------------
// class QStyleOptionViewItemV3

//  Constructor QStyleOptionViewItemV3::QStyleOptionViewItemV3()


static void _init_ctor_QStyleOptionViewItemV3_0 (qt_gsi::GenericStaticMethod *decl)
{
  decl->set_return_new<QStyleOptionViewItemV3> ();
}

static void _call_ctor_QStyleOptionViewItemV3_0 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  ret.write<QStyleOptionViewItemV3 *> (new QStyleOptionViewItemV3 ());
}


//  Constructor QStyleOptionViewItemV3::QStyleOptionViewItemV3(const QStyleOptionViewItemV3 &other)


static void _init_ctor_QStyleOptionViewItemV3_3503 (qt_gsi::GenericStaticMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("other");
  decl->add_arg<const QStyleOptionViewItemV3 & > (argspec_0);
  decl->set_return_new<QStyleOptionViewItemV3> ();
}

static void _call_ctor_QStyleOptionViewItemV3_3503 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QStyleOptionViewItemV3 &arg1 = args.read<const QStyleOptionViewItemV3 & > (heap);
  ret.write<QStyleOptionViewItemV3 *> (new QStyleOptionViewItemV3 (arg1));
}


//  Constructor QStyleOptionViewItemV3::QStyleOptionViewItemV3(const QStyleOptionViewItem &other)


static void _init_ctor_QStyleOptionViewItemV3_3366 (qt_gsi::GenericStaticMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("other");
  decl->add_arg<const QStyleOptionViewItem & > (argspec_0);
  decl->set_return_new<QStyleOptionViewItemV3> ();
}

static void _call_ctor_QStyleOptionViewItemV3_3366 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QStyleOptionViewItem &arg1 = args.read<const QStyleOptionViewItem & > (heap);
  ret.write<QStyleOptionViewItemV3 *> (new QStyleOptionViewItemV3 (arg1));
}


// QStyleOptionViewItemV3 &QStyleOptionViewItemV3::operator =(const QStyleOptionViewItem &other)


static void _init_f_operator_eq__3366 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("other");
  decl->add_arg<const QStyleOptionViewItem & > (argspec_0);
  decl->set_return<QStyleOptionViewItemV3 & > ();
}

static void _call_f_operator_eq__3366 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QStyleOptionViewItem &arg1 = args.read<const QStyleOptionViewItem & > (heap);
  ret.write<QStyleOptionViewItemV3 & > ((QStyleOptionViewItemV3 &)((QStyleOptionViewItemV3 *)cls)->operator = (arg1));
}



namespace gsi
{

static gsi::Methods methods_QStyleOptionViewItemV3 () {
  gsi::Methods methods;
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QStyleOptionViewItemV3::QStyleOptionViewItemV3()\nThis method creates an object of class QStyleOptionViewItemV3.", &_init_ctor_QStyleOptionViewItemV3_0, &_call_ctor_QStyleOptionViewItemV3_0);
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QStyleOptionViewItemV3::QStyleOptionViewItemV3(const QStyleOptionViewItemV3 &other)\nThis method creates an object of class QStyleOptionViewItemV3.", &_init_ctor_QStyleOptionViewItemV3_3503, &_call_ctor_QStyleOptionViewItemV3_3503);
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QStyleOptionViewItemV3::QStyleOptionViewItemV3(const QStyleOptionViewItem &other)\nThis method creates an object of class QStyleOptionViewItemV3.", &_init_ctor_QStyleOptionViewItemV3_3366, &_call_ctor_QStyleOptionViewItemV3_3366);
  methods += new qt_gsi::GenericMethod ("assign", "@brief Method QStyleOptionViewItemV3 &QStyleOptionViewItemV3::operator =(const QStyleOptionViewItem &other)\n", false, &_init_f_operator_eq__3366, &_call_f_operator_eq__3366);
  return methods;
}

gsi::Class<QStyleOptionViewItemV2> &qtdecl_QStyleOptionViewItemV2 ();

gsi::Class<QStyleOptionViewItemV3> decl_QStyleOptionViewItemV3 (qtdecl_QStyleOptionViewItemV2 (), "QStyleOptionViewItemV3",
  methods_QStyleOptionViewItemV3 (),
  "@qt\n@brief Binding of QStyleOptionViewItemV3");


GSIQT_PUBLIC gsi::Class<QStyleOptionViewItemV3> &qtdecl_QStyleOptionViewItemV3 () { return decl_QStyleOptionViewItemV3; }

}
