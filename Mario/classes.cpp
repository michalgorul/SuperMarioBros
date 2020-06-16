#include "classes.h"

void configure(RenderWindow& W, SoundBuffer& theme_, SoundBuffer& themeU_, SoundBuffer& themeB_, Sound& theme, Texture& texture_m, Sprite& sprite_m,
               SoundBuffer& pauseBuffer, Sound& pause)
{
	W.create(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32), "Super Mario Bros");
	W.setActive(true);
	W.setKeyRepeatEnabled(false);
	W.setFramerateLimit(60);
	W.setPosition(Vector2i(350, 200));

	theme_.loadFromFile("overworld.ogg");
	themeU_.loadFromFile("underworld.ogg");
	themeB_.loadFromFile("Castle.ogg");
	
	theme.setBuffer(theme_);
	theme.setLoop(true);
	theme.setVolume(50);

	pauseBuffer.loadFromFile("pause.wav");
	pause.setBuffer(pauseBuffer);

	texture_m.loadFromFile("menu.jpg");
	sprite_m.setTexture(texture_m);

	Image icon;
	icon.loadFromFile("mario.ico");
	W.setIcon(256, 256, icon.getPixelsPtr());
}

bool isAnyKeyPressed()
{
	for (auto k = -1; k < Keyboard::KeyCount; ++k)
	{
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(k)))
		{
			return true;
		}
	}
	return false;
}

float giveRandomNumber(float begin, float end)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<float> dist(begin, end);

	return dist(mt);
}

template <typename TContainer>
void eraseFromUnorderedByIndex(TContainer& inContainer, size_t inIndex)
{
	if (inIndex < inContainer.size())
	{
		if (inIndex != inContainer.size() - 1)
			inContainer[inIndex] = inContainer.back();
		inContainer.pop_back();
	}
}
