#include "taskswindow.h"
#include "basewindow.h"
#include "ui_taskswindow.h"
#include "descriptionwindow.h"
#include "edittaskwindow.h"
#include "taskitemwidget.h"
#include <QInputDialog>
#include <QSettings>
#include <QMessageBox>

TasksWindow::TasksWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TasksWindow)
    , descriptionWindow(nullptr)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow { background-image: url(:/img/images/background_start.png); background-position: center; background-repeat: no-repeat; }");
    ui->title->setStyleSheet("font-size: 50px;color:white");

    QString buttonStyle = "QPushButton {"
                          "color: white;"
                          "font-size: 24px;"
                          "background-color: darkred;"
                          "border-radius: 10px;"
                          "padding: 10px 20px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #D94343;"
                          "}"
                          "QPushButton:pressed {"
                          "background-color: #D13636;"
                          "}";

    ui->exit->setStyleSheet(buttonStyle);
    ui->add->setStyleSheet(buttonStyle);
    ui->taskListWidget->setStyleSheet("font-size: 24px; color: black;");
    connect(descriptionWindow, &DescriptionWindow::textSaved, this, &TasksWindow::addTask);
    connect(ui->taskListWidget, &QListWidget::itemDoubleClicked, this, &TasksWindow::on_taskItem_doubleClicked);
    ui->search->setPlaceholderText("Поиск...");


    // Подключаем слот для изменения текста в поле поиска
    connect(ui->search, &QLineEdit::textChanged, this, &TasksWindow::on_searchTextChanged);

    connect(ui->filterAZ, &QRadioButton::clicked, this, &TasksWindow::on_filterChanged);
    connect(ui->filterZA, &QRadioButton::clicked, this, &TasksWindow::on_filterChanged);
    connect(ui->filterNo, &QRadioButton::clicked, this, &TasksWindow::on_filterChanged);

    // Установим фильтр "Нет" по умолчанию
    ui->filterNo->setChecked(true);

    loadTasks();
}

TasksWindow::~TasksWindow()
{
    saveTasks();
    delete ui;
}

void TasksWindow::on_exit_clicked()
{
    delete this;
}

void TasksWindow::on_add_clicked()
{
    if (!descriptionWindow) {
        descriptionWindow = new DescriptionWindow(this);

        // Подключаем сигнал только один раз
        connect(descriptionWindow, &DescriptionWindow::textSaved, this, &TasksWindow::addTask);

        connect(descriptionWindow, &QWidget::destroyed, this, [this]() {
            descriptionWindow = nullptr;
        });

        descriptionWindow->show();
    } else {
        descriptionWindow->raise();
        descriptionWindow->activateWindow();
    }
}



void TasksWindow::addTask(const QString &taskText, const QDate &date)
{
    QListWidgetItem *item = new QListWidgetItem(taskText);
    item->setData(Qt::UserRole, date); // Сохраняем дату в пользовательских данных элемента

    // Добавляем заметку в BaseWindow
    BaseWindow *baseWindow = qobject_cast<BaseWindow*>(parent());
    if (baseWindow) {
        baseWindow->addNoteForDate(date, taskText);
    }

    QPushButton *deleteButton = new QPushButton("Удалить");
    QPushButton *editButton = new QPushButton("Изменить");

    // Настройки кнопок удаления и редактирования
    deleteButton->setFixedSize(100, 40);
    deleteButton->setStyleSheet(
        "QPushButton {"
        "background-color: red;"
        "color: white;"
        "font-weight:bold;"
        "border-radius: 5px;"
        "padding: 10px;"
        "font-size: 16px;"
        "} "
        "QPushButton:hover {"
        "background-color: darkred;"
        "} "
        "QPushButton:pressed {"
        "background-color: #D13636;"
        "}"
        );

    editButton->setFixedSize(100, 40);
    editButton->setStyleSheet(
        "QPushButton {"
        "background-color: blue;"
        "font-weight:bold;"
        "color: white;"
        "border-radius: 5px;"
        "padding: 10px;"
        "font-size: 16px;"
        "} "
        "QPushButton:hover {"
        "background-color: darkblue;"
        "} "
        "QPushButton:pressed {"
        "background-color: #1A4D8F;"
        "}"
        );

    // Проверяем, какой режим выбран
    QSettings settings("YourCompany", "CourseWork");
    QString userMode = settings.value("userMode").toString();

    // Если режим "Садовод-любитель", скрываем кнопку "Изменить"
    if (userMode == "Садовод-любитель") {
        editButton->setVisible(false);
    }

    QWidget *taskWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(taskWidget);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);

    item->setSizeHint(taskWidget->sizeHint());
    ui->taskListWidget->addItem(item);
    ui->taskListWidget->setItemWidget(item,taskWidget);

    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        removeTask(item);
    });

    connect(editButton, &QPushButton::clicked, this, [=]() {
        editTask(item);
    });
}




void TasksWindow::editTask(QListWidgetItem *item)
{
    QSettings settings("YourCompany", "CourseWork");
    QString userMode = settings.value("userMode").toString();

    if (userMode != "Профессионал") {
        QMessageBox::warning(this, "Доступ запрещен", "Эта функция доступна только для режима 'Профессионал'.");
        return;
    }

    // Создаем окно для редактирования задачи
    EditTaskWindow *editWindow = new EditTaskWindow(item->text(), this);
    connect(editWindow, &EditTaskWindow::taskEdited, this, [=](const QString &newText) {
        // Обновляем текст задачи после редактирования
        item->setText(newText);

        QWidget *taskWidget = ui->taskListWidget->itemWidget(item);
        QLabel *label = taskWidget->findChild<QLabel *>();
        if (label) {
            label->setText(newText);
        }

        QDate date = item->data(Qt::UserRole).toDate();
        emit taskUpdated(date, newText); // Отправляем обновление задачи
    });

    editWindow->exec();  // Показываем окно для редактирования
}


void TasksWindow::removeTask(QListWidgetItem *item)
{
    QDate date = item->data(Qt::UserRole).toDate(); // Получаем дату задачи

    int row = ui->taskListWidget->row(item);
    delete ui->taskListWidget->takeItem(row);  // Удаляем элемент из списка

    emit taskRemoved(date);  // Уведомляем о том, что задача была удалена
}

void TasksWindow::saveTasks()
{
    QFile file("tasks.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing");
        return;
    }

    QJsonArray tasksArray;
    for (int i = 0; i < ui->taskListWidget->count(); ++i) {
        QListWidgetItem *item = ui->taskListWidget->item(i);
        QString taskText = item->text();

        // Получаем дату из пользовательских данных элемента
        QDate date = item->data(Qt::UserRole).toDate();
        QString dateString = date.toString("dd.MM.yyyy");

        QJsonObject taskObject;
        taskObject["text"] = taskText;
        taskObject["date"] = dateString;
        tasksArray.append(taskObject);
    }

    QJsonDocument saveDoc(tasksArray);
    file.write(saveDoc.toJson());
    file.close();
}



void TasksWindow::loadTasks()
{
    QFile file("tasks.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for reading");
        return;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    QJsonArray tasksArray = loadDoc.array();

    QSet<QString> existingTasks; // Для отслеживания уникальных задач

    for (int i = 0; i < tasksArray.size(); ++i) {
        QJsonObject taskObject = tasksArray[i].toObject();
        QString taskText = taskObject["text"].toString();
        QDate date = QDate::fromString(taskObject["date"].toString(), "dd.MM.yyyy");

        QString uniqueKey = taskText + date.toString("dd.MM.yyyy");
        if (!existingTasks.contains(uniqueKey)) {
            addTask(taskText, date);
            existingTasks.insert(uniqueKey);
        }
    }

    file.close();
    ui->filterNo->setChecked(true);
}


void TasksWindow::on_taskItem_doubleClicked(QListWidgetItem *item)
{
    QDate date = item->data(Qt::UserRole).toDate(); // Получаем дату из данных элемента
    QString message = QString("Дата задачи: %1").arg(date.toString("dd.MM.yyyy"));
    QMessageBox::information(this, "Дата задачи", message);
}


void TasksWindow::on_searchTextChanged(const QString &searchText)
{
    // Проходим по всем элементам списка задач
    for (int i = 0; i < ui->taskListWidget->count(); ++i) {
        QListWidgetItem *item = ui->taskListWidget->item(i);
        QString taskText = item->text();

        // Показываем элемент, если он содержит текст поиска (регистр не важен)
        item->setHidden(!taskText.contains(searchText, Qt::CaseInsensitive));
    }
}



void TasksWindow::on_filterChanged()
{
    // Получаем текущий список задач
    QList<QListWidgetItem*> items;
    for (int i = 0; i < ui->taskListWidget->count(); ++i) {
        items.append(ui->taskListWidget->takeItem(0));
    }

    // Проверяем выбранный фильтр
    if (ui->filterAZ->isChecked()) {
        // Сортировка A-Z
        std::sort(items.begin(), items.end(), [](QListWidgetItem *a, QListWidgetItem *b) {
            return a->text() < b->text();
        });
    } else if (ui->filterZA->isChecked()) {
        // Сортировка Z-A
        std::sort(items.begin(), items.end(), [](QListWidgetItem *a, QListWidgetItem *b) {
            return a->text() > b->text();
        });
    }
    // Если фильтр "Нет", список остаётся в текущем порядке (ничего не делаем)

    // Перезаполняем виджет задач
    for (QListWidgetItem *item : items) {
        ui->taskListWidget->addItem(item);
    }

    // Обновляем пользовательские виджеты после сортировки
    updateTaskWidgets();
}


void TasksWindow::updateTaskWidgets()
{
    for (int i = 0; i < ui->taskListWidget->count(); ++i) {
        QListWidgetItem *item = ui->taskListWidget->item(i);

        QWidget *taskWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(taskWidget);

        // Добавляем внешние отступы
        layout->setContentsMargins(10, 10, 10, 10); // Отступы: слева, сверху, справа, снизу
        layout->setSpacing(15); // Расстояние между кнопками и текстом

        QLabel *label = new QLabel(item->text());
        layout->addWidget(label);

        QPushButton *editButton = new QPushButton("Изменить");
        QPushButton *deleteButton = new QPushButton("Удалить");

        // Настраиваем кнопки
        editButton->setFixedSize(100, 40);
        editButton->setStyleSheet(
            "QPushButton {"
            "background-color: blue;"
            "font-weight:bold;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 10px;"
            "font-size: 16px;"
            "} "
            "QPushButton:hover {"
            "background-color: darkblue;"
            "} "
            "QPushButton:pressed {"
            "background-color: #1A4D8F;"
            "}"
            );

        deleteButton->setFixedSize(100, 40);
        deleteButton->setStyleSheet(
            "QPushButton {"
            "background-color: red;"
            "color: white;"
            "font-weight:bold;"
            "border-radius: 5px;"
            "padding: 10px;"
            "font-size: 16px;"
            "} "
            "QPushButton:hover {"
            "background-color: darkred;"
            "} "
            "QPushButton:pressed {"
            "background-color: #D13636;"
            "}"
            );

        // Проверяем режим пользователя
        QSettings settings("YourCompany", "CourseWork");
        QString userMode = settings.value("userMode").toString();

        // Если режим "Садовод-любитель", скрываем кнопку "Изменить"
        if (userMode == "Садовод-любитель") {
            editButton->setVisible(false);
        }

        // Добавляем кнопки в макет
        layout->addWidget(editButton);
        layout->addWidget(deleteButton);

        taskWidget->setLayout(layout);
        item->setSizeHint(taskWidget->sizeHint());
        ui->taskListWidget->setItemWidget(item, taskWidget);

        // Связываем сигналы кнопок
        connect(deleteButton, &QPushButton::clicked, this, [=]() {
            removeTask(item);
        });

        connect(editButton, &QPushButton::clicked, this, [=]() {
            editTask(item);
        });
    }
}
