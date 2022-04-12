#include"vec3.h"
#include"color.h"

#include<iostream>

#pragma region PPM文件输出说明
//在对应.exe文件夹下使用cmd命令行，输入：\Debug>Project1.exe>Project1.ppm可以得到.ppm文件。
void CreatePPMFile()
{
	//Image
	const int image_width = 256;
	const int image_height = 256;

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
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}
#pragma endregion

int main()
{
	CreatePPMFile();
}