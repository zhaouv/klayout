
DESTDIR = $$OUT_PWD/..
TARGET = klayout_rdb

include($$PWD/../klayout.pri)

DEFINES += MAKE_RDB_LIBRARY

TEMPLATE = lib

FORMS = \

RESOURCES = \

SOURCES = \
  gsiDeclRdb.cc \
  rdb.cc \
  rdbFile.cc \
  rdbReader.cc \
  rdbRVEReader.cc \
  rdbTiledRdbOutputReceiver.cc \
  rdbUtils.cc \

HEADERS = \
  rdb.h \
  rdbReader.h \
  rdbTiledRdbOutputReceiver.h \
  rdbUtils.h \
    rdbCommon.h

INCLUDEPATH += ../tl ../gsi ../db
DEPENDPATH += ../tl ../gsi ../db
LIBS += -L$$DESTDIR -lklayout_tl -lklayout_gsi -lklayout_db

