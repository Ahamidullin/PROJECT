#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->searchClusterButton, &QPushButton::clicked, this, &MainWindow::on_searchClusterButton_clicked);
    connect(ui->clearTableButton, &QPushButton::clicked, this, &MainWindow::on_clearTableButton_clicked);
    connect(ui->restoreTableButton, &QPushButton::clicked, this, &MainWindow::on_restoreTableButton_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_fetchDataButton_clicked() {
    bool ok;
    QString apiKey = QInputDialog::getText(this, tr("Enter API Key"),
                                           tr("API Key:"), QLineEdit::Normal,
                                           "", &ok);
    if (ok && !apiKey.isEmpty()) {
        neos = fetch_neo_data(apiKey.toStdString());
        originalNeos = neos;
        if (neos.empty()) {
            QMessageBox::warning(this, tr("Error"), tr("No NEO data available"));
        } else {
            QMessageBox::information(this, tr("Success"), tr("Data fetched successfully"));
        }
    }
}

void MainWindow::on_clusterButton_clicked() {
    if (neos.empty()) {
        QMessageBox::warning(this, tr("Error"), tr("No NEO data to cluster"));
        return;
    }

    bool ok;
    int k = QInputDialog::getInt(this, tr("Enter K value"),
                                 tr("K:"), 5, 1, 100, 1, &ok);
    if (ok) {
        int iterations = 200;
        k_means_clustering(neos, k, iterations);

        originalNeos = neos;

        ui->tableWidget->setColumnCount(9);
        ui->tableWidget->setRowCount(neos.size());
        QStringList headers;
        headers << "Cluster ID" << "Name" << "Diameter" << "Distance" << "Speed"
                << "Absolute Magnitude" << "Close Approach Date" << "Orbiting Body"
                << "Miss Distance (AU)";
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        int row = 0;
        for (const auto &neo : neos) {
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(neo.cluster_id)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(neo.name)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(neo.diameter)));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(neo.distance)));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(neo.speed)));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(neo.absolute_magnitude)));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(neo.close_approach_date)));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(neo.orbiting_body)));
            ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(neo.miss_distance_astronomical)));
            row++;
        }

        QMessageBox::information(this, tr("Clustering Result"), tr("Clustering completed successfully."));
    }
}

void MainWindow::on_searchClusterButton_clicked() {
    bool ok;
    int clusterId = ui->clusterIdInput->text().toInt(&ok);
    if (ok) {
        std::vector<NEO> filteredNeos;
        for (const auto &neo : neos) {
            if (neo.cluster_id == clusterId) {
                filteredNeos.push_back(neo);
            }
        }

        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(filteredNeos.size());
        int row = 0;
        for (const auto &neo : filteredNeos) {
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(neo.cluster_id)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(neo.name)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(neo.diameter)));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(neo.distance)));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(neo.speed)));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(neo.absolute_magnitude)));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(neo.close_approach_date)));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(neo.orbiting_body)));
            ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(neo.miss_distance_astronomical)));
            row++;
        }

        QMessageBox::information(this, tr("Search Result"), tr("Filtered by cluster ID successfully."));
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Invalid cluster ID"));
    }
}

void MainWindow::on_clearTableButton_clicked() {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void MainWindow::on_restoreTableButton_clicked() {
    neos = originalNeos;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(neos.size());
    QStringList headers;
    headers << "Cluster ID" << "Name" << "Diameter" << "Distance" << "Speed"
            << "Absolute Magnitude" << "Close Approach Date" << "Orbiting Body"
            << "Miss Distance (AU)";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const auto &neo : neos) {
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(neo.cluster_id)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(neo.name)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(neo.diameter)));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(neo.distance)));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(neo.speed)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(neo.absolute_magnitude)));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(neo.close_approach_date)));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(neo.orbiting_body)));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(neo.miss_distance_astronomical)));
        row++;
    }
}
