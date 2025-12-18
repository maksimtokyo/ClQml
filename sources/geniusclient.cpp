#include "geniusclient.h"

static constexpr auto authorizationUrl = "https://api.genius.com/oauth/authorize";
static constexpr auto accessTokenUrl = "https://api.genius.com/oauth/token";

geniusclient::geniusclient(QString clientId, QString clientSecret, QObject *parent) \
    : clientId(clientId), clientSecret(clientSecret), QObject(parent)
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(QHostAddress::Any, 8080, this);

    oauth2.setReplyHandler(replyHandler);
    oauth2.setAuthorizationUrl(QUrl(authorizationUrl));
    oauth2.setClientIdentifier(this->clientId);
    oauth2.setClientIdentifierSharedKey(this->clientSecret);

    oauth2.setModifyParametersFunction([this](QAbstractOAuth::Stage stage,
                                       QMultiMap<QString, QVariant> *parameters){
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization){
            this->codeVerifier = generateCodeVerifier();
            QString challenge = generateCodeChallenge(this->codeVerifier);
            parameters->insert("code_challenge", challenge);
            parameters->insert("code_challenge_method", "S256");
            parameters->insert("response_type", "code");

            parameters->insert("client_id", this->clientId);
            parameters->insert("redirect_uri", "http://localhost:8080");
            parameters->insert("scope", "me create_annotation manage_annotation vote");
        }

        if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {

            parameters->insert("code_verifier", this->codeVerifier);
            parameters->insert("client_id", this->clientId);
            parameters->insert("client_secret", this->clientSecret);
            parameters->insert("grant_type", "authorization_code");
            parameters->insert("redirect_uri", "http://localhost:8080");
        }
    });

}

QString geniusclient::generateCodeVerifier(){
    return generateRandomString(123);
}

QString geniusclient::generateRandomString(int length){
    const QString possibleCharacters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";

    QString randomString;
    randomString.reserve(length);

    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        randomString.append(possibleCharacters.at(index));
    }

    return randomString;
}

QString geniusclient::generateCodeChallenge(const QString &verifier){
    QByteArray hash = QCryptographicHash::hash(
        verifier.toUtf8(),
        QCryptographicHash::Sha256
    );
    return QString(hash.toBase64(QByteArray::Base64UrlEncoding |
                                 QByteArray::OmitTrailingEquals));
}
