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

#include "highscoredialog.h"

#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent) {
  setupActions();
  setupMenuBar();

  setWindowTitle(tr("Centipede"));

  tb = addToolBar(tr("Game"));

  score = new QLabel(tr("Score: 0   "));
  tb->addWidget(score);
  
  lives = new QLabel(tr("Lives: 3   "));
  tb->addWidget(lives);
  
  level = new QLabel(tr("Level: 1   "));
  tb->addWidget(level);

  qset = new QSettings(QSettings::IniFormat, QSettings::UserScope,
		       "Centipede", "Centipede");
  
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
  delete theGame;
}

void GameWindow::readHighScores() {
  qset->sync();
  for (int i=0;i<10;++i) {
    best_entry bob;
    bob.name = qset->value(tr("name_%n","",i), tr("Anon")).toString();
    bob.when = qset->value(tr("date_%n","",i), QDateTime(QDate(2000, 1,1))).toDateTime();
    bob.score = qset->value(tr("score_%n", "",i), 1000).toInt();
    high_scores.push_back(bob);
  }
}

void GameWindow::writeHighScores() {
  for (int i=0;i<10;++i) {
    qset->setValue(tr("name_%n","",i), high_scores[i].name);
    qset->setValue(tr("date_%n","",i), high_scores[i].when);
    qset->setValue(tr("score_%n","",i), high_scores[i].score);
  }
  qset->sync();
}


void GameWindow::fillInAction(QAction **action, QString text,
			       QString toolTip, const char *method,
			       Qt::ConnectionType type, QIcon icon = QIcon()) {
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
			       QIcon icon = QIcon()) {
  *action = new QAction(this);
  (*action)->setText(text);
  (*action)->setToolTip(toolTip);
  (*action)->setStatusTip(toolTip);
  (*action)->setIcon(icon);
  (*action)->setDisabled(false);
  connect(*action, SIGNAL(triggered()), this, method);
}
void GameWindow::setupFileActions() {
  fillInAction(&newGameAction, tr("&New"), tr("New Game"), SLOT(newGame()));
  fillInAction(&pauseAction, tr("&Pause"), tr("Pause the game"), SLOT(pause()));
  pauseAction->setCheckable(true);
  pauseAction->setChecked(false);
  fillInAction(&highScoreAction, tr("&High Scores"), tr("View High Scores"), SLOT(highScores()));
  fillInAction(&muteAction, tr("&Mute"), tr("Turn off sound"), SLOT(mute()));
  muteAction->setCheckable(true);
  muteAction->setChecked(false);
  fillInAction(&exitAction, tr("E&xit"), tr("Exit Game"), SLOT(close()));
}

void GameWindow::setupHelpActions() {
  fillInAction(&aboutAction, tr("&About"), tr("About GameWindow"),
	       SLOT(about()), QIcon(":/images/globe.png"));

}

void GameWindow::setupActions() {
  setupFileActions();
  setupHelpActions();
}

void GameWindow::setupMenuBar() {
  QMenu *fileMenu = menuBar()->addMenu(tr("&Game"));
  fileMenu->addAction(newGameAction);
  fileMenu->addSeparator();
  fileMenu->addAction(pauseAction);
  fileMenu->addSeparator();
  fileMenu->addAction(muteAction);
  fileMenu->addSeparator();
  fileMenu->addAction(highScoreAction);
  fileMenu->addSeparator();
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
  score->setText(tr("Score: %n   ","", sc));
}

void GameWindow::updateLevel(int lv) {
  level->setText(tr("Level: %n   ","",lv));
}

void GameWindow::updateLives(int lvs) {
  lives->setText(tr("Lives: %n   ","", lvs));
}

void GameWindow::gameLost() {
  readHighScores();

  best_entry bob;
  bob.name = getenv("USERNAME");
  bob.when = QDateTime::currentDateTime();
  bob.score = theGame->score();
  high_scores.push_back(bob);
  std::sort(high_scores.begin(), high_scores.end(), best_greater);
  high_scores.pop_back();
  writeHighScores();

  QMessageBox::critical(this, tr("Lost!"),
			tr("You've lost!"),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}

void GameWindow::newGame() {
  theGame->newGame();
}
void GameWindow::highScores() {
  readHighScores();
  
  theGame->setPaused(true);

  HighScoreDialog *hsdb = new HighScoreDialog(high_scores, this);
  hsdb->show();
}
void GameWindow::mute() {
  theGame->setMute(!theGame->isMute());
  muteAction->setChecked(theGame->isMute());
}

void GameWindow::pause() {
  theGame->setPaused(!theGame->isPaused());
  pauseAction->setChecked(theGame->isPaused());
}
