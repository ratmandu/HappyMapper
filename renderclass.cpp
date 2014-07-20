#include "renderclass.h"

RenderClass::RenderClass(QObject *parent) :
  QObject(parent)
{

}

QPixmap RenderClass::RenderOutput()
{
  QPixmap output;
  return output;
}

QPixmap RenderClass::RenderPreview()
{
  QPixmap output;
  return output;
}
