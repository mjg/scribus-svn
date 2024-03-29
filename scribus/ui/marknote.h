#ifndef MARKNOTE_H
#define MARKNOTE_H

#include "markinsert.h"
#include "notesstyles.h"
#include "ui_marknote.h"

class SCRIBUS_API MarkNote : public MarkInsert, private Ui::MarkNoteDlg
{
	Q_OBJECT
	
public:
	explicit MarkNote(const QList<NotesStyle*>& notesStylesList, QWidget *parent = nullptr);
	NotesStyle* values() override;
	void setValues(const NotesStyle* defaultStyle) override;

protected:
	void changeEvent(QEvent *e) override;

private slots:
	void on_buttonBox_accepted();
};

#endif // MARKNOTE_H
