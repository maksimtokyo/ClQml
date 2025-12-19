#include "tgcontroller.h"

TgController::TgController(QObject *parent)
    : QObject(parent),
      client(new tgclient),
      m_mlist(new musiclist(this)),
      db(new dbuser(this)),
      m_cluser(new ClUser(this))
{

    client->moveToThread(&wThread);

    connect(&wThread, &QThread::started, client, &tgclient::loop);
    connect(&wThread, &QThread::finished, client, &QObject::deleteLater);
    connect(client, &tgclient::send_id_aut, this, &TgController::get_id_aut);
    connect(this, &TgController::idChanged, this,  &TgController::idautprocessing);
    connect(this, &TgController::phoneNumberChanged, this, &TgController::idautprocessing);
    connect(this, &TgController::codeChanged, this, &TgController::idautprocessing);
    connect(this, &TgController::passwordChanged, this, &TgController::idautprocessing);
    connect(client, &tgclient::addAudio, m_mlist, &musiclist::addmusic, Qt::QueuedConnection);
    connect(this, &TgController::installMusicSignal, this, &TgController::installMusic);

    wThread.start();
}

void TgController::get_id_aut(int32_t idaut){
    m_curid = idaut;
    emit idChanged(OUT);
}

void TgController::idautprocessing(TypeSignal type = OUT){
    switch (m_curid) {
        case td::td_api::authorizationStateReady::ID:{
            m_view = "qmlfiles/authorizationreadyview.qml";
            emit viewChanged();

            break;
        }
        case td::td_api::authorizationStateLoggingOut::ID:

            break;

        case td::td_api::authorizationStateClosing::ID:
            break;

        case td::td_api::authorizationStateClosed::ID:
            break;

        case td::td_api::authorizationStateWaitPhoneNumber::ID:{
            if (type != IN){
                m_view = "qmlfiles/phoneview.qml";
                emit viewChanged();
            } else{
                auto cmd = [this, phone = m_phoneNumber]() {
                    auto request = td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(phone.toStdString(), nullptr);
                    this->client->send_query(std::move(request), {});
                };
                client->postCommand(std::move(cmd));
            }
            break;
        }

        case td::td_api::authorizationStateWaitEmailAddress::ID:{
            if (type != IN){

            } else{
            }
            break;
        }

        case td::td_api::authorizationStateWaitEmailCode::ID:{
        }

        case td::td_api::authorizationStateWaitCode::ID:{
            if (type != IN){
                m_view = "qmlfiles/codeview.qml";
                emit viewChanged();
            } else{
                auto cmd = [this, code = m_code](){
                    auto request = td::td_api::make_object<td::td_api::checkAuthenticationCode>(code.toStdString());
                    this->client->send_query(std::move(request), {});
                };
                client->postCommand(std::move(cmd));
            }
            break;
        }

        case td::td_api::authorizationStateWaitPassword::ID:{
            if (type != IN){
                m_view = "qmlfiles/passwordview.qml";
                emit viewChanged();
            } else{
                auto cmd = [this, password = m_password](){
                    auto request = td::td_api::make_object<td::td_api::checkAuthenticationPassword>(password.toStdString());
                    this->client->send_query(std::move(request), {});
                };
                client->postCommand(std::move(cmd));
            }
            break;
        }

        case td::td_api::authorizationStateWaitTdlibParameters::ID:
            break;

        default:
            break;
    }
}

void TgController::setphoneNumber(const QString &phoneNumber){
    if (m_cluser->getphonenumber()!= phoneNumber){
        m_cluser->setphonenumber(phoneNumber);
        emit phoneNumberChanged(IN);
    }
}

QString TgController::phoneNumber() const{
    return m_cluser->getphonenumber();
}

TgController::~TgController(){}

QString TgController::view(){
    return m_view;
}


QString TgController::code(){
    return m_code;
}

void TgController::setcode(const QString &code){
    if (m_code != code){
        m_code = code;
        emit codeChanged(IN);
    }
}

QString TgController::password(){
    return m_password;
}

void TgController::setpassword(const QString &password){
    if (m_password != password){
        m_password = password;
        emit passwordChanged(IN);
    }
}

void TgController::installMusic(int id_m)
{
    auto cmd = [this, idmusic = m_mlist->getidmusic(id_m), index = id_m](){
       auto request = td::td_api::make_object<td::td_api::downloadFile>();
       request->file_id_ = idmusic;
       request->priority_  = 1;
       request->offset_ = 0;
       request->limit_ = 0;
       request->synchronous_ = false;

       this->client->music_queue.push(index);

       this->client->send_query(std::move(request), [this, mindex = index](Object obj){
           if (obj->get_id() == td::td_api::file::ID){
               auto file = td::move_tl_object_as<td::td_api::file>(obj);
               this->m_mlist->m_musics[mindex].musicpath = QString::fromStdString(file->local_->path_);
           }
       });
    };

    client->postCommand(cmd);
}

void TgController::setPath(int index, QString path){
    m_mlist->setPath(index, path);
}

void TgController::sendlink(QString link){

    auto cmd = [this, cmdlink = link](){
        std::string text = cmdlink.toStdString();
        auto request = td::td_api::make_object<td::td_api::sendMessage>();
        request->chat_id_ = this->client->get_bot_id();
        auto  message = td::td_api::make_object<td::td_api::inputMessageText>();
        message->text_ = td::td_api::make_object<td::td_api::formattedText>();
        message->text_->text_ = std::move(text);
        request->input_message_content_ = std::move(message);
        this->client->send_query(std::move(request), {});
    };

    client->postCommand(std::move(cmd));
}

