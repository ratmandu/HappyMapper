#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  connect(&framerateTimer, SIGNAL(timeout()), this, SLOT(framerateTimerTick()));
//  framerateTimer.start(1000/60);
  setAutoFillBackground(false);
  this->setMouseTracking(true);
  xScale = 1;
  yScale = 1;
  allowedDragOffset = 15;
  mouseDown = false;
  movingAPoint = false;
  this->setFocusPolicy(Qt::ClickFocus);
  needsUpdate = true;
}

void MapWidget::paintEvent(QPaintEvent *)
{
  QPainter p(this);
//  p.setRenderHint(QPainter::Antialiasing);
  p.fillRect(this->rect(), Qt::black);

  // drawQuads
  if (quadList != NULL) {
    p.setPen(QColor(Qt::white));
    for (int i = 0; i < quadList->size(); i++) {
      QPolygon q = quadList->at(i)->getQuad();
      if (quadList->at(i)->getSelected()) {
        p.setPen(QColor(Qt::red));
//        p.pen().setWidth(3);
      } else {
        p.setPen(QColor(Qt::white));
//        p.pen().setWidth(1);
      }
      QPoint polyCenter = q.boundingRect().center();
      if ((abs(polyCenter.x() - pointerPos.x()/xScale) < allowedDragOffset) &&
          (abs(polyCenter.y() - pointerPos.y()/yScale))) {
        p.drawEllipse(polyCenter - QPoint(allowedDragOffset/2, allowedDragOffset/2), allowedDragOffset/2, allowedDragOffset/2);
      }

      for (int j = 0; j<q.size(); j++) {
        q.setPoint(j, QPoint(q.at(j).x()/xScale, q.at(j).y()/yScale));
        QPoint testPoint = q.at(j) - pointerPos;
        testPoint.setX(testPoint.x()/xScale);
        testPoint.setY(testPoint.y()/yScale);

        if ((abs((q.at(j).x() - pointerPos.x()) / xScale) < allowedDragOffset) &&
            (abs((q.at(j).y() - pointerPos.y()) / yScale) < allowedDragOffset)) {
//          qDebug() << testPoint << pointerPos;
          p.drawEllipse(q.at(j), allowedDragOffset/2, allowedDragOffset/2);
        }
      }
      p.drawPolygon(q);
    }
  }

  p.setPen(QColor(Qt::white));
  p.drawLine(0, pointerPos.y(), this->width(), pointerPos.y());
  p.drawLine(pointerPos.x(), 0, pointerPos.x(), this->height());
}

void MapWidget::mouseMoveEvent(QMouseEvent *e)
{
  QPoint outpoint;
  pointerPos = e->pos();
  //  outpoint = e->pos();
  outpoint.setX(e->pos().x() * xScale);
  outpoint.setY(e->pos().y() * yScale);
  emit mouseMove(outpoint);

  if (mouseDown) { // we are dragging something
    if (movingAPoly) { // we are dragging a polygon
      QPolygon p = quadList->at(movingPoly)->getQuad();
      p.translate(outpoint - lastPointerPos);
//      qDebug() << outpoint << lastPointerPos << (outpoint - lastPointerPos);
      quadList->at(movingPoly)->setQuad(p);
      quadList->at(movingPoly)->updateTransform();
    }else if (movingAPoint) { // we are dragging a point
      QPolygon p = quadList->at(movingPoly)->getQuad();
      p.setPoint(movingPoint, outpoint);
      quadList->at(movingPoly)->setQuad(p);
      quadList->at(movingPoly)->updateTransform();
    }

//    emit updateDisplays();
  }

//  needsUpdate = true;
  this->update();
  lastPointerPos = outpoint;

//  emit mouseMove(outTransform.map(e->pos));
//  qDebug() << outTransform.map(e->pos());
}

void MapWidget::resizeEvent(QResizeEvent *e)
{
  xScale = double(outputSize.width()) / double(e->size().width());
  yScale = double(outputSize.height()) / double(e->size().height());
//  qDebug() << xScale << yScale;
//  this->setOutputSize(outputSize);
  //  outTransform.scale(xScale, yScale);
}

void MapWidget::mousePressEvent(QMouseEvent *e)
{
  QPoint clickPos = e->pos();

  for (int i = 0; i < quadList->size(); i++) {
    QPolygon p = quadList->at(i)->getQuad();

    QPoint polyCenter = p.boundingRect().center();
    if ((abs(polyCenter.x() - clickPos.x()*xScale) < allowedDragOffset) &&
        (abs(polyCenter.y() - clickPos.y()*yScale) < allowedDragOffset)) {
      movingPoly = i;
      movingAPoly = true;
      mouseDown = true;
      return;
    }

    for (int j = 0; j < p.size(); j++) {
      if ((abs(p.at(j).x() - clickPos.x()*xScale) < allowedDragOffset) &&
          (abs(p.at(j).y() - clickPos.y()*yScale) < allowedDragOffset)) {
        movingPoint = j;
        movingPoly = i;
        movingAPoint = true;
        mouseDown = true;
        return;
      }
    }
  }
}

void MapWidget::mouseReleaseEvent(QMouseEvent *e)
{
  if (e->button()) {}
  movingAPoint = false;
  movingAPoly = false;
  mouseDown = false;

//  needsUpdate = true;
  updateDisplay();
  emit updateDisplays();
}

void MapWidget::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Left ||
      e->key() == Qt::Key_Right ||
      e->key() == Qt::Key_Up ||
      e->key() == Qt::Key_Down) {
    QPolygon p;
    bool found = false;
    int i = 0;
    for (i = 0; i<quadList->size(); i++) {
      if (quadList->at(i)->getSelected()) {
        p = quadList->at(i)->getQuad();
        found = true;
        break;
      }
    }

    if (!found) {
      return;
    }

    int move = 1;
    if (e->modifiers() & Qt::ShiftModifier) {
      move = 10;
    }
    switch (e->key()) {
    case Qt::Key_Left:
      p.translate(-move, 0);
      break;
    case Qt::Key_Right:
      p.translate(move, 0);
      break;
    case Qt::Key_Up:
      p.translate(0, -move);
      break;
    case Qt::Key_Down:
      p.translate(0, move);
      break;
    }

    quadList->at(i)->setQuad(p);
    quadList->at(i)->updateTransform();

//    needsUpdate = true;
    updateDisplay();
    emit updateDisplays();
  }
}

QList<Quad *> *MapWidget::getQuadList() const
{
  return quadList;
}

void MapWidget::setQuadList(QList<Quad *> *value)
{
  quadList = value;
}


void MapWidget::outputResized(QSize size)
{
  setOutputSize(size);
}

void MapWidget::framerateTimerTick()
{
//  if (needsUpdate) {
//    this->update();
//    needsUpdate = false;
//  }
}

void MapWidget::updateDisplay()
{
//  needsUpdate = true;
  this->update();
}

QSize MapWidget::getOutputSize() const
{
  return outputSize;
}

void MapWidget::setOutputSize(const QSize &value)
{
  outputSize = value;
  xScale = double(outputSize.width()) / double(this->width());
  yScale = double(outputSize.height()) / double(this->height());
//  qDebug() << value << this->size() << xScale << yScale;

}

