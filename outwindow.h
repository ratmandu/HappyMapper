#ifndef OUTWINDOW_H
#define OUTWINDOW_H

#include <QMainWindow>
#include "quad.h"

namespace Ui {
class OutWindow;
}

class OutWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit OutWindow(QWidget *parent = 0);
  ~OutWindow();
  QSize getSize();

  QList<Quad*> *getQuadList() const;
  void setQuadList(QList<Quad *> *value);

private:
  Ui::OutWindow *ui;
  QList<Quad*> *quadList;

signals:
  void outputSizeChanged(QSize size);

public slots:
  void MapMouseMoved(QPoint pos);
  void updateDisplay();

private slots:
  void outSizeChanged(QSize size);
};

#endif // OUTWINDOW_H
