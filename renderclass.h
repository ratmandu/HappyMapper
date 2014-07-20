#ifndef RENDERCLASS_H
#define RENDERCLASS_H

#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QPaintDevice>

#include "quad.h"
#include "outwidget.h"
#include "mapwidget.h"

class RenderClass : public QObject
{
  Q_OBJECT
public:
  explicit RenderClass(QObject *parent = 0);
  QPixmap RenderOutput();
  QPixmap RenderPreview();

  void setOutputSize(QSize size);
  void setPreviewSize(QSize size);

private:
  QList<Quad*> *quadList;
  QPoint cursorPos;

signals:
  void outputRenderComplete(QPixmap render);
  void previewRenderComplete(QPixmap render);

public slots:

};

#endif // RENDERCLASS_H
