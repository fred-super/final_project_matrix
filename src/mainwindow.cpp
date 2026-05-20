#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../include/type_detector.h"
#include <QStyle>
#include <QComboBox>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBoxSize->setMinimum(1);
    ui->spinBoxSize->setMaximum(500);

    comboBackend = new QComboBox(this);

    comboBackend->addItem(
        "Собственный метод Гаусса",
        static_cast<int>(matrix::CalculationBackend::OwnGauss)
    );
    
    #ifdef USE_EIGEN
    comboBackend->addItem(
        "Eigen / открытая библиотека",
        static_cast<int>(matrix::CalculationBackend::Eigen)
    );
    #else
    comboBackend->setToolTip("Eigen недоступен. Соберите проект с флагом -DUSE_EIGEN=ON.");
    #endif
    
    ui->verticalLayout->insertWidget(1, comboBackend);

    ui->btnImport->setText("Загрузить");
    ui->btnImport->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    
    ui->btnExport->setText("Сохранить");
    ui->btnExport->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    
    ui->btnSolve->setText("Решить СЛАУ");
    ui->btnSolve->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    on_spinBoxSize_valueChanged(ui->spinBoxSize->value()); 
}

MainWindow::~MainWindow()
{
    delete ui;
}

matrix::CalculationBackend MainWindow::selectedBackend() const
{
#ifdef USE_EIGEN
    if (
        comboBackend &&
        comboBackend->currentData().toInt() ==
            static_cast<int>(matrix::CalculationBackend::Eigen)
    )
    {
        return matrix::CalculationBackend::Eigen;
    }
#endif

    return matrix::CalculationBackend::OwnGauss;
}


void MainWindow::on_btnCalculate_clicked()
{
    int size = ui->tableWidget->rowCount();
    matrix::DataType final_type = matrix::DataType::Int; 

    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            QString text = item->text().trimmed();
            
            if (text.isEmpty()) 
            {
                QMessageBox::warning(this, "Внимание", "Пожалуйста, заполните все ячейки матрицы!");
                return;
            }

            matrix::DataType cell_type = matrix::TypeDetector::detectSingleValue(text.toStdString());

            if (cell_type == matrix::DataType::Invalid) 
            {
                QMessageBox::critical(this, "Ошибка ввода", 
                    QString("Некорректное значение в ячейке [%1, %2]: '%3'")
                    .arg(i + 1).arg(j + 1).arg(text));
                return;
            }

            final_type = matrix::TypeDetector::promoteTypes(final_type, cell_type);
        }
    }

    try 
    {
        QString result_text = "Определитель: ";

        switch (final_type) 
        {
            case matrix::DataType::Int:
            {
                matrix::Matrix<int> mat(size, size);
            
                for (int i = 0; i < size; ++i)
                {
                    for (int j = 0; j < size; ++j)
                    {
                        mat.at(i, j) = ui->tableWidget->item(i, j)->text().toInt();
                    }
                }
            
                QElapsedTimer timer;
                timer.start();
            
                auto det = mat.determinant(selectedBackend());
            
                qint64 elapsedNs = timer.nsecsElapsed();
                double elapsedMs = static_cast<double>(elapsedNs) / 1000000.0;
            
                result_text += QString::number(det) + "  (Тип: int)";
                result_text += "  | Метод: " + comboBackend->currentText();
                result_text += "  | Время: " + QString::number(elapsedMs, 'f', 3) + " мс";
            
                break;
            }

            case matrix::DataType::Double:
            {
                matrix::Matrix<double> mat(size, size);
            
                for (int i = 0; i < size; ++i)
                {
                    for (int j = 0; j < size; ++j)
                    {
                        mat.at(i, j) = ui->tableWidget->item(i, j)->text().toDouble();
                    }
                }
            
                QElapsedTimer timer;
                timer.start();
            
                auto det = mat.determinant(selectedBackend());
            
                qint64 elapsedNs = timer.nsecsElapsed();
                double elapsedMs = static_cast<double>(elapsedNs) / 1000000.0;
            
                result_text += QString::number(det, 'g', 10) + "  (Тип: double)";
                result_text += "  | Метод: " + comboBackend->currentText();
                result_text += "  | Время: " + QString::number(elapsedMs, 'f', 3) + " мс";
            
                break;
            }

            case matrix::DataType::LongDouble:
            {
                matrix::Matrix<long double> mat(size, size);
            
                for (int i = 0; i < size; ++i)
                {
                    for (int j = 0; j < size; ++j)
                    {
                        mat.at(i, j) = std::stold(ui->tableWidget->item(i, j)->text().toStdString());
                    }
                }
            
                QElapsedTimer timer;
                timer.start();
            
                auto det = mat.determinant(selectedBackend());
            
                qint64 elapsedNs = timer.nsecsElapsed();
                double elapsedMs = static_cast<double>(elapsedNs) / 1000000.0;
            
                result_text += QString::fromStdString(std::to_string(det)) + "  (Тип: long double)";
                result_text += "  | Метод: " + comboBackend->currentText();
                result_text += "  | Время: " + QString::number(elapsedMs, 'f', 3) + " мс";
            
                break;
            }

            default:
                throw std::runtime_error("Неизвестный тип данных");
        }

        ui->labelResult->setText(result_text);
        lastDeterminantResult = result_text;

    } 
    catch (const std::exception& e) 
    {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::on_btnImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text Files (*.txt);;CSV Files (*.csv)");
    if (fileName.isEmpty()) return;

    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) 
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    std::vector<std::vector<std::string>> data;
    std::string line;
    
    while (std::getline(file, line))
    {
        size_t firstSymbol = line.find_first_not_of(" \t\r\n");

        if (firstSymbol == std::string::npos)
        {
            continue;
        }

        if (line[firstSymbol] == '#')
        {
            continue;
        }

        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream ss(line);
        std::string val;
        std::vector<std::string> row;
        while (ss >> val) 
        {
            row.push_back(val);
        }
        if (!row.empty()) data.push_back(row);
    }

    if (data.empty()) return;

    size_t rows = data.size();
    size_t cols = data[0].size();

    for (const auto& row : data) 
    {
        if (row.size() != cols) 
        {
            QMessageBox::warning(this, "Ошибка", "Неровная матрица! Строки имеют разное количество элементов.");
            return;
        }
    }

    if (rows > static_cast<size_t>(ui->spinBoxSize->maximum()))
    {
        ui->spinBoxSize->setMaximum(static_cast<int>(rows));
    }
    
    auto setCellText = [](QTableWidget *table, int row, int col, const QString &text)
    {
        if (!table->item(row, col))
        {
            table->setItem(row, col, new QTableWidgetItem);
        }
    
        table->item(row, col)->setText(text);
    };

    if (cols == rows) 
    {
        ui->spinBoxSize->setValue(rows); 
        
        for (size_t i = 0; i < rows; ++i) 
        {
            for (size_t j = 0; j < cols; ++j) 
            {
                setCellText(ui->tableWidget, static_cast<int>(i), static_cast<int>(j), QString::fromStdString(data[i][j]));
            }
            setCellText(ui->tableVectorB, static_cast<int>(i), 0, "0");
        }
        QMessageBox::information(this, "Импорт", "Распознана квадратная матрица (Определитель).");

    } 
    else if (cols == rows + 1) 
    {
        ui->spinBoxSize->setValue(rows);
        
        for (size_t i = 0; i < rows; ++i) 
        {
            for (size_t j = 0; j < rows; ++j) 
            {
                setCellText(ui->tableWidget, static_cast<int>(i), static_cast<int>(j), QString::fromStdString(data[i][j]));
            }
            setCellText(ui->tableVectorB, static_cast<int>(i), 0, QString::fromStdString(data[i][rows]));
        }
        QMessageBox::information(this, "Импорт", "Распознана расширенная матрица (СЛАУ).\nПоследний столбец загружен как вектор B.");
        
    } else 
    {
        QMessageBox::warning(this, "Ошибка размерности", 
            QString("Неподдерживаемый формат: %1 строк и %2 столбцов.\n"
                    "Ожидается либо N x N (Определитель), либо N x (N+1) (СЛАУ).").arg(rows).arg(cols));
    }
}

void MainWindow::on_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить данные",
        "",
        "Text Files (*.txt);;CSV Files (*.csv)"
    );

    if (fileName.isEmpty()) return;

    std::ofstream file(fileName.toStdString());

    if (!file.is_open())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        return;
    }

    int size = ui->tableWidget->rowCount();
    bool isCsv = fileName.endsWith(".csv", Qt::CaseInsensitive);
    std::string separator = isCsv ? "," : " ";

    file << "# Matrix A\n";

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            QString text = (item && !item->text().trimmed().isEmpty())
                               ? item->text()
                               : "0";

            file << text.toStdString();

            if (j < size - 1)
            {
                file << separator;
            }
        }

        file << "\n";
    }

    file << "\n# Vector B\n";

    for (int i = 0; i < size; ++i)
    {
        QTableWidgetItem *item = ui->tableVectorB->item(i, 0);
        QString text = (item && !item->text().trimmed().isEmpty())
                           ? item->text()
                           : "0";

        file << text.toStdString() << "\n";
    }

    file << "\n# Results\n";

    bool hasResults = false;

    if (!lastDeterminantResult.isEmpty())
    {
        file << "# " << lastDeterminantResult.toStdString() << "\n";
        hasResults = true;
    }

    if (!lastSlaeResult.isEmpty())
    {
        file << "# " << lastSlaeResult.toStdString() << "\n";
        hasResults = true;
    }

    if (!hasResults)
    {
        file << "# Результаты еще не вычислялись\n";
    }

    QMessageBox::information(this, "Сохранение", "Данные успешно сохранены в файл.");
}

void MainWindow::on_spinBoxSize_valueChanged(int size)
{
    ui->tableWidget->setRowCount(size);
    ui->tableWidget->setColumnCount(size);
    
    ui->tableVectorB->setRowCount(size);
    ui->tableVectorB->setColumnCount(1);
    ui->tableVectorB->setHorizontalHeaderLabels({"B"});

    auto initTable = [](QTableWidget* t) 
    {
        for(int i = 0; i < t->rowCount(); ++i)
            for(int j = 0; j < t->columnCount(); ++j)
                if(!t->item(i, j)) t->setItem(i, j, new QTableWidgetItem("0"));
    };
    initTable(ui->tableWidget);
    initTable(ui->tableVectorB);
}

void MainWindow::on_btnSolve_clicked()
{
    int n = ui->tableWidget->rowCount();
    try 
    {
        matrix::Matrix<double> A(n, n);
        std::vector<double> B(n);

        for(int i = 0; i < n; ++i) 
        {
            B[i] = ui->tableVectorB->item(i, 0)->text().toDouble();
            for(int j = 0; j < n; ++j) 
            {
                A.at(i, j) = ui->tableWidget->item(i, j)->text().toDouble();
            }
        }

        std::vector<double> x = A.solve(B, selectedBackend());

        QString res = "Решение x: (";
        for(size_t i = 0; i < x.size(); ++i) 
        {
            res += QString::number(x[i], 'g', 6) + (i == x.size()-1 ? "" : "; ");
        }
        res += ")";
        res += "  | Метод: " + comboBackend->currentText();
        ui->labelResult->setText(res);
        ui->labelResult->setStyleSheet("color: #00FF00;");
        lastSlaeResult = res;
    } 
    catch (const std::exception& e) 
    {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}