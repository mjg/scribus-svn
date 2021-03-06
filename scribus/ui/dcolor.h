/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DELCOLOR_H
#define DELCOLOR_H

class QLabel;
class QPushbutton;
class ColorCombo;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

#include <QDialog>
#include "scribusapi.h"
#include "sccolor.h"

class SCRIBUS_API DelColor : public QDialog
{ 
    Q_OBJECT

public:
	DelColor(QWidget* parent, const ColorList& colorList, const QString& colorName, bool haveDoc );
	~DelColor() {};

	const QString &getReplacementColor() const;

private:
	QLabel* deleteLabel { nullptr };
	QLabel* colorToDelLabel { nullptr };
	QLabel* replaceLabel { nullptr };
	QPushButton* okButton { nullptr };
	QPushButton* cancelButton { nullptr };
	ColorCombo* replacementColData { nullptr };
	QVBoxLayout* dialogLayout { nullptr };
	QGridLayout* delColorLayout { nullptr };
	QHBoxLayout* okCancelLayout { nullptr };

	QString replacementColor;
	ColorList cList;

private slots:
	virtual void ReplaceColor(int);
};

#endif // DELCOLOR_H
