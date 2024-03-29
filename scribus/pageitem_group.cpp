/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_group.cpp  -  description
                             -------------------
    begin                : Sat December 4 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cmath>
#include <cassert>

#include <QApplication>
#include <QFontInfo>

#include "commonstrings.h"
#include "pageitem_group.h"
#include "pageitemiterator.h"
#include "prefsmanager.h"
#include "scpainter.h"
#include "scpage.h"
#include "scpaths.h"
#include "scraction.h"

#include "scribusstructs.h"
#include "scribusdoc.h"

#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"

using namespace std;

PageItem_Group::PageItem_Group(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline)
	: PageItem(pa, PageItem::Group, x, y, w, h, 0, CommonStrings::None, CommonStrings::None)
{
}

PageItem_Group::~PageItem_Group()
{
//	while (!groupItemList.isEmpty())
//	{
//		delete groupItemList.takeFirst();
//	}
}

bool PageItem_Group::isTextContainer() const
{
	PageItemIterator it(groupItemList, PageItemIterator::IterateInGroups);
	for (; *it; ++it)
	{
		PageItem* item = *it;
		if (item->isGroup())
			continue;
		if (item->isTextContainer())
			return true;
	}

	return false;
}

void PageItem_Group::adjustXYPosition()
{
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->setXYPos(xPos() + embedded->gXpos, yPos() + embedded->gYpos, true);
		if (embedded->isGroup())
			embedded->asGroupFrame()->adjustXYPosition();
	}
}

QList<PageItem*> PageItem_Group::getAllChildren() const
{
	QList<PageItem*> ret;
	ret.reserve(groupItemList.count());
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		ret.append(embedded);
		if (embedded->isGroup())
			ret += embedded->getAllChildren();
	}
	return ret;
}

void PageItem_Group::layout()
{
	for (int i = 0; i < groupItemList.count(); ++i)
	{
		PageItem* embedded = groupItemList.at(i);
		embedded->layout();
	}
}

void PageItem_Group::setLayer(int newLayerID)
{
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->setLayer(newLayerID);
	}
	m_layerID = newLayerID;
}

void PageItem_Group::setMasterPage(int page, const QString& mpName)
{
	PageItem::setMasterPage(page, mpName);

	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->setMasterPage(page, mpName);
	}
}

void PageItem_Group::setMasterPageName(const QString& mpName)
{
	PageItem::setMasterPageName(mpName);

	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->setMasterPageName(mpName);
	}
}

void PageItem_Group::replaceNamedResources(ResourceCollection& newNames)
{
	QMap<QString,QString>::ConstIterator it;
	
	it = newNames.colors().find(softShadowColor());
	if (it != newNames.colors().end())
		setSoftShadowColor(*it);
	
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->replaceNamedResources(newNames);
	}
}

void PageItem_Group::getNamedResources(ResourceCollection& lists) const
{
	if (hasSoftShadow())
		lists.collectColor(softShadowColor());
	
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->getNamedResources(lists);
	}
}

void PageItem_Group::DrawObj_Item(ScPainter *p, const QRectF& /*e*/)
{
	if (m_Doc->RePos)
		return;

	if (groupItemList.isEmpty())
	{
		if (m_Doc->guidesPrefs().framesShown)
		{
			p->save();
			p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->drawLine(FPoint(0, 0), FPoint(m_width, m_height));
			p->drawLine(FPoint(0, m_height), FPoint(m_width, 0));
			const QFont &font = QApplication::font();
			p->setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.findFont(font.family(), QFontInfo(font).styleName()), font.pointSizeF());
			p->drawLine(FPoint(0, 0), FPoint(m_width, 0));
			p->drawLine(FPoint(m_width, 0), FPoint(m_width, m_height));
			p->drawLine(FPoint(m_width, m_height), FPoint(0, m_height));
			p->drawLine(FPoint(0, m_height), FPoint(0, 0));
			p->setBrush(QColor(255, 255, 255));
			p->setBrushOpacity(0.0);
			p->drawText(QRectF(0.0, 0.0, m_width, m_height), "Empty Group");
			p->restore();
		}
		return;
	}

	p->save();
	if (imageFlippedH())
	{
		p->translate(m_width, 0);
		p->scale(-1, 1);
	}
	if (imageFlippedV())
	{
		p->translate(0, m_height);
		p->scale(1, -1);
	}
	if ((maskType() == GradMask_Linear) || (maskType() == GradMask_Radial) || (maskType() == GradMask_LinearLumAlpha) || (maskType() == GradMask_RadialLumAlpha))
	{
		if ((maskType() == GradMask_Linear) || (maskType() == GradMask_Radial))
			p->setMaskMode(1);
		else
			p->setMaskMode(3);
		if ((!gradientMask().isEmpty()) && (!m_Doc->docGradients.contains(gradientMask())))
			gradientMaskVal.clear();
		if (!(gradientMask().isEmpty()) && (m_Doc->docGradients.contains(gradientMask())))
			mask_gradient = m_Doc->docGradients[gradientMask()];
		p->mask_gradient = mask_gradient;
		if ((maskType() == GradMask_Linear) || (maskType() == GradMask_LinearLumAlpha))
			p->setGradientMask(VGradient::linear, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskStartX, GrMaskStartY), GrMaskScale, GrMaskSkew);
		else
			p->setGradientMask(VGradient::radial, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskFocalX, GrMaskFocalY), GrMaskScale, GrMaskSkew);
	}
	else if ((maskType() == GradMask_Pattern) || (maskType() == GradMask_PatternLumAlpha) || (maskType() == GradMask_PatternLumAlphaInverted) || (maskType() == GradMask_PatternInverted))
	{
		if ((patternMask().isEmpty()) || (!m_Doc->docPatterns.contains(patternMask())))
			p->setMaskMode(0);
		else
		{
			double scw = m_width / groupWidth;
			double sch = m_height / groupHeight;
			ScMaskTransform patternMaskTrans = patternMaskTransfrm;
			patternMaskTrans.scaleX *= scw;
			patternMaskTrans.scaleY *= sch;
			p->setPatternMask(&m_Doc->docPatterns[patternMask()], patternMaskTrans, patternMaskMirrorX, patternMaskMirrorY);
			if (maskType() == GradMask_Pattern)
				p->setMaskMode(2);
			else if (maskType() == GradMask_PatternLumAlpha)
				p->setMaskMode(4);
			else if (maskType() == GradMask_PatternLumAlphaInverted)
				p->setMaskMode(5);
			else
				p->setMaskMode(6);
		}
	}
	else
		p->setMaskMode(0);
	p->setFillRule(fillRule);
	if (m_groupClips)
		p->beginLayer(1.0 - fillTransparency(), fillBlendmode(), &PoLine);
	else
		p->beginLayer(1.0 - fillTransparency(), fillBlendmode());
	p->setMaskMode(0);
	p->scale(m_width / groupWidth, m_height / groupHeight);
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		p->save();
		p->translate(embedded->gXpos, embedded->gYpos);
		embedded->isEmbedded = true;
		embedded->invalidateLayout();
		embedded->DrawObj(p, QRectF());
		embedded->isEmbedded = false;
		p->restore();
		if (m_Doc->guidesPrefs().framesShown)
		{
			p->save();
			double x = embedded->xPos();
			double y = embedded->yPos();
			embedded->setXYPos(embedded->gXpos, embedded->gYpos, true);
			embedded->DrawObj_Decoration(p);
			embedded->setXYPos(x, y, true);
			p->restore();
		}
		if (m_Doc->layerOutline(m_layerID))
		{
			p->save();
			p->setPen(m_Doc->layerMarker(m_layerID), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setFillMode(ScPainter::None);
			p->setBrushOpacity(1.0);
			p->setPenOpacity(1.0);
			p->setupPolygon(&PoLine);
			p->strokePath();
			p->restore();
		}
	}
	p->endLayer();
	p->restore();
}

void PageItem_Group::applicableActions(QStringList & actionList)
{
}

QString PageItem_Group::infoDescription() const
{
	return QString();
}
