/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TEXTPALETTE_H
#define TEXTPALETTE_H

#include <QListWidgetItem>
#include <QLineEdit>

class QCloseEvent;
class QEvent;
class QFocusEvent;
class QToolBox;
class QVBoxLayout;
class QWidget;


#include "pageitem.h"
#include "scdockpalette.h"
#include "scribusapi.h"

class PropertiesPalette_Text;
class ScribusDoc;
class ScribusMainWindow;
class UndoManager;

class SCRIBUS_API TextPalette : public ScDockPalette
{
	Q_OBJECT

public:
	TextPalette(QWidget* parent);
	~TextPalette() {}

	void updateColorList();
	void endPatchAdd();

	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // committed

	PropertiesPalette_Text*  textPal { nullptr };

private:
	PageItem* currentItemFromSelection();
	
public slots:
	void setMainWindow(ScribusMainWindow *mw);
	
	void setDoc(ScribusDoc *d);
	void AppModeChanged();
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	
	void unitChange();
	void languageChange();

private slots:
	void NewLineMode(int mode);

protected:
	ScribusMainWindow *m_ScMW { nullptr };

	bool      m_haveDoc { false };
	bool      m_haveItem { false };
	double    m_unitRatio { 1.0 };
	int       m_unitIndex { 0 };
	PageItem* m_item { nullptr };
	UndoManager *undoManager { nullptr };
	
	ScGuardedPtr<ScribusDoc> m_doc;

	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *closeEvent) override;
};

#endif
