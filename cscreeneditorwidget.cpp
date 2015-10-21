#include "cscreeneditorwidget.h"
#include "ui_cscreeneditorwidget.h"

CScreenEditorWidget::CScreenEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CScreenEditorWidget)
{
    ui->setupUi(this);
}

CScreenEditorWidget::~CScreenEditorWidget()
{
    delete ui;
}
