#ifndef OUTWIDGET_H
#define OUTWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QResizeEvent>
#include <QSettings>
#include "quad.h"

class OutWidget : public QGLWidget
{
  Q_OBJECT
public:
  explicit OutWidget(QWidget *parent = 0);
  QSize getOutputSize();

  QPoint getCursorPos() const;
  void setCursorPos(const QPoint &value);

  QList<Quad*> *getQuadList() const;
  void setQuadList(QList<Quad *> *value);

  void setNeedsUpdate(bool value);

private:
  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent* e);
  void paintCursor(QPainter *p);
  void paintOverlayPolys(QPainter *p);
  void paintSurfacePolys(QPainter *p);

  QSettings settings;
  QList<Quad*> *quadList;
  QPoint cursorPos;
  QTimer framerateTimer;
  QTransform oldTransform;
  bool needsUpdate;
//  QTransform *trans;

signals:
  void outputResized(QSize size);

public slots:

private slots:
  void frameRateTimerTick();
};

#endif // OUTWIDGET_H
