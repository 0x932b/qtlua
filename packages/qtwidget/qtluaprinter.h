// -*- C++ -*-

#ifndef QTLUAPRINTER_H
#define QTLUAPRINTER_H

#include "lua.h"
#include "lauxlib.h"
#include "qtluaengine.h"
#include "qtluautils.h"

#include "qtwidget.h"

#include <QObject>
#include <QPrinter>
#include <QRect>
#include <QSizeF>
#include <QString>
#include <QPageLayout>



class QTWIDGET_API QtLuaPrinter : public QObject, public QPrinter
{
  Q_OBJECT
  Q_PROPERTY(bool colorMode READ colorMode WRITE setColorMode)
  Q_PROPERTY(bool collateCopies READ collateCopies WRITE setCollateCopies)
  Q_PROPERTY(QString creator READ creator WRITE setCreator)
  Q_PROPERTY(QString docName READ docName WRITE setDocName)
  Q_PROPERTY(bool doubleSidedPrinting READ doubleSidedPrinting 
             WRITE setDoubleSidedPrinting)
  Q_PROPERTY(bool fontEmbeddingEnabled READ fontEmbeddingEnabled 
             WRITE setFontEmbeddingEnabled)
  Q_PROPERTY(int fromPage READ fromPage)
  Q_PROPERTY(bool fullPage READ fullPage WRITE setFullPage)
  Q_PROPERTY(bool landscape READ landscape WRITE setLandscape)
  Q_PROPERTY(int numCopies READ numCopies WRITE setNumCopies)
  Q_PROPERTY(QString outputFileName READ outputFileName WRITE setOutputFileName)
  Q_PROPERTY(QString outputFormat READ outputFormat WRITE setOutputFormat)
  Q_PROPERTY(QString pageSize READ pageSize WRITE setPageSize)
  Q_PROPERTY(QString printerName READ printerName WRITE setPrinterName)
  Q_PROPERTY(QString printProgram READ printProgram WRITE setPrintProgram)
  Q_PROPERTY(int resolution READ resolution WRITE setResolution)
  Q_PROPERTY(int toPage READ toPage)
  Q_PROPERTY(QRectF paperRect READ paperRect)
  Q_PROPERTY(QRectF pageRect READ pageRect)
  Q_PROPERTY(QSizeF paperSize READ paperSize WRITE setPaperSize)
  Q_PROPERTY(QString printerState READ printerState)
  Q_ENUMS(PrinterState)

public:
 ~QtLuaPrinter();
  QtLuaPrinter(int resolution=72, QObject *parent=0)
    : QObject(parent), QPrinter() {
    custom = false;
    setResolution(resolution);
  }

  Q_INVOKABLE QPrinter* printer() { return static_cast<QPrinter*>(this);}
  Q_INVOKABLE QPaintDevice* device() { return static_cast<QPaintDevice*>(this);}
  Q_INVOKABLE void setFromTo(int f, int t) { QPrinter::setFromTo(f, t); }
  Q_INVOKABLE bool newPage() { return QPrinter::newPage(); }
  Q_INVOKABLE bool abort() { return QPrinter::abort(); }
  Q_INVOKABLE bool setup(QWidget *parent=0);

  bool colorMode() const { return QPrinter::colorMode()==Color;}
  void setColorMode(bool b) { QPrinter::setColorMode(b ? QPrinter::Color : QPrinter::GrayScale); }
  bool landscape() const { return QPrinter::pageLayout().orientation() == QPageLayout::Landscape; }
  void setLandscape(bool b) {
    QPageLayout layout = QPrinter::pageLayout();
    layout.setOrientation(b ? QPageLayout::Landscape : QPageLayout::Portrait);
    QPrinter::setPageLayout(layout);
  }
  bool lastPageFirst() const { return QPrinter::pageOrder() == QPrinter::LastPageFirst; }
  void setLastPageFirst(bool b) { QPrinter::setPageOrder(b ? QPrinter::LastPageFirst : QPrinter::FirstPageFirst); }
  QString pageSize() const;
  void setPageSize(QString s);
  QString outputFormat() const;
  void setOutputFormat(QString s);
  QString printerState() const;
  QSizeF paperSize() const;
  void setPaperSize(QSizeF s);
  
  // Qt6 compatibility methods
  QRectF paperRect() const { return QPrinter::paperRect(QPrinter::Point); }
  QRectF pageRect() const { return QPrinter::pageRect(QPrinter::Point); }
  bool doubleSidedPrinting() const { return QPrinter::duplex() != QPrinter::DuplexNone; }
  void setDoubleSidedPrinting(bool enable) { QPrinter::setDuplex(enable ? QPrinter::DuplexAuto : QPrinter::DuplexNone); }
  int numCopies() const { return QPrinter::copyCount(); }
  void setNumCopies(int count) { QPrinter::setCopyCount(count); }
private:
  QSizeF papSize;
  bool custom;
signals:
  void closing(QObject*);
};




#endif


/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ("\\sw+_t" "\\(lua_\\)?[A-Z]\\sw*[a-z]\\sw*" "qreal")
   End:
   ------------------------------------------------------------- */


