#ifndef OAUTHENTICATOR_H
#define OAUTHENTICATOR_H

#include <QDateTime>
#include <QObject>

#include "core/network.h"

class LocalRedirectServer;
class QTcpSocket;

class OAuthenticator : public QObject {
  Q_OBJECT
 public:
  explicit OAuthenticator(QObject* parent = 0);
  void StartAuthorisation();
  void RefreshAuthorisation(const QString& refresh_token);

  // Token to use now.
  const QString& access_token() const { return access_token_; }

  // Token to use to get a new access token when it expires.
  const QString& refresh_token() const { return refresh_token_; }

  const QDateTime& expiry_time() const { return expiry_time_; }
  const QString& user_email() const { return user_email_; }

 signals:
  void Finished();

 private slots:
  void RedirectArrived(LocalRedirectServer* server);
  void FetchAccessTokenFinished(QNetworkReply* reply);
  void RefreshAccessTokenFinished(QNetworkReply* reply);
  void FetchUserInfoFinished(QNetworkReply* reply);

 private:
  QByteArray ParseHttpRequest(const QByteArray& request) const;
  void RequestAccessToken(const QByteArray& code, const QUrl& url);
  void SetExpiryTime(int expires_in_seconds);

  NetworkAccessManager network_;

  QString access_token_;
  QString refresh_token_;
  QDateTime expiry_time_;
  QString user_email_;
};

#endif
