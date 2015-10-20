#ifndef CSREENEDITORWIDGET_H
#define CSREENEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class CSreenEditorWidget;
}

class CSreenEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CSreenEditorWidget(QWidget *parent = 0);
    ~CSreenEditorWidget();

private:
    Ui::CSreenEditorWidget *ui;
};

#endif // CSREENEDITORWIDGET_H
