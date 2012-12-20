// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QThread)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerFileOperThread
   Description: Base class for threaded file operation classes
*/
#ifndef XMLERFILEOPERTHREAD_H
#define XMLERFILEOPERTHREAD_H

#include <QThread>
#include "documentxmlnode.h"

class XMLerFileOperThread : public QThread
{
  Q_OBJECT
public:
  explicit XMLerFileOperThread(QObject *parent = 0);
  ~XMLerFileOperThread();

  virtual void run ();

  void setFileName ( const QString &fn );
  QString fileName () const;

protected:
  virtual QString progressMessage () const;

private:
  QString _fileName;

signals:
  void beginProgress ( QString process, qint64 totalSize );
  void progress ( qint64 pos );
  void endProgress ();
  void done ( DocumentXMLNode *document );
  void done ();
  void error ( QString message );
  void warning ( QString message );

public slots:
  void on_beginProgress ( qint64 totalSize );
  void on_progress ( qint64 pos );
  void on_endProgress ( );
  
};

#endif
