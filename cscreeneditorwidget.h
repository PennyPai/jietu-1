#ifndef CSREENEDITORWIDGET_H
#define CSREENEDITORWIDGET_H

#include <QWidget>
#include "clogsetting.h"
namespace Ui {
class CScreenEditorWidget;
}

class CScreenEditorWidget : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit CScreenEditorWidget(QWidget *parent = 0);
    ~CScreenEditorWidget();

private:
    Ui::CScreenEditorWidget *ui;
};

#endif // CSREENEDITORWIDGET_H
