#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTransform>
#include <QTimer>
#include <QDebug>

#include "quad.h"

class MapWidget : public QGLWidget
{
  Q_OBJECT
public:
  explicit MapWidget(QWidget *parent = 0);

  QSize getOutputSize() const;
  void setOutputSize(const QSize &value);

  QList<Quad *> *getQuadList() const;
  void setQuadList(QList<Quad *> *value);

private:
  void paintEvent(QPaintEvent*);
  void mouseMoveEvent(QMouseEvent *e);
  void resizeEvent(QResizeEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent *e);

  qint32 allowedDragOffset;
  QSize outputSize;
  QTransform outTransform;
  QPoint pointerPos;
  QPoint lastPointerPos;
  int movingPoint;
  int movingPoly;
  bool movingAPoint;
  bool movingAPoly;
  QTimer framerateTimer;
  QList<Quad*> *quadList;
  double xScale, yScale;
  bool mouseDown;
  bool needsUpdate;


signals:
  void mouseMove(QPoint cursorPos);
  void updateDisplays();

public slots:
  void outputResized(QSize size);
  void framerateTimerTick();
  void updateDisplay();
};

#endif // MAPWIDGET_H
