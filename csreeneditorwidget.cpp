#include "csreeneditorwidget.h"
#include "ui_csreeneditorwidget.h"

CSreenEditorWidget::CSreenEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSreenEditorWidget)
{
    ui->setupUi(this);
}

CSreenEditorWidget::~CSreenEditorWidget()
{
    delete ui;
}
