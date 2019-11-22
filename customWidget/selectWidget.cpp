#include "selectWidget.h"
#include "ui_selectWidget.h"

selectWidget::selectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectWidget)
{
    ui->setupUi(this);
    connect(ui->allCBox, &QCheckBox::clicked, [this](bool checked){
        checked ? selectAllChecked() : clearAllChecked();
    });
    connect(ui->reverseCBox, &QCheckBox::toggled, [this]{
        reverseAllChecked();
    });
    connect(ui->cancelSelectBtn, &QPushButton::clicked, [this]{
        clearAllChecked();
    });
}

selectWidget::~selectWidget()
{
    delete ui;
}

void selectWidget::setTableWidget(QTableWidget *tableWidget)
{
    m_tableWidget = tableWidget;
}

/**
  * @brief MainWindow::clearAllChecked 取消所有选中
 */
void selectWidget::clearAllChecked()
{
    if(!m_tableWidget)
        return;
    for(int i = 0; i < m_tableWidget->rowCount(); i++)
        m_tableWidget->item(i, 0)->setCheckState(Qt::Unchecked);
    emit checkedNumber(0);
}

/**
  * @brief MainWindow::selectAllChecked 全选
 */
void selectWidget::selectAllChecked()
{
    if(!m_tableWidget)
        return;
    for(int i = 0; i < m_tableWidget->rowCount(); i++)
        m_tableWidget->item(i, 0)->setCheckState(Qt::Checked);
    emit checkedNumber(m_tableWidget->rowCount());
}

/**
  * @brief MainWindow::reverseAllChecked 反选
 */
void selectWidget::reverseAllChecked()
{
    if(!m_tableWidget)
        return;
    int k = 0;
    for(int i = 0; i < m_tableWidget->rowCount(); i++) {
        Qt::CheckState state = m_tableWidget->item(i, 0)->checkState();
        state == Qt::Checked ? m_tableWidget->item(i, 0)->setCheckState(Qt::Unchecked)
                             : m_tableWidget->item(i, 0)->setCheckState(Qt::Checked);
        if(state == Qt::Unchecked)
            k++;
    }
    emit checkedNumber(k);
}

void selectWidget::setUnCheck()
{
    ui->allCBox->setChecked(false);
    ui->reverseCBox->setChecked(false);
}
