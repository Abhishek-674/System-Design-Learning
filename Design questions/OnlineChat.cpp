#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ctime>

using namespace std;

/* ===================== ENUMS ===================== */

enum class UserStatus {
    ONLINE,
    OFFLINE
};

enum class MessageStatus {
    SENT,
    DELIVERED,
    READ
};

/* ===================== FORWARD DECLARATIONS ===================== */

class Message;
class IMessageObserver;

/* ===================== INTERFACES ===================== */

class IMessageObserver {
public:
    virtual void update(Message* msg) = 0;
    virtual ~IMessageObserver() = default;
};

class IMessageSubject {
public:
    virtual void registerObserver(IMessageObserver* obs) = 0;
    virtual void removeObserver(IMessageObserver* obs) = 0;
    virtual void notifyObservers(Message* msg) = 0;
    virtual ~IMessageSubject() = default;
};

/* ===================== NOTIFICATION ===================== */

class Notification {
public:
    string notificationId;
    string userId;
    string text;
    time_t timestamp;
    bool isRead;

    Notification(string uid, string txt)
        : userId(uid), text(txt), isRead(false) {
        timestamp = time(nullptr);
    }
};

/* ===================== USER ===================== */

class User : public IMessageObserver {
private:
    string userId;
    string name;
    UserStatus status;
    vector<Notification> notifications;

public:
    User(string id, string name)
        : userId(id), name(name), status(UserStatus::OFFLINE) {}

    string getId() const { return userId; }
    string getName() const { return name; }
    UserStatus getStatus() const { return status; }

    void setStatus(UserStatus s) {
        status = s;
    }

    void update(Message* msg) override;

    void addNotification(const string& text) {
        notifications.emplace_back(userId, text);
    }
};

/* ===================== MESSAGE ===================== */

class Message {
public:
    string messageId;
    User* sender;
    string content;
    time_t timestamp;
    MessageStatus status;

    Message(string id, User* sender, string content)
        : messageId(id), sender(sender), content(content),
          status(MessageStatus::SENT) {
        timestamp = time(nullptr);
    }

    void markDelivered() {
        status = MessageStatus::DELIVERED;
    }

    void markRead() {
        status = MessageStatus::READ;
    }
};

/* ===================== CHAT ROOM ===================== */

class ChatRoom : public IMessageSubject {
protected:
    string roomId;
    unordered_set<User*> participants;
    unordered_set<IMessageObserver*> observers;
    vector<Message*> messages;

public:
    ChatRoom(string id) : roomId(id) {}

    virtual ~ChatRoom() {
        for (auto msg : messages)
            delete msg;
    }

    void addUser(User* user) {
        participants.insert(user);
        registerObserver(user);
    }

    void removeUser(User* user) {
        participants.erase(user);
        removeObserver(user);
    }

    void registerObserver(IMessageObserver* obs) override {
        observers.insert(obs);
    }

    void removeObserver(IMessageObserver* obs) override {
        observers.erase(obs);
    }

    void notifyObservers(Message* msg) override {
        for (auto obs : observers) {
            obs->update(msg);
        }
    }

    void broadcast(Message* msg) {
        messages.push_back(msg);
        notifyObservers(msg);
    }
};

/* ===================== PRIVATE CHAT ===================== */

class PrivateChat : public ChatRoom {
public:
    PrivateChat(string id, User* u1, User* u2) : ChatRoom(id) {
        addUser(u1);
        addUser(u2);
    }
};

/* ===================== GROUP CHAT ===================== */

class GroupChat : public ChatRoom {
private:
    string groupName;
    User* admin;

public:
    GroupChat(string id, string name, User* admin)
        : ChatRoom(id), groupName(name), admin(admin) {
        addUser(admin);
    }

    void addMember(User* user) {
        addUser(user);
    }

    void removeMember(User* user) {
        if (user != admin)
            removeUser(user);
    }
};

/* ===================== CHAT SERVICE (SINGLETON) ===================== */

class ChatService {
private:
    unordered_map<string, User*> users;
    unordered_map<string, ChatRoom*> rooms;

    ChatService() {}

public:
    static ChatService& getInstance() {
        static ChatService instance;
        return instance;
    }

    void addUser(User* user) {
        users[user->getId()] = user;
    }

    PrivateChat* createPrivateChat(const string& roomId, User* u1, User* u2) {
        auto room = new PrivateChat(roomId, u1, u2);
        rooms[roomId] = room;
        return room;
    }

    GroupChat* createGroupChat(const string& roomId, const string& name, User* admin) {
        auto room = new GroupChat(roomId, name, admin);
        rooms[roomId] = room;
        return room;
    }

    void sendMessage(const string& roomId, const string& senderId, const string& text) {
        if (!rooms.count(roomId) || !users.count(senderId))
            return;

        ChatRoom* room = rooms[roomId];
        User* sender = users[senderId];

        Message* msg = new Message("msg_" + to_string(rand()), sender, text);
        room->broadcast(msg);
    }
};

/* ===================== USER UPDATE IMPLEMENTATION ===================== */

void User::update(Message* msg) {
    if (status == UserStatus::ONLINE) {
        cout << "[" << name << "] received message from "
             << msg->sender->getName() << ": "
             << msg->content << endl;
        msg->markDelivered();
    } else {
        addNotification("New message from " + msg->sender->getName());
    }
}

/* ===================== MAIN ===================== */

int main() {
    ChatService& chatService = ChatService::getInstance();

    User* alice = new User("1", "Alice");
    User* bob = new User("2", "Bob");
    User* charlie = new User("3", "Charlie");

    alice->setStatus(UserStatus::ONLINE);
    bob->setStatus(UserStatus::ONLINE);
    charlie->setStatus(UserStatus::OFFLINE);

    chatService.addUser(alice);
    chatService.addUser(bob);
    chatService.addUser(charlie);

    // Private chat
    chatService.createPrivateChat("room1", alice, bob);
    chatService.sendMessage("room1", "1", "Hi Bob!");

    // Group chat
    GroupChat* group = chatService.createGroupChat("room2", "Friends", alice);
    group->addMember(bob);
    group->addMember(charlie);

    chatService.sendMessage("room2", "2", "Hello everyone!");

    delete alice;
    delete bob;
    delete charlie;

    return 0;
}

/*

INTERFACES
IMessageObserver
+-----------------------------------+
| IMessageObserver                  |
+-----------------------------------+
| +update(msg: Message): void       |
+-----------------------------------+

IMessageSubject
+--------------------------------------------+
| IMessageSubject                            |
+--------------------------------------------+
| +registerObserver(obs: IMessageObserver*): void |
| +removeObserver(obs: IMessageObserver*): void   |
| +notifyObservers(msg: Message*): void           |
+--------------------------------------------+


⚠️ NO data members in interface

🔹 CLASSES
User

IS-A → IMessageObserver

+------------------------------------------------+
| User                                           |
+------------------------------------------------+
| - userId: string                               |
| - name: string                                 |
| - status: UserStatus                           |
| - notifications: vector<Notification>          |
+------------------------------------------------+
| +sendMessage(roomId: string, text: string): void |
| +receiveMessage(msg: Message*): void           |
| +update(msg: Message*): void                   |
| +setStatus(status: UserStatus): void           |
+------------------------------------------------+

Notification

user HAS-A notification 

+---------------------------------------------+
| Notification                                |
+---------------------------------------------+
| - notificationId: string                    |
| - userId: string                            |
| - text: string                              |
| - timestamp: time_t                         |
| - isRead: bool                              |
+---------------------------------------------+

Message

HAS-A → User (sender)

+------------------------------------------------+
| Message                                        |
+------------------------------------------------+
| - messageId: string                            |
| - sender: User*                                |
| - content: string                              |
| - timestamp: time_t                            |
| - status: MessageStatus                        |
+------------------------------------------------+
| +markDelivered(): void                         |
| +markRead(): void                              |
+------------------------------------------------+

ChatRoom (ABSTRACT)

IS-A → IMessageSubject
HAS-A → Users, Messages, Observers

+------------------------------------------------+
| ChatRoom (abstract)                            |
+------------------------------------------------+
| - roomId: string                               |
| - participants: unordered_set<User*>           |
| - observers: unordered_set<IMessageObserver*>  |
| - messages: vector<Message*>                   |
+------------------------------------------------+
| +addUser(user: User*): void                    |
| +removeUser(user: User*): void                 |
| +registerObserver(obs: IMessageObserver*): void|
| +removeObserver(obs: IMessageObserver*): void  |
| +notifyObservers(msg: Message*): void          |
| +broadcast(msg: Message*): void                |
+------------------------------------------------+

PrivateChat

IS-A → ChatRoom

+----------------------------------------------+
| PrivateChat : ChatRoom                       |
+----------------------------------------------+
| +PrivateChat(u1: User*, u2: User*)           |
+----------------------------------------------+


📌 Exactly 2 users

GroupChat

IS-A → ChatRoom

+----------------------------------------------+
| GroupChat : ChatRoom                         |
+----------------------------------------------+
| - groupName: string                          |
| - admin: User*                               |
+----------------------------------------------+
| +addMember(user: User*): void                |
| +removeMember(user: User*): void             |
+----------------------------------------------+

ChatService (Singleton)

HAS-A → Users, ChatRooms

+----------------------------------------------------------------+
| ChatService (Singleton)                                        |
+----------------------------------------------------------------+
| - users: unordered_map<string, User*>                          |
| - rooms: unordered_map<string, ChatRoom*>                     |
+----------------------------------------------------------------+
| +createPrivateChat(u1: User*, u2: User*): PrivateChat*         |
| +createGroupChat(name: string, admin: User*): GroupChat*      |
| +sendMessage(roomId: string, senderId: string, text: string): void |
+----------------------------------------------------------------+
*/