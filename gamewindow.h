/*
  gamewindow.h
 
  Copyright (C) 2008 Jeremiah LaRocco

  This file is part of Centipede

  Centipede is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Centipede is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Centipede.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HMWINDOW_INCLUDE_H
#define HMWINDOW_INCLUDE_H

#include <QMainWindow>
#include "centigame.h"

class QMenu;
class QAction;
class QLabel;
class QSplitter;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QFrame;

class GameWindow : public QMainWindow {
  Q_OBJECT;

  QMenu *mainMenu;
 public:
  GameWindow(QWidget *parent = 0);
  ~GameWindow();
  
  public slots:
  void about();
  void updateScore(int sc);
  void updateLevel(int lv);
  void updateLives(int lvs);
  void gameLost();
  
 private:
  void readSettings();
  void setupActions();
  
  void setupFileActions();
  void setupHelpActions();
  
  void fillInAction(QAction **action, QString text,
		    QString toolTip, const char *method,
		    Qt::ConnectionType type, QIcon icon);
  void fillInAction(QAction **action, QString text,
		    QString toolTip, const char *method,
		    QIcon icon);


  void setupMenuBar();

  void notYetImplemented();

  bool canContinue();

  CentiGame *theGame;
  
  QAction *exitAction;
  QAction *aboutAction;
  
  QToolBar *tb;
  QLabel *score;
  QLabel *lives;
  QLabel *level;
};

#endif
