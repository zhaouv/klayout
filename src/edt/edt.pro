
DESTDIR = $$OUT_PWD/..
TARGET = klayout_edt

include($$PWD/../klayout.pri)

DEFINES += MAKE_EDT_LIBRARY

TEMPLATE = lib

HEADERS = \
  edtConfig.h \
  edtDialogs.h \
  edtEditorOptionsPages.h \
  edtInstPropertiesPage.h \
  edtMainService.h \
  edtPartialService.h \
  edtPCellParametersPage.h \
  edtPlugin.h \
  edtPropertiesPages.h \
  edtPropertiesPageUtils.h \
  edtService.h \
  edtServiceImpl.h \
  edtUtils.h \
    edtCommon.h

FORMS = \
  AlignOptionsDialog.ui \
  BoxPropertiesPage.ui \
  CopyModeDialog.ui \
  ChangeLayerOptionsDialog.ui \
  EditablePathPropertiesPage.ui \
  EditorOptionsDialog.ui \
  EditorOptionsGeneric.ui \
  EditorOptionsInst.ui \
  EditorOptionsPath.ui \
  EditorOptionsText.ui \
  InstantiationForm.ui \
  InstPropertiesPage.ui \
  MakeArrayOptionsDialog.ui \
  MakeCellOptionsDialog.ui \
  PathPropertiesPage.ui \
  PolygonPropertiesPage.ui \
  RoundCornerOptionsDialog.ui \
  TextPropertiesPage.ui \

SOURCES = \
  edtConfig.cc \
  edtDialogs.cc \
  edtEditorOptionsPages.cc \
  edtInstPropertiesPage.cc \
  edtMainService.cc \
  edtPartialService.cc \
  edtPCellParametersPage.cc \
  edtPlugin.cc \
  edtPropertiesPages.cc \
  edtPropertiesPageUtils.cc \
  edtService.cc \
  edtServiceImpl.cc \
  edtUtils.cc \
  gsiDeclEdt.cc \

INCLUDEPATH += ../tl ../gsi ../laybasic ../db
DEPENDPATH += ../tl ../gsi ../laybasic ../db

# Note: this accounts for UI-generated headers placed into the output folders in
# shadow builds:
INCLUDEPATH += $$DESTDIR/laybasic
DEPENDPATH += $$DESTDIR/laybasic

LIBS += -L$$DESTDIR -lklayout_tl -lklayout_gsi -lklayout_laybasic -lklayout_db

