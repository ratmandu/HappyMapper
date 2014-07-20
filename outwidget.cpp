#include "outwidget.h"

OutWidget::OutWidget(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  connect(&framerateTimer, SIGNAL(timeout()), this, SLOT(frameRateTimerTick()));
//  framerateTimer.start(1000/60);

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_LINE_SMOOTH);

  quadList = new QList<Quad*>;
  setAutoFillBackground(false);
  needsUpdate = true;

//  trans = new QTransform();
  //  this->setAttribute(
  //  cursorPos.setX(120);
  //  cursorPos.setY(340);
}

QSize OutWidget::getOutputSize()
{
  QSize out;
  out.setWidth(this->width());
  out.setHeight(this->height());
  return out;
}

void OutWidget::paintEvent(QPaintEvent *)
{

//  QPixmap out(this->size());
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setRenderHint(QPainter::HighQualityAntialiasing);
  p.setRenderHint(QPainter::SmoothPixmapTransform);

  p.fillRect(this->rect(), Qt::black);

  // drawQuads
  if (quadList != NULL) {
    p.setPen(QColor(Qt::white));
    p.setBrush(QBrush(Qt::white));
    for (int i = 0; i < quadList->size(); i++) {
      QPolygon q = quadList->at(i)->getQuad();
      if (quadList->at(i)->getSelected()) {
        p.setPen(QColor(255, 0, 0, 128));
        p.setBrush(QBrush(QColor(255, 0, 0, 128)));
//        p.pen().setWidth(3);
      } else {
        p.setPen(QColor(255, 255, 255, 128));
        p.setBrush(QBrush(QColor(255, 255, 255, 128)));
//        p.pen().setWidth(1);
      }

      if (quadList->at(i)->getTextureType() == Quad::TEX_TYPE_TEST) {
//        QTransform trans;
//      if (QTransform::quadToQuad(QPolygon(QImage(":/tests/checkerboard.jpg").rect()), q, trans)) {
//        trans = quadList->at(i)->getTransform();
        oldTransform = p.transform();
        p.setTransform(quadList->at(i)->getTransform());
        p.drawPixmap(QPoint(0,0), quadList->at(i)->getTexture());
//        p.drawImage(QPoint(0,0), QPixmap(":/tests/checkerboard.jpg"));//.transformed(trans, Qt::SmoothTransformation));
        p.setTransform(oldTransform);
        if (settings.value("viewSettings/polyOverlay", false).toBool()) {
          p.drawPolygon(q);
        }
      } else if (quadList->at(i)->getTextureType() == Quad::TEX_TYPE_IMAGE) {
        oldTransform = p.transform();
        p.setTransform(quadList->at(i)->getTransform());

        p.drawPixmap(QPoint(0,0), quadList->at(i)->getTexture());//, quadList->at(i)->getQuad().boundingRect());
//        p.drawImage(QPoint(0,0), QPixmap(quadList->at(i)->getTextureFileName()));
        p.setTransform(oldTransform);
        if (settings.value("viewSettings/polyOverlay", false).toBool()) {
          p.drawPolygon(q);
        }
      } else if (quadList->at(i)->getTextureType() == Quad::TEX_TYPE_SOLIDCOLOR) {
        p.setBrush(QBrush(quadList->at(i)->getPolyColor()));
        p.setPen(QPen(quadList->at(i)->getPolyColor()));
        p.drawPolygon(q);
      }
//      p.drawPolygon(q);
    }
  }

  // top layer, draw cursor position, if enabled
  // TODO: add toggle to enable/disable cursor display
  if (settings.value("viewSettings/cursorVisible", true).toBool()) {
    this->paintCursor(&p);
  }

//  QPainter o(this);
//  o.drawPixmap(this->rect(), out);
}

void OutWidget::resizeEvent(QResizeEvent *e)
{
  emit outputResized(e->size());
//  needsUpdate = true;
  setNeedsUpdate(true);
}

void OutWidget::paintCursor(QPainter *p)
{
//  if (cursorEnabled) {
  if (cursorPos.x() != 0 && cursorPos.y() != 0) {
    p->setPen(QColor(Qt::white));
    p->drawLine(0, cursorPos.y(), this->width(), cursorPos.y());
    p->drawLine(cursorPos.x(), 0, cursorPos.x(), this->height());
  }
//  }
}

void OutWidget::setNeedsUpdate(bool value)
{
  needsUpdate = value;
  this->update();
}


QList<Quad *> *OutWidget::getQuadList() const
{
  return quadList;
}

void OutWidget::setQuadList(QList<Quad*> *value)
{
  quadList = value;
  setNeedsUpdate(true);
//  needsUpdate = true;
}


void OutWidget::frameRateTimerTick()
{
  if (needsUpdate) {
    this->update();
    needsUpdate = false;
  }
}

QPoint OutWidget::getCursorPos() const
{
  return cursorPos;
}

void OutWidget::setCursorPos(const QPoint &value)
{
  cursorPos = value;
//  needsUpdate = true;
  setNeedsUpdate(true);
}

