#ifndef FORM_MAPTAB_H
#define FORM_MAPTAB_H

#include <QWidget>
#include <QResizeEvent>

namespace Ui {
    class Form_Maptab;
}

class Form_Maptab : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Maptab(QWidget *parent = 0);
    ~Form_Maptab();

protected:
	void resizeEvent(QResizeEvent *event) ;

private:
    Ui::Form_Maptab *ui;
};

#endif // FORM_MAPTAB_H
