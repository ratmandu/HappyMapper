#ifndef QUADCONFIGDIALOG_H
#define QUADCONFIGDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QColorDialog>
#include "quad.h"

namespace Ui {
class QuadConfigDialog;
}

class QuadConfigDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QuadConfigDialog(QWidget *parent = 0);
  ~QuadConfigDialog();
  void setQuad(Quad *q);
  Quad* getQuad();

private slots:
  void on_textureTypeComboBox_currentIndexChanged(const QString &arg1);
  void on_pushButton_clicked();

  void on_customTextureOpenButton_clicked();

  void on_selectColorButton_clicked();

private:
  Ui::QuadConfigDialog *ui;
  Quad *quadToConfigure;
};

#endif // QUADCONFIGDIALOG_H
