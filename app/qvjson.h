
/*
 * qvjson.h
 *
 *  Created on: Apr 23, 2010
 *      Author: drh
 */

#ifndef QVJSON_H_
#define QVJSON_H_
#include <QMap>
#include <QList>
#include <QVariant>
#include <QRegExp>

class QvJson {

public:
    QvJson();
    virtual ~QvJson();

    QVariant parseJson(const QString& jsonString);

    // The jsonObject is a QVariant that may be a --
    // -- QMap<QString, QVariant>
    // -- QList<QVariant>
    // -- QString
    // -- integer
    // -- real number
    // -- bool
    // -- null (implemented as a QVariant() object)
    // Where QVariant appears in the above list, it may recursively
    // be one of the above.
    QString encodeJson(const QVariant& jsonObject);

private:
    enum Error {
        ErrorUnrecognizedToken,
        ErrorMustBeString,
        ErrorMissingColon,
        ErrorMissingCommaOrClose,
        ErrorNoClosingQuote,
        ErrorEndOfInput,
        ErrorInvalidNumericValue,
        ErrorInvalidHexValue,
        ErrorUnrecognizedObject
    };

    QRegExp mNonBlank;
    QRegExp mFindQuote;

    QString nextToken(const QString& jsonString, int& position);
    QString peekToken(const QString& jsonString, int position);
    QVariant parseInternal(const QString& jsonString, int& position, int nesting);
    QVariant parseObject(const QString& jsonString, int& position, int nesting);
    QVariant parseArray(const QString& jsonString, int& position, int nesting);
    QVariant parseString(const QString& jsonString, int& position, int nesting);
    QVariant parseBool(const QString& jsonString, int& position, int nesting);
    QVariant parseNull(const QString& jsonString, int& position, int nesting);
    QVariant parseNumber(const QString& jsonString, int& position, int nesting);
    QVariant syntaxError(const QString& method, const QString& jsonString, int position, int nesting, Error error);

    QString encodeObject(const QVariant& jsonObject);
    QString encodeArray(const QVariant& jsonObject);
    QString encodeString(const QVariant& jsonObject);
    QString encodeString(const QString& value);
    QString encodeNumeric(const QVariant& jsonObject);
    QString encodeBool(const QVariant& jsonObject);
    QString encodeNull(const QVariant& jsonObject);
    QString encodingError(const QString& method, const QVariant& jsonObject, Error error);
};

#endif // QVJSON_H
