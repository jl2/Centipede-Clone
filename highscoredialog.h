/*
  highscoredialog.h
 
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

#ifndef HIGH_SCORE_DIALOG_H
#define HIGH_SCORE_DIALOG_H

#include <QtGui>
#include <QDialog>
#include <QTableView>
#include "utils.h"
#include <vector>

class HighScoreDialog : public QDialog {
  Q_OBJECT;
public:
  HighScoreDialog(std::vector<best_entry> &scores, QWidget *parent = 0);

private:
  QTableWidget *scoreTable;
  QPushButton *okayButton;
  std::vector<best_entry> sc;
};
  

#endif
