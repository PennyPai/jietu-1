#include "dialog.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QGraphicsView>
#include <QMainWindow>
//#include <QMainWindowLayout>
#include "cscreenshotmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Dialog w;
//    w.setWindowFlags(w.windowFlags() | Qt::Popup);
//    w.show();
//    w.exec();
//    w.showFullScreen();
//   QList<QScreen *> screens = QApplication::screens();
//   QList<QGraphicsView *> widgets;
//   foreach (QScreen *d, screens) {
//       qDebug()<<"d "<<d->availableGeometry()<<d->availableVirtualGeometry();
//       QGraphicsView *w = new QGraphicsView;
//       w->setWindowModality(Qt::WindowModal);
//       w->setWindowFlags(Qt::FramelessWindowHint);
//       w->setWindowOpacity(0.8);
//       w->setAttribute(Qt::WA_Mapped,true);

////       w->setWindowModality(Qt::ApplicationModal);
//       //ApplicationModal
////       w->setWindowFlags(Qt::Popup);
//       w->setGeometry(d->availableGeometry());
//       widgets.append(w);
//       w->showFullScreen();
////       w->setUnifiedTitleAndToolBarOnMac(false);
////       QMainWindowLayout *mainLayout = qobject_cast<QMainWindowLayout*>(w->layout());
////       w->activateUnifiedToolbarAfterFullScreen = true;
////       break;

//   }
//   foreach (QGraphicsView *w, widgets) {
////       w->showMaximized();
////       w->setUnifiedTitleAndToolBarOnMac(false);
////       w->setVisible(true);
//   }

//   return -1;
    CScreenShotManager manager;
    manager.startScreenShot();

    return a.exec();
}
