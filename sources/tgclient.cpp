#include "sources/tgclient.h"

static constexpr auto bot_name = "musicliobot";

tgclient::tgclient(QObject *parent) : QObject(parent)
{
    td::ClientManager::execute(td::td_api::make_object<td::td_api::setLogVerbosityLevel>());
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
    send_query(td::td_api::make_object<td::td_api::getOption>("version"), {});
    m_deblist = new DebugModel();
}

void tgclient::restart() {
    client_manager_.reset();
}

void tgclient::check_authentication_error(Object object) {
    if (object->get_id() == td::td_api::error::ID) {
        auto error = td::move_tl_object_as<td::td_api::error>(object);
        on_authorization_state_update();
    }
}

std::uint64_t tgclient::next_query_id() {
    return ++current_query_id_;
}

std::function<void(Object)>  tgclient::create_authentication_query_handler() {
    return [this, id = authentication_query_id_](Object object) {
    if (id == authentication_query_id_) {
        check_authentication_error(std::move(object));
    }
  };
}

void tgclient::send_query(td::td_api::object_ptr<td::td_api::Function> f,  std::function<void(Object)> handler) {
    auto query_id = next_query_id();
    if (handler) {
        handlers_.emplace(query_id, std::move(handler));
    }
    client_manager_->send(client_id_, query_id, std::move(f));
}

void tgclient::on_authorization_state_update(){
    authentication_query_id_++;

    td::td_api::downcast_call(*authorization_state_,
                      overloaded(
                            [this](td::td_api::authorizationStateReady &){
                                are_authorized_ = true;
                                send_id_aut(authorization_state_->get_id());

                                send_query(td::td_api::make_object<td::td_api::searchPublicChat>(bot_name),
                                  [this](Object obj){
                                      if (obj->get_id() == td::td_api::chat::ID){
                                          auto chat = td::move_tl_object_as<td::td_api::chat>(obj);
                                          auto* type = static_cast<td::td_api::chatTypePrivate*>(chat->type_.get());
                                          bot_user_id = type->user_id_;

                                          auto request = td::td_api::make_object<td::td_api::getChatHistory>();
                                          request->chat_id_ = bot_user_id;
                                          request->from_message_id_ = 0;
                                          request->offset_ = 0;
                                          request->limit_ = 50;
                                          request->only_local_ = false;

                                          send_query(std::move(request),
                                              [this](Object ob) {
                                                  auto result = td::move_tl_object_as<td::td_api::messages>(ob);
                                                  for (auto &msg : result->messages_) {
                                                      if (msg->content_->get_id() == td::td_api::messageAudio::ID) {
                                                          auto audio_msg = static_cast<td::td_api::messageAudio*>(msg->content_.get());
                                                          QString file_name = QString::fromStdString(audio_msg->audio_->file_name_);
                                                          emit addAudio({file_name, audio_msg->audio_->audio_->id_});
                                                      }
                                                }
                                            });
                                      }
                                });

                                send_query(td::td_api::make_object<td::td_api::setOption>(
                                    "disable_chat_history",
                                    td::td_api::make_object<td::td_api::optionValueBoolean>(true)
                                ), {});

                                qDebug() << "Now authorizationStateReady";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateLoggingOut &){
                                qDebug() << "Now authorizationStateLoggingOut";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateClosing &){
                                qDebug() << "Now authorizationStateClosing";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateClosed &){
                                qDebug() << "Now authorizationStateClosed";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateWaitPhoneNumber &) {
                                qDebug() << "Now authorizationStateWaitPhoneNumber";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateWaitEmailAddress &) {
                                qDebug() << "Now authorizationStateWaitEmailAddress";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateWaitEmailCode &) {
                                qDebug() << "Now authorizationStateWaitEmailCode";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateWaitCode &) {
                                qDebug() << "Now authorizationStateWaitCode";
                                send_id_aut(authorization_state_->get_id());
                            },

                            [this](td::td_api::authorizationStateWaitPassword &) {
                                qDebug() << "Now authorizationStateWaitPassword";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateWaitPremiumPurchase &) {
                                qDebug() << "Now authorizationStateWaitPremiumPurchase";
                                send_id_aut(authorization_state_->get_id());
                            },
                            [this](td::td_api::authorizationStateWaitOtherDeviceConfirmation &){
                                qDebug() << "Now authorizationStateWaitOtherDeviceConfirmation";
                            },
                            [this](td::td_api::authorizationStateWaitRegistration &){
                                qDebug() << "Now authorizationStateWaitRegistration";

                            },
                            [this](td::td_api::authorizationStateWaitTdlibParameters &) {
                                auto request = td::td_api::make_object<td::td_api::setTdlibParameters>();
                                request->database_directory_ = "tdlib";
                                request->use_message_database_ = true;
                                request->use_secret_chats_ = true;
                                request->api_id_ = 24056062;
                                request->api_hash_ = "bf793de4c780164ebc16d5fce390e69e";
                                request->system_language_code_ = "en";
                                request->device_model_ = "Desktop";
                                request->application_version_ = "1.0";
                                send_query(std::move(request), create_authentication_query_handler());
                            }));
}

void tgclient::process_update(Object update)
{
    td::td_api::downcast_call(
        *update, overloaded(
        [this](td::td_api::updateAuthorizationState &update_authorization_state) {
            authorization_state_ = std::move(update_authorization_state.authorization_state_);
            on_authorization_state_update();
        },
        [this](td::td_api::updateNewMessage &update_new_m){
            if (update_new_m.message_->chat_id_ != bot_user_id)
                return;


            auto &msg = update_new_m.message_;

            if (msg->content_->get_id() == td::td_api::messageAudio::ID) {
                auto audio_msg = static_cast<td::td_api::messageAudio*>(msg->content_.get());
                QString file_name = QString::fromStdString(audio_msg->audio_->file_name_);
                emit addAudio({file_name, audio_msg->audio_->audio_->id_});
            }
        },
        [this](td::td_api::updateChatLastMessage &u_chat_last_m){
            if (u_chat_last_m.chat_id_ != bot_user_id)
                return;
        },
        [this](td::td_api::updateNewChat &update_new_c){
            if (update_new_c.chat_->id_ != bot_user_id)
                return;
        },
        [this](td::td_api::updateFile &update_file){
            auto &file = update_file.file_;

            int file_id = file->id_;
            int downloaded = file->local_->downloaded_size_;
            int total = file->size_;

            qDebug() << "Downloading file" << file_id
                     << downloaded << "/" << total;

            if (file->local_->is_downloading_completed_) {
                emit setPath(music_queue.front(), QString::fromStdString(file->local_->path_));
            }
        },
        [](auto &update) {}
        ));
}

void tgclient::process_response(td::ClientManager::Response response) {
    if (!response.object) {
        return;
    }

    if (response.request_id == 0 || response.object->get_id() == td::td_api::updateFile::ID) {
        return process_update(std::move(response.object));
    }

    auto it = handlers_.find(response.request_id);
    if (it != handlers_.end()) {
        it->second(std::move(response.object));
        handlers_.erase(it);
    }
}

void tgclient::processCommands(){
    std::queue<std::function<void()>> localQueue;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::swap(localQueue, m_queue);
    }

    while (!localQueue.empty()) {
        auto cmd = localQueue.front();
        localQueue.pop();
        cmd();
    }
}

void tgclient::loop()
{
    while (true)
    {
        processCommands();
        process_response(client_manager_->receive(1));
    }
}

void tgclient::postCommand(std::function<void()> cmd){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(cmd));
}
