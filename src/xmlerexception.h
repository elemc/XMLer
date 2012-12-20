// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QSharedData)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerException
   Description:
*/
#ifndef XMLEREXCEPTION_H
#define XMLEREXCEPTION_H

#include <QSharedData>
#include <QXmlParseException>

namespace XMLer {
  enum ExceptionType { Error=0, FatalError=1, Warning=2 };
};

class XMLerException : public QSharedData
{
public:
  explicit XMLerException( );
  explicit XMLerException( const XMLerException &other );
  explicit XMLerException( XMLer::ExceptionType et, const QXmlParseException &exception );
  /* ~XMLerException(); */

  XMLerException & operator=(const XMLerException &other);
  /* XMLerException & operator=(const QXmlParseException &other); */

  /* self */
  int columnNumber () const;
  int lineNumber () const;
  QString message () const;
  QString publicId () const;
  QString systemId () const;
  XMLer::ExceptionType exceptionType () const;
  QString printMessage () const;
  QString exceptionTypeStr () const;
  static QString exceptionTypeStr (XMLer::ExceptionType et);
  
private:
  int _column;
  int _line;
  QString _message;
  QString _pid;
  QString _sid;
  XMLer::ExceptionType _type;

  void copy_other ( const XMLerException &other );
  void copy_other ( const QXmlParseException &other );
};
typedef QList<XMLerException> XMLerExceptionList;

#endif
