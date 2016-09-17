#include "desktopwidgetdialog.h"
#include "ui_desktopwidgetdialog.h"

#include <QStringListModel>
#include <QDebug>
#include <QLabel>
#include <QIcon>
#include <QFileDialog>

#include "desktopwidget.h"

DesktopWidgetDialog::DesktopWidgetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DesktopWidgetDialog)
{
    ui->setupUi(this);

    ui->spinBox->setRange (24, 300);

    model_ = new QStringListModel();

    QIcon icon (APP_ICON_STRING);

    ui->iconLabel->setPixmap(icon.pixmap(QSize (64,64)));
}

DesktopWidgetDialog::~DesktopWidgetDialog()
{
    delete model_;

    delete ui;
}

void DesktopWidgetDialog::setInfo (const QVariantHash &info)
{
    int size = DesktopWidget::getIconSizeFromHash (info);
    int direction = DesktopWidget::getDirectionFromHash (info);

    ui->spinBox->setValue (size);

    ui->comboBox->addItem (tr("Horizontal"));
    ui->comboBox->addItem (tr("Vertical"));

    ui->comboBox->setCurrentIndex (direction);

    QStringList items = DesktopWidget::getItemsFromHash (info);

    model_->setStringList (items);

    ui->listView->setModel(model_);
}

QVariantHash DesktopWidgetDialog::getInfo ()
{
    QVariantHash info;

    DesktopWidget::setIconSizeToHash (info, ui->spinBox->value ());
    DesktopWidget::setDirectionToHash (info, ui->comboBox->currentIndex());

    DesktopWidget::setItemsToHash (info, model_->stringList());

    return info;
}

void DesktopWidgetDialog::on_upButton_clicked()
{
    QModelIndex index = ui->listView->currentIndex();

    QModelIndex prevIndex = model_->index (index.row () - 1);

    int count = model_->rowCount();

    if (index.row () > 0 && count > 1)
    {
        QVariant tmp = prevIndex.data ();

        model_->setData (prevIndex, index.data ());
        model_->setData (index, tmp);

        ui->listView->setCurrentIndex (prevIndex);

        qDebug () << Q_FUNC_INFO << tmp;
    }

}

void DesktopWidgetDialog::on_downButton_clicked()
{
    QModelIndex index = ui->listView->currentIndex();

    QModelIndex nextIndex = model_->index (index.row () + 1);

    int count = model_->rowCount();

    if (index.row () < count - 1 && count > 1)
    {
        QVariant tmp = nextIndex.data ();

        model_->setData (nextIndex, index.data ());
        model_->setData (index, tmp);

        ui->listView->setCurrentIndex (nextIndex);

        qDebug () << Q_FUNC_INFO << tmp;
    }
}

void DesktopWidgetDialog::on_addButton_clicked()
{
    /*
    QModelIndex index;

    model_->insertRow (model_->rowCount(), index);

    index = model_->index (model_->rowCount() - 1);

    model_->setData(index, QString(":/qIconPanel.desktop"));
    */

    QString format = "desktop";
    QString initialPath;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Add"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(format.toUpper())
                               .arg(format));

    if ( ! fileName.isEmpty())
    {
        QModelIndex index;

        model_->insertRow (model_->rowCount(), index);

        index = model_->index (model_->rowCount() - 1);

        model_->setData(index, fileName);
    }
}

void DesktopWidgetDialog::on_deleteButton_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    model_->removeRow (index.row ());
}
