#include "launcher.h"
#include "irrKlang.h"
#include<random>
const double PI = 3.14159265358979323846;
#include"glm/gtx/string_cast.hpp"

using namespace irrklang;
ISoundEngine* soundEngine = createIrrKlangDevice();

float getRandomNumber(float min, float max);
glm::vec3 fadeColor(glm::vec3 startColor, glm::vec3 destColor, float ratio);
glm::vec4 getRandomBrightColor();//随机生成颜色

auto gravityForce = glm::vec3(0.0f, -GRAVITY, 0.0f);
auto noForce = glm::vec3(1.0f, 1.0f, 1.0f);

float thera1[]{
	0.000000, 0.209440,0.418879,0.628319,0.837758,1.047198,
	1.256637,1.466076,1.675516,1.884956,2.094395,2.303834,
	2.513274,2.722713,2.932153,3.141593,3.351032,3.560472,
	3.769911,3.979351,4.188790,4.398230,4.607669,4.817109,
	5.026548,5.235988,5.445427,5.654867,5.864306,6.073746
};
float R1 = 37.0f;

Launcher::Launcher()
	: Launcher(glm::vec3(0.0f, 0.0f, 0.0f))
{}

Launcher::Launcher(glm::vec3 position)
	: position(position)
{
	for (int i = 0; i < maxParticles; i++) 
	{
		// Initializing default particle values
		particles[i].life = -1.0f;
		particles[i].cameraDst = -1.0f;
		particles[i].trailTime = trailDelay;
		particles[i].type = Particle::Type::DEAD;
	}
}

void Launcher::simulate(Camera& camera, GLfloat* particle_position, GLubyte* particle_color)
{
	float deltaTime = Camera::getDeltaTime();
	particlesCount = 0;

	for (int i = 0; i < maxParticles; i++)
	{
		// Simulating each particule
		Particle& p = particles[i];

		if (p.life > 0.0f)
		{
			// Alive particle
			p.speed += gravityForce * deltaTime;
			p.pos += p.speed * deltaTime;
			p.cameraDst = glm::distance(p.pos, camera.getPosition());

			renderTrails(p, deltaTime);

			// Fill the GPU buffer
			particle_position[4 * particlesCount + 0] = p.pos.x;
			particle_position[4 * particlesCount + 1] = p.pos.y;
			particle_position[4 * particlesCount + 2] = p.pos.z;

			float lifeRatio = 1.0f;
			auto newColor = glm::vec3(p.r, p.g, p.b);
			switch (p.type)
			{
				case Particle::Type::SPARKLE:
					lifeRatio = p.life / sparkleLife;
					break;
				case Particle::Type::TRAIL:
					lifeRatio = p.life / trailLife;
					newColor = fadeColor(newColor, trailFade, lifeRatio);
					break;
				case Particle::Type::FOUNTAIN:
					lifeRatio = p.life / fountainLife;
					newColor = fadeColor(fountainColor, fountainFade, lifeRatio);
					break;
			}
			particle_position[4 * particlesCount + 3] = p.size * lifeRatio;

			particle_color[4 * particlesCount + 0] = newColor.r;
			particle_color[4 * particlesCount + 1] = newColor.g;
			particle_color[4 * particlesCount + 2] = newColor.b;
			particle_color[4 * particlesCount + 3] = p.life;

			particlesCount++;
			p.life -= deltaTime;
			continue;
		}

		// Particle just died
		if (p.type == Particle::Type::LAUNCHING)
			explode(p);

		p.type = Particle::Type::DEAD;
		p.cameraDst = -1.0f;
	}
}



void Launcher::renderTrails(Particle& p, float deltaTime)
{
	if (p.trailTime <= 0 && (p.type == Particle::Type::SPARKLE || p.type == Particle::Type::LAUNCHING))
	{

		spawnParticle(
			p.pos,
			glm::vec3(0.0f),
			glm::vec4(p.r, p.g, p.b, p.a),
			trailSize,
			trailLife,
			Particle::Type::TRAIL
		);
		p.trailTime = trailDelay;
	}
	p.trailTime -= deltaTime;
}

void Launcher::spawnParticle(glm::vec3 position, glm::vec3 speed, glm::vec4 color, float size, float life, Particle::Type type)//生成粒子
{
	int idx = findUnusedParticle();

	particles[idx].pos = position;
	particles[idx].speed = speed;
	particles[idx].r = color.r;
	particles[idx].g = color.g;
	particles[idx].b = color.b;
	particles[idx].a = color.a;
	particles[idx].size = size;
	particles[idx].life = life;
	particles[idx].type = type;
}

void Launcher::explode(Particle &p)
{
	int randomSound = getRandomNumber(1, 6);
	soundEngine->play2D(explosionSounds[randomSound - 1]);

	float randSize = getRandomNumber(0, explosionSpread);
	for (int i = 0; i < sparklesPerExplosion; i++)
	{
		float randX = getRandomNumber(-1, 2);
		float randY = getRandomNumber(-1, 2);
		float randZ = getRandomNumber(-1, 2);
		float randSpread = getRandomNumber(0, explosionSpread);
		float randLife = getRandomNumber(0, 1.5f);

		spawnParticle(
			p.pos,
			glm::normalize(glm::vec3(randX, randY, randZ)) * (explosionSize - randSpread + randSize),
			glm::vec4(p.r, p.g, p.b, p.a),
			sparkleSize,
			sparkleLife + randLife,
			Particle::Type::SPARKLE
		);
	}
}

void Launcher::launchFirework()
{

	int i= rand() % 30;
	float randX, initialY = 50.0f, randZ;
	randX = R1 * cos(thera1[i]);
	randZ = R1 * sin(thera1[i]);
	glm::vec3 place(randX, initialY, randZ);

	

	// 生成随机的水平速度分量（x和z方向），模拟随风飘动，速度范围较小，示例设为[-0.5, 0.5]
	float randXSpeed = getRandomNumber(-0.5f, 0.5f);
	float randZSpeed = getRandomNumber(-0.5f, 0.5f);
	// 主要的向下速度分量，这里设为-1.0（可调整飘落速度）
	float downwardSpeed = -1.0f;


	spawnParticle(
		glm::vec3(randX, initialY, randZ),
		glm::vec3(randXSpeed, downwardSpeed, randZSpeed),
		getRandomBrightColor(),
		rocketSize,
		rocketLife,
		Particle::Type::LAUNCHING
	);//产生粒子
}




void Launcher::launchSnowflake()  
{
	// 生成随机的水平位置偏移（x和z方向），范围可根据实际场景大小等因素调整，这里示例为[-50, 50]
	float randX = getRandomNumber(-600.0f, 600.0f);
	float randZ = getRandomNumber(-600.0f, 600.0f);
	// 雪花初始位置在场景上方一定高度，这里设为200（可按需调整）
	float initialY = 200.0f;

	// 生成随机的水平速度分量（x和z方向），模拟随风飘动，速度范围较小，示例设为[-0.5, 0.5]
	float randXSpeed = getRandomNumber(-0.5f, 0.5f);
	float randZSpeed = getRandomNumber(-0.5f, 0.5f);
	// 主要的向下速度分量，这里设为-1.0（可调整飘落速度）
	float downwardSpeed = -1.0f;
	glm::vec4 color(255.0f, 255.0f, 255.0f, 1.0f);


	// 调用生成粒子的函数，传入调整后的参数
	spawnParticle(
		position + glm::vec3(randX, initialY, randZ),  // 初始位置在上方且有一定水平随机偏移
		glm::vec3(randXSpeed, downwardSpeed, randZSpeed),  // 初始速度有向下分量及水平随机飘动分量
		color,
		snowSize,  // 雪花大小（需定义该变量并赋值，类似之前的rocketSize但针对雪花调整合适值）
		snowLife,  // 雪花生命周期（同样需定义并赋值，根据期望的飘落时长等设定）
		Particle::Type::SNOWFLAKING  // 可自定义一个表示雪花类型的枚举值，用于区分粒子类型（可选操作）
	);
}



void Launcher::launchFountain()
{

	for (int i = 0; i < fountainDensity; i++)
	{
		float randX = getRandomNumber(-fountainSpread, fountainSpread);
		float randZ = getRandomNumber(-fountainSpread, fountainSpread);
		float randY = getRandomNumber(-10.0f, 10.0f);

		spawnParticle(
			position,
			glm::vec3(randX, fountainSpeed + randY, randZ),
			glm::vec4(fountainColor, 1.0f),
			fountainSize,
			fountainLife,
			Particle::Type::FOUNTAIN
		);
	}
}

void Launcher::update(Camera& camera, GLfloat* particle_position, GLubyte* particle_color)
{
	float deltaTime = Camera::getDeltaTime();

	//launchTime -= deltaTime;
	//if (launchTime <= 0)
	//{
	//	launchFirework();
	//	launchTime = launchDelay;
	//}
	
	/*fountainTime -= deltaTime;
	if (fountainTime <= 0) 
	{
		launchFountain();
		fountainTime = fountainDelay;
	}*/

	snowTime = -deltaTime;

	if (snowTime <= 0)
	{
		launchSnowflake();
		snowTime = snowDelay;
	}

	simulate(camera, particle_position, particle_color);
	sortParticles();
}

void Launcher::sortParticles() {
	std::sort(&particles[0], &particles[maxParticles]);
}

int Launcher::findUnusedParticle() {

	for (int i = lastUsedId; i < maxParticles; i++) {
		if (particles[i].life < 0) {
			lastUsedId = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedId; i++) {
		if (particles[i].life < 0) {
			lastUsedId = i;
			return i;
		}
	}

	return 0;
}

glm::vec3 fadeColor(glm::vec3 startColor, glm::vec3 destColor, float ratio)
{
	return { 
		(ratio * startColor.r + (1.0f - ratio) * destColor.r) / 1.0f,
		(ratio * startColor.g + (1.0f - ratio) * destColor.g) / 1.0f,
		(ratio * startColor.b + (1.0f - ratio) * destColor.b) / 1.0f 
	};
	
}

float getRandomNumber(float min, float max)
{
	return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

glm::vec4 getRandomBrightColor()
{
	auto rgb = glm::vec3(0.0f, 0.0f, 0.0f);
	unsigned char region, remainder, p, q, t;

	float h = getRandomNumber(0, 256);
	unsigned char s = 255;
	unsigned char v = 255;

	region = h / 43;
	remainder = (h - (region * 43)) * 6;

	p = (v * (255 - s)) >> 8;
	q = (v * (255 - ((s * remainder) >> 8))) >> 8;
	t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		rgb.r = v; rgb.g = t; rgb.b = p;
		break;
	case 1:
		rgb.r = q; rgb.g = v; rgb.b = p;
		break;
	case 2:
		rgb.r = p; rgb.g = v; rgb.b = t;
		break;
	case 3:
		rgb.r = p; rgb.g = q; rgb.b = v;
		break;
	case 4:
		rgb.r = t; rgb.g = p; rgb.b = v;
		break;
	default:
		rgb.r = v; rgb.g = p; rgb.b = q;
		break;
	}

	return glm::vec4(rgb, 1.0f);
}