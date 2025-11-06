// UIManager class

# include "UIManager.hpp"

UIManager::UIManager()
{

}

UIManager& UIManager::GetInstance()
{
	static UIManager instance;
	return instance;
}

void UIManager::update()
{

}

void UIManager::draw() const
{

}