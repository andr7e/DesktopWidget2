#ifndef DESKTOPWIDGETDIALOG_H
#define DESKTOPWIDGETDIALOG_H

#include <QDialog>
#include <QVariantHash>
#include <QStringListModel>

namespace Ui {
class DesktopWidgetDialog;
}

class DesktopWidgetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DesktopWidgetDialog(QWidget *parent = 0);
    ~DesktopWidgetDialog();

    void setInfo (const QVariantHash &info);
    QVariantHash getInfo ();
    
private slots:
    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::DesktopWidgetDialog *ui;
    QStringListModel *model_;
};

#endif // DESKTOPWIDGETDIALOG_H
