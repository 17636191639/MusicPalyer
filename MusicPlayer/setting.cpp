#include "setting.h"
#include "ui_setting.h"
#include <QDebug>

Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    m_musicFilePath = "D:/day day up/UNIGRESS/UnigressTrain/UnigressMusic";//默认音乐路径
}

Setting::~Setting()
{
    delete ui;
}
QString Setting::get_musicFilePath(void)
{
    //qDebug() << ui->le_musicfilepath->text();
    //return m_musicFilePath;
}


void Setting::on_le_musicfilepath_textChanged(const QString &arg1)
{
    //m_musicFilePath = arg1;
    //qDebug() << arg1;
}


void Setting::on_pb_saveFilePathSettings_clicked()
{
    m_musicFilePath = ui->le_musicfilepath->text();
    //qDebug() << m_musicFilePath;
    emit signalsaveFilePath(m_musicFilePath);
}
