#ifndef QUAD_H
#define QUAD_H

#include <QObject>
#include <QPolygon>
#include <QTransform>
#include <QPixmap>
#include <QColor>

class Quad : public QObject
{
  Q_OBJECT
public:
  explicit Quad(QObject *parent = 0);

  QString getName() const;
  void setName(const QString &value);

  QPolygon getQuad() const;
  void setQuad(const QPolygon &value);

  QPolygon* getQuadRef();

  bool getSelected() const;
  void setSelected(bool value);

  typedef enum {
    TEX_TYPE_SOLIDCOLOR,
    TEX_TYPE_IMAGE,
    TEX_TYPE_VIDEO,
    TEX_TYPE_TEST
  } QuadTextureType_enum_t;

  int getTextureType() const;
  void setTextureType(const int &value);

  void updateTransform(QPolygon poly);
  void updateTransform();

  QTransform getTransform() const;
  void setTransform(QTransform value);

  QString getTextureFileName() const;
  void setTextureFileName(const QString &value);

  QColor getPolyColor() const;
  void setPolyColor(const QColor &value);

  QPolygon getQuadToMap() const;
  void setQuadToMap(const QPolygon &value);

  QDataStream &serialize(QDataStream &out);

  friend QDataStream &operator <<(QDataStream &out, const Quad &q);
  friend QDataStream &operator >>(QDataStream &in, Quad &q);

  QPixmap getTexture() const;
  void setTexture(const QPixmap &value);

private:
  QPolygon quad;
  QPolygon quadToMap;
  QString name;
  bool selected;
  int textureType;
  QTransform transform;
  QString textureFileName;
  QColor polyColor;
  QPixmap texture;

signals:

public slots:

};


#endif // QUAD_H
