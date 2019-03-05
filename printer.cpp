#include "printer.h"

void Printer::printPlotLine(QVector<sample> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));

    if (file.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream output(&file);

        output << "plot([";

        for (sample i : data)
            output << QString::number(i) << " ";

        output << "]);\r\n";
    }
}

void Printer::printPlotLine(QVector<float> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));

    if (file.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream output(&file);

        output << "plot([";

        for (float i : data)
            output << QString::number(i) << " ";

        output << "]);\r\n";
    }
}

void Printer::printPlot(QVector<float> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));
    bool exits = file.exists();

    QIODevice::OpenMode mode = exits ? QFile::WriteOnly | QFile::Append : QFile::WriteOnly | QFile::Truncate;

    if (file.open(mode)) {
        QTextStream output(&file);

        if (exits) {
            output.seek(file.size() - 3);
            output << ", ";
        }
        else output << "plot([";

        for (int i = 0; i < data.size(); i++) {
            output << QString::number(data[i]);

            if (i < data.size() - 1)
                output << " ";
        }
        output << "]);";
    }

    file.close();
}

void Printer::printPlot(QVector<sample> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));
    bool exits = file.exists();

    QIODevice::OpenMode mode = exits ? QFile::WriteOnly | QFile::Append : QFile::WriteOnly | QFile::Truncate;

    if (file.open(mode)) {
        QTextStream output(&file);

        if (exits) {
            output.seek(file.size() - 3);
            output << ", ";
        }
        else output << "plot([";

        for (int i = 0; i < data.size(); i++) {
            output << QString::number(data[i]);

            if (i < data.size() - 1)
                output << " ";
        }
        output << "]);";
    }

    file.close();
}

void Printer::print(QVector<float> data) {
    QTextStream out(stdout);

    for (int i = 0; i < data.size(); i++) {
        out << QString::number(data[i]);

        if (i < data.size() - 1)
            out << ", ";
    }

    out << endl;
}

void Printer::print(QVector<sample> data) {
    QTextStream out(stdout);

    for (int i = 0; i < data.size(); i++) {
        out << QString::number(data[i]);

        if (i < data.size() - 1)
            out << ", ";
    }

    out << endl;
}

void Printer::saveRawData(QVector<float> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));

    if (file.open(QFile::WriteOnly | QFile::Append)) {
        QDataStream output(&file);

        if (output.writeRawData((const char *)data.constData(), data.size() * sizeof(float)) < 0)
            QTextStream(stdout) << "Chyba při zapisování do raw souboru.";
    }
}

void Printer::saveRawData(QVector<sample> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));

    if (file.open(QFile::WriteOnly | QFile::Append)) {
        QDataStream output(&file);

        if (output.writeRawData((const char *)data.constData(), data.size() * sizeof(sample)) < 0)
            QTextStream(stdout) << "Chyba při zapisování do raw souboru.";
    }
}

void Printer::printToCsv(QVector<float> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));

    if (file.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream output(&file);

        for (int i = 0; i < data.size(); i++)
            output << QString::number(data[i]) << ";";

        output << "\n";
    }
}

void Printer::printToCsv(QVector<sample> data, QString fileName) {
    QFile file(QString("%1%2").arg(PLOT_DRIVE_LOCATION).arg(fileName));

    if (file.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream output(&file);

        for (int i = 0; i < data.size(); i++)
            output << QString::number(data[i]) << ";";

        output << "\n";
    }
}
