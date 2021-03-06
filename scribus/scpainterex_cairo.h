/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCPAINTEREX_CAIRO_H
#define SCPAINTEREX_CAIRO_H

#include <cairo.h>
#include "scpainterexbase.h"

#include "mesh.h"

class ScPainterEx_Cairo : public ScPainterExBase
{
public:
	ScPainterEx_Cairo(cairo_t* context, const QRect& rect, ScribusDoc* doc, bool gray );
	virtual ~ScPainterEx_Cairo();

	Capabilities capabilities() const override { return transparencies; }

	int supportedColorModes() const override { return (int) rgbMode; }
	ColorMode preferredColorMode() const override { return rgbMode; }
	ImageMode imageMode() const override { return rgbImages; }

	void begin() override;
	void end() override;
	void clear() override;
	void clear(ScColorShade &) override;

	// matrix manipulation
	void setWorldMatrix(const QTransform &) override;
	QTransform worldMatrix() const override;
	void translate( double, double ) override;
	void translate( const QPointF& offset ) override;
	void rotate( double ) override;
	void scale( double, double ) override;

	// drawing
	void moveTo( const double &, const double & ) override;
	void lineTo( const double &, const double & ) override;
	void curveTo( FPoint p1, FPoint p2, FPoint p3 ) override;
	void newPath() override;
	virtual void closePath();
	void fillPath() override;
	void strokePath() override;
	void setFillRule( bool fillRule ) override;
	bool fillRule() const override  { return m_fillRule; }
	void setFillMode( int fill ) override;
	int  fillMode() const  override  { return m_fillMode; }
	void setStrokeMode( int fill ) override;
	int  strokeMode()  const override { return m_strokeMode; }
	void setGradient( VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew) override;
	void setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY) override;

	void setMaskMode( int mask ) override;
	void setGradientMask(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew) override;
	void setPatternMask(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY) override;

	void set4ColorGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4) override;
	void set4ColorColors(const ScColorShade& col1, const ScColorShade& col2, const ScColorShade& col3, const ScColorShade& col4) override;
	void setDiamondGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4, FPoint c5) override;
	void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<QList<MeshPoint> > meshArray) override;
	void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<meshGradientPatch> meshPatches) override;

	void setClipPath() override;

	void drawImage( ScImage *image, ScPainterExBase::ImageMode mode ) override;
	void setupPolygon(const FPointArray *points, bool closed = true) override;
	void drawPolygon() override;
	void drawPolyLine() override;
	void drawLine(FPoint start, FPoint end) override;
	void drawLine(const QPointF& start, const QPointF& end) override;
	void drawRect(double, double, double, double) override;

	// pen + brush
	ScColorShade pen() override;
	ScColorShade brush() override;
	void setPen( const ScColorShade & ) override;
	void setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo ) override;
	void setPenOpacity( double op ) override;
	void setLineWidth( double w) override;
	void setDash(const QVector<double>& array, double ofs) override;
	void setBrush( const ScColorShade & ) override;
	void setBrushOpacity( double op ) override;
	void setOpacity( double op ) override;
	void setFont( const QFont &f ) override;
	QFont font() override;

	// stack management
	void save() override;
	void restore() override;

	void setRasterOp( int op ) override;
	void setBlendModeFill( int blendMode ) override;
	void setBlendModeStroke( int blendMode ) override;

private:
	void fillPathHelper();
	void strokePathHelper();

	void drawGradient( VGradientEx& gradient );
	void drawLinearGradient( VGradientEx& gradient, const QRect& rect );
	void drawCircularGradient( VGradientEx& gradient, const QRect& rect );
	void drawFourColorGradient( const QRect& rect );
	void drawDiamondGradient( VGradientEx& gradient, const QRect& rect );
	void drawMeshGradient( const QRect& rect );
	void drawFreeMeshGradient( const QRect& rect );

	void strokeGradient( VGradientEx& gradient );
	void strokeLinearGradient( VGradientEx& gradient );
	void strokeCircularGradient( VGradientEx& gradient );

	void getClipPathDimensions( QRect& r );

	ScribusDoc* m_doc { nullptr };

	unsigned int m_width;
	unsigned int m_height;
	QTransform m_matrix;
	QFont m_font;
/* Layer blend mode*/
	int  m_blendModeLayer { 0 };
	int  m_blendModeFill { 0 };
	int  m_blendModeStroke { 0 };
/* Filling */
	ScColorShade m_fillColor;
	double m_fillTrans { 1.0 };
	bool   m_fillRule { true };
	int    m_fillMode { 1 };			// 0 = none, 1 = solid, 2 = gradient
	int    m_gradientMode;		// 1 = linear, 2 = radial

	double m_patternScaleX;
	double m_patternScaleY;
	double m_patternOffsetX;
	double m_patternOffsetY;
	double m_patternRotation;
	double m_patternSkewX;
	double m_patternSkewY;
	bool   m_patternMirrorX;
	bool   m_patternMirrorY;

	FPoint m_gradPatchP1;
	FPoint m_gradPatchP2;
	FPoint m_gradPatchP3;
	FPoint m_gradPatchP4;
	FPoint m_gradControlP1;
	FPoint m_gradControlP2;
	FPoint m_gradControlP3;
	FPoint m_gradControlP4;
	FPoint m_gradControlP5;
	ScColorShade m_gradPatchColor1;
	ScColorShade m_gradPatchColor2;
	ScColorShade m_gradPatchColor3;
	ScColorShade m_gradPatchColor4;
	QList<QList<MeshPoint> > m_meshGradientArray;
	QList<meshGradientPatch> m_meshGradientPatches;

	double m_gradientScale;
	double m_gradientSkew;
/* Stroking */
	ScColorShade m_strokeColor;
	double m_strokeTrans { 1.0 };
	double m_lineWidth { 1.0 };
	int    m_strokeMode;				// 0 = none, 1 = solid, 2 = gradient 3 = pattern
/* Masking */
	int    m_maskMode;				// 0 = none, 1 = gradient 2 = pattern
	double m_maskPatternScaleX;
	double m_maskPatternScaleY;
	double m_maskPatternOffsetX;
	double m_maskPatternOffsetY;
	double m_maskPatternRotation;
	double m_maskPatternSkewX;
	double m_maskPatternSkewY;
	bool   m_maskPatternMirrorX;
	bool   m_maskPatternMirrorY;
	double m_maskGradientScale;
	double m_maskGradientSkew;

/* Grayscale conversion option */
	bool   m_convertToGray { false };

/* Line End Style */
	Qt::PenCapStyle m_lineEnd { Qt::SquareCap };
/* Line Join Style */
	Qt::PenJoinStyle m_lineJoin { Qt::RoundJoin };
/* The Dash Array */
	QVector<double> m_array;
	double m_offset { 0.0 };
/* Transformation Stack */
	QStack<QTransform> m_stack;

/* Cairo context */
	cairo_t* m_cr { nullptr };

/* Color conversion function */
	QColor transformColor( const ScColorShade& colorShade, double trans ) const;
	void   transformImage( QImage& image ) const;

	QStack<int> m_gStates;
	double      m_positionX { 0.0 };
	double      m_positionY { 0.0 };
};

#endif
