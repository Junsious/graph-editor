#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QPainter>
#include <QVBoxLayout>
#include <QMessageBox>
#include <cmath>

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(600, 400);
    }

    void drawGraph(double a, double b, double c) {
        this->a = a;
        this->b = b;
        this->c = c;
        update(); // Перерисовать виджет
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event); // Вызов базового класса
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Отрисовка сетки
        painter.setPen(Qt::lightGray); // Устанавливаем цвет линий сетки
        int gridSpacing = 20; // Расстояние между клетками

        // Вертикальные линии
        for (int x = 0; x < width(); x += gridSpacing) {
            painter.drawLine(x, 0, x, height());
        }

        // Горизонтальные линии
        for (int y = 0; y < height(); y += gridSpacing) {
            painter.drawLine(0, y, width(), y);
        }

        // Оси координат
        painter.setPen(Qt::blue);
        painter.drawLine(0, height() / 2, width(), height() / 2); // OX
        painter.drawLine(width() / 2, 0, width() / 2, height()); // OY

        // Метки на осях
        painter.setPen(Qt::black);
        for (int i = -width() / 2; i <= width() / 2; i += gridSpacing) {
            if (i != 0) {
                painter.drawText(width() / 2 + i, height() / 2 + 15, QString::number(i / 20.0));
            }
        }

        for (int j = -height() / 2; j <= height() / 2; j += gridSpacing) {
            if (j != 0) {
                painter.drawText(width() / 2 + 5, height() / 2 - j, QString::number(j / 20.0));
            }
        }

        // Рисуем график
        painter.setPen(Qt::red);

        // Увеличим количество проложенных точек для создания плавного графика
        QPolygonF graphPoints;
        double previousY = 0.0;

        for (int x = -width() / 2; x < width() / 2; ++x) {
            double xScaled = x / 20.0; // Масштабируем x
            double y = a * std::pow(xScaled, 2) + b * xScaled + c; // Вычисляем y

            graphPoints.append(QPointF(width() / 2 + x, height() / 2 - (y * 20))); // Добавляем точку

            // Если это не первая точка, соединяем её с предыдущей
            if (graphPoints.size() > 1) {
                painter.drawLine(graphPoints[graphPoints.size() - 2], graphPoints[graphPoints.size() - 1]);
            }
        }
    }

private:
    double a = 0;
    double b = 0;
    double c = 0;
};

class InputFieldExample : public QWidget {
    Q_OBJECT
public:
    InputFieldExample() {
        setWindowTitle("Graphing");

        QVBoxLayout *vLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        // Поля ввода
        inputFieldA = new QLineEdit(this);
        formLayout->addRow("A:", inputFieldA);

        inputFieldB = new QLineEdit(this);
        formLayout->addRow("B:", inputFieldB);

        inputFieldC = new QLineEdit(this);
        formLayout->addRow("C:", inputFieldC);

        QPushButton *drawButton = new QPushButton("Draw Graph", this);
        connect(drawButton, &QPushButton::clicked, this, &InputFieldExample::drawGraph);

        graphWidget = new GraphWidget(this);

        vLayout->addLayout(formLayout);
        vLayout->addWidget(drawButton);
        vLayout->addWidget(graphWidget);

        setLayout(vLayout);
    }

private slots:
    void drawGraph() {
        bool okA, okB, okC;
        double a = inputFieldA->text().toDouble(&okA);
        double b = inputFieldB->text().toDouble(&okB);
        double c = inputFieldC->text().toDouble(&okC);

        if (okA && okB && okC) {
            graphWidget->drawGraph(a, b, c);
        } else {
            QMessageBox::warning(this, "Input Error", "Please enter valid numbers for A, B, and C.");
        }
    }

private:
    QLineEdit *inputFieldA;
    QLineEdit *inputFieldB;
    QLineEdit *inputFieldC;
    GraphWidget *graphWidget;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    InputFieldExample window;
    window.resize(600, 700);
    window.show();

    return app.exec();
}

#include "main.moc" // Не забудьте добавить этот include в конец вашего файла
