/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

// Qt includes
#include <QApplication>
#include <QCheckBox>
#include <QLineEdit>
#include <QSettings>
#include <QTimer>
#include <QVariant>

// CTK includes
#include "ctkBooleanMapper.h"
#include "ctkCoreTestingMacros.h"
#include "ctkSettingsPanel.h"
#include "ctkSettingsPanelTest2Helper.h"

// STD includes
#include <cstdlib>
#include <iostream>

namespace
{
//-----------------------------------------------------------------------------
class ctkSettingsPanelForTest : public ctkSettingsPanel
{
public:
  QVariant myDefaultPropertyValue(const QString& key) const
    {
    return this->defaultPropertyValue(key);
    }
  QVariant myPreviousPropertyValue(const QString& key) const
    {
    return this->previousPropertyValue(key);
    }
  QVariant myPropertyValue(const QString& key) const
    {
    return this->propertyValue(key);
    }
};

} // end of anonymous namespace

//-----------------------------------------------------------------------------
int ctkSettingsPanelTest1(int argc, char * argv [] )
{
  QApplication app(argc, argv);
  
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Common ToolKit", "CTK");
  settings.clear();

  ctkSettingsPanelForTest settingsPanel;
  settingsPanel.setSettings(&settings);

  //
  // QCheckBox
  //
  QCheckBox* box = new QCheckBox(&settingsPanel);

  settingsPanel.registerProperty("key 1", box, "checked",
                                  SIGNAL(toggled(bool)));
  
  // Check settings value after a property is registered
  QVariant boxVal = settings.value("key 1");
  CHECK_BOOL(boxVal.isValid(), true);
  CHECK_BOOL(boxVal.toBool(), false);
  CHECK_BOOL(settingsPanel.myPreviousPropertyValue("key 1").toBool(), false);
  CHECK_BOOL(settingsPanel.myPropertyValue("key 1").toBool(), false);
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API
  box->setChecked(true);

  // Check settings value after it has been updated using object/widget API
  boxVal = settings.value("key 1");
  CHECK_BOOL(boxVal.isValid(), true);
  CHECK_BOOL(boxVal.toBool(), true);
  CHECK_BOOL(settingsPanel.myPreviousPropertyValue("key 1").toBool(), false);
  CHECK_BOOL(settingsPanel.myPropertyValue("key 1").toBool(), true);
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key 1");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  boxVal = settings.value("key 1");
  CHECK_BOOL(boxVal.isValid(), true);
  CHECK_BOOL(boxVal.toBool(), true);
  CHECK_BOOL(settingsPanel.myPreviousPropertyValue("key 1").toBool(), true);
  CHECK_BOOL(settingsPanel.myDefaultPropertyValue("key 1").toBool(), false);
  CHECK_BOOL(settingsPanel.myPropertyValue("key 1").toBool(), true);
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  //
  // QLineEdit
  //
  QLineEdit* lineEdit = new QLineEdit("default", &settingsPanel);
  settingsPanel.registerProperty("key 2", lineEdit, "text",
                                  SIGNAL(textChanged(QString)));

  // Check value after a property is registered
  QVariant lineEditVal = settings.value("key 2");
  CHECK_BOOL(lineEditVal.isValid(), true);
  CHECK_QSTRING(lineEditVal.toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myPreviousPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myDefaultPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API
  lineEdit->setText("first edit");

  // Check settings value after it has been updated using object/widget API
  lineEditVal = settings.value("key 2");
  CHECK_BOOL(lineEditVal.isValid(), true);
  CHECK_QSTRING(lineEditVal.toString(), QString("first edit"));
  CHECK_QSTRING(settingsPanel.myPreviousPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myDefaultPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myPropertyValue("key 2").toString(), QString("first edit"));
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key 2");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  lineEditVal = settings.value("key 2");
  CHECK_BOOL(lineEditVal.isValid(), true);
  CHECK_QSTRING(lineEditVal.toString(), QString("first edit"));
  CHECK_QSTRING(settingsPanel.myPreviousPropertyValue("key 2").toString(), QString("first edit"));
  CHECK_QSTRING(settingsPanel.myDefaultPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myPropertyValue("key 2").toString(), QString("first edit"));
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API
  lineEdit->setText("second edit");

  // Check settings value after it has been updated using object/widget API
  lineEditVal = settings.value("key 2");
  CHECK_BOOL(lineEditVal.isValid(), true);
  CHECK_QSTRING(lineEditVal.toString(), QString("second edit"));
  CHECK_QSTRING(settingsPanel.myPreviousPropertyValue("key 2").toString(), QString("first edit"));
  CHECK_QSTRING(settingsPanel.myDefaultPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myPropertyValue("key 2").toString(), QString("second edit"));
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key 2");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  lineEditVal = settings.value("key 2");
  CHECK_BOOL(lineEditVal.isValid(), true);
  CHECK_QSTRING(lineEditVal.toString(), QString("second edit"));
  CHECK_QSTRING(settingsPanel.myPreviousPropertyValue("key 2").toString(), QString("second edit"));
  CHECK_QSTRING(settingsPanel.myDefaultPropertyValue("key 2").toString(), QString("default"));
  CHECK_QSTRING(settingsPanel.myPropertyValue("key 2").toString(), QString("second edit"));
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  //
  // QCheckBox + ctkBooleanMapper
  //
  box = new QCheckBox(&settingsPanel);

  settingsPanel.registerProperty("key complement",
                                 new ctkBooleanMapper(box, "checked", SIGNAL(toggled(bool))),
                                 "complement",
                                  SIGNAL(complementChanged(bool)));

  // Check settings value after a property is registered
  boxVal = settings.value("key complement");
  CHECK_BOOL(boxVal.isValid(), true);
  CHECK_BOOL(boxVal.toBool(), true);
  CHECK_BOOL(settingsPanel.myPreviousPropertyValue("key complement").toBool(), true);
  CHECK_BOOL(settingsPanel.myPropertyValue("key complement").toBool(), true);
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API
  box->setChecked(true);

  // Check settings value after it has been updated using object/widget API
  boxVal = settings.value("key complement");
  CHECK_BOOL(boxVal.isValid(), true);
  CHECK_BOOL(boxVal.toBool(), false);
  CHECK_BOOL(settingsPanel.myPreviousPropertyValue("key complement").toBool(), true);
  CHECK_BOOL(settingsPanel.myPropertyValue("key complement").toBool(), false);
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key complement");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  boxVal = settings.value("key complement");
  CHECK_BOOL(boxVal.isValid(), true);
  CHECK_BOOL(boxVal.toBool(), false);
  CHECK_BOOL(settingsPanel.myPreviousPropertyValue("key complement").toBool(), false);
  CHECK_BOOL(settingsPanel.myDefaultPropertyValue("key complement").toBool(), true);
  CHECK_BOOL(settingsPanel.myPropertyValue("key complement").toBool(), false);
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  //
  // ctkSettingsPanelTest2Helper: Test QStringList property
  //
  ctkSettingsPanelTest2Helper* list = new ctkSettingsPanelTest2Helper(&settingsPanel);
  settingsPanel.registerProperty("key list", list, "list",
                                 SIGNAL(listChanged()));

  // Check value after a property is registered
  QVariant listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API: Add one item
  list->setList(QStringList() << "first item");

  // Check settings value after it has been updated using object/widget API
  listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList() << "first item");
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList() << "first item");
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key list");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList() << "first item");
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList() << "first item");
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList() << "first item");
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API: Add one other item
  list->setList(QStringList() << "first item" << "second item");

  // Check settings value after it has been updated using object/widget API
  listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList() << "first item" << "second item");
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList() << "first item");
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList() << "first item" << "second item");
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key list");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList() << "first item" << "second item");
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList() << "first item" << "second item");
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList() << "first item" << "second item");
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  // Update value using the object/widget API: Remove items
  list->setList(QStringList());

  // Check settings value after it has been updated using object/widget API
  listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList() << "first item" << "second item");
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList() << "key list");

  // Check settings value after applySettings() has been called
  settingsPanel.applySettings();
  listVal = settings.value("key list");
  CHECK_BOOL(listVal.isValid(), true);
  CHECK_QSTRINGLIST(listVal.toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPreviousPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myDefaultPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.myPropertyValue("key list").toStringList(), QStringList());
  CHECK_QSTRINGLIST(settingsPanel.changedSettings(), QStringList());

  settingsPanel.show();

  if (argc < 2 || QString(argv[1]) != "-I" )
    {
    QTimer::singleShot(200, &app, SLOT(quit()));
    }

  return app.exec();
}

