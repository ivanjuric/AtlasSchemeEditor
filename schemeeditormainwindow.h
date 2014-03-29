#ifndef SCHEMEEDITORMAINWINDOW_H
#define SCHEMEEDITORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SchemeEditorMainWindow;
}

class SchemeEditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SchemeEditorMainWindow(QWidget *parent = 0);
    ~SchemeEditorMainWindow();

private:
    Ui::SchemeEditorMainWindow *ui;
};

#endif // SCHEMEEDITORMAINWINDOW_H
