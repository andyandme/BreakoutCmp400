#include "PowerupManager.h"


PowerupManager::PowerupManager(sf::RenderWindow* window, Paddle* paddle, Ball* ball)
    : _window(window), _paddle(paddle), _ball(ball)
{
}

PowerupManager::~PowerupManager()
{
    for (auto powerup : _powerups)
    {
        delete powerup;
    }
    _powerups.clear();
}

void PowerupManager::update(float dt)
{
    // tick down powerup effect time. Reset if elapsed.
    if (_powerupInEffect)
    {
        _powerupInEffect->second -= dt;
        if (_powerupInEffect->second <= 0)
        {
            _powerupInEffect.reset();
        }
    }


    for (auto it = _powerups.begin(); it != _powerups.end(); )
    {
        checkCollision();
        
        // Delete powerups queued for removal
        (*it)->update(dt);
        if (!(*it)->isAlive())
        {
            delete* it;
            it = _powerups.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PowerupManager::render()
{
    for (auto& powerup : _powerups)
    {
        powerup->render();
    }
}

void PowerupManager::spawnPowerup(const sf::Vector2f& pos)
{
    PowerupBase* p = nullptr;

    // TODO finish this.
    switch (rand() % 5)
    {
    case 0:
        p = new PowerupBigPaddle(_window, _paddle, _ball);
        break;
    case 1:
        p = new PowerupSlowBall(_window, _paddle, _ball);
        break;
    case 2:
        p = new PowerupFastBall(_window, _paddle, _ball);
        break;
    case 3:
        p = new PowerupSmallPaddle(_window, _paddle, _ball);
        break;
    case 4:
        p = new PowerupFireBall(_window, _paddle, _ball);
        break;
    case 5:
       break;
    }

    if (p)
    {
        p->setPosition(pos);
        _powerups.push_back(p);
    }
}

void PowerupManager::checkCollision()
{
    for (auto& powerup : _powerups)
    {

        if (powerup->checkCollisionWithPaddle())
        {
            _powerupInEffect = powerup->applyEffect();
            powerup->setAlive(false);
        }
    }
}

int PowerupManager::getPowerupsSpawned()
{
    return _powerups.size();
}

std::pair<POWERUPS, float> PowerupManager::getPowerupInEffect()
{
    if (!_powerupInEffect) return { none, 0.f };
    return *_powerupInEffect;
}
