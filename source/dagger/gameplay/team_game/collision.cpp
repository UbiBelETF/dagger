#include "collision.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "team_game_main.h"

#include <cmath>

using namespace dagger;



void CollisionSystem::Run()
{

    auto viewAll   = Engine::Registry().view<Collision, Transform>();
    auto viewWalls = Engine::Registry().view<CollisionType::Wall>();
    auto viewChars = Engine::Registry().view<CollisionType::Character>();
    auto viewSlimes = Engine::Registry().view<CollisionType::Slime>();
    // Reset generic collision flags
    auto it = viewAll.begin();
    while (it != viewAll.end())
    {
        auto& col = viewAll.get<Collision>(*it);
        col.colided = false;
        it++;
    }

    // Find collisions
    auto it1 = viewAll.begin();
    while (it1 != viewAll.end())
    {
        auto& col1 = viewAll.get<Collision>(*it1);
        auto& tr1  = viewAll.get<Transform>(*it1);

        auto it2 = it1;
        it2++;
        while (it2 != viewAll.end())
        {
            auto& col2 = viewAll.get<Collision>(*it2);
            auto& tr2  = viewAll.get<Transform>(*it2);

            // processing one collision per frame for each colider
            if (col1.IsCollided(tr1.position, col2, tr2.position))
            {
                if ((viewChars.contains(*it1) || viewSlimes.contains(*it1)) && viewWalls.contains(*it2))
                {
                    ResolveCharWall(col1, col2, tr1, tr2);
                } else if (viewWalls.contains(*it1) && (viewChars.contains(*it2)||viewSlimes.contains(*it2)) )
                {
                    ResolveCharWall(col2, col1, tr2, tr1);
                }
                else if ((viewChars.contains(*it1) || viewSlimes.contains(*it1)) && (viewChars.contains(*it2) || viewSlimes.contains(*it2))) {
                    ResolveCharChar(col2, col1, tr2, tr1);
                    ResolveCharChar(col1, col2, tr1, tr2);
                }
                else // Generic collision
                {
                    col1.colided = true;
                    col1.colidedWith = *it2;

                    col2.colided = true;
                    col2.colidedWith = *it1;
                    //Logger::info("Generic collision");
                }
            }
            it2++;
        }
        //if (col1.colided) Logger::info(*it1);
        it1++;
    }
}

void CollisionSystem::ResolveCharWall(Collision& colChar_, Collision& colWall_, Transform& trChar_, Transform& trWall_)
{
    //Logger::info("CharWall collision");
    while (colChar_.IsCollided(trChar_.position, colWall_, trWall_.position))
    {
        Vector2 colSides = colChar_.GetCollisionSides(trChar_.position, colWall_, trWall_.position);
        trChar_.position.x -= colSides.x / 10;
        trChar_.position.y -= colSides.y / 10;
    } 
}
void CollisionSystem::ResolveCharChar(Collision& colChar_, Collision& colChar2_, Transform& trChar_, Transform& trChar2_)
{
    //Logger::info("CharWall collision");
    while (colChar_.IsCollided(trChar_.position, colChar2_, trChar2_.position))
    {
        Vector2 colSides = colChar_.GetCollisionSides(trChar_.position, colChar2_, trChar2_.position);
        trChar_.position.x -= colSides.x / 5;
        trChar_.position.y -= colSides.y / 5;
    }
}

bool Collision::IsCollided(const Vector3& pos_, const Collision& other_, const Vector3& posOther_)
{
    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    if (p.x < p2.x + other_.size.x &&
        p.x + size.x > p2.x&&
        p.y < p2.y + other_.size.y &&
        p.y + size.y > p2.y)
    {
        return true;
    }

    return false;
}

Vector2 Collision::GetCollisionSides(const Vector3& pos_, const Collision& other_, const Vector3& posOther_)
{
    Vector2 res(0, 0);

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    Float32 xDif = std::min(std::abs(p.x + size.x - p2.x), std::abs(p2.x + other_.size.x - p.x));
    Float32 yDif = std::min(std::abs(p.y + size.y - p2.y), std::abs(p2.y + other_.size.y - p.y));

    if (xDif < yDif)
    {
        res.x = std::abs(p.x + size.x - p2.x) < std::abs(p2.x + other_.size.x - p.x) ? 1 : -1;
    }
    else
    {
        res.y = std::abs(p.y + size.y - p2.y) < std::abs(p2.y + other_.size.y - p.y) ? 1 : -1;
    }

    return res;
}

Vector3 Collision::GetCollisionCenter(const Vector3& pos_, const Collision& other_, const Vector3& posOther_)
{
    Vector3 res;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
    res.z = 1.f;

    return res;
}
