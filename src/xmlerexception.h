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

#include <QtCore/QSharedData>
#include <QtXml/QXmlParseException>

class XMLerException : public QSharedData
{
public:
  enum ExceptionType { Error, FatalError, Warning };

  explicit XMLerException( const XMLerException &other );
  explicit XMLerException( XMLerException::ExceptionType et, const QXmlParseException &exception );
  /* ~XMLerException(); */

  XMLerException & operator=(const XMLerException &other);
  /* XMLerException & operator=(const QXmlParseException &other); */

  /* self */
  int columnNumber () const;
  int lineNumber () const;
  QString message () const;
  QString publicId () const;
  QString systemId () const;
  XMLerException::ExceptionType exceptionType () const;
  QString printMessage () const;
  QString exceptionTypeStr () const;
  static QString exceptionTypeStr (XMLerException::ExceptionType et);
  
private:
  int _column;
  int _line;
  QString _message;
  QString _pid;
  QString _sid;
  XMLerException::ExceptionType _type;

  void copy_other ( const XMLerException &other );
  void copy_other ( const QXmlParseException &other );
};

typedef QList<XMLerException> XMLerExceptionList;

#endif
