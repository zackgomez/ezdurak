#include "MenuState.h"
#include "net/NetworkHost.h"
#include "net/NetworkClient.h"
#include "net/NetworkGame.h"
#include "net/NetworkPlayer.h"
#include "GUIApp.h"
#include "InGameState.h"
#include "QuitState.h"
#include "ai/AIPlayer.h"
#include <iostream>

MenuState::MenuState() :
    host_(0),
    client_(0),
    ready_(false)
{
}

GUIStatePtr MenuState::create()
{
    GUIStatePtr ret(new MenuState());
    return ret;
}

MenuState::~MenuState()
{
}

void MenuState::render()
{
    if (!ready_)
    {
        joinstr_ = GUIString::create("Press J to join a LAN game.");
        hoststr_ = GUIString::create("Press H to host a LAN game.");
        singlestr_ = GUIString::create("Press N for a single player game.");
        statusstr_ = GUIString::create("Idle.");
        ready_ = true;
    }

    // Check for connections
    kissnet::tcp_socket_ptr sock;
    if (host_.get())
    {
        assert(!client_.get());
        kissnet::tcp_socket_ptr sock = host_->getConnection("_EZDurak 4player JOIN NOW!!!");
        if (sock.get())
        {
            NetworkPlayerPtr netp(new NetworkPlayer());
            if (!netp->getConnection(sock))
            {
                std::cerr << "ERROR - MenuState: Error during NetworkPlayer::getConnection\n";
                return;
            }

            game_ = GamePtr(new Game());
            game_->addPlayer(netp);
            PlayerPtr aip1(new AIPlayer("AIPlayer1"));
            game_->addPlayer(aip1);
            PlayerPtr aip2(new AIPlayer("AIPlayer2"));
            game_->addPlayer(aip2);

            next_ = InGameState::create(game_);
        }
    }

    if (client_.get())
    {
        assert(!host_.get());
        std::set<NetworkClient::Connection> conns = client_->getConnectionList();
        if (!conns.empty())
        {
            kissnet::tcp_socket_ptr sock = kissnet::tcp_socket::create();;
            NetworkClient::Connection conn = *conns.begin();
            std::cout << "Trying " << conn.addr << " on " << conn.port << '\n';
            sock->connect(conn.addr, conn.port);

            NetworkGame *ng = new NetworkGame();
            game_ = GamePtr(ng);
            if (!ng->connectTo(sock))
            {
                std::cerr << "ERROR - MenuState: Error during NetworkGame::connectTo\n";
                return;
            }

            next_ = InGameState::create(game_);
        }
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glColor3f(1,1,1);

    // Draw status string
    glLoadIdentity();
    glTranslatef(15 + statusstr_->getWidth() / 2, 15 + statusstr_->getHeight() / 2, 0);
    statusstr_->draw();

    // Draw menu strings
    glLoadIdentity();
    glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);
    glPushMatrix();
    glTranslatef(0, -40 - singlestr_->getHeight(), 0);
    singlestr_->draw();
    glPopMatrix();

    hoststr_->draw();

    glTranslatef(0, 20 + joinstr_->getHeight(), 0);
    joinstr_->draw();
}

void MenuState::processEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        next_ = QuitState::create();
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n)
    {
        game_ = GamePtr(new Game());

        PlayerPtr aip1(new AIPlayer("AIPlayer1"));
        PlayerPtr aip2(new AIPlayer("AIPlayer2"));
        PlayerPtr aip3(new AIPlayer("AIPlayer3"));
        game_->addPlayer(aip1);
        game_->addPlayer(aip2);
        game_->addPlayer(aip3);

        next_ = InGameState::create(game_);
    }
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_h)
    {
        host_.reset();
        client_.reset();
        host_.reset(new NetworkHost("12345", "54321"));

        statusstr_ = GUIString::create("Hosting.");
    }
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_j)
    {
        host_.reset();
        client_.reset();

        client_.reset(new NetworkClient("12345"));
        client_->listen();
        statusstr_ = GUIString::create("Looking for game to join.");
    }
}

bool MenuState::needsTransition() const
{
    return next_.get();
}

GUIStatePtr MenuState::nextState()
{
    if (next_.get())
        return next_;
    else
        assert(false);
}
