#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "neo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fetchDataButton_clicked();
    void on_clusterButton_clicked();
    void on_searchClusterButton_clicked();
    void on_clearTableButton_clicked();
    void on_restoreTableButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<NEO> neos;
    std::vector<NEO> originalNeos;
};

#endif
