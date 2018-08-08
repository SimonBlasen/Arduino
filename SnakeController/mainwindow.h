#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_comboBox_currentIndexChanged(int index);


    void on_up_clicked();

    void on_down_clicked();

    void on_right_clicked();

    void on_left_clicked();

    void on_speed_valueChanged(int value);

    void on_slower_clicked();

    void on_faster_clicked();

    void onReadyRead();

private:
    void setSpeed();

private:
    Ui::MainWindow *ui;

    QList<QSerialPortInfo> m_ports;
    QSerialPort m_port;

    int m_speed = 1800;
};

#endif // MAINWINDOW_H
