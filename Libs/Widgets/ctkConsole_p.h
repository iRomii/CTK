/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.commontk.org/LICENSE

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __ctkConsole_p_h
#define __ctkConsole_p_h

// Qt includes
#include <QTextEdit>
#include <QPointer>

// CTK includes
#include "ctkConsole.h"
#include "ctkWidgetsExport.h"

class CTK_WIDGETS_EXPORT ctkConsolePrivate : public QTextEdit
{
  Q_OBJECT
  Q_DECLARE_PUBLIC(ctkConsole);
protected:
  ctkConsole* const q_ptr;
public:

  ctkConsolePrivate(ctkConsole& object);

  void init();

  void keyPressEvent(QKeyEvent* e);
  
  /// Returns the end of the document
  int documentEnd() const;

  void focusOutEvent(QFocusEvent *e);

  void updateCompleterIfVisible();

  /// If there is exactly 1 completion, insert it and hide the completer,
  /// else do nothing.
  void selectCompletion();

  void updateCompleter();
  
  /// Update the contents of the command buffer from the contents of the widget
  void updateCommandBuffer();
  
  /// Replace the contents of the command buffer, updating the display
  void replaceCommandBuffer(const QString& Text);
  
  /// References the buffer where the current un-executed command is stored
  QString& commandBuffer();
  
  /// Implements command-execution
  void internalExecuteCommand();

  void setCompleter(ctkConsoleCompleter* completer);

  /// Writes the supplied text to the console
  void printString(const QString& text);

  /// Updates the current command.
  /// Unlike printMessage(), this will affect the current command being typed.
  void printCommand(const QString& cmd);

  /// Puts out an input accepting prompt.
  /// It is recommended that one uses prompt instead of printString() to print
  /// an input prompt since this call ensures that the prompt is shown on a new
  /// line.
  void prompt(const QString& text);

public slots:

  /// Inserts the given completion string at the cursor.  This will replace
  /// the current word that the cursor is touching with the given text.
  /// Determines the word using QTextCursor::StartOfWord, EndOfWord.
  void insertCompletion(const QString& text);

  void printOutputMessage(const QString& text);

  void printErrorMessage(const QString& text);

public:

  /// A custom completer
  QPointer<ctkConsoleCompleter> Completer;

  /// Stores the beginning of the area of interactive input, outside which
  /// changes can't be made to the text edit contents
  int InteractivePosition;

  /// Stores command-history, plus the current command buffer
  QStringList CommandHistory;

  /// Stores the current position in the command-history
  int CommandPosition;

  /// Prompt  color
  QColor PromptColor;

  /// Output text color
  QColor OutputTextColor;

  /// Error ext color
  QColor ErrorTextColor;

  /// Command text color
  QColor CommandTextColor;

  /// Welcome text color
  QColor WelcomeTextColor;
};


#endif
