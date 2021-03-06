
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

 
#ifndef HDR_utHead
#define HDR_utHead

#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cmath>

#include "utCommon.h"
#include "tlString.h"
#include "tlException.h"
#include "tlStaticObjects.h"
#include "dbStatic.h"
#include "dbTypes.h"
#include "gsiExpression.h"
#include "gsiInterpreter.h"

#include <QCoreApplication>
#include <QFileInfo>

namespace db {
  class Layout;
  class LayerMap;
}

namespace rba {
  class RubyInterpreter;
}

namespace pya {
  class PythonInterpreter;
}

namespace ut {

/**
 *  @brief The unit test execution function
 */
UT_PUBLIC int main (int argc, char **argv);

/**
 *  @brief Prints a error message to the unit test output stream
 */
UT_PUBLIC void print_error (const std::string &s);

/**
 *  @brief A detailed diff printer
 */
UT_PUBLIC void write_detailed_diff (std::ostream &os, const std::string &subject, const std::string &ref);

/**
 *  @brief Returns true, if the test is run in verbose mode
 *  Verbose mode is enabled through the "-v" option
 */
UT_PUBLIC bool verbose ();

/**
 *  @brief Returns the Ruby interpreter
 */
UT_PUBLIC rba::RubyInterpreter *ruby_interpreter ();

/**
 *  @brief Returns the Python interpreter
 */
UT_PUBLIC pya::PythonInterpreter *python_interpreter ();

/**
 *  @brief Returns true, if the unit test is run in debug mode
 *  In debug mode, the unit tests shall offer information on how to fix the 
 *  test. Specifically if layout compare is involved, it shall display the golden
 *  file name and the actual one and terminate to allow updating the files. 
 */
UT_PUBLIC bool is_debug_mode ();

/**
 *  @brief Specifies the normalization mode for compare_layouts
 */
enum NormalizationMode
{
  NoNormalization,        //  no normalization - take the test subject as it is
  WriteGDS2,              //  normalize subject by writing to GDS2 and reading back
  WriteOAS                //  normalize subject by writing to OASIS and reading back
};

/**
 *  @brief Gets the path of the test data
 *  This path is specified through the environment variable $TESTSRC
 */
UT_PUBLIC std::string testsrc ();

/**
 *  @brief Gets the path of the private test data
 *  This path is specified through the environment variable $TESTSRC and the
 *  private testdata directory. If no private test data is available, this
 *  method will throw a CancelException which makes the test skipped.
 */
UT_PUBLIC std::string testsrc_private ();

/**
 *  @brief A basic exception for the unit test framework
 */
struct Exception 
  : public tl::Exception
{
  Exception (const std::string &msg)
    : tl::Exception (msg)
  { }
};

/**
 *  @brief The base class for tests
 */
struct UT_PUBLIC TestBase
{
  /**
   *  @brief Constructor
   */
  TestBase (const std::string &file, const std::string &name);

  /**
   *  @brief Destructor
   */
  virtual ~TestBase () { }

  /**
   *  @brief Actually runs the test
   *  @return True, if the test was successful
   */
  bool do_test (const std::string &mode);

  /**
   *  @brief Raises an exception with the given string
   *  This version prints the last checkpoint for reference.
   */
  void raise (const std::string &s);

  /**
   *  @brief Raises an exception with the given string, file and line number
   */
  void raise (const std::string &file, int line, const std::string &s);

  /**
   *  @brief Registers a checkpoint
   */
  void checkpoint (const std::string &file, int line);

  /**
   *  @brief Resets the checkpoints set
   */
  void reset_checkpoint ();

  /**
   *  @brief Compares a layout with a golden layout file
   *  @param layout The layout to compare
   *  @param au_file The golden file path
   *  @param norm The normalization mode (see NormalizationMode)
   *  @param tolerance A tolerance applied when comparing shapes in database units
   *  The layout is normalized by writing to a file and reading back
   */
  void compare_layouts (const db::Layout &layout, const std::string &au_file, NormalizationMode norm = WriteGDS2, db::Coord tolerance = 0);

  /**
   *  @brief Compares a layout with a golden layout file with layer mapping
   *  @param layout The layout to compare
   *  @param au_file The golden file path
   *  @param lmap The layer mapping object
   *  @param read_all_others If true, all other layers will be read too
   *  @param norm The normalization mode (see NormalizationMode)
   *  @param tolerance A tolerance applied when comparing shapes in database units
   *  The layout is normalized by writing to a file and reading back
   */
  void compare_layouts (const db::Layout &layout, const std::string &au_file, const db::LayerMap &lmap, bool read_all_others, NormalizationMode norm = WriteGDS2, db::Coord tolerance = 0);

  /**
   *  @brief The test's name
   *  @return The name of the test
   */
  const std::string &name () const
  {
    return m_test;
  }

  /**
   *  @brief Prepares a temporary file path
   *  @param fn The actual name of the file
   *  @return A path suitable for writing a temporary file
   *  The directory for the file will be created within this method.
   */
  std::string tmp_file (const std::string &fn = "tmp") const;

  /**
   *  @brief Removes all temporary files
   */
  void remove_tmp_folder ();

  /**
   *  @brief A generic diff printer
   */
  template <class X, class Y>
  void diff (const std::string &file, int line, const std::string &msg, const X &subject, const Y & /*ref*/)
  {
    std::ostringstream sstr;
    sstr << msg << " (actual value is " << subject << ")";
    raise (file, line, sstr.str ());
  }

  /**
   *  @brief A generic diff printer
   */
  template <class X, class Y>
  void detailed_diff (const std::string &file, int line, const std::string &msg, const X &subject, const Y &ref)
  {
    std::ostringstream sstr;
    sstr << msg << std::endl;
    ut::write_detailed_diff (sstr, tl::to_string (subject), tl::to_string (ref));
    raise (file, line, sstr.str ());
  }

  /**
   *  @brief A diff printer for int vs. something
   */
  template <class Y>
  void diff (const std::string &file, int line, const std::string &msg, int subject, const Y &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for unsigned int vs. something
   */
  template <class Y>
  void diff (const std::string &file, int line, const std::string &msg, unsigned int subject, const Y &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for long vs. something
   */
  template <class Y>
  void diff (const std::string &file, int line, const std::string &msg, long subject, const Y &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for unsigned long vs. something
   */
  template <class Y>
  void diff (const std::string &file, int line, const std::string &msg, unsigned long subject, const Y &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for long long vs. something
   */
  template <class Y>
  void diff (const std::string &file, int line, const std::string &msg, long long subject, const Y &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for unsigned long long vs. something
   */
  template <class Y>
  void diff (const std::string &file, int line, const std::string &msg, unsigned long long subject, const Y &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for bool
   */
  inline void diff (const std::string &file, int line, const std::string &msg, bool subject, bool ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for double
   */
  inline void diff (const std::string &file, int line, const std::string &msg, double subject, double ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for strings
   */
  inline void diff (const std::string &file, int line, const std::string &msg, const std::string &subject, const std::string &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for strings vs. const char *
   */
  inline void diff (const std::string &file, int line, const std::string &msg, const std::string &subject, const char *ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for strings vs. const char *
   */
  inline void diff (const std::string &file, int line, const std::string &msg, const char *subject, const std::string &ref)
  {
    detailed_diff (file, line, msg, subject, ref);
  }

  /**
   *  @brief A diff printer for C strings
   */
  inline void diff (const std::string &file, int line, const std::string &msg, const char *subject, const char *ref)
  {
    diff (file, line, msg, std::string (subject), std::string (ref));
  }

private:
  virtual void execute (ut::TestBase *_this) throw (tl::Exception) = 0;

  std::string m_test;
  std::string m_testdir;
  //  last checkpoint
  std::string m_cp_file;
  int m_cp_line;
  bool m_any_failed;
  QString m_testtmp;
};

/**
 *  @brief The registration facility for tests
 */
struct Registrar
{
  static void reg (ut::TestBase *t)
  {
    if (! ms_instance) {
      ms_instance = new Registrar ();
    }
    ms_instance->m_tests.push_back (t);
  }

  static Registrar *instance ()
  {
    return ms_instance;
  }

  const std::vector <ut::TestBase *> &tests () const
  {
    return m_tests;
  }
  
private:
  static Registrar *ms_instance;

  Registrar () : m_tests () { }

  std::vector <ut::TestBase *> m_tests;
};

/**
 *  @brief A generic compare operator
 */
template <class X, class Y>
inline bool equals (const X &a, const Y &b)
{
  return a == b;
}

/**
 *  @brief A specialization of the compare operator for doubles
 */
UT_PUBLIC bool equals (double a, double b);

/**
 *  @brief Specialization of comparison of pointers vs. integers (specifically "0")
 */
template <class X>
inline bool equals (X *a, int b)
{
  return a == (X *) size_t (b);
}

/**
 *  @brief A specialization of comparison of double vs "anything"
 */
template <class Y>
inline bool equals (double a, const Y &b)
{
  return equals (a, double (b));
}

/**
 *  @brief A specialization of comparison of "anything" vs. double
 */
template <class X>
inline bool equals (const X &a, double b)
{
  return equals (double (a), b);
}

/**
 *  @brief A specialization of the compare operator for const char *
 */
inline bool equals (const char *a, const char *b)
{
  return equals (std::string (a), std::string (b));
}

/**
 *  @brief A specialization of the compare operator for std::string vs. const char *
 */
inline bool equals (const std::string &a, const char *b)
{
  return equals (a, std::string (b));
}

/**
 *  @brief A specialization of the compare operator for std::string vs. const char *
 */
inline bool equals (const char *a, const std::string &b)
{
  return equals (std::string (a), b);
}

} // namespace ut

#define TEST(NAME) \
  namespace {\
struct TestImpl##NAME \
      : public ut::TestBase \
    { \
      TestImpl##NAME () : TestBase (__FILE__, #NAME) { } \
      virtual void execute (ut::TestBase *_this) throw (tl::Exception); \
    }; \
    static TestImpl##NAME TestImpl_Inst##NAME; \
  } \
  void TestImpl##NAME::execute (ut::TestBase *_this) throw (tl::Exception)

#define EXPECT_EQ(WHAT,EQUALS) \
  _this->checkpoint (__FILE__, __LINE__); \
  if (! ut::equals ((WHAT), (EQUALS))) { \
    std::ostringstream sstr; \
    sstr << #WHAT << " does not equal " << #EQUALS; \
    _this->diff (__FILE__, __LINE__, sstr.str (), (WHAT), (EQUALS)); \
  } 

#define EXPECT_NE(WHAT,EQUALS) \
  _this->checkpoint (__FILE__, __LINE__); \
  if (ut::equals ((WHAT), (EQUALS))) { \
    std::ostringstream sstr; \
    sstr << #WHAT << " equals " << #EQUALS; \
    _this->raise (__FILE__, __LINE__, sstr.str ()); \
  } 

#define EXPECT(WHAT) \
  _this->checkpoint (__FILE__, __LINE__); \
  if (!(WHAT)) { \
    std::ostringstream sstr; \
    sstr << #WHAT << " is not true"; \
    _this->raise (__FILE__, __LINE__, sstr.str ()); \
  } 

#define CHECKPOINT() \
  _this->checkpoint (__FILE__, __LINE__);

#define FAIL_ARG(MSG,WHAT) \
  { \
    std::ostringstream sstr; \
    sstr << MSG << ", value is " << (WHAT); \
    _this->raise (__FILE__, __LINE__, sstr.str ()); \
  } 

#endif

