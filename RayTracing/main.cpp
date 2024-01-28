#include <QApplication>
#include <QFile>
#include <memory>
#include "Moving_Sphere.h"
#include "aarect.h"
#include "constant_medium.h"

#include "Camera.h"
#include "Ray.h"
#include "color.h"
#include "vec3.h"
#include "rtweekend.h"
#include "color.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Material.h"
#include "box.h"

// specify output's path here
const QString output_filename = "./result.ppm";

class material;

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(Color(.65, .05, .05));
    auto white = make_shared<lambertian>(Color(.73, .73, .73));
    auto green = make_shared<lambertian>(Color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(Color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(Point3(0,0,0), Point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265,0,295));

    shared_ptr<hittable> box2 = make_shared<box>(Point3(0,0,0), Point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, Vec3(130,0,65));

    objects.add(make_shared<constant_medium>(box1, 0.01, Color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, Color(1,1,1)));

    return objects;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(Point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}


hittable_list cornell_box() {
    hittable_list objects;

    auto green   = make_shared<lambertian>(Color(0, 1, 0));
    auto white = make_shared<lambertian>(Color(.73, .73, .73));
    auto red = make_shared<lambertian>(Color(1, 0, 0));
    auto light = make_shared<diffuse_light>(Color(15, 15, 15));
    auto transparent_ball_mat = make_shared<dielectric>(1.5);
    auto metal_ball_mat= make_shared<metal>(Color(0.8, 0.8, 0.9), 0.7);

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, red));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, green));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    shared_ptr<hittable> box1 = make_shared<box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, Vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(Point3(0,0,0), Point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, Vec3(130,0,65));
    objects.add(box2);

    shared_ptr<hittable> transparent_ball = make_shared<sphere>(Point3(0,0,0), 80, transparent_ball_mat);
    transparent_ball = make_shared<translate>(transparent_ball, Vec3(100, 280, 250));
    objects.add(transparent_ball);

    shared_ptr<hittable> metal_ball = make_shared<sphere>(Point3(0,0,0), 80, metal_ball_mat);
    metal_ball = make_shared<translate>(metal_ball, Vec3(270, 250, 250));
    objects.add(metal_ball);
    return objects;
}


hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(Point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(Point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(Color(4,4,4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}


hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(Point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(Point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(Point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(Point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

Color ray_color(const Ray& r, const Color& background, const hittable& world, const int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(Point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}


hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(Color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30,0,0);
    auto moving_sphere_material = make_shared<lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(Point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        Point3(0, 150, 145), 50, make_shared<metal>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(Point3(360,150,145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(Point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, Color(1,1,1)));

    auto white = make_shared<lambertian>(Color(.73, .73, .73));
    shared_ptr<hittable> whiteBox = make_shared<box>(Point3(0, 0, 0), Point3(400,200,400), white);
    whiteBox = make_shared<rotate_y>(whiteBox, 15);
    whiteBox = make_shared<translate>(whiteBox, Vec3(265,0,295));
    objects.add(whiteBox);
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(Point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(Point3::random(0,165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            Vec3(-100,270,395)
        )
    );

    return objects;
}

void output_PPM(){
    qDebug() << "start outputing ppm file!";

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    const int max_depth = 30;

    // World

    hittable_list world;

    Point3 lookfrom;
    Point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    Color background(0,0,0);

    switch (4) {
        case 1:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            lookfrom = Point3(278, 278, -800);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;

        case 2:
            world = two_spheres();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;

        case 3:
            world = two_perlin_spheres();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;

        case 4:
            world = earth();
            background = Color(0.70, 0.80, 1.00);
            lookfrom = Point3(13,2,3);
            lookat = Point3(0,0,0);
            vfov = 20.0;
            break;

        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = Color(0,0,0);
            lookfrom = Point3(26,3,6);
            lookat = Point3(0,2,0);
            vfov = 20.0;
            break;

        default:
        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 3000;
            samples_per_pixel = 25;
            background = Color(0,0,0);
            lookfrom = Point3(278, 278, -800);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;

        case 7:
            world = final_scene();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 100;
            background = Color(0,0,0);
            lookfrom = Point3(478, 278, -600);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;
        }

    // Camera
    Vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // render
    const QString filename = output_filename;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Couldn't open the file " << filename;
        exit(2);
    }

    QTextStream stream(&file);
    stream << "P3\n" << image_width << " " << image_height << "\n255\n";
    for ( int j = image_height-1; j >= 0; --j ) {
        for ( int i = 0; i < image_width; ++i ) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(stream, pixel_color, samples_per_pixel);
        }
        qDebug() << "j =" <<  j << "finished";
    }

    file.close();
    qDebug() << "output ppm done!";
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    output_PPM();


    return 0;
}

