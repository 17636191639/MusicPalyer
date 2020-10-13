#ifndef SETTING_H
#define SETTING_H

#include <QWidget>

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT
signals:
    void signalsaveFilePath(QString filePath);

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

    QString get_musicFilePath(void);

private slots:
    void on_le_musicfilepath_textChanged(const QString &arg1);


    void on_pb_saveFilePathSettings_clicked();

private:
    Ui::Setting *ui;
    QString m_musicFilePath;

};

#endif // SETTING_H
