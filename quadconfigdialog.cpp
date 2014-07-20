#include "quadconfigdialog.h"
#include "ui_quadconfigdialog.h"

QuadConfigDialog::QuadConfigDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QuadConfigDialog)
{
  ui->setupUi(this);
  ui->testTextureDisplayLabel->setPixmap(QPixmap(":/tests/checkerboard.jpg"));
}

QuadConfigDialog::~QuadConfigDialog()
{
  delete ui;
}

void QuadConfigDialog::setQuad(Quad *q)
{
  quadToConfigure = q;

  ui->nameEdit->setText(q->getName());

  switch (q->getTextureType()) {
  case Quad::TEX_TYPE_TEST:
    ui->textureTypeComboBox->setCurrentIndex(0);
    ui->testTextureDisplayLabel->setPixmap(QPixmap(":/tests/checkerboard.jpg"));
    break;
  case Quad::TEX_TYPE_IMAGE:
    ui->textureTypeComboBox->setCurrentIndex(1);
    ui->customTextureDisplayLabel->setPixmap(QPixmap(q->getTextureFileName()));
    ui->customTextureLineEdit->setText(q->getTextureFileName());
    break;
  case Quad::TEX_TYPE_VIDEO:
    ui->textureTypeComboBox->setCurrentIndex(2);
    break;
  case Quad::TEX_TYPE_SOLIDCOLOR:
    ui->textureTypeComboBox->setCurrentIndex(3);
    break;
  }
}

Quad* QuadConfigDialog::getQuad() {
  return quadToConfigure;
}

void QuadConfigDialog::on_textureTypeComboBox_currentIndexChanged(const QString &arg1)
{
  ui->testTextureTab->setEnabled(false);
  ui->customTextureTab->setEnabled(false);
  ui->videoTextureTab->setEnabled(false);
  ui->solidColorTab->setEnabled(false);

  if (arg1 == "Test Texture") {
    ui->testTextureTab->setEnabled(true);
    ui->tabWidget->setCurrentIndex(0);
  } else if (arg1 == "Custom Texture") {
    ui->customTextureTab->setEnabled(true);
    ui->tabWidget->setCurrentIndex(1);
  } else if (arg1 == "Video") {
    ui->videoTextureTab->setEnabled(true);
    ui->tabWidget->setCurrentIndex(2);
  } else if (arg1 == "Solid Color") {
    ui->solidColorTab->setEnabled(true);
    ui->tabWidget->setCurrentIndex(3);
  }
}

void QuadConfigDialog::on_pushButton_clicked()
{
  quadToConfigure->setName(ui->nameEdit->text());

  switch (ui->textureTypeComboBox->currentIndex()) {
  case 0:
    quadToConfigure->setTextureType(Quad::TEX_TYPE_TEST);
    break;
  case 1:
    quadToConfigure->setTextureType(Quad::TEX_TYPE_IMAGE);
    quadToConfigure->setTextureFileName(ui->customTextureLineEdit->text());
    quadToConfigure->setQuadToMap(QPixmap(ui->customTextureLineEdit->text()).rect());
    quadToConfigure->updateTransform();
    break;
  case 2:
    quadToConfigure->setTextureType(Quad::TEX_TYPE_VIDEO);
    break;
  case 3:
    quadToConfigure->setTextureType(Quad::TEX_TYPE_SOLIDCOLOR);
    break;
  }

  emit accepted();
  this->close();
}

void QuadConfigDialog::on_customTextureOpenButton_clicked()
{
  QString filename = QFileDialog::getOpenFileName(0, "Open Texture", QDir::home().absolutePath().append("/Pictures"));//, "Image Files (*.jpg, *.png, *.gif);; All Files (*.*)");
  if (!filename.isEmpty()) {
    ui->customTextureLineEdit->setText(filename);
    ui->customTextureDisplayLabel->setPixmap(QPixmap(filename));
  }
}

void QuadConfigDialog::on_selectColorButton_clicked()
{
    quadToConfigure->setPolyColor(QColorDialog::getColor());
}
