#include "outwindow.h"
#include "ui_outwindow.h"

OutWindow::OutWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::OutWindow)
{
  ui->setupUi(this);
  connect(ui->widget, SIGNAL(outputResized(QSize)), this, SLOT(outSizeChanged(QSize)));
}

OutWindow::~OutWindow()
{
  delete ui;
}

QSize OutWindow::getSize()
{
  return ui->widget->getOutputSize();
}
QList<Quad*> *OutWindow::getQuadList() const
{
  return quadList;
}

void OutWindow::setQuadList(QList<Quad*> *value)
{
  quadList = value;
  ui->widget->setQuadList(value);
}


void OutWindow::MapMouseMoved(QPoint pos)
{
  ui->widget->setCursorPos(pos);
}

void OutWindow::updateDisplay()
{
  ui->widget->setNeedsUpdate(true);
}

void OutWindow::outSizeChanged(QSize size) {
  emit outputSizeChanged(size);
}
