#include "quad.h"

Quad::Quad(QObject *parent) :
  QObject(parent)
{
  quad.append(QPoint(10,10));
  quad.append(QPoint(100, 10));
  quad.append(QPoint(100, 100));
  quad.append(QPoint(10, 100));
  selected = false;
  textureType = TEX_TYPE_TEST;
  textureFileName = ":/tests/checkerboard.jpg";
  texture = QPixmap(textureFileName);
  quadToMap = texture.rect();
  this->updateTransform();
}

QString Quad::getName() const
{
  return name;
}

void Quad::setName(const QString &value)
{
  name = value;
}

QPolygon Quad::getQuad() const
{
  return quad;
}

void Quad::setQuad(const QPolygon &value)
{
  quad = value;
//  this->updateTransform();
}

QPolygon *Quad::getQuadRef()
{
  return &quad;
}

bool Quad::getSelected() const
{
  return selected;
}

void Quad::setSelected(bool value)
{
  selected = value;
}

int Quad::getTextureType() const
{
  return textureType;
}

void Quad::setTextureType(const int &value)
{
  textureType = value;
}

void Quad::updateTransform(QPolygon poly)
{
  QTransform::quadToQuad(poly, quad, transform);
}

void Quad::updateTransform()
{
  QTransform::quadToQuad(quadToMap, quad, transform);
}

QTransform Quad::getTransform() const
{
  return transform;
}

void Quad::setTransform(QTransform value)
{
  transform = value;
}

QString Quad::getTextureFileName() const
{
  return textureFileName;
}

void Quad::setTextureFileName(const QString &value)
{
  textureFileName = value;
  texture = QPixmap(value);
}

QColor Quad::getPolyColor() const
{
  return polyColor;
}

void Quad::setPolyColor(const QColor &value)
{
  polyColor = value;
}

QPolygon Quad::getQuadToMap() const
{
  return quadToMap;
}

void Quad::setQuadToMap(const QPolygon &value)
{
  quadToMap = value;
}

QDataStream &Quad::serialize(QDataStream &out)
{
//  QDataStream out(&ba, QIODevice::ReadWrite);
  out << this->getName() << this->getQuad() << this->getQuadToMap() << this->getSelected() << this->getTextureType() << this->getTextureFileName() << this->getPolyColor() << this->getTransform() << this->getTexture();
//  out.device()->reset();
//  out.device()->close();
  return out;
}
QPixmap Quad::getTexture() const
{
  return texture;
}

void Quad::setTexture(const QPixmap &value)
{
  texture = value;
}


QDataStream &operator <<(QDataStream &out, const Quad &q)
{
  out << q.getName() << q.getQuad() << q.getQuadToMap() << q.getSelected() << q.getTextureType() << q.getTextureFileName() << q.getPolyColor() << q.getTransform() << q.getTexture();
  return out;
}

QDataStream &operator >>(QDataStream &in, Quad &q)
{
  QString name;
  QPolygon poly;
  QPolygon polyToMap;
  bool selected;
  int textureType;
  QString textureFileName;
  QColor polyColor;
  QTransform transform;
  QPixmap tex;

  in >> name >> poly >> polyToMap >> selected >> textureType >> textureFileName >> polyColor >> transform >> tex;

  q.setName(name);
  q.setQuad(poly);
  q.setQuadToMap(polyToMap);
  q.setSelected(selected);
  q.setTextureType(textureType);
  q.setTextureFileName(textureFileName);
  q.setPolyColor(polyColor);
  q.setTransform(transform);
  q.setTexture(tex);

  return in;
}
