// TitleScene class

# include "TitleScene.hpp"

TitleScene::TitleScene()
	: backgroundRenderer{}
{
	// ”wŒiF‚ğİ’è
	m_backgroundColor = { 0.0f, 0.0f, 0.251f, 1.0f };
}

TitleScene::~TitleScene()
{

}

void TitleScene::update()
{
	
}

void TitleScene::draw() const
{
	backgroundRenderer.draw();
}