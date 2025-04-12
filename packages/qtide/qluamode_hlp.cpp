/* -*- C++ -*- */

#include <QtGlobal>
#include <QDebug>
#include <QList>
#include <QRegularExpression>
#include <QStack>


#include "qluatextedit.h"
#include "qluamode.h"

#include <string.h>



// ========================================
// USERDATA


namespace {

  struct UserData : public QLuaModeUserData
  {
    bool verbatim;
    UserData() : verbatim(false) { }
    virtual int highlightState() { return (verbatim) ? 1 : 0; }
  };
  
}




// ========================================
// QLUAMODETEXT




class QLuaModeHelp : public QLuaMode
{
  Q_OBJECT
public:
  QLuaModeHelp(QLuaTextEditModeFactory *f, QLuaTextEdit *e);
  virtual bool doEnter();
  virtual bool supportsHighlight() { return true; }
  virtual bool supportsMatch() { return true; }
  virtual bool supportsBalance() { return false; }
  virtual bool supportsIndent() { return true; }
  virtual void parseBlock(int pos, const QTextBlock &block, 
                          const QLuaModeUserData *idata, 
                          QLuaModeUserData *&odata );
  void gotLine(UserData *d, int pos, int len, QString);
private:
  QRegularExpression reSection, reHRule, reIndent, reEmpty, reToken;
  QRegularExpression reFormat1, reFormat2, reEVerb;
};


QLuaModeHelp::QLuaModeHelp(QLuaTextEditModeFactory *f, QLuaTextEdit *e)
  : QLuaMode(f,e), 
    reSection(QStringLiteral("^\\-\\-\\-\\+.*$")),
    reHRule(QStringLiteral("^(\\-\\-\\-)+")),
    reIndent(QStringLiteral("^(   |\t)+(\\*|[1aAiI]\\.|\\$(?=.+:\\s+.*\\$))[ \t]+")),
    reEmpty(QStringLiteral("^\\s*$")),
    reToken(QStringLiteral("(<[^>]*>?|#\\w+|\\[\\[|\\]\\[|\\]\\]|==|__|\\*|=|_)")),
    reFormat1(QStringLiteral("^(==|__|\\*|=|_)(\\S+\\S?)(\\1)"), QRegularExpression::InvertedGreedinessOption),
    reFormat2(QStringLiteral("^(==|__|\\*|=|_)(\\S.*\\S)(\\1)")),
    reEVerb(QStringLiteral("</verbatim>"))
{
}


bool
QLuaModeHelp::doEnter()
{
  e->textCursor().insertBlock();
  return true;
}


void 
QLuaModeHelp::parseBlock(int pos, const QTextBlock &block, 
                         const QLuaModeUserData *idata, 
                         QLuaModeUserData *&odata )
{
  QString text = block.text();
  UserData *data = new UserData;
  // input state
  if (idata)
    *data = *static_cast<const UserData*>(idata);
  // process line
  gotLine(data, pos, block.length(), block.text());
  // output state
  odata = data;
}


void
QLuaModeHelp::gotLine(UserData *d, int pos, int len, QString line)
{
  int i = 0;
  int matchPos = 0;
  int matchLen = 0;
  QString matchToken;
  while (i < len)
    {
      int p;
      if (d->verbatim)
        {
          QRegularExpressionMatch match = reEVerb.match(line, i);
          if (match.hasMatch())
            {
              p = match.capturedStart();
              setFormat(pos, p, "string");
              setIndentOverlay(pos+p);
              d->verbatim = false;
              i = p;
            }
          else
            {
              i = len;
              setFormat(pos, len-1, "string");
            }
          continue;
        }
      if (i == 0)
        {
          if (pos == 0 || reSection.match(line.mid(i)).hasMatch())
            {
              setFormat(pos,len-1,"comment");
              setIndent(pos+0, 0);
              i = len;
              continue;
            }
          QRegularExpressionMatch hruleMatch = reHRule.match(line, i);
          if (hruleMatch.hasMatch())
            {
              int l = hruleMatch.capturedLength();
              setFormat(pos, l, "keyword");
              setIndent(pos, -1);
              i = i + l;
              continue;
            }
          QRegularExpressionMatch indentMatch = reIndent.match(line, i);
          if (indentMatch.hasMatch())
            {
              int l = indentMatch.capturedLength();
              int m = indentMatch.capturedStart(2);
              setFormat(pos, l, "keyword");
              setIndent(pos, e->indentAt(m));
              setIndent(pos+l, e->indentAt(pos+l));
              i = i + l;
              continue;
            }
          if (reEmpty.match(line, i).hasMatch())
            {
              setIndent(pos+1, -1);
              i = len;
              continue;
            }
        }
      QRegularExpressionMatch tokenMatch = reToken.match(line, i);
      if (tokenMatch.hasMatch())
        {
          p = tokenMatch.capturedStart();
          int l = tokenMatch.capturedLength();
          QString k = tokenMatch.captured();
          setFormat(pos+p, l, "keyword");
          if (k == "]]" && matchLen>0)
            {
              if (matchToken == "[[") {
                int opos = matchPos+matchLen;
                setFormat(opos, pos+p-opos, "url");
              }
              setRightMatch(pos+p, l, matchPos, matchLen);
              matchLen = 0;
            }
          else if (k == "][" && matchLen>0)
            {
              if (matchToken == "[[") {
                int opos = matchPos+matchLen;
                setFormat(opos, pos+p-opos, "url");
                setMiddleMatch(pos+p, l, matchPos, matchLen);
              } else
                setErrorMatch(pos+p, l, matchPos, matchLen);
              matchPos = pos+p;
              matchLen = l;
              matchToken = k;
            }
          else if (k == "[[" && matchLen<=0)
            {
              setLeftMatch(pos+p, l);
              matchPos = pos+p;
              matchLen = l;
              matchToken = k;
            }
          else if (k == "[[" || k == "][" || k == "]]")
            {
              setErrorMatch(pos+p, l, matchPos, matchLen);
              matchLen = 0;
            }
          else if (k[0] ==  '#')
            {
              setFormat(pos+p, l, "url");
            }
          else if (k == "<verbatim>")
            {
              d->verbatim = true;
              setFormat(pos+p+l, len-p-1, "string");
              setIndentOverlay(pos+p+l, -1);
              i = len;
              continue;
            }
          else if (k[0] != '<')
            {
              int q;
              int q1 = p;
              int q2 = p;
              QRegularExpressionMatch match1 = reFormat1.match(line, p, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
              if (match1.hasMatch() && match1.capturedStart() == p)
                {
                  l = match1.capturedLength();
                  k = match1.captured(1);
                  q1 = match1.capturedStart(2);
                  q2 = match1.capturedStart(3);
                  q = p;
                }
              else
                {
                  QRegularExpressionMatch match2 = reFormat2.match(line, p, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
                  if (match2.hasMatch() && match2.capturedStart() == p)
                    {
                      l = match2.capturedLength();
                      k = match2.captured(1);
                      q1 = match2.capturedStart(2);
                      q2 = match2.capturedStart(3);
                      q = p;
                    }
                }
              QTextCharFormat fmt = e->format("Help/string");
              if (k == "__" || k == "==" || k == "*")
                fmt.setFontWeight(QFont::Bold);
              setFormat(pos+p, l, (q>=0) ? "keyword" : "normal");
              if (q >= 0) 
                {
                  setLeftMatch(pos+p, q1-p);
                  setFormat(pos+q1, q2-q1, fmt);
                  setRightMatch(pos+q2, p+l-q2, pos+p, q1-p);
                }
            }
          i = p+l;
          continue;
        }
      i = len;
    }
}


// ========================================
// FACTORY


const char *helpName = QT_TRANSLATE_NOOP("QLuaTextEditMode", "Help");

static QLuaModeFactory<QLuaModeHelp> textModeFactory(helpName, "hlp");



// ========================================
// MOC


#include "qluamode_hlp.moc"





/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ("\\sw+_t" "\\(lua_\\)?[A-Z]\\sw*[a-z]\\sw*")
   End:
   ------------------------------------------------------------- */
