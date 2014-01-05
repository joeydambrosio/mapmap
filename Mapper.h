/*
 * Mapper.h
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 * (c) 2013 Alexandre Quessy -- alexandre(@)quessy(.)net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MAPPER_H_
#define MAPPER_H_

#include <QtGlobal>

#if __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tr1/memory>

#include <stdlib.h>
#include <stdio.h>

#include "Shape.h"
#include "Paint.h"
#include "Mapping.h"

#include "Util.h"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"

/**
 * A way to draw on some kind of shape.
 * 
 * This is an abstract class for specific ways to draw
 * a given paint of a shape.
 * 
 * Each shape x paint combination that is possible in 
 * this software are implemented using a child of this
 * class.
 */
class Mapper : public QObject
{
  Q_OBJECT

protected:
  Mapping::ptr _mapping;

public:
  typedef std::tr1::shared_ptr<Mapper> ptr;

  Mapper(Mapping::ptr mapping) : _mapping(mapping) {}
  virtual ~Mapper() {}

  virtual QWidget* getPropertiesEditor() = 0;
  virtual void draw() = 0;
  virtual void drawControls() = 0;

public slots:
  virtual void updateShape(Shape* shape)
  {
    Q_UNUSED(shape);
  }
};

/**
 * Draws a texture.
 */
class TextureMapper : public Mapper
{
  Q_OBJECT

public:
  TextureMapper(std::tr1::shared_ptr<TextureMapping> mapping);
  virtual ~TextureMapper() {}

  virtual QWidget* getPropertiesEditor();

  virtual void draw();
  virtual void drawInput();

  virtual void drawControls();
  virtual void drawInputControls();

  static void drawShapeContour(const Shape& shape, int lineWidth, const QColor& color);

signals:
  void valueChanged();

public slots:
  virtual void setValue(QtProperty* property, const QVariant& value);
  virtual void updateShape(Shape* shape);

protected:
  virtual void _doDraw() = 0;

protected:
  QtAbstractPropertyBrowser* _propertyBrowser;
  QtVariantEditorFactory* _variantFactory;
  QtVariantPropertyManager* _variantManager;
  QtProperty* _topItem;
  QtProperty* _inputItem;
  QtProperty* _outputItem;

  QtVariantProperty* _meshItem;

  std::map<QtProperty*, std::pair<Shape*, int> > _propertyToVertex;

  // FIXME: use typedefs, member of the class for type names that are too long to type:
  std::tr1::shared_ptr<TextureMapping> textureMapping;
  std::tr1::shared_ptr<Texture> texture;
  std::tr1::shared_ptr<Shape> outputShape;
  std::tr1::shared_ptr<Shape> inputShape;

  virtual void _buildShapeProperty(QtProperty* shapeItem, Shape* shape);
  virtual void _updateShapeProperty(QtProperty* shapeItem, Shape* shape);
};

class TriangleTextureMapper : public TextureMapper
{
  Q_OBJECT

public:
  TriangleTextureMapper(std::tr1::shared_ptr<TextureMapping> mapping);
  virtual ~TriangleTextureMapper() {}

protected:
  virtual void _doDraw();
};


class MeshTextureMapper : public TextureMapper
{
  Q_OBJECT

public:
  MeshTextureMapper(std::tr1::shared_ptr<TextureMapping> mapping);
  virtual ~MeshTextureMapper() {}

  virtual void drawControls();
  virtual void drawInputControls();

public slots:
  virtual void setValue(QtProperty* property, const QVariant& value);

protected:
  virtual void _doDraw();

private:
  QtVariantProperty* _meshItem;
};

#endif /* MAPPER_H_ */
