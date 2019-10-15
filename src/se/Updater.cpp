#include "Updater.hpp"

using namespace se;

void Updater::activateUpdate()
{
	this->updateState = true;
}

void Updater::disactivateUpdate()
{
	this->updateState = false;
}

void Updater::activateRender()
{
	this->renderState = true;
}

void Updater::disactivateRender()
{
	this->renderState = false;
}

bool Updater::getUpdateState()
{
	return this->updateState;
}

bool Updater::getRenderState()
{
	return this->renderState;
}

void Updater::pause()
{
	this->disactivateUpdate();
	this->disactivateRender();
}

void Updater::play()
{
	this->activateUpdate();
	this->activateRender();
}
