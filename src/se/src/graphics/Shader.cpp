#include "Shader.hpp"

using namespace se;

std::vector<Shader *> Shader::threadShaders = std::vector<Shader *>();

Shader::Shader(std::string name, unsigned radius, float power, sf::Vector2i lux) : name(name), radius(radius), power(power), lux(lux)
{
	fix = true;
	trace("Shader created");
}

Shader::Shader(std::string name, unsigned radius, float power, Entity *lux) : name(name), radius(radius), power(power), luxEntity(lux)
{
	fix = false;
	trace("Shader created");
}

Shader::~Shader()
{
	for(auto te : entityList)
	{
		te.other->getTexture()->update(te.imgCpy);
	}
	trace("Shader deleted");
}

void Shader::add(Entity& other)
{
	TextureEntity te;
	te.other = &other;
	te.imgCpy = other.getTexture()->copyToImage();
	entityList.push_back(te);
	entityListSize++;
}

void Shader::setLuxEntity(Entity *luxEntity)
{
	this->luxEntity = luxEntity;
}

void Shader::setLux(sf::Vector2i lux)
{
	this->lux = lux;
}

std::string Shader::getName()
{
	return name;
}

void Shader::update()
{
	for(int k = 0; k < entityListSize; k++)
	{
		Entity *e = entityList[k].other;
		e->editTexture([k, e, this](sf::Image& img)
		{
			auto size = img.getSize();
			float memAngle = e->getRotation();
			auto s = e->getSize();
			auto basicPos = e->getTLPosition();
			auto normPos = e->getPosition();
			int newLuxX;
			int newLuxY;
			auto imgFactorSize = sf::Vector2f((s.x / (float)size.x), (s.y / (float)size.y));
			auto pos = fix ? lux : static_cast<sf::Vector2i>(luxEntity->getPosition());
			newLuxX = (pos.x-normPos.x) * cos(radians(-memAngle)) - (pos.y-normPos.y) * sin(radians(-memAngle)) + normPos.x;
			newLuxY = (pos.x-normPos.x) * sin(radians(-memAngle)) + (pos.y-normPos.y) * cos(radians(-memAngle)) + normPos.y;
			for(int i = 0; i < size.x; i++)
			{
				for(int j = 0; j < size.y; j++)
				{
					sf::Color cpyPx = entityList[k].imgCpy.getPixel(i, j);
					if(cpyPx.a == 0) continue;
					int x = (i * imgFactorSize.x + basicPos.x);
					int y = (j * imgFactorSize.y + basicPos.y);
					int distance = util::getDistance(x, y, newLuxX, newLuxY);
					float fact = power;
					fact += 1 - distance / (float)radius;
					if(fact < power) fact = power;
					else if(fact > 1) fact = 1;
					img.setPixel(i, j, sf::Color(
						(unsigned char)(cpyPx.r * fact),
						(unsigned char)(cpyPx.g * fact),
						(unsigned char)(cpyPx.b * fact), 
						cpyPx.a));
				}
			}
		});
	}
}
/*
fact += ((float)radius / distance);
if(fact > 1) fact = 1; -> avec une source lumineuse plus forte (visuellement)
*/

Entity *Shader::getLuxEntity()
{
	return luxEntity;
}

void Shader::removeEntity(const Entity& other)
{
	for(int i = 0; i < entityListSize; i++)
	{
		if(entityList[i].other == &other)
		{
			entityList.erase(entityList.begin() + i);
			entityListSize--;
			return;
		}
	}
}

/*
if distance < radius then
	fact = 1 - distance / radius
end
*/

void Shader::join(Thread& th)
{
	setThreadRef(th);
	this->setThreadFunctionId(
		th.add([this]()
		{
			this->update();
	}));
	threadShaders.push_back(this);
}

void Shader::flush()
{
	for(auto te : Shader::threadShaders)
	{
		delete te;
	}
	Shader::threadShaders.clear();
}
