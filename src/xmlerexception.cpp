// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QSharedData)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerException
   Description:
*/
#include "xmlerexception.h"

XMLerException::XMLerException ( ) :
  QSharedData()
{
}

XMLerException::XMLerException (const XMLerException &other ) :
  QSharedData()
{
  copy_other ( other );
}

XMLerException::XMLerException ( XMLer::ExceptionType et, const QXmlParseException &exception ) :
  QSharedData()
{
  copy_other ( exception );
  _type = et;
}
XMLerException& XMLerException::operator=(const XMLerException &other)
{
  copy_other ( other );
  return *this;
}
/* XMLerException& XMLerException::operator=(const QXmlParseException &other)
{
  copy_other ( other );
  return *this;
  }*/

/* XMLerException::~XMLerException ()
{
} */

/* self */
int XMLerException::columnNumber () const
{
  return _column;
}
int XMLerException::lineNumber () const
{
  return _line;
}
QString XMLerException::message () const
{
  return _message;
}
QString XMLerException::publicId () const
{
  return _pid;
}
QString XMLerException::systemId () const
{
  return _sid;
}
XMLer::ExceptionType XMLerException::exceptionType () const
{
  return _type;
}
QString XMLerException::printMessage () const
{
  QString result = QString("%1 \"%2\" line=%3, column=%4").arg(exceptionTypeStr()).arg(_message).arg(_line).arg(_column);
  return result;
}
QString XMLerException::exceptionTypeStr () const
{
  return XMLerException::exceptionTypeStr ( _type );
}
QString XMLerException::exceptionTypeStr (XMLer::ExceptionType et)
{
  QString msg_type;
  switch ( et ) {
  case XMLer::Warning:
    msg_type = QObject::tr("Warning");
    break;
  case XMLer::Error:
    msg_type = QObject::tr("Error");
    break;
  case XMLer::FatalError:
    msg_type = QObject::tr("Fatal error");
    break;
  default:
    break;
  };
  return msg_type;
}

/* private self */
void XMLerException::copy_other ( const XMLerException &other )
{
  _column       = other.columnNumber();
  _line         = other.lineNumber();
  _message      = other.message();
  _pid          = other.publicId();
  _sid          = other.systemId();
  _type         = other.exceptionType();
}
void XMLerException::copy_other ( const QXmlParseException &other )
{
  _column       = other.columnNumber();
  _line         = other.lineNumber();
  _message      = other.message();
  _pid          = other.publicId();
  _sid          = other.systemId();
}
