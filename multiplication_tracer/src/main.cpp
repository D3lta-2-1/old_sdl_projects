#define SDL_MAIN_HANDLED
#include <iostream>
#include <exception>
#include <SDL2/SDL.h>
#include <math.h>

template<typename T>
constexpr T windowX = 1080;

template < typename T>
constexpr T windowY = 600;

template<typename T>
constexpr T rayon = 290;



class App
{
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
public:
    App() : _window{ nullptr }, _renderer{ nullptr } {}

    void initialize()
    {
        if (SDL_Init(SDL_INIT_VIDEO)) { throw std::runtime_error("SLD2 can't be initilized\n"); }

        _window = SDL_CreateWindow("ass renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX<int>, windowY<int>, SDL_WINDOW_SHOWN);
        if (!_window) { throw std::runtime_error("the window cannot be created\n"); }

        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(!_renderer) { throw std::runtime_error("the renderer cannot be created\n"); }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    }

    void run(int factor, int howManyPoint)
    {
        std::cout << "app run\n";
        bool isRunning{ true };
        SDL_Event event;
        while (isRunning)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT: 
                    isRunning = false;
                    break;
                default:
                    break;
                }
            }

            SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
            SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0); //buffer blanc
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 25);
            double step = 2*M_PI / howManyPoint;
            for (double i = 0; i < 2 * M_PI; i += step)
            {
                SDL_Point p1{};
                p1.x = windowX<double>/2 + (std::cos(i) * rayon<double>);
                p1.y = windowY<double> / 2 + (std::sin(i) * rayon<double>);

                SDL_Point p2{};
                p2.x = windowX<double> / 2 + (std::cos(i + step) * rayon<double>);
                p2.y = windowY<double> / 2 + (std::sin(i + step) * rayon<double>);
                SDL_RenderDrawLine(_renderer, p1.x, p1.y, p2.x, p2.y);
            }
            SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
            for (int i = 0; i < howManyPoint; i++)
            {
                double angle1 = (i % howManyPoint) * step;
                double angle2 = ((i * factor) % howManyPoint) * step;
                SDL_Point p1{};
                p1.x = windowX<double> / 2 + (std::cos(angle1) * rayon<double>);
                p1.y = windowY<double> / 2 + (std::sin(angle1) * rayon<double>);

                SDL_Point p2{};
                p2.x = windowX<double> / 2 + (std::cos(angle2) * rayon<double>);
                p2.y = windowY<double> / 2 + (std::sin(angle2) * rayon<double>);
                SDL_RenderDrawLine(_renderer, p1.x, p1.y, p2.x, p2.y);
            }

            SDL_RenderPresent(_renderer);
        }
    }

    void cleanup()
    {
        std::cout << "APP is closing down\n";
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }
};

int main(int argc, char* argv[])
{
    try
    {
        int nbPoint{ 0 };
        int factor{ 0 };
        std::cout << "entrez un facteur\n";
        std::cin >> factor;
        std::cout << "entrez un nombre de points\n";
        std::cin >> nbPoint;
        App myApp;
        myApp.initialize();
        myApp.run(factor, nbPoint);
        myApp.cleanup();
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what();
        std::cin.get();
        return -1;
    }
    return 0;
}