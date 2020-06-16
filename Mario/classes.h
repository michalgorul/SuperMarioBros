#ifndef classes_h
#define classes_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <string>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <vector>
#include <random>

using namespace sf;
using namespace std;

constexpr auto WINDOW_SIZE_X = 1300; //szerokoœæ okna
constexpr auto WINDOW_SIZE_Y = 650; //wysokoœæ okna
constexpr auto BEGIN_END_SIZE = 60; //rozmiar obszaru koñcz¹cego i rozpoczynaj¹cego

constexpr auto MAX_NUMBER_OF_ITEMS = 4; //max elementów menu


//funkcja konfiguruje pocz¹tkowe elementy gry
void configure(RenderWindow& W, SoundBuffer& theme_, SoundBuffer& themeU_, SoundBuffer& themeB_, Sound& theme, Texture& texture_m, Sprite& sprite_m,
               SoundBuffer& pauseBuffer, Sound& pause);

bool isAnyKeyPressed();

float giveRandomNumber(float begin, float end);

template <typename TContainer>
void eraseFromUnorderedByIndex(TContainer& inContainer, size_t inIndex);

#endif
