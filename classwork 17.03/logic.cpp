#include "logic.h"
#include "Vector2.h"

void MaterialPoint::UpdatePosition(float dt)
{
	position += velocity * dt;
}

void Map::update(float dt)
{
	for (auto& b : balls)
	{
		if (b.position.x + 2 * b.radius >= size.x)
		{
			b.velocity = Vector2(b.velocity.x * -1, b.velocity.y);
		}
		if (b.position.x <= 0)
		{
			b.velocity = Vector2(b.velocity.x * -1, b.velocity.y);
		}
		if (b.position.y + 2 * b.radius >= size.y)
		{
			b.velocity = Vector2(b.velocity.x, b.velocity.y * -1);
		}
		if (b.position.y <= 0)
		{
			b.velocity = Vector2(b.velocity.x, b.velocity.y * -1);
		}
		b.UpdatePosition(dt);
	}

	for (int i = 0; i < balls.size(); i++)
	{
		for (int j = i + 1; j < balls.size(); j++)
		{
			Vector2 d = balls[j].position - balls[i].position + Vector2(balls[j].radius - balls[i].radius, balls[j].radius - balls[i].radius);
			Vector2 dv = balls[j].velocity - balls[i].velocity;
			if ( (d.len() <= balls[i].radius + balls[j].radius) && (dv * d < 0) )
			{
				float m = 1 / balls[j].mass + 1 / balls[i].mass;
				Vector2 dp = 2 * (dv / m) * d.norm() * d.norm();
				balls[i].velocity += dp / balls[i].mass;
				balls[j].velocity -= dp / balls[j].mass;
			}
		}
		balls[i].UpdatePosition(dt);
	}
}