#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();
signals:
    void startGameSignal();
    void gameModeSelectSignal(int index);
    void firstPlayerSelectSignal(int index);
    void humanTimeEditSignal(const QString &arg1);
    void botTimeEditSignal(const QString &arg1);
private slots:
    void on_OKPushButton_clicked();


    void on_GameModeSelect_currentIndexChanged(int index);

    void on_FirstPlayerSelect_currentIndexChanged(int index);

    void on_HumanTimeEdit_textChanged(const QString &arg1);

    void on_BotTimeEdit_textChanged(const QString &arg1);

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
