#include <iostream>
#include <fstream>
#include "svpng.inc"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <cstdlib>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <thread>
#include <future>
#include <memory>
#include <iomanip>
#include "material.h"
#include "world.h"
#include "scenes/scene2.h"

using namespace std;

void draw_png(const char *filename, const uint8_t *img, int w, int h)
{
    FILE *fp = fopen(filename, "wb");
    svpng(fp, w, h, img, 0);
    fclose(fp);
}

void test_rgb()
{
    auto p   = new unsigned char[200 * 100 * 3];
    auto rgb = p;
    unsigned x, y;
    FILE *fp = fopen("rgbtest.png", "wb");
    for (y = 0; y < 100; y++)
        for (x = 0; x < 200; x++) {
            *p++ = (unsigned)x;
            *p++ = (unsigned)y;
            *p++ = 128;
        }
    svpng(fp, x, y, rgb, 0);
    fclose(fp);
}

void render_sample(float *workspace, world *world, camera *cam, int w, int h)
{
    auto p = workspace;
    for (int j = h - 1; j >= 0; j--) {
        for (int i = 0; i < w; i++) {
            float u = float(i + rand_canonical()) / float(w);
            float v = float(j + rand_canonical()) / float(h);

            auto ray   = cam->get_ray(u, v);
            auto pixel = world->color(ray, 0);

            *p++ = pixel.r();
            *p++ = pixel.g();
            *p++ = pixel.b();
        }
    }
}

void draw_canvas(uint8_t *canvas, const float *img, int ssaa, float gamma, int w, int h)
{
    for (int i = 0; i < w * h * 3; ++i) {
        canvas[i] = min(255, static_cast<int>(255.f * powf(img[i] / ssaa, 1 / gamma)));
    }
}

void dump_result(const char *path, const float *img, int ssaa, int w, int h)
{
    ofstream file(path);

    file << 0xdeadbeaf << ' ';
    file << ssaa << ' ';
    file << w << ' ';
    file << h << ' ';

    for (int i = 0; i < w * h * 3; ++i) {
        file << img[i] << ' ';
    }
}

void render_session(int w, int h, int SSAA, int thd)
{
    auto canvas = new unsigned char[w * h * 3];
    auto world  = create_world();
    auto cam    = create_camera();

    vector<future<float *>> future_vec;
    for (int i = 0; i < thd; ++i) {
        auto future = std::async(std::launch::async, [=]() {
            auto buffer    = new float[w * h * 3]{};
            auto workspace = new float[w * h * 3];

            for (int s = 0; s < SSAA / thd; ++s) {
                printf("[worker %d] %d/%d\n", i, s, SSAA / thd);

                render_sample(workspace, world, cam, w, h);
                for (int j = 0; j < w * h * 3; ++j) {
                    buffer[j] += workspace[j];
                }
            }

            return buffer;
        });

        future_vec.push_back(move(future));
    }

    auto result = new float[w * h * 3]{};
    for (auto &future : future_vec) {
        future.wait();

        auto buffer = future.get();
        for (int i = 0; i < w * h * 3; ++i) {
            result[i] += buffer[i];
        }
    }

    draw_canvas(canvas, result, SSAA, 2, w, h);
    dump_result("result.dump.txt", result, SSAA, w, h);
    draw_png("result.png", canvas, w, h);
}

int main()
{
    while (true) {
        constexpr int w    = 400;
        constexpr int h    = 300;
        constexpr int SSAA = 64;
        constexpr int thd  = 4;

        static_assert(SSAA % thd == 0, "jobs must be evenly sliced!");

        printf("enter width of your canvas(default is %d):\n", w);
        printf("enter height of your canvas(default is %d):\n", h);
        printf("enter SSAA of rendering process(default is %d):\n", SSAA);
        printf("enter number of threads to render(default is %d):\n", thd);

        render_session(w, h, SSAA, thd);
        return 0;
    }
}