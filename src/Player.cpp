#include "Player.h"
#include "LoadTexture.h"
//#include "MakeFixture.h" // TODO
#include "globals.h"

Player::Player() : rect(250, 700, 20, 30), /*_standingTexture(NULL),*/ _walkingTexture(NULL), _frameIndex(0), _walkingLeft(false), _lastPos(rect.topLeft()), touchingGround(false)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(rect.x * Physics::METERS_PER_PIXEL, rect.y * Physics::METERS_PER_PIXEL);
    body = Physics::world.CreateBody(&bodyDef);
    body->SetUserData(this);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((rect.w - 4) * Physics::METERS_PER_PIXEL)/2.0, (2 * Physics::METERS_PER_PIXEL)/2.0, b2Vec2(0.0, (rect.h/2 - 2) * Physics::METERS_PER_PIXEL), 0.0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 2;

    fixture = body->CreateFixture(&fixtureDef);
    // fixture->SetFriction(100.0);

    fixtureDef.friction = 0.0;
    dynamicBox.SetAsBox((rect.w * Physics::METERS_PER_PIXEL)/2.0, ((rect.h - 4) * Physics::METERS_PER_PIXEL)/2.0, b2Vec2(0.0, -2 * Physics::METERS_PER_PIXEL), 0.0);
    body->CreateFixture(&fixtureDef);
}

Player::~Player()
{
    // if (_standingTexture)
        // SDL_DestroyTexture(_standingTexture);
    if (_walkingTexture)
        SDL_DestroyTexture(_walkingTexture);
}

bool Player::init()
{
    // _standingTexture = LoadTexture("assets/standing.png");
    _walkingTexture = LoadTexture("assets/running.png");
    return _walkingTexture;
}

void Player::walkLeft()
{
    b2Vec2 vel = player.body->GetLinearVelocity();
    vel.x = -3.0;
    body->SetLinearVelocity(vel);
    _walkingLeft = true;
}

void Player::walkRight()
{
    b2Vec2 vel = player.body->GetLinearVelocity();
    vel.x = 3.0;
    body->SetLinearVelocity(vel);
    _walkingLeft = false;
}

void Player::jump()
{
    if (touchingGround)
    {
        b2Vec2 vel = player.body->GetLinearVelocity();
        vel.y = -4.0;
        body->SetLinearVelocity(vel);
    }
}

void Player::advance()
{
    const Point newPos = rect.topLeft();
    static const int PIXELS_PER_FRAME = 4;
    const int frameDelta = std::abs((newPos - _lastPos).x)/PIXELS_PER_FRAME;
    if (frameDelta)
    {
        _lastPos = newPos;
        _frameIndex++;
        if (_frameIndex >= 16)
            _frameIndex = 0;
    }
}

void Player::draw(const Point &offset) const
{
    // const b2Vec2 vel = player.body->GetLinearVelocity();
    const SDL_Rect dst = rect.translated(-offset).toSDL();
    // SDL_SetRenderDrawColor(renderer, 237, 191, 157, 255);
    // SDL_RenderDrawRect(renderer, &dst);
    /*if (std::abs(vel.x) < 0.01 && std::abs(vel.y) < 0.01)
    {
        SDL_RenderCopy(renderer, _standingTexture, NULL, &dst);
    }
    else*/
    {
        const SDL_Rect src = {32*_frameIndex, 0, 32, 64};
        // SDL_QueryTexture(_walkingTexture, NULL, NULL, &w, &h);
        SDL_RenderCopyEx(renderer, _walkingTexture, &src, &dst, 0.0, NULL, _walkingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
}
