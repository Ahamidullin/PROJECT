#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->fetchDataButton, &QPushButton::clicked, this, &MainWindow::on_fetchDataButton_clicked);
    connect(ui->runClusteringButton, &QPushButton::clicked, this, &MainWindow::on_runClusteringButton_clicked);

    // Setup NEO Table
    ui->neoTable->setColumnCount(3);
    QStringList neoTableHeaders;
    neoTableHeaders << "Diameter" << "Distance" << "Speed";
    ui->neoTable->setHorizontalHeaderLabels(neoTableHeaders);

    // Setup Cluster Table
    ui->clusterTable->setColumnCount(4);
    QStringList clusterTableHeaders;
    clusterTableHeaders << "Cluster" << "Diameter" << "Distance" << "Speed";
    ui->clusterTable->setHorizontalHeaderLabels(clusterTableHeaders);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fetchDataButton_clicked() {
    QString apiKey = ui->apiKeyInput->text();
    neos = fetchNEOData(apiKey);

    // Debug statement
    qDebug() << "Fetched NEOs: " << neos.size();

    if (!neos.empty()) {
        updateNEOTable();
        ui->statusOutput->append("Data fetched successfully.");
    } else {
        ui->statusOutput->append("Failed to fetch data.");
    }
}

void MainWindow::on_runClusteringButton_clicked() {
    bool ok1, ok2;
    int k = ui->clustersInput->text().toInt(&ok1);
    int iterations = ui->iterationsInput->text().toInt(&ok2);

    if (ok1 && ok2 && k > 0 && iterations > 0) {
        kMeansClustering(neos, k, iterations);
        ui->statusOutput->append("Clustering completed.");
    } else {
        ui->statusOutput->append("Invalid input for clusters or iterations.");
    }
}

void MainWindow::updateNEOTable() {
    ui->neoTable->setRowCount(neos.size());
    for (size_t i = 0; i < neos.size(); ++i) {
        ui->neoTable->setItem(i, 0, new QTableWidgetItem(QString::number(neos[i].diameter)));
        ui->neoTable->setItem(i, 1, new QTableWidgetItem(QString::number(neos[i].distance)));
        ui->neoTable->setItem(i, 2, new QTableWidgetItem(QString::number(neos[i].speed)));

        // Debug statement
        qDebug() << "NEO Table Row:" << neos[i].diameter << neos[i].distance << neos[i].speed;
    }
}

void MainWindow::updateClusterTable(const std::vector<int>& labels, int k) {
    ui->clusterTable->setRowCount(0);
    for (int i = 0; i < k; ++i) {
        for (size_t j = 0; j < labels.size(); ++j) {
            if (labels[j] == i) {
                int row = ui->clusterTable->rowCount();
                ui->clusterTable->insertRow(row);
                ui->clusterTable->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
                ui->clusterTable->setItem(row, 1, new QTableWidgetItem(QString::number(neos[j].diameter)));
                ui->clusterTable->setItem(row, 2, new QTableWidgetItem(QString::number(neos[j].distance)));
                ui->clusterTable->setItem(row, 3, new QTableWidgetItem(QString::number(neos[j].speed)));

                // Debug statement
                qDebug() << "Cluster Table Row:" << i << neos[j].diameter << neos[j].distance << neos[j].speed;
            }
        }
    }

    // Update the chart with the clustering results
    updateChart(labels, k);
}

void MainWindow::updateChart(const std::vector<int>& labels, int k) {
    QChart *chart = new QChart();
    QVector<QScatterSeries*> series(k);

    for (int i = 0; i < k; ++i) {
        series[i] = new QScatterSeries();
        series[i]->setName("Cluster " + QString::number(i + 1));
        series[i]->setMarkerSize(10.0);
    }

    for (size_t i = 0; i < neos.size(); ++i) {
        int cluster = labels[i];
        series[cluster]->append(neos[i].distance, neos[i].diameter);
    }

    for (int i = 0; i < k; ++i) {
        chart->addSeries(series[i]);
    }

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Distance");
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Diameter");
    chart->addAxis(axisY, Qt::AlignLeft);

    for (int i = 0; i < k; ++i) {
        series[i]->attachAxis(axisX);
        series[i]->attachAxis(axisY);
    }

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    ui->chartView->setChart(chart);
}

void MainWindow::kMeansClustering(std::vector<NEO>& neos, int k, int iterations) {
    int n = neos.size();
    std::vector<NEO> centroids(k);
    std::vector<int> labels(n);

    // Initialize centroids (implementation depends on your initialization strategy)

    for (int iter = 0; iter < iterations; ++iter) {
        // Assign each NEO to the nearest centroid (implementation depends on your distance metric)

        // Update centroids
        std::vector<NEO> new_centroids(k, {0, 0, 0});
        std::vector<int> counts(k, 0);

        for (int i = 0; i < n; ++i) {
            int label = labels[i];
            new_centroids[label].diameter += neos[i].diameter;
            new_centroids[label].distance += neos[i].distance;
            new_centroids[label].speed += neos[i].speed;
            counts[label] += 1;
        }

        for (int j = 0; j < k; ++j) {
            if (counts[j] != 0) {
                new_centroids[j].diameter /= counts[j];
                new_centroids[j].distance /= counts[j];
                new_centroids[j].speed /= counts[j];
            }
        }

        centroids = new_centroids;
    }

    // Update the UI with the clustering results
    updateClusterTable(labels, k);
}
