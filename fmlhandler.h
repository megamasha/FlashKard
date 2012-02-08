#ifndef FMLHANDLER_H
#define FMLHANDLER_H

#include <QXmlDefaultHandler>

class flashCard;

class FMLHandler : public QXmlDefaultHandler
{
public:
    FMLHandler();

    bool startDocument();

    bool startElement( const QString & namespaceURI, const QString & name, const QString & qName, const QXmlAttributes & atts );

    bool characters ( const QString & text );

    bool endElement( const QString & namespaceURI, const QString & name, const QString & qName );

private:
    bool inFlashcardsSection;
    bool inFlashcard;
    QString currentCardProperty;
    flashCard * currentCard;
};

#endif // FMLHANDLER_H
