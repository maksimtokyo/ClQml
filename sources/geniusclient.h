#ifndef GENIUSCLIENT_H
#define GENIUSCLIENT_H


#include <QOAuth2AuthorizationCodeFlow>
#include <QNetworkReply>
#include <QOAuthHttpServerReplyHandler>
#include <QRandomGenerator>

class geniusclient : public QObject
{
    Q_OBJECT
public:
    geniusclient(QString clientId, QString clientSecret, QObject* parent = nullptr);

private:

    QString generateCodeVerifier();
    QString generateRandomString(int length);
    QString generateCodeChallenge(const QString &verifier);

    QOAuth2AuthorizationCodeFlow oauth2;
    QString codeVerifier;
    QString clientId;
    QString clientSecret;
};

#endif // GENIUSCLIENT_H
