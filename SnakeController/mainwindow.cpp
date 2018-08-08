#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>

#include <QPushButton>

#include <QLabel>
#include <QTextEdit>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_ports = QSerialPortInfo::availablePorts();

    for (int i = 0; i < m_ports.size(); ++i)
    {
        ui->comboBox->addItem(m_ports[i].description(), i);
    }

    ui->speedLabel->setText(QString::number(m_speed));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    m_port.close();
    std::cout << index << std::endl;
    int port = ui->comboBox->itemData(index).toInt();
    std::cout << port << std::endl;
    m_port.setPort(m_ports.at(port));
    m_port.open(QSerialPort::ReadWrite);
    m_port.setBaudRate(9600);

    connect(&m_port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::UpArrow:
    case Qt::Key_W:
        ui->up->click();
        break;
    case Qt::DownArrow:
    case Qt::Key_S:
        ui->down->click();
        break;
    case Qt::RightArrow:
    case Qt::Key_D:
        ui->right->click();
        break;
    case Qt::LeftArrow:
    case Qt::Key_A:
        ui->left->click();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::on_up_clicked()
{
    char send = 1;
    m_port.write(&send);
}

void MainWindow::on_down_clicked()
{
    char send = 2;
    m_port.write(&send);
}

void MainWindow::on_right_clicked()
{
    char send = 4;
    m_port.write(&send);
}

void MainWindow::on_left_clicked()
{
    char send = 3;
    m_port.write(&send);
}

void MainWindow::setSpeed()
{
    int i = 2000 - m_speed;

    char send = 5;
    m_port.write(&send, 1);

    char send2[4];
    for (int j = 0; j < 4; j++)
    {
        send2[j] = (i >> (8 * j));
    }
    m_port.write(send2, 4);

    std::cout << (unsigned int)send << std::endl;

    ui->speedLabel->setText(QString::number(m_speed));
}

void MainWindow::on_speed_valueChanged(int value)
{
    m_speed = value;
    setSpeed();
}

void MainWindow::on_slower_clicked()
{
    m_speed -= 20;
    ui->speed->setValue(m_speed);
}

void MainWindow::on_faster_clicked()
{
    m_speed += 20;
    ui->speed->setValue(m_speed);
}

void MainWindow::onReadyRead()
{
    ui->textEdit->insertPlainText(m_port.readAll());
}
