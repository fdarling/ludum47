#include "Player.h"
#include "Bullet.h"
#include "LoadTexture.h"
//#include "MakeFixture.h" // TODO
#include "globals.h"

#include <box2d/b2_contact.h>
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

int Player::type() const
{
    return GAMEOBJECT_TYPE_PLAYER;
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

void Player::climbUp()
{
    if (!isClimbing())
        return;
    b2Vec2 vel = player.body->GetLinearVelocity();
    vel.y = -3.0;
    body->SetLinearVelocity(vel);
}

void Player::climbDown()
{
    if (!isClimbing())
        return;
    b2Vec2 vel = player.body->GetLinearVelocity();
    vel.y = 3.0;
    body->SetLinearVelocity(vel);
}

void Player::shootBullet()
{
    const b2Vec2 player_pos = body->GetPosition();
    const b2Vec2 player_vel = body->GetLinearVelocity();

    static const float BULLET_X_SPEED = 20.0;
    static const float BULLET_Y_SPEED = -3.0;
    b2Vec2 bullet_vel(_walkingLeft ? -BULLET_X_SPEED : BULLET_X_SPEED, player_vel.y + BULLET_Y_SPEED);

    /*Bullet * const bullet = */new Bullet(player_pos + b2Vec2(std::copysign(rect.w/2.0*Physics::METERS_PER_PIXEL, bullet_vel.x), 0.0), bullet_vel);
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

    if (isClimbing())
    {
        const float mass = body->GetMass();
        body->ApplyForceToCenter(b2Vec2(0.0, -9.8f * mass), false);
    }
    else if (_jetpackOn)
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
        // const float player_height = (rect.h * Physics::METERS_PER_PIXEL);
        b2EdgeShape * const edge_shape = *hanging_under.begin();

        // calculate the direction unit vector of the edge
        b2Vec2 edge_dir = edge_shape->m_vertex2 - edge_shape->m_vertex1;
        const float edge_len = edge_dir.Normalize();

        // calculate where the top-middle of the player would end up along the edge
        const b2Vec2 grab_point = b2Vec2(player_pos.x, edge_shape->m_vertex1.y);
        b2Vec2 grab_point_dir = grab_point - edge_shape->m_vertex1;
        const float grab_point_distance = grab_point_dir.Normalize();

        // calculate the dot product to determine if we are facing the same direction as the edge direction vector
        // this is needed to make sure that length checking is talking about the same direction (sign error)
        const float grab_dot_product = b2Dot(grab_point_dir, edge_dir);

        const float grab_start_distance = player_width/2.0;
        const float grab_end_distance   = edge_len - player_width/2.0;
        const bool same_direction = (grab_dot_product >= 0.0);
        const bool within_range = (grab_point_distance >= grab_start_distance && grab_point_distance <= grab_end_distance);
        // std::cout << "same_direction = " << (int)same_direction << ", within_range = " << (int)within_range << std::endl;
        // std::cout << "grab_point = (" << grab_point.x << ", " << grab_point.y << "); grab_point_dir = (" << grab_point_dir.x << ", " << grab_point_dir.y << "); edge_dir = (" << edge_dir.x << ", " << edge_dir.y << ")" << std::endl;

        if (same_direction && within_range)
        {
            // make sure the player is initially "snapped" to the hanging position
            // const b2Vec2 snapped_player_pos(grab_point.x, grab_point.y + player_height/2.0);
            // player.body->SetTransform(snapped_player_pos, player.body->GetAngle());

            // constraint the player to the ceiling
            b2PrismaticJointDef testJointDef;
            testJointDef.Initialize(world.groundBody, player.body, grab_point, edge_dir);
            testJointDef.enableLimit = true;
            testJointDef.lowerTranslation = -grab_point_distance + grab_start_distance;
            testJointDef.upperTranslation = -grab_point_distance + grab_end_distance;
            if (testJointDef.lowerTranslation > testJointDef.upperTranslation)
                std::swap(testJointDef.lowerTranslation, testJointDef.upperTranslation);
            hanging_joint = Physics::world.CreateJoint(&testJointDef);
        }
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

void Player::beginContact(b2Contact *contact, b2Fixture *other)
{
    GameObject * const obj = reinterpret_cast<GameObject*>(other->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    b2WorldManifold contactWorldManifold;
    contact->GetWorldManifold(&contactWorldManifold);

    if (obj->type() == GAMEOBJECT_TYPE_WORLD)
    {
        // determine if we are standing on the colliding object
        if (contactWorldManifold.normal.y < 0.0 && contactWorldManifold.normal.y < -0.4)
        {
            player.standing_on.insert(other);
            contact->SetFriction(0.95);
        }
        else
        {
            player.standing_on.erase(other);
            contact->SetFriction(0.0);
        }

        if (contactWorldManifold.normal.y > 0.4)
        {
            // std::cout << "Hit Ceiling!" << std::endl;
            b2Shape * const generic_shape = other->GetShape();
            if (generic_shape->GetType() == b2Shape::e_edge)
            {
                b2EdgeShape * const edge_shape = static_cast<b2EdgeShape*>(generic_shape);

                b2Vec2 dir = edge_shape->m_vertex2 - edge_shape->m_vertex1;
                dir.Normalize();

                if (std::abs(dir.y) < 0.01)
                    player.hanging_under.insert(edge_shape);
            }
        }
    }
    else if (obj->type() == GAMEOBJECT_TYPE_LADDER)
    {
        climbing_up.insert(other);
    }
}

void Player::endContact(b2Contact *contact, b2Fixture *other)
{
    (void)contact;

    GameObject * const obj = reinterpret_cast<GameObject*>(other->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    if (obj->type() == GAMEOBJECT_TYPE_WORLD)
    {
        standing_on.erase(other);

        b2Shape * const generic_shape = other->GetShape();
        if (generic_shape->GetType() == b2Shape::e_edge)
        {
            b2EdgeShape * const edge_shape = static_cast<b2EdgeShape*>(generic_shape);
            hanging_under.erase(edge_shape);
        }
    }
    else if (obj->type() == GAMEOBJECT_TYPE_LADDER)
    {
        climbing_up.erase(other);
    }
}
