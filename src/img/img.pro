
DESTDIR = $$OUT_PWD/..
TARGET = klayout_img

include($$PWD/../klayout.pri)

DEFINES += MAKE_IMG_LIBRARY

TEMPLATE = lib

HEADERS = \
  imgLandmarksDialog.h \
  imgNavigator.h \
  imgObject.h \
  imgPlugin.h \
  imgPropertiesPage.h \
  imgService.h \
  imgWidgets.h \
    imgForceLink.h \
    imgCommon.h

FORMS = \
  AddNewImageDialog.ui \
  ImageLandmarksDialog.ui \
  ImagePropertiesPage.ui \

SOURCES = \
  gsiDeclImg.cc \
  imgLandmarksDialog.cc \
  imgNavigator.cc \
  imgObject.cc \
  imgPlugin.cc \
  imgPropertiesPage.cc \
  imgService.cc \
  imgWidgets.cc \
    imgForceLink.cc

INCLUDEPATH += ../tl ../gsi ../laybasic ../db
DEPENDPATH += ../tl ../gsi ../laybasic ../db
LIBS += -L$$DESTDIR -lklayout_tl -lklayout_gsi -lklayout_laybasic -lklayout_db

