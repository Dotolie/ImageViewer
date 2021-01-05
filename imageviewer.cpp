#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QScrollBar>
#include <QPainter>

#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    setWindowTitle(tr("image viewer"));
    resize(500,400);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}


void ImageViewer::on_openAct_triggered()
{
    qDebug() << "open()";
    QString fileName = QFileDialog::getOpenFileName(this,
                            tr("OpenFile"), QDir::currentPath());
    if( !fileName.isEmpty()) {
        QImage image(fileName);
        if( image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                             tr("%1 can't load").arg(fileName));
            return;
        }
        imageLabel->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;

        ui->printAct->setEnabled(true);
        ui->fitToWindowAct->setEnabled(true);
        updateActions();

        if(!ui->fitToWindowAct->isChecked())
            imageLabel->adjustSize();
    }

}

void ImageViewer::on_zoominAct_triggered()
{
    scaleImage(1.25);
}

void ImageViewer::on_zoomOutAct_triggered()
{
    scaleImage(0.8);
}

void ImageViewer::on_normalSizeAct_triggered()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::on_fitToWindowAct_triggered()
{
    bool fitToWindow = ui->fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if(!fitToWindow) {
        on_normalSizeAct_triggered();
    }
    updateActions();
}

void ImageViewer::updateActions()
{
    ui->zoominAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->zoomOutAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->normalSizeAct->setEnabled(!ui->fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    ui->zoominAct->setEnabled(scaleFactor < 3.0);
    ui->zoomOutAct->setEnabled(scaleFactor > 0.333 );
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor*scrollBar->value() + ((factor -1) * scrollBar->pageStep()/2)));
}


void ImageViewer::on_printAct_triggered()
{
    Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);
    if(dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0,0, *imageLabel->pixmap());
    }
#endif
}

void ImageViewer::on_aboutAct_triggered()
{
    QMessageBox::about(this, tr("About Image Viewer"),
                       tr("<b>Image Viewer</b> example."));
}
