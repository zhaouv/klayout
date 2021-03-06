
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2017 Matthias Koefferlein

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


#include "gsiDecl.h"
#include "tlLog.h"
#include "tlAssert.h"

#include <QMetaMethod>
#include <QMetaObject>

#include <cstdio>
#include <set>

namespace gsi
{

// -------------------------------------------------------------------------
//  ClassBase implementation

ClassBase::class_collection *ClassBase::mp_class_collection = 0;

ClassBase::ClassBase (const std::string &doc, const Methods &mm, bool do_register)
  : mp_base (0), mp_parent (0), m_doc (doc), m_methods (mm)
{ 
  if (do_register) {
    if (! mp_class_collection) {
      mp_class_collection = new class_collection ();
    }
    mp_class_collection->push_back (this);
  }
}

ClassBase::~ClassBase ()
{
  //  .. nothing yet ..
}

void
ClassBase::set_parent (const ClassBase *p)
{
  mp_parent = p;
}

void
ClassBase::set_base (const ClassBase *b)
{
  mp_base = b;
}

std::string 
ClassBase::qname () const
{
  std::string qn = name ();
  const gsi::ClassBase *p = this;
  while (p->parent ()) {
    p = p->parent ();
    qn = p->name () + "::" + qn;
  }
  return qn;
}

void
ClassBase::add_child_class (const ClassBase *cls)
{
  //  TODO: ugly const_cast hack
  ClassBase *non_const_cls = const_cast<ClassBase *> (cls);
  non_const_cls->set_parent (this);
  m_child_classes.push_back (const_cast <ClassBase *> (cls));
}

bool 
ClassBase::is_derived_from (const ClassBase *base) const
{
  if (base == this) {
    return true;
  } else if (!mp_base) {
    return false;
  } else {
    return mp_base->is_derived_from (base);
  }
}

/**
 *  @brief This function returns true if the given method m of target can convert an object of type from into target.
 */
static bool 
is_constructor_of (const ClassBase *target, const MethodBase *m, const ClassBase *from)
{
  if (! m->is_static ()) {
    //  only static methods are constructors
    return false;
  }
  if (! m->ret_type ().pass_obj () || ! m->ret_type ().is_ptr () || m->ret_type ().cls () != target) {
    //  the return type has to be a new'd pointer of the right type
    return false;
  }
  if (! m->compatible_with_num_args (1)) {
    //  the constructor has to accept one argument
    return false;
  }

  const ArgType a0 = m->arg (0);

  if (! a0.cls () || ! from->is_derived_from (a0.cls ())) {
    //  the single argument must be of the right type
    return false;
  }

  //  And finally the argument must be of const ref or direct type
  if (a0.is_cref ()) {
    return true;
  } else if (! a0.is_ptr () && ! a0.is_cptr () && ! a0.is_ref ()) {
    return true;
  } else {
    return false;
  }
}

bool 
ClassBase::can_convert_to (const ClassBase *target) const
{
  for (method_iterator m = target->begin_methods (); m != target->end_methods (); ++m) {
    if (is_constructor_of (target, *m, this)) {
      return true;
    }
  }
  return false;
}

void *
ClassBase::create_obj_from (const ClassBase *from, void *obj) const
{
  const MethodBase *ctor = 0;

  for (method_iterator m = begin_methods (); m != end_methods (); ++m) {
    if (is_constructor_of (this, *m, from)) {
      if (ctor) {
        throw tl::Exception (tl::to_string (QObject::tr ("There are multiple conversion constructors available to convert object of type %s to type %s")), from->name (), name ());
      }
      ctor = *m;
    }
  }

  tl_assert (ctor != 0);

  SerialArgs ret (ctor->retsize ());

  SerialArgs args (ctor->argsize ());
  if (ctor->arg (0).is_cref ()) {
    args.write<void *> (obj);
  } else {
    //  direct type objects will transfer ownership to the caller by convention
    args.write<void *> (from->clone (obj));
  }

  ctor->call (0, args, ret);

  tl::Heap heap;
  return ret.read<void *> (heap);
}

const ClassBase::class_collection &
ClassBase::collection () 
{
  if (!mp_class_collection) {
    static const class_collection empty;
    return empty;
  } else {
    return *mp_class_collection;
  }
}

static SpecialMethod *
sm_default_ctor (const char *name, const gsi::ClassBase *cls)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Creates a new object of this class\n")), 
    false,   //  non-const
    true,    //  static
    MethodBase::DefaultCtor);

  gsi::ArgType ret;
  ret.set_is_ptr (true);
  ret.set_type (gsi::T_object);
  ret.set_pass_obj (true);
  ret.set_cls (cls);
  sm->set_return (ret);

  return sm;
}

static SpecialMethod *
sm_destroy (const char *name)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Explicitly destroys the object\nExplicitly destroys the object on C++ side if it was owned by the script interpreter. Subsequent access to this object will throw an exception.\n" 
                                "If the object is not owned by the script, this method will do nothing.")), 
    false,   //  non-const
    false,   //  non-static
    MethodBase::Destroy);

  return sm;
}

static SpecialMethod *
sm_create (const char *name)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Ensures the C++ object is created\n"
                                "Use this method to ensure the C++ object is created, for example to ensure that resources are allocated. "
                                "Usually C++ objects are created on demand and not necessarily when the script object is created.")),
    false,   //  non-const
    false,   //  non-static
    MethodBase::Create);

  return sm;
}

static SpecialMethod *
sm_keep (const char *name)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Marks the object as no longer owned by the script side.\n" 
                                "Calling this method will make this object no longer owned by the script's memory management. "
                                "Instead, the object must be managed in some other way. Usually this method may be called if it is known that some C++ object holds and manages this object. "
                                "Technically speaking, this method will turn the script's reference into a weak reference. "
                                "After the script engine decides to delete the reference, the object itself will still exist. "
                                "If the object is not managed otherwise, memory leaks will occur.\n\n"
                                "Usually it's not required to call this method. It has been introduced in version 0.24.")), 
    false,   //  non-const
    false,   //  non-static
    MethodBase::Keep);

  return sm;
}

static SpecialMethod *
sm_release (const char *name)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Marks the object as managed by the script side.\n"
                                "After calling this method on an object, the script side will be responsible for the management of the object. "
                                "This method may be called if an object is returned from a C++ function and the object is known not to be owned by any C++ instance. "
                                "If necessary, the script side may delete the object if the script's reference is no longer required.\n\n"
                                "Usually it's not required to call this method. It has been introduced in version 0.24.")),
    false,   //  non-const
    false,   //  non-static
    MethodBase::Release);

  return sm;
}

static SpecialMethod *
sm_is_const (const char *name)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Returns a value indicating whether the reference is a const reference\nThis method returns true, if self is a const reference.\n" 
                                "In that case, only const methods may be called on self.")),
    true,    //  const
    false,   //  non-static
    MethodBase::IsConst);

  gsi::ArgType ret;
  ret.set_type (gsi::T_bool);
  sm->set_return (ret);

  return sm;
}

static SpecialMethod *
sm_destroyed (const char *name)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Returns a value indicating whether the object was already destroyed\nThis method returns true, if the object was destroyed, either explicitly or by the C++ side.\n" 
                                "The latter may happen, if the object is owned by a C++ object which got destroyed itself.")),
    true,    //  const
    false,   //  non-static
    MethodBase::Destroyed);

  gsi::ArgType ret;
  ret.set_type (gsi::T_bool);
  sm->set_return (ret);

  return sm;
}

static SpecialMethod *
sm_dup (const char *name, const gsi::ClassBase *cls)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Creates a copy of self\n")),
    true,    //  const
    false,   //  non-static
    MethodBase::Dup);

  gsi::ArgType ret;
  ret.set_is_ptr (true);
  ret.set_type (gsi::T_object);
  ret.set_pass_obj (true);
  ret.set_cls (cls);
  sm->set_return (ret);

  return sm;
}

static SpecialMethod *
sm_assign (const char *name, const gsi::ClassBase *cls)
{
  SpecialMethod *sm = new SpecialMethod (name,
    tl::to_string (QObject::tr ("@brief Assigns another object to self\n@args other")),
    false,   //  non-const
    false,   //  non-static
    MethodBase::Assign);

  gsi::ArgType a;
  a.set_is_cref (true);
  a.set_type (gsi::T_object);
  a.set_cls (cls);
  sm->add_arg (a);

  return sm;
}

void
ClassBase::merge_declarations ()
{
  tl_assert (mp_class_collection != 0);

  //  merge the extensions to the main declaration
  static bool merged = false;
  if (! merged) {

    //  HINT: this code block must be called exactly ONCE!
    merged = true;

    //  Check for duplicate declarations
    std::set<const std::type_info *> types;
    std::set<std::string> names;
    for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
      if (c->declaration () == &*c && !types.insert (&c->type ()).second) {
        tl::warn << "Duplicate GSI declaration of type " << c->type ().name (); 
      }
      if (c->declaration () == &*c && !names.insert (c->declaration ()->name ()).second) {
        tl::warn << "Duplicate GSI declaration of name " << c->declaration ()->name (); 
      }
    }

    std::vector <const gsi::ClassBase *> to_remove;

    //  Consolidate the classes (merge, remove etc.)
    for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
      if (! c->consolidate()) {
        to_remove.push_back (&*c);
      }
    }

    //  removed the classes which are no longer required
    for (std::vector <const gsi::ClassBase *>::const_iterator ed = to_remove.begin (); ed != to_remove.end (); ++ed) {
      //  TODO: ugly const_cast hack
      mp_class_collection->erase (const_cast<gsi::ClassBase *> (*ed));
    }

    //  collect the subclasses of a class
    for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
      if (c->base ()) {
        //  TODO: ugly const_cast hack
        const_cast<gsi::ClassBase *> (c->base ())->m_subclasses.push_back (const_cast<gsi::ClassBase *> (c.operator-> ()));
      }
    }

    //  Add to the classes the special methods and clean up the method table
    for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {

      std::set<std::pair<std::string, bool> > name_map;
      for (gsi::ClassBase::method_iterator m = c->begin_methods (); m != c->end_methods (); ++m) {
        for (gsi::MethodBase::synonym_iterator syn = (*m)->begin_synonyms (); syn != (*m)->end_synonyms (); ++syn) {
          name_map.insert (std::make_pair (syn->name, (*m)->is_static ()));
        }
      }

      //  We don't want the declaration object to be non-const except for this case. So
      //  we const_cast here.
      gsi::ClassBase *non_const_decl = const_cast<gsi::ClassBase *> (&*c);

      if (name_map.find (std::make_pair ("new", true)) == name_map.end ()) {
        non_const_decl->add_method (sm_default_ctor ("new", &*c), false);
      }

      //  Note: "unmanage" and "manage" is a better name ...
      non_const_decl->add_method (sm_keep ("_unmanage"));
      non_const_decl->add_method (sm_release ("_manage"));

      if (name_map.find (std::make_pair ("create", false)) == name_map.end ()) {
        //  deprecate "create"
        non_const_decl->add_method (sm_create ("_create|#create"));
      } else {
        //  fallback name is "_create" to avoid conflicts
        non_const_decl->add_method (sm_create ("_create"));
      }

      if (c->can_destroy ()) {
        if (name_map.find (std::make_pair ("destroy", false)) == name_map.end ()) {
          //  deprecate "destroy"
          non_const_decl->add_method (sm_destroy ("_destroy|#destroy"));
        } else {
          //  fallback name is "_destroy" to avoid conflicts
          non_const_decl->add_method (sm_destroy ("_destroy"));
        }
      }

      if (c->can_copy ()) {

        if (name_map.find (std::make_pair ("dup", false)) == name_map.end ()) {
          non_const_decl->add_method (sm_dup ("dup", &*c));
        } else {
          //  fallback name is "_dup" to avoid conflicts
          non_const_decl->add_method (sm_dup ("_dup", &*c));
        }

        if (name_map.find (std::make_pair ("assign", false)) == name_map.end ()) {
          non_const_decl->add_method (sm_assign ("assign", &*c));
        } else {
          //  fallback name is "_assign" to avoid conflicts
          non_const_decl->add_method (sm_assign ("_assign", &*c));
        }

      }

      if (name_map.find (std::make_pair ("destroyed", false)) == name_map.end ()) {
        //  deprecate "destroyed"
        non_const_decl->add_method (sm_destroyed ("_destroyed?|#destroyed?"));
      } else {
        //  fallback name is "_destroyed" to avoid conflicts
        non_const_decl->add_method (sm_destroyed ("_destroyed?"));
      }

      if (name_map.find (std::make_pair ("is_const_object", false)) == name_map.end ()) {
        //  deprecate "is_const"
        non_const_decl->add_method (sm_is_const ("_is_const_object?|#is_const_object?"));
      } else {
        //  fallback name is "_is_const" to avoid conflicts
        non_const_decl->add_method (sm_is_const ("_is_const_object?"));
      }

    }

  }
}

void
ClassBase::add_method (MethodBase *method, bool /*base_class*/)
{
  m_methods.add_method (method);
}

const ClassBase *class_by_name (const std::string &name)
{
  for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
    if (c->declaration () == &*c && c->name () == name) {
      return &*c;
    }
  }
  //  No class with that name
  tl_assert (false);
}

bool has_class (const std::string &name)
{
  for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
    if (c->declaration () == &*c && c->name () == name) {
      return true;
    }
  }
  return false;
}

const ClassBase *class_by_typeinfo (const std::type_info &ti)
{
  for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
    if (c->declaration () == &*c && c->is_of_type (ti)) {
      return &*c;
    }
  }
  //  No class with that type
  tl_assert (false);
}

bool has_class (const std::type_info &ti)
{
  for (gsi::ClassBase::class_iterator c = gsi::ClassBase::begin_classes (); c != gsi::ClassBase::end_classes (); ++c) {
    if (c->declaration () == &*c && c->is_of_type (ti)) {
      return true;
    }
  }
  return false;
}

}

