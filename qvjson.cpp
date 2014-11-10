/*
 * qvjson.cpp
 *
 *  Created on: Apr 23, 2010
 *      Author: drh
 */

#include "qvjson.h"
#include <QStringList>


QvJson::QvJson() {
    mNonBlank = QRegExp("\\S", Qt::CaseSensitive, QRegExp::RegExp);
    mFindQuote = QRegExp("[^\\\\]\"", Qt::CaseSensitive, QRegExp::RegExp);
}

QvJson::~QvJson()
    {
    // TODO Auto-generated destructor stub
    }

QVariant QvJson::parseJson(const QString& jsonString) {

    int position = 0;
    QVariant result;

    result = parseInternal(jsonString, position, 0);

    if (result.type() == QVariant::StringList) {
        qWarning("Error in string handling");
    }

    QString token(nextToken(jsonString, position));

    if (!token.isEmpty()) {
        //QMessageBox::critical(NULL, "", QObject::tr("Invalid JSON string -- remaining token %1 at position %2.").arg(token).arg(position));
    qCritical("Invalid JSON string");
    }
    return result;
}

QString QvJson::nextToken(const QString& jsonString, int& position) {
    if (position >= jsonString.count()) {
        return QString();
    }
    while (jsonString.at(position) == ' ') {
        position++;
        if (position >= jsonString.count()) {
            return QString();
        }
    }
    if (jsonString.at(position).isLetter()) {
        if (position + 4 < jsonString.count()) {
            QString word = jsonString.mid(position, 4);
            if ((word == "true") || (word == "null")) {
                position += 4;
                while (position < jsonString.count() && jsonString.at(position) == ' ') {
                    position++;
                }
                return word;
            }
            if ((word == "fals") && (position + 5 < jsonString.count()) && jsonString.at(position + 4) == 'e') {
                position += 5;
                while (position < jsonString.count() && jsonString.at(position) == ' ') {
                    position++;
                }
                return "false";
            }
        }
    }
    QString result = QString(jsonString.at(position));
    position++;
    while (position < jsonString.count() && jsonString.at(position) == ' ') {
        position++;
    }
    return result;
}

// By virtue of its non-& position parm, this method "peeks" at the token without consuming it.
QString QvJson::peekToken(const QString& jsonString, int position) {
    return (nextToken(jsonString, position));
}

QVariant QvJson::parseInternal(const QString& jsonString, int& position, int nesting) {
    QString token(peekToken(jsonString, position));
    QVariant result;
    int startPosition = position;  // For debug
    Q_UNUSED(startPosition);
    if (token.isNull()) {
        result = syntaxError("parseInternal", jsonString, position, nesting + 1, ErrorEndOfInput);
    }
    else if (token == "{") {
        result = parseObject(jsonString, position, nesting + 1);
    }
    else if (token == "[") {
        result = parseArray(jsonString, position, nesting + 1);
    }
    else if (token == "\"") {
        result = parseString(jsonString, position, nesting + 1);
    }
    else if ((token == "true") || (token == "false")) {
        result = parseBool(jsonString, position, nesting + 1);
    }
    else if (token == "null") {
        result = parseNull(jsonString, position, nesting + 1);
    }
    else if ((token == "-") || (token.at(0).isDigit())) {
        result = parseNumber(jsonString, position, nesting + 1);
    }
    else {
        result = syntaxError("parseInternal", jsonString, position, nesting + 1, ErrorUnrecognizedToken);
    }
    return result;
}

QVariant QvJson::parseObject(const QString& jsonString, int& position, int nesting) {
    QMap<QString, QVariant> resultObject;
    QString token;
    int startPosition = position;  // For debug
    Q_UNUSED(startPosition);
    token = nextToken(jsonString, position);
    Q_ASSERT(token == "{");

    // Handle case of empty object
    token = peekToken(jsonString, position);
    if (token == "}") {
        return QVariant(resultObject);
    }

    do {

        // Next item must be a string
        token = peekToken(jsonString, position);
        if (token != "\"") {
            return syntaxError("parseObject", jsonString, position, nesting + 1, ErrorMustBeString);
        }
        QVariant propName;
        propName = parseString(jsonString, position, nesting + 1);
        // Check for error
        if (propName.type() == QVariant::StringList) {
            QStringList propNameError(propName.toStringList());
            propNameError << QObject::tr("in parseObject(%1,%2)").arg(position).arg(nesting);
            return QVariant(propNameError);
        }

        // Expect a ":"
        token = nextToken(jsonString, position);
        if (token != ":") {
            return syntaxError("parseObject", jsonString, position, nesting + 1, ErrorMissingColon);
        }

        // Now get the value
        QVariant propValue;
        propValue = parseInternal(jsonString, position, nesting + 1);
        // Check for error
        if (propValue.type() == QVariant::StringList) {
            QStringList propValueError(propValue.toStringList());
            propValueError << QObject::tr("in parseObject(%1,%2)").arg(position).arg(nesting);
            return QVariant(propValueError);
        }

        resultObject[propName.toString()] = propValue;

        // Next token must be a "," or "}"
        token = nextToken(jsonString, position);
        if (token == "}") {
            return QVariant(resultObject);
        }
        else if (token != ",") {
            return syntaxError("parseObject", jsonString, position, nesting + 1, ErrorMissingCommaOrClose);
        }
    } while (true);
}

QVariant QvJson::parseArray(const QString& jsonString, int& position, int nesting) {
    QList<QVariant> resultArray;
    QString token;

    token = nextToken(jsonString, position);
    Q_ASSERT(token == "[");

    // Handle case of empty object
    token = peekToken(jsonString, position);
    if (token == "]") {
        return QVariant(resultArray);
    }

    do {

        // Get the element of the array
        QVariant propValue;
        propValue = parseInternal(jsonString, position, nesting + 1);
        // Check for error
        if (propValue.type() == QVariant::StringList) {
            QStringList propValueError(propValue.toStringList());
            propValueError << QObject::tr("in parseObject(%1,%2)").arg(position).arg(nesting);
            return QVariant(propValueError);
        }

        resultArray << propValue;

        // Next token must be a "," or "]"
        token = nextToken(jsonString, position);
        if (token == "]") {
            return QVariant(resultArray);
        }
        else if (token != ",") {
            return syntaxError("parseArray", jsonString, position, nesting + 1, ErrorMissingCommaOrClose);
        }
    } while (true);
}

QVariant QvJson::parseString(const QString& jsonString, int& position, int nesting) {
    QString result;

    // Skip over the double quote character
    Q_ASSERT(jsonString.at(position) == '"');
    position++;

    do {
        if (position >= jsonString.count()) {
            return syntaxError("parseString", jsonString, position, nesting, ErrorNoClosingQuote);
        }
        if (jsonString.at(position) == '"') {
            break;
        }
        if (jsonString.at(position) == '\\') {
            position++;
            if (position >= jsonString.count()) {
                return syntaxError("parseString", jsonString, position, nesting, ErrorNoClosingQuote);
            }
            switch (jsonString.at(position).unicode()) {
                case 'b':
                    result.append('\b');
                    break;
                case 'f':
                    result.append('\f');
                    break;
                case 'n':
                    result.append('\n');
                    break;
                case 'r':
                    result.append('\r');
                    break;
                case 't':
                    result.append('\t');
                    break;
                case 'u':
                    {
                        if (position + 4 >= jsonString.count()) {
                            return syntaxError("parseString", jsonString, position, nesting, ErrorNoClosingQuote);
                        }
                        QString hex = jsonString.mid(position + 1, 4);
                        bool ok;
                        int value = hex.toInt(&ok, 16);
                        if (!ok) {
                            return syntaxError("parseString", jsonString, position+1, nesting, ErrorInvalidHexValue);
                        }
                        result.append(QChar(value));
                        position += 4;
                    }

                default:
                    result.append(jsonString.at(position));
            }
            position++;
        }
        else {
            result.append(jsonString.at(position));
            position++;
        }
    } while(true);
    Q_ASSERT(jsonString.at(position) == '"');
    position++;
    return QVariant(result);
}

QVariant QvJson::parseBool(const QString& jsonString, int& position, int nesting) {
    bool resultBool;
    QString token;

    token = nextToken(jsonString, position);

    if (token == "true") {
        resultBool = true;
    }
    else if (token == "false") {
        resultBool = false;
    }
    else {
        Q_ASSERT(false);
    }
    return QVariant(resultBool);
}

QVariant QvJson::parseNull(const QString& jsonString, int& position, int nesting) {
    QString token;
    token = nextToken(jsonString, position);
    Q_ASSERT(token == "null");
    Q_UNUSED(token);
    return QVariant();
}

QVariant QvJson::parseNumber(const QString& jsonString, int& position, int nesting) {
    int startPosition = position;

    // Allow a leading minus sign
    if (jsonString.at(position) == '-') position++;

    if (position >= jsonString.count()) {
        return syntaxError("parseNumber", jsonString, position, nesting + 1, ErrorEndOfInput);
    }

    // Allow one or more decimal digits
    if (!jsonString.at(position).isDigit()) {
        return syntaxError("parseNumber", jsonString, position, nesting + 1, ErrorInvalidNumericValue);
    }
    while (position < jsonString.count() && jsonString.at(position).isDigit()) {
        position++;
    }
    if (position >= jsonString.count() ||
        (jsonString.at(position) != '.' && jsonString.at(position) != 'e' && jsonString.at(position) != 'E')) {
        bool ok = false;
        int resultInt = jsonString.mid(startPosition, position - startPosition).toInt(&ok);
        if (!ok) {
            return syntaxError("parseNumber", jsonString, position, nesting + 1, ErrorInvalidNumericValue);
        }
        return QVariant(resultInt);
    }

    // Consume any fraction part
    if (jsonString.at(position) == '.') {
        position++;
        while (position < jsonString.count() && jsonString.at(position).isDigit()) {
            position++;
        }
    }

    // Consume any exponent part
    if (jsonString.at(position) == 'e' || jsonString.at(position) == 'E') {
        position++;
        // Consume +/- if present
        if (jsonString.at(position) == '+' || jsonString.at(position) == '-') {
            position++;
        }
        // Must have at least one digit
        if (!jsonString.at(position).isDigit()) {
            return syntaxError("parseNumber", jsonString, position, nesting + 1, ErrorInvalidNumericValue);
        }
        while (position < jsonString.count() && jsonString.at(position).isDigit()) {
            position++;
        }
    }

    // Should have a valid floating point value at this point
    bool ok = false;
    qreal resultReal = jsonString.mid(startPosition, position - startPosition).toDouble(&ok);
    if (!ok) {
        return syntaxError("parseNumber", jsonString, position, nesting + 1, ErrorInvalidNumericValue);
    }
    return QVariant(resultReal);
}

QVariant QvJson::syntaxError(const QString& method, const QString& jsonString, int position, int nesting, Error error) {
    QString text;
    switch (error) {
        case ErrorInvalidNumericValue:
            text = QObject::tr("Invalid numeric value");
            break;
        case ErrorInvalidHexValue:
            text = QObject::tr("Invalid hex value");
            break;
        case ErrorEndOfInput:
            text = QObject::tr("Unexpected end of input");
            break;
        case ErrorNoClosingQuote:
            text = QObject::tr("No closing quote for literal string");
            break;
        case ErrorMissingColon:
            text = QObject::tr("Missing ':' between attribute name and value");
            break;
        case ErrorMissingCommaOrClose:
            text = QObject::tr("Missing comma, '}', or ']'");
            break;
        case ErrorMustBeString:
            text = QObject::tr("The name of an attribute must be a valid character string");
            break;
        case ErrorUnrecognizedToken:
            text = QObject::tr("The token was not recognized");
            break;
    default:
            Q_ASSERT(false);
    }
    QString errorMsg = QObject::tr("*** Error %1 in QvJson::%2 at position %3 -- %4").arg(error).arg(method).arg(position).arg(text);
    QStringList errorList;
    errorList << errorMsg;
    return QVariant(errorList);
}

QString QvJson::encodeJson(const QVariant& jsonObject) {
    QVariant::Type type = jsonObject.type();
    switch (type) {
        case QVariant::Map:
            return encodeObject(jsonObject);
        case QVariant::List:
            return encodeArray(jsonObject);
        case QVariant::String:
            return encodeString(jsonObject);
        case QVariant::Int:
        case QVariant::Double:
            return encodeNumeric(jsonObject);
        case QVariant::Bool:
            return encodeBool(jsonObject);
        case QVariant::Invalid:
            return encodeNull(jsonObject);
        default:
            return encodingError("encodeJson", jsonObject, ErrorUnrecognizedObject);
    }
}

QString QvJson::encodeObject(const QVariant& jsonObject) {
    QString result("{ ");
    QMap<QString, QVariant> map = jsonObject.toMap();
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        result.append(encodeString(i.key()));

        result.append(" : ");

        result.append(encodeJson(i.value()));

        if (i.hasNext()) {
            result.append(", ");
        }
    }
    result.append(" }");
    return result;
}

QString QvJson::encodeArray(const QVariant& jsonObject) {
    QString result("[ ");
    QList<QVariant> list = jsonObject.toList();
    for (int i = 0; i < list.count(); i++) {
        result.append(encodeJson(list.at(i)));
        if (i+1 < list.count()) {
            result.append(", ");
        }
    }
    result.append(" ]");
    return result;
}

QString QvJson::encodeString(const QVariant &jsonObject) {
    return encodeString(jsonObject.toString());
}

QString QvJson::encodeString(const QString& value) {
    QString result = "\"";
    for (int i = 0; i < value.count(); i++) {
        ushort chr = value.at(i).unicode();
        if (chr < 32) {
            switch (chr) {
                case '\b':
                    result.append("\\b");
                    break;
                case '\f':
                    result.append("\\f");
                    break;
                case '\n':
                    result.append("\\n");
                    break;
                case '\r':
                    result.append("\\r");
                    break;
                case '\t':
                    result.append("\\t");
                    break;
                default:
                    result.append("\\u");
                    result.append(QString::number(chr, 16).rightJustified(4, '0'));
            }  // End switch
        }
        else if (chr > 255) {
            result.append("\\u");
            result.append(QString::number(chr, 16).rightJustified(4, '0'));
        }
        else {
            result.append(value.at(i));
        }
    }
    result.append('"');
    QString displayResult = result;  // For debug, since "result" often doesn't show
    Q_UNUSED(displayResult);
    return result;
}

QString QvJson::encodeNumeric(const QVariant& jsonObject) {
    return jsonObject.toString();
}

QString QvJson::encodeBool(const QVariant& jsonObject) {
    return jsonObject.toString();
}

QString QvJson::encodeNull(const QVariant& jsonObject) {
    return "null";
}

QString QvJson::encodingError(const QString& method, const QVariant& jsonObject, Error error) {
    QString text;
    switch (error) {
        case ErrorUnrecognizedObject:
            text = QObject::tr("Unrecognized object type");
            break;
    default:
            Q_ASSERT(false);
    }
    return QObject::tr("*** Error %1 in QvJson::%2 -- %3").arg(error).arg(method).arg(text);
}
