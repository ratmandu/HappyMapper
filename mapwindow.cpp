#include "mapwindow.h"
#include "ui_mapwindow.h"

MapWindow::MapWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MapWindow)
{
  ui->setupUi(this);
  ui->actionPoly_Overlay->setChecked(settings.value("viewSettings/polyOverlay", false).toBool());
  ui->actionCursor->setChecked(settings.value("viewSettings/cursorVisible", true).toBool());
  quadList = new QList<Quad*>;

  outWin = new OutWindow();
  outWin->show();

  outWin->setQuadList(quadList);
  ui->widget->setQuadList(quadList);
  ui->widget->setOutputSize(outWin->getSize());
  quadDialog = new QuadConfigDialog();

  connect(outWin, SIGNAL(outputSizeChanged(QSize)), ui->widget, SLOT(outputResized(QSize)));
  connect(ui->widget, SIGNAL(updateDisplays()), outWin, SLOT(updateDisplay()));
  connect(this, SIGNAL(updateAllDisplays()), outWin, SLOT(updateDisplay()));
  connect(this, SIGNAL(updateAllDisplays()), ui->widget, SLOT(updateDisplay()));
  connect(ui->widget, SIGNAL(mouseMove(QPoint)), outWin, SLOT(MapMouseMoved(QPoint)));
  connect(quadDialog, SIGNAL(accepted()), this, SLOT(configureQuadAccepted()));
}

MapWindow::~MapWindow()
{
  delete ui;
}

void MapWindow::outputTemplates(QDir outDir)
{
  if (!outDir.exists()) {
    outDir.mkdir(outDir.absolutePath());
  }

  QPainter p;
  p.setRenderHint(QPainter::HighQualityAntialiasing);
  for (int i = 0; i<quadList->size(); i++) {
    Quad *q = quadList->at(i);
    QPixmap *temp = new QPixmap(q->getQuad().boundingRect().size());
    temp->fill(Qt::transparent);
    qDebug() << "Quad: " << q->getName() << " poly: " << q->getQuad();
    p.begin(temp);
    p.setPen(QColor(Qt::black));
    p.setBrush(QColor(Qt::black));
    p.translate(-q->getQuad().at(0));
    p.drawPolygon(q->getQuad());
    p.end();
    temp->save(outDir.absolutePath().append("/").append(q->getName()).append(".png"));
  }
}

void MapWindow::MapMouseMove(QPoint pos)
{
  emit mouseMoved(pos);
}

void MapWindow::updateDisplays()
{
  emit updateDisplays();
}

void MapWindow::configureQuadAccepted()
{
  for (int i = ui->listWidget->count()-1; i >= 0; i--) {
    ui->listWidget->takeItem(i);
  }
  for (int i = 0; i<quadList->size(); i++) {
    new QListWidgetItem(quadList->at(i)->getName(), ui->listWidget);
    if (quadList->at(i)->getSelected()) {
      ui->listWidget->item(ui->listWidget->count()-1)->setSelected(true);
    }
  }
}

void MapWindow::on_actionQuad_triggered()
{
  Quad *q = new Quad();
  q->setName(QString("Quad %1").arg(quadList->size()+1));
  quadList->append(q);

  new QListWidgetItem(quadList->last()->getName(), ui->listWidget);
  ui->listWidget->item(ui->listWidget->count() - 1)->setFlags(ui->listWidget->item(ui->listWidget->count()-1)->flags() | Qt::ItemIsEditable);
  on_clearSelectionButton_clicked();
//  on_listWidget_currentItemChanged(ui->listWidget->currentItem(), ui->listWidget->item(ui->listWidget->count()-1));
  ui->listWidget->item(ui->listWidget->count() - 1)->setSelected(true);
  q->setSelected(true);
  ui->widget->setFocus();

  emit updateAllDisplays();
}

void MapWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
  QString newName;
  QString oldName;

  if (previous != NULL) {
    oldName = previous->text();
  } else {
    oldName = "";
  }

  if (current != NULL) {
    newName = current->text();
  } else {
    newName = "";
  }

  for (int i = 0; i<quadList->size(); i++) {
    if (quadList->at(i)->getName() == newName) {
      quadList->at(i)->setSelected(true);
    } else if (quadList->at(i)->getName() == oldName){
      quadList->at(i)->setSelected(false);
    }
  }

  emit updateAllDisplays();
}

void MapWindow::on_addQuadButton_clicked()
{
    this->on_actionQuad_triggered();
}

void MapWindow::on_delQuadButton_clicked()
{
    if (ui->listWidget->currentItem() != NULL) {
      QString itemName = ui->listWidget->currentItem()->text();

      for (int i = 0; i < quadList->size(); i++) {
        if (quadList->at(i)->getName() == itemName) {
          quadList->removeAt(i);
          ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
//          ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
        }
      }
    }

    emit updateAllDisplays();
}

void MapWindow::on_clearSelectionButton_clicked()
{
    int i;
    for (i=0; i<ui->listWidget->count(); i++) {
      ui->listWidget->item(i)->setSelected(false);
    }
    for (i=0; i<quadList->size(); i++) {
      quadList->at(i)->setSelected(false);
    }

    emit updateAllDisplays();
}

void MapWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    for (int i=0; i<quadList->size(); i++) {
      if (quadList->at(i)->getSelected()) {
        quadList->at(i)->setName(currentText);
        break;
      }
    }
}

void MapWindow::on_configureQuadButton_clicked()
{
  for (int i = 0; i < quadList->size(); i++) {
    if (quadList->at(i)->getSelected()) {
//      quadDialog = new QuadConfigDialog();
      quadDialog->setQuad(quadList->at(i));
      quadDialog->show();
    }
  }
}

void MapWindow::on_actionSave_triggered()
{
  QString filename;
  filename = QFileDialog::getSaveFileName(0, "Save File", QDir::home().absolutePath(), "ProjMapper Files (*.pmap)");
  if (!filename.isEmpty()) {
    QFile fd(filename);
    fd.open(QIODevice::ReadWrite);
    QDataStream ds(&fd);
    qDebug() << "Serializing all quads to: " << fd.fileName();
    ds << quadList->size();
    for (int i = 0; i<quadList->size(); i++) {
      quadList->at(i)->serialize(ds);
      qDebug() << i;
    }
    ds.device()->reset();
    fd.close();
  }
}

void MapWindow::on_actionOpen_triggered()
{
  QString filename;
  filename = QFileDialog::getOpenFileName(0, "Open File", QDir::home().absolutePath(), "ProjMapper Files (*.pmap)");
  if (!filename.isEmpty()) {
    QFile fd(filename);
    fd.open(QIODevice::ReadWrite);
    QDataStream ds(&fd);
    Quad *q;
    int size;
    ds >> size;
    quadList->clear();
    for (int i = ui->listWidget->count()-1; i >= 0; i--) {
      ui->listWidget->takeItem(i);
    }

    for (int i = 0; i<size; i++) {
      q = new Quad();
      ds >> *q;
      quadList->append(q);
      new QListWidgetItem(q->getName(), ui->listWidget);
      qDebug() << q->getName();
    }
    fd.close();

    emit updateAllDisplays();
  }
}

void MapWindow::on_actionCursor_toggled(bool arg1)
{
    settings.setValue("viewSettings/cursorVisible", arg1);
    emit updateAllDisplays();
}

void MapWindow::on_actionPoly_Overlay_toggled(bool arg1)
{
    settings.setValue("viewSettings/polyOverlay", arg1);
    emit updateAllDisplays();
}

void MapWindow::on_actionExport_Templates_triggered()
{
    QString outDirPath = QFileDialog::getExistingDirectory(0, "Select output directory", QDir::homePath().append("/Pictures"));
    if (!outDirPath.isEmpty()) {
      outputTemplates(QDir(outDirPath));
    }
}
