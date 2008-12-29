/*
  highscoredialog.cpp
 
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

#include "highscoredialog.h"


HighScoreDialog::HighScoreDialog(std::vector<best_entry> &scores, QWidget *parent) :
  QDialog(parent) {
  
  setWindowTitle(tr("High Scores"));
  
  for (int i=0;i<10;++i) {
    sc.push_back(scores[i]);
  }
  
  scoreTable = new QTableWidget(0,3);
  scoreTable->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Date") << tr("Score"));
  for (int i=0;i<10;++i) {
    scoreTable->insertRow(i);
    QTableWidgetItem *nm = new QTableWidgetItem;
    nm->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    nm->setText(sc[i].name);
    scoreTable->setItem(i, 0, nm);

    QTableWidgetItem *dt = new QTableWidgetItem;
    dt->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    dt->setText(sc[i].when.toString());
    scoreTable->setItem(i, 1, dt);

    QTableWidgetItem *scr = new QTableWidgetItem;
    scr->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    scr->setText(QString::number(sc[i].score));
    scoreTable->setItem(i, 2, scr);
  }

  scoreTable->resizeColumnToContents(0);
  scoreTable->resizeColumnToContents(1);
  scoreTable->resizeColumnToContents(2);
  
  okayButton = new QPushButton(tr("Okay"));
  okayButton->setDefault(true);
  QHBoxLayout *ml = new QHBoxLayout;
  ml->addWidget(scoreTable);
  QVBoxLayout *rs = new QVBoxLayout;
  rs->addWidget(okayButton);
  rs->addStretch();
  ml->addLayout(rs);
  setLayout(ml);

  connect(okayButton, SIGNAL(clicked()),
	  this, SLOT(close()));
    resize(540, 358);
}
