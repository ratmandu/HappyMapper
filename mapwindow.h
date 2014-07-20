#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSaveFile>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QPixmap>
#include "outwindow.h"
#include "quad.h"
#include "quadconfigdialog.h"

namespace Ui {
class MapWindow;
}

class MapWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MapWindow(QWidget *parent = 0);
  ~MapWindow();

private:
  void outputTemplates(QDir outDir);
  QSettings settings;
  Ui::MapWindow *ui;
  QPixmap outputImage;
  OutWindow *outWin;
  QList<Quad*> *quadList;
  QuadConfigDialog *quadDialog;

signals:
  void mouseMoved(QPoint pos);
  void updateAllDisplays();

public slots:
  void MapMouseMove(QPoint pos);
  void updateDisplays();
  void configureQuadAccepted();

private slots:
  void on_actionQuad_triggered();
  void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
  void on_addQuadButton_clicked();
  void on_delQuadButton_clicked();
  void on_clearSelectionButton_clicked();
  void on_listWidget_currentTextChanged(const QString &currentText);
  void on_configureQuadButton_clicked();
  void on_actionSave_triggered();
  void on_actionOpen_triggered();
  void on_actionCursor_toggled(bool arg1);
  void on_actionPoly_Overlay_toggled(bool arg1);
  void on_actionExport_Templates_triggered();
};

#endif // MAPWINDOW_H
