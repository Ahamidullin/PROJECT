#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include "neo.h"
#include "fetch_neo_data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fetchDataButton_clicked();
    void on_runClusteringButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<NEO> neos;

    void updateNEOTable();
    void updateClusterTable(const std::vector<int>& labels, int k);
    void updateChart(const std::vector<int>& labels, int k);
    void kMeansClustering(std::vector<NEO>& neos, int k, int iterations);
};

#endif // MAINWINDOW_H
