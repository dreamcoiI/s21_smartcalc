#include "mainwindow.h"
#include "ui_mainwindow.h"
extern "C" {
#include "stack.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_1_clicked()
{

   ui->display->insert("1");
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->display->insert("2");
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->display->insert("3");
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->display->insert("4");
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->display->insert("5");
}


void MainWindow::on_pushButton_6_clicked()
{
    ui->display->insert("6");
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->display->insert("7");
}


void MainWindow::on_pushButton_8_clicked()
{
    ui->display->insert("8");
}


void MainWindow::on_pushButton_9_clicked()
{
    ui->display->insert("9");
}


void MainWindow::on_pushButton_0_clicked()
{
    ui->display->insert("0");
}


void MainWindow::on_pushButton_dot_clicked()
{
    ui->display->insert(".");
}

void MainWindow::on_pushButton_all_clean_clicked()
{
     ui->display->clear();
     x.clear();
     y.clear();
     ui->widget->clearGraphs();
}


void MainWindow::on_pushButton_acos_clicked()
{
    ui->display->insert("acos(");
}


void MainWindow::on_pushButton_add_clicked()
{
    ui->display->insert("+");
}


void MainWindow::on_pushButton_asin_clicked()
{
    ui->display->insert("asin(");
}


void MainWindow::on_pushButton_atan_clicked()
{
    ui->display->insert("atan(");
}


void MainWindow::on_pushButton_bracket_op_clicked()
{
    ui->display->insert("(");
}


void MainWindow::on_pushButton_bracket_closed_clicked()
{
    ui->display->insert(")");
}


void MainWindow::on_pushButton_cos_clicked()
{
    ui->display->insert("cos(");
}


void MainWindow::on_pushButton_div_clicked()
{
    ui->display->insert("/");
}


void MainWindow::on_pushButton_ln_clicked()
{
    ui->display->insert("ln(");
}


void MainWindow::on_pushButton_log_clicked()
{
    ui->display->insert("log(");
}


void MainWindow::on_pushButton_mult_clicked()
{
    ui->display->insert("*");
}


void MainWindow::on_pushButton_pow_clicked()
{
    ui->display->insert("^");
}


void MainWindow::on_pushButton_sin_clicked()
{
    ui->display->insert("sin(");
}


void MainWindow::on_pushButton_sqrt_clicked()
{
    ui->display->insert("sqrt(");
}


void MainWindow::on_pushButton_sub_clicked()
{
    ui->display->insert("-");
}


void MainWindow::on_pushButton_tan_clicked()
{
    ui->display->insert("tan(");
}


void MainWindow::on_pushButton_x_clicked()
{
    ui->display->insert("x");
}



void MainWindow::on_pushButton_equal_clicked()
{
    double result = 0.0;
    std::string text=ui->display->text().toUtf8().constData();
    str = text.c_str();
    int err_code = 0;
    err_code = calcsmart(str,&result, x_flag);
    if (err_code == 0) {
        sprintf((char*)str, "%.7lf", result);
        ui->display->setText(str);

    } else {
        ui->display->setText("ERROR");
    }
}


void MainWindow::on_Graph_button_clicked()
{
    std::string text1 = ui->display->text().toUtf8().constData();
    str = text1.c_str();
    double Y;

    x.clear();
    y.clear();
    ui->widget->clearGraphs();
    ui->widget->replot();
    h = 0.5;

    double xmax = ui->X_max_edit->text().toDouble();
    double xmin = ui->X_min_edit->text().toDouble();
    double ymax = ui->Y_max_edit->text().toDouble();
    double ymin = ui->Y_min_edit->text().toDouble();
    xBegin =xmin;
    xEnd = xmax+h;

    if (xmin > xmax || ymin > ymax) {
        if (xmin > xmax) ui->X_min_edit->setText("ERROR");
        if (ymin > ymax) ui->Y_min_edit->setText("ERROR");
    } else if (xmax > 1000000) {
        ui->X_max_edit->setText("ERROR");
    } else if (xmin < -1000000) {
        ui->X_min_edit->setText("ERROR");
    } else if (ymax > 1000000) {
        ui->Y_max_edit->setText("ERROR");
    } else if (xmin < -1000000) {
        ui->Y_min_edit->setText("ERROR");
    } else {
        ui-> widget->xAxis->setRange(xmin,xmax);
        ui-> widget->yAxis->setRange(ymin,ymax);

        if(((xmax-xmin) + (ymax-ymin)) >= 100) h*=4;
        if(((xmax-xmin) + (ymax-ymin)) >= 1000) h*=2;
        if(((xmax-xmin) + (ymax-ymin)) >= 10000) h*=4;
        if(((xmax-xmin) + (ymax-ymin)) >= 100000) h*=5;
        for (X = xBegin; X <= xEnd; X +=h) {
            if (!calcsmart(str, &Y, X)) {
                x.push_back(X);
                y.push_back(Y);
            }
        }
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x, y);
        ui->widget->replot();
    }

}


