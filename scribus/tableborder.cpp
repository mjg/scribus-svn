/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <algorithm>
#include <functional>

#include <QList>
#include <QString>
#include <QStringList>
#include <QtAlgorithms>

#include "tableborder.h"

/*
 * TableBorderLine definitions.
 */

TableBorderLine::TableBorderLine() = default;

TableBorderLine::TableBorderLine(double width, Qt::PenStyle style, const QString& color, double shade)
{
	m_width = width;
	m_style = style;
	m_color = color;
	m_shade = shade;
}

QString TableBorderLine::asString() const
{
	return QString("(%1,%2,%3,%4)").arg(width()).arg(style()).arg(color()).arg(shade());
}

/*
 * TableBorder definitions.
 */

TableBorder::TableBorder(double width, Qt::PenStyle style, const QString& color, double shade)
{
	addBorderLine(TableBorderLine(width, style, color, shade));
}

double TableBorder::width() const
{
	return isNull() ? 0.0 : m_borderLines.first().width();
}

TableBorderLine TableBorder::borderLine(int index) const
{
	if (index < 0 || index >= m_borderLines.size())
		return TableBorderLine();

	return m_borderLines.at(index);
}

void TableBorder::addBorderLine(const TableBorderLine& borderLine)
{
	m_borderLines.append(borderLine);
	std::stable_sort(m_borderLines.begin(), m_borderLines.end(), std::greater<TableBorderLine>());
}

void TableBorder::removeBorderLine(int index)
{
	if (index < 0 || index >= m_borderLines.size())
		return;

	m_borderLines.removeAt(index);
}

void TableBorder::replaceBorderLine(int index, const TableBorderLine& borderLine)
{
	if (index < 0 || index >= m_borderLines.size())
		return;

	m_borderLines.replace(index, borderLine);
	std::stable_sort(m_borderLines.begin(), m_borderLines.end(), std::greater<TableBorderLine>());
}

QString TableBorder::asString() const
{
	QStringList lines;
	for (const TableBorderLine& line : m_borderLines)
		lines << line.asString();
	return QString("TableBorder(%1)").arg(lines.join(","));
}
