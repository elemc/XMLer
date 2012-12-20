// -*- C++ -*-
/* ------------------------------------------ */
/* C++ class header (QObject,QXmlInputSource) */
/* Author: Alexei Panov <me@elemc.name>       */
/* ------------------------------------------ */
/* Name: XMLerInputSource
   Description:
*/
#ifndef XMLERINPUTSOURCE_H
#define XMLERINPUTSOURCE_H

#include "config.h"

#include <QObject>
#include <QXmlInputSource>

class XMLerInputSource : public QObject, public QXmlInputSource
{
  Q_OBJECT
public:
  explicit XMLerInputSource(QIODevice * dev, QObject *parent = 0);
  ~XMLerInputSource();

  QChar next ();
  
private:
  qint64 _data_size;
  qint64 _data_pos;
  qint64 _step;

signals:
  void beginRead ( qint64 totalSize );
  void readProgress ( qint64 pos );
  void endRead ();
};

#endif
