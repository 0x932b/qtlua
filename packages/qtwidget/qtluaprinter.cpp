/* -*- C++ -*- */


#include "qtluaprinter.h"

#include <QDialog>
#include <QPrintDialog>
#include <QPageSize>



struct Option 
{
  const char *name; 
  int value;
};

#define F(t) {#t, (int) QPrinter::t}
#define P(t) {#t, (int) QPageSize::t}

static Option pageSizes[] = {
  P(A4), P(B5), P(Letter), P(Legal), P(Executive),
  P(A0), P(A1), P(A2), P(A3), P(A5), P(A6), P(A7), P(A8), P(A9), P(B0), P(B1),
  P(B10), P(B2), P(B3), P(B4), P(B6), P(B7), P(B8), P(B9), P(C5E), P(Comm10E),
  P(DLE), P(Folio), P(Ledger), P(Tabloid), P(Custom),
  {0} };

static Option outputFormats[] = {
  F(NativeFormat), F(PdfFormat),
  {0} };

static Option printerStates[] = {
  F(Idle), F(Active), F(Aborted), F(Error),
  {0} };


static const char *
value_to_name(int value, Option *opts)
{
  for(; opts->name; opts++)
    if  (opts->value == value)
      return opts->name;
  return "unknown";
}


static int
name_to_value(const char *name, Option *opts)
{
  for(; opts->name; opts++)
    if  (! strcmp(name, opts->name))
      return opts->value;
  return -1;
}


QString
QtLuaPrinter::pageSize() const
{
  QPageSize size = QPrinter::pageLayout().pageSize();
  return QString::fromLatin1(value_to_name((int)size.id(), pageSizes));
}


void
QtLuaPrinter::setPageSize(QString r)
{
  int s = name_to_value(r.toLocal8Bit().constData(), pageSizes);
  if (s >= 0)
    {
      custom = false;
      if (s != QPageSize::Custom)
      {
        QPageLayout layout = QPrinter::pageLayout();
        layout.setPageSize(QPageSize(QPageSize::PageSizeId(s)));
        QPrinter::setPageLayout(layout);
      }
      else
      {
        custom = true;
        if (custom && papSize.isValid())
        {
          QPageLayout layout = QPrinter::pageLayout();
          layout.setPageSize(QPageSize(papSize, QPageSize::Point));
          QPrinter::setPageLayout(layout);
        }
      }
    }
}


QSizeF 
QtLuaPrinter::paperSize() const 
{
  return papSize;
}


void
QtLuaPrinter::setPaperSize(QSizeF s)
{
  papSize = s;
  if (custom && papSize.isValid())
  {
    QPageLayout layout = QPrinter::pageLayout();
    layout.setPageSize(QPageSize(papSize, QPageSize::Point));
    QPrinter::setPageLayout(layout);
  }
}


QString
QtLuaPrinter::outputFormat() const
{
  int s = (int) QPrinter::outputFormat();
  return QString::fromLatin1(value_to_name(s, outputFormats));
}


void 
QtLuaPrinter::setOutputFormat(QString r)
{
  int s = name_to_value(r.toLocal8Bit().constData(), outputFormats);
  if (s >= 0)
    QPrinter::setOutputFormat(QPrinter::OutputFormat(s));
}


QString
QtLuaPrinter::printerState() const
{
  int s = (int) QPrinter::printerState();
  return QString::fromLatin1(value_to_name(s, printerStates));
}


bool
QtLuaPrinter::setup(QWidget *parent)
{
  QPointer<QPrintDialog> dialog = new QPrintDialog(this, parent);
  dialog->setFromTo(fromPage(), toPage());
  
  // Set options via QPrintDialog::Options
  QPrintDialog::PrintDialogOptions options = QPrintDialog::PrintToFile |
                                           QPrintDialog::PrintPageRange |
                                           QPrintDialog::PrintCollateCopies;
  dialog->setOptions(options);
  
  // exec
  int result = dialog->exec();
  delete dialog;
  return (result == QDialog::Accepted);
}


QtLuaPrinter::~QtLuaPrinter()
{
  emit closing(this);
}






/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ("\\sw+_t" "\\(lua_\\)?[A-Z]\\sw*[a-z]\\sw*" "qreal")
   End:
   ------------------------------------------------------------- */
