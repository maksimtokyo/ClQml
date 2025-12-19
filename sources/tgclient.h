#ifndef TGCLIENT_H
#define TGCLIENT_H


#include "td/telegram/Client.h"
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "sources/debugmodel.h"
#include "sources/musiclist.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <queue>

#include <QMetaObject>
#include <QThread>
#include <QDebug>
#include <QtQml/qqmlregistration.h>

template <typename... Ls>
struct overload : Ls...{
    explicit overload(Ls... ls) noexcept : Ls{std::move(ls)}...{}
    using Ls::operator()...;
};

template<class... F>
auto overloaded(F... f)
{
    return overload<F...>(f...);
}


using Object = td::td_api::object_ptr<td::td_api::Object>;


class tgclient : public QObject
{

    Q_OBJECT

public:

    tgclient(QObject *parent = nullptr);

    void postCommand(std::function<void()> cmd);
    void send_query(td::td_api::object_ptr<td::td_api::Function> f,  std::function<void(Object)> handler);

    std::uint64_t  get_bot_id() {return bot_user_id;}
    std::queue<int> music_queue;
    DebugModel *m_deblist;


public slots:
    void loop();

signals:

    void send_id_aut(int32_t idaut);
    void addAudio(music msg);
    void setPath(int index, QString path);

private:

    void restart();
    void check_authentication_error(Object object);
    void on_authorization_state_update();
    void process_response(td::ClientManager::Response response);
    void processCommands();
    std::function<void(Object)> create_authentication_query_handler();
    void process_update(Object update);

    std::unique_ptr<td::ClientManager> client_manager_;
    std::int32_t client_id_{0};
    td::td_api::object_ptr<td::td_api::AuthorizationState> authorization_state_;
    bool are_authorized_{false};
    bool need_restart_{false};
    std::uint64_t current_query_id_{0};
    std::uint64_t authentication_query_id_{0};
    std::mutex m_mutex;
    std::queue<std::function<void()>> m_queue;
    std::map<std::uint64_t, std::function<void(Object)>> handlers_;
    std::map<std::int64_t, td::td_api::object_ptr<td::td_api::user>> users_;
    std::map<std::int64_t, std::string> chat_title_;
    std::uint64_t next_query_id();
    std::uint64_t bot_user_id{0};
};

#endif // TGCLIENT_H
