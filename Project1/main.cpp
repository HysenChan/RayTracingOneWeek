#include"rtweekend.h"

#include"color.h"
#include"hittable_list.h"
#include"sphere.h"
#include"camera.h"
#include"material.h"

#include<iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return -1.0;
	else
		return (-half_b - sqrt(discriminant)) / (2.0 * a);
}

color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

#pragma region PPM文件输出说明
//在对应.exe文件夹下使用cmd命令行，输入：\Debug>Project1.exe>Project1.ppm可以得到.ppm文件。
void CreatePPMFile()
{
	//Image
	const int image_width = 256;
	const int image_height = 256;
	const int samples_per_pixel = 100;

	//Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			/*auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/
			color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}
#pragma endregion

#pragma region 射线将光线发送到场景中
void CreateRay()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	// World
	auto R = cos(pi / 4);
	hittable_list world;

	auto material_left = make_shared<lambertian>(color(0, 0, 1));
	auto material_right = make_shared<lambertian>(color(1, 0, 0));

	world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
	world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

	//auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	//auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	//auto material_center = make_shared<dielectric>(1.5);
	//auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	//auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	//auto material_left = make_shared<dielectric>(1.5);
	//auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
	//auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	//world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	//world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	//world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	//world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
	//world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera

	camera cam(90.0, aspect_ratio);

	// Render

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
}
#pragma endregion


int main()
{
	//CreatePPMFile();
	CreateRay();
}