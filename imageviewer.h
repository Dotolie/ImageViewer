#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QScrollArea>
#include <QLabel>

#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class ImageViewer; }
QT_END_NAMESPACE

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

private slots:
    void on_openAct_triggered();

    void on_zoominAct_triggered();

    void on_zoomOutAct_triggered();

    void on_normalSizeAct_triggered();

    void on_fitToWindowAct_triggered();

    void on_printAct_triggered();

    void on_aboutAct_triggered();

private:
    Ui::ImageViewer *ui;
    QLabel *imageLabel;
    QScrollArea *scrollArea;

    double scaleFactor;

    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
};
#endif // IMAGEVIEWER_H
