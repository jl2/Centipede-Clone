/*
  gamewindow.cpp
 
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

#include <QtGui>
#include <stdexcept>
#include "gamewindow.h"

void GameWindow::readSettings() {
}
GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent) {
  readSettings();
  setupActions();
  setupMenuBar();

  setWindowTitle(tr("Centipede"));

  tb = addToolBar(tr("Game"));

  score = new QLabel(tr("0"));
  tb->addWidget(score);
  
  lives = new QLabel(tr("3"));
  tb->addWidget(lives);
  
  level = new QLabel(tr("1"));
  tb->addWidget(level);

  theGame = new CentiGame(this);
  setCentralWidget(theGame);

  connect(theGame, SIGNAL(scoreChanged(int)),
	  this, SLOT(updateScore(int)));

  connect(theGame, SIGNAL(livesChanged(int)),
	  this, SLOT(updateLives(int)));
  
  connect(theGame, SIGNAL(gameLost()),
	  this, SLOT(gameLost()));
}

GameWindow::~GameWindow() {
  ;
}

void GameWindow::fillInAction(QAction **action, QString text,
			       QString toolTip, const char *method,
			       Qt::ConnectionType type, QIcon icon = QIcon(":/images/blank.png")) {
  *action = new QAction(this);
  (*action)->setText(text);
  (*action)->setToolTip(toolTip);
  (*action)->setStatusTip(toolTip);
  (*action)->setIcon(icon);
  (*action)->setDisabled(true);
  connect(*action, SIGNAL(triggered()), this, method, type);
}
void GameWindow::fillInAction(QAction **action, QString text,
			       QString toolTip, const char *method,
			       QIcon icon = QIcon(":/images/blank.png")) {
  *action = new QAction(this);
  (*action)->setText(text);
  (*action)->setToolTip(toolTip);
  (*action)->setStatusTip(toolTip);
  (*action)->setIcon(icon);
  (*action)->setDisabled(true);
  connect(*action, SIGNAL(triggered()), this, method);
}
void GameWindow::setupFileActions() {
  fillInAction(&exitAction, tr("E&xit"), tr("Exit GameWindow"), SLOT(close()));
}

void GameWindow::setupHelpActions() {
  fillInAction(&aboutAction, tr("&About"), tr("About GameWindow"),
	       SLOT(about()), QIcon(":/images/globe.png"));

}

void GameWindow::setupActions() {
  setupFileActions();
  setupHelpActions();
  aboutAction->setDisabled(false);
  exitAction->setDisabled(false);
}

void GameWindow::setupMenuBar() {
  QMenu *fileMenu = menuBar()->addMenu(tr("&Game"));
  fileMenu->addAction(exitAction);

  menuBar()->addSeparator();
  
  QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);
}

void GameWindow::notYetImplemented() {
  QMessageBox::critical(this, tr("Not Yet Implemented"),
			tr("Oops! That feature is not yet implemented!"),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}

bool GameWindow::canContinue() {
  return true;
}

void GameWindow::about() {
  QMessageBox::about(this, tr("About"), tr("<h2>Centipede Clone</h2><p>Qt version</p><p>"
					   "By Jeremiah LaRocco.</p>"));
}

void GameWindow::updateScore(int sc) {
  score->setText(tr("%n","", sc));
}

void GameWindow::updateLevel(int lv) {
  level->setText(tr("%n","",lv));
}

void GameWindow::updateLives(int lvs) {
  lives->setText(tr("%n","", lvs));
}

void GameWindow::gameLost() {
  QMessageBox::critical(this, tr("Lost!"),
			tr("You've lost!"),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}

