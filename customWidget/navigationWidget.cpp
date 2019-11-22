#include "navigationWidget.h"
#include "ui_navigationWidget.h"

navigationWidget::navigationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::navigationWidget)
{
    ui->setupUi(this);
    initWidget();
}

navigationWidget::~navigationWidget()
{
    delete ui;
}

void navigationWidget::initWidget()
{
    ui->firstTBn->setToolTip(QStringLiteral("首页"));
    ui->lastTBn->setToolTip(QStringLiteral("尾页"));
    ui->prevTBn->setToolTip(QStringLiteral("上一页"));
    ui->nextTBn->setToolTip(QStringLiteral("下一页"));

    ui->firstTBn->setIconSize(QSize(24, 24));
    ui->lastTBn->setIconSize(QSize(24, 24));
    ui->prevTBn->setIconSize(QSize(24, 24));
    ui->nextTBn->setIconSize(QSize(24, 24));

    ui->firstTBn->setIcon(QIcon("://first"));
    ui->lastTBn->setIcon(QIcon("://last"));
    ui->prevTBn->setIcon(QIcon("://prev"));
    ui->nextTBn->setIcon(QIcon("://next"));

    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    connect(mTimer, &QTimer::timeout, this, &navigationWidget::timerUpdate);

    connect(ui->prevTBn, &QToolButton::clicked, [this]{
        int value = ui->spinBox->value();
        value--;
        ui->spinBox->setValue(value);
    });
    connect(ui->nextTBn, &QToolButton::clicked, [this]{
        int value = ui->spinBox->value();
        value++;
        ui->spinBox->setValue(value);
    });
    connect(ui->firstTBn, &QToolButton::clicked, [this]{
        ui->spinBox->setValue(1);
    });
    connect(ui->lastTBn, &QToolButton::clicked, [this]{
        int max = ui->spinBox->maximum();
        ui->spinBox->setValue(max);
    });

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));

//    ui->spinBox->setReadOnly(true);
    ui->showSpinBox->setValue(30);

    searchNull();
}

void navigationWidget::searchNull()
{
    ui->spinBox->setMaximum(1);
    ui->label->setText(tr("总数：%1，共 %2 页").arg(0).arg(0));
    ui->firstTBn->setEnabled(false);
    ui->prevTBn->setEnabled(false);
    ui->lastTBn->setEnabled(false);
    ui->nextTBn->setEnabled(false);
    hide();
}

void navigationWidget::setNavWidget(QJsonObject dataObj)
{
    int totalElements = dataObj.value("totalElements").toInt(); //总数
    int totalPages = dataObj.value("totalPages").toInt(); //当前页个数
    ui->label->setText(tr("总数：%1，共 %2 页").arg(totalElements).arg(totalPages));
    ui->spinBox->setMaximum(totalPages);
    int number = dataObj.value("number").toInt() + 1; // 当前页
    ui->spinBox->setValue(number);
    bool first = dataObj.value("first").toBool(); //是否首页
    ui->firstTBn->setEnabled(!first);
    ui->prevTBn->setEnabled(!first);
    bool last = dataObj.value("last").toBool(); //是否尾页
    ui->lastTBn->setEnabled(!last);
    ui->nextTBn->setEnabled(!last);
    show();
}

void navigationWidget::onValueChanged(int value)
{
//    QString str = QString("page=%1&size=%2").arg(value - 1).arg(ui->showSpinBox->value());
//    emit searchChanged(str);
    emit pageChanged(value);
    mTimer->start(1000);
}

void navigationWidget::timerUpdate()
{
    qDebug() << "当前页：" << ui->spinBox->value();
    int value = ui->spinBox->value();
    QString str = QString("page=%1&size=%2").arg(value).arg(ui->showSpinBox->value());
    emit searchChanged(str);
}

int navigationWidget::pageSize()
{
    return ui->showSpinBox->value();
}

void navigationWidget::setShowSize(int value)
{
    ui->showSpinBox->setValue(value);
}

void navigationWidget::setButtonFixedSize(int size)
{
    ui->firstTBn->setFixedSize(size, size);
    ui->lastTBn->setFixedSize(size, size);
    ui->prevTBn->setFixedSize(size, size);
    ui->nextTBn->setFixedSize(size, size);
}

int navigationWidget::pageNum()
{
    return ui->spinBox->value();
}

void navigationWidget::setNavWidget2(QJsonObject dataObj)
{
    int total = dataObj.value("total").toInt(); //总个数
//    int size = dataObj.value("size").toInt(); // 每页个数
    int current = dataObj.value("current").toInt(); //当前页码
    int pages = dataObj.value("pages").toInt(); //总页数

    ui->label->setText(tr("总数：%1，共 %2 页").arg(total).arg(pages));
    ui->spinBox->setMaximum(pages);
    ui->spinBox->setValue(current);

    bool first = (current == 1); //是否首页
    ui->firstTBn->setEnabled(!first);
    ui->prevTBn->setEnabled(!first);
    bool last = (current == pages); //是否尾页
    ui->lastTBn->setEnabled(!last);
    ui->nextTBn->setEnabled(!last);
    show();
}
