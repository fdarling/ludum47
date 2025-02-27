#include "Player.h"
#include "LoadTexture.h"
//#include "MakeFixture.h" // TODO
#include "globals.h"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_prismatic_joint.h>

Player::Player() : rect(250, 700, 20, 30), /*_standingTexture(NULL),*/ _walkingTexture(NULL), _frameIndex(0), _walkingLeft(false), _jetpackOn(false), _grappling(false), _lastPos(rect.topLeft()), body(nullptr), fixture(nullptr), hanging_joint(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(rect.x * Physics::METERS_PER_PIXEL, rect.y * Physics::METERS_PER_PIXEL);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((rect.w * Physics::METERS_PER_PIXEL)/2.0, (rect.h * Physics::METERS_PER_PIXEL)/2.0, b2Vec2(0.0, 0.0), 0.0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 2.0;

    fixture = body->CreateFixture(&fixtureDef);
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
    // if (!standing_on.empty())
    // if (!standing_on.empty() || blocked_left_by.empty())
    {
        // std::cout << "SETTING LEFT VEL" << std::endl;
        body->SetLinearVelocity(vel);
    }
    _walkingLeft = true;
}

void Player::walkRight()
{
    b2Vec2 vel = player.body->GetLinearVelocity();
    vel.x = 3.0;
    // if (!standing_on.empty())
    // if (!standing_on.empty() || blocked_right_by.empty())
    {
        // std::cout << "SETTING RIGHT VEL" << std::endl;
        body->SetLinearVelocity(vel);
    }
    _walkingLeft = false;
}

void Player::setJetpack(bool on)
{
    _jetpackOn = on;
}

void Player::setGrappling(bool on)
{
    _grappling = on;
}

void Player::jump()
{
    if (isStandingOnGround())
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

    if (_jetpackOn)
    {
        const b2Vec2 vel = body->GetLinearVelocity();
        const float mass = body->GetMass();
        static const float MAX_JETPACK_FORCE = 20.0; // not including the gravity counteracting force
        static const float MAX_JETPACK_SPEED = 5;
        float extra = std::max(0.0, MAX_JETPACK_FORCE*(1.0 + vel.y/MAX_JETPACK_SPEED));
        body->ApplyForceToCenter(b2Vec2(0.0, (-9.8f - extra) * mass), true);
    }

    if (!_grappling && hanging_joint)
    {
        Physics::world.DestroyJoint(hanging_joint);
        hanging_joint = nullptr;
    }
    else if (_grappling && !hanging_joint && !hanging_under.empty())
    {
        const b2Vec2 &player_pos = body->GetPosition();
        const float player_width = (rect.w * Physics::METERS_PER_PIXEL);
        const float player_height = (rect.h * Physics::METERS_PER_PIXEL);
        b2EdgeShape * const edge_shape = *hanging_under.begin();

        // calculate the direction unit vector of the edge
        b2Vec2 dir = edge_shape->m_vertex1 - edge_shape->m_vertex2;
        const float edge_len = dir.Normalize();

        // calculate the top-middle of the player
        const b2Vec2 player_top_middle = player_pos + b2Vec2(0.0, -player_height/2.0);
        b2Vec2 player_top_dir = player_top_middle - edge_shape->m_vertex2;
        const float player_distance = player_top_dir.Normalize();

        // calculate the dot product to determine if we are facing the same direction as the edge direction vector
        // this is needed to make sure that length checking is talking about the same direction (sign error)
        const float player_dot_product = b2Dot(player_top_dir, dir);

        b2PrismaticJointDef testJointDef;
        testJointDef.Initialize(world.groundBody, player.body, player_top_middle, dir);
        testJointDef.enableLimit = true;
        testJointDef.lowerTranslation = -player_distance + player_width/2.0;
        testJointDef.upperTranslation = -player_distance + edge_len - player_width/2.0;
        if (testJointDef.lowerTranslation > testJointDef.upperTranslation)
            std::swap(testJointDef.lowerTranslation, testJointDef.upperTranslation);
        if (player_dot_product >= 0.0 && player_distance >= player_width/2.0 && player_distance <= edge_len - player_width/2.0)
            hanging_joint = Physics::world.CreateJoint(&testJointDef);
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
    /*if (touchingGround)
    {
        for (int i = 0; i < 1; i++)
        {
            printf("%d: %f, %f\n", i, contactWorldManifold.points[i].x, contactWorldManifold.points[i].y);
            const RectF r = RectF(contactWorldManifold.points[i].x-4, contactWorldManifold.points[i].y-4, 8, 8).scaled(Physics::PIXELS_PER_METER);//.translated(-offset);
            // const RectF r = RectF(contactWorldManifold.points[i].x, contactWorldManifold.points[i].y, 1, 1);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            const SDL_Rect dst2 = r.translated(-offset).toSDL();
            printf("\tdsta: %d, %d\n", rect.x, rect.y);
            printf("\tdstb: %f, %f\n", r.x, r.y);
            printf("\tdst1: %i, %i\n", dst.x, dst.y);
            printf("\tdst2: %i, %i\n", dst2.x, dst2.y);
            // SDL_RenderDrawRect(renderer, &dst);
            SDL_RenderDrawRect(renderer, &dst2);
        }
    }*/
}
