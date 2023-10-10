#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Uncomment this to display all images in console!
// Push \n at the end of line
//#define IMGGEN_DISPLAY

#define SYM_WHITE "0"
#define SYM_BLACK "1"
#define CHAR_WHITE '0'
#define CHAR_BLACK '1'

//#define SYM_WHITE "_"
//#define SYM_BLACK "*"
//#define CHAR_WHITE '_'
//#define CHAR_BLACK '*'

//#define SYM_WHITE "-"
//#define SYM_BLACK "X"
//#define CHAR_WHITE '-'
//#define CHAR_BLACK 'X'

// Generate output (console or file)
#define DISPLAY
// Generate output file "output.txt"
#define FILED
// TEST LEARN SET
#define TEST true


struct Rect {
	int x = 0;
	int y = 0;
	int w = 5;
	int h = 5;

	Rect() {
		x = 0;
		y = 0;
		w = 5;
		h = 5;
	}

	Rect(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

struct Pixel {
	stbi_uc r;
	stbi_uc g;
	stbi_uc b;

	//bool is_white() { return (r > 200 && b > 200 && g > 200); }
	bool is_white() { return (r > 200); }
};

class ImgGen
{
private:
	int img_in_w;
	int img_in_h;
	int img_out_w;
	int img_out_h;

	std::vector<std::string> vec_in;
	std::vector<std::string> vec_out;

	std::string gen_img_square(Rect square, int img_w, int img_h) {
		std::string img;
		for (int i = 0; i < img_w; ++i) {
			for (int j = 0; j < img_h; ++j)
			{
				bool is_square = false;
				//:: Top and Down sides
				if ((i >= square.x && i <= (square.x + square.w - 1)) && (j == square.y || j == (square.y + square.h - 1))) is_square = true;
				// :: Left and Right sides
				else if ((j >= square.y && j <= (square.y + square.h - 1)) && (i == square.x || i == (square.x + square.w - 1))) is_square = true;
				// :: Append col
				img.append((is_square) ? SYM_BLACK : SYM_WHITE);
			}
			#ifdef IMGGEN_DISPLAY
			img.append("\n");
			#endif
		}
		#ifdef IMGGEN_DISPLAY
		std::cout << img << "\n\n";
		#endif

		//str_print(img, img_w);

		return img;
	}
	void str_print(std::string str, int w) {
		int i = 0;
		for (auto it = str.cbegin(); it != str.cend(); ++it) {
			std::cout << *it;
			i++; if (i % w == 0) std::cout << "\n";
		}
		std::cout << "\n";
	}
	void str_print_file(std::string str, int w, std::ofstream &file) {
		int i = 0;
		for (auto it = str.cbegin(); it != str.cend(); ++it) {
			file << *it;
			i++; if (i % w == 0) file << "\n";
		}
		file << "\n";
	}

public:
	ImgGen(int in_w, int in_h, int out_w, int out_h) {
		set_img_in_size(in_w, in_h);
		set_img_out_size(out_w, out_h);
	}

	// IMAGE IN/OUT GET-SET-ters
	int get_img_in_width() { return img_in_w; }
	int get_img_in_height() { return img_in_h; }
	int get_img_out_width() { return img_out_h; }
	int get_img_out_height() { return img_out_h; }
	void set_img_in_size(int w, int h) { img_in_w = w; img_in_h = h; }
	void set_img_out_size(int w, int h) { img_out_w = w; img_out_h = h; }

	// Generate a learning set
	void generate() {
		// Init DISPLAY
		
		#ifdef DISPLAY
			#define FILED
			#ifdef FILED
				std::ofstream outfile("output.txt"); // "output.txt"
				#define PRINT(str, w) str_print_file(str, w, outfile)
				#define CLOSE() outfile.close()
			#else
				#define PRINT(str, w) str_print_file(str, w)
				#define CLOSE() 
			#endif
		#else
			#define PRINT(str, w)
			#define CLOSE()
		#endif
		
		// I
		int square_in_w = 5, square_in_h = 5, rect_in_w = 4, rect_in_h = 7;

		// :: In-string of square & rectangle
		std::string str_in_square = gen_img_square(Rect(4, 4, square_in_w, square_in_h), img_in_w, img_in_h);
		std::string str_in_rect = gen_img_square(Rect(4, 3, rect_in_w, rect_in_h), img_in_w, img_in_h);

		str_print(str_in_square, img_in_w);
		str_print(str_in_rect, img_in_w);

		// :: Out-string of square & rectangle
		std::string str_out_square = gen_img_square(Rect(2, 2, 5, 5), img_out_w, img_out_h);
		std::string str_out_rect = gen_img_square(Rect(2, 1, 4, 7), img_out_w, img_out_h);

		str_print(str_out_square, img_out_w);
		str_print(str_out_rect, img_out_w);

		int _cur_sz = 0;

		// 1. Noise
		int noise_repeat = 50;
		if (TEST) noise_repeat = 15;	// TEST
		//	:: Square
		std::cout << "[GEN]: NOISE square..." << std::endl;
		for (int i = 0; i < noise_repeat; ++i) {
			std::string str = str_noise(str_in_square);
			//if(_display) str_print(str, img_in_w); // DISPLAY
			PRINT(str, img_in_w);
			vec_in.push_back(str);
			vec_out.push_back(str_out_square);
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		//	:: Rect
		std::cout << "[GEN]: NOISE rect..." << std::endl;
		for (int i = 0; i < noise_repeat; ++i) {
			std::string str = str_noise(str_in_rect);
			//if (_display) str_print(str, img_in_w); // DISPLAY
			PRINT(str, img_in_w);
			vec_in.push_back(str);
			vec_out.push_back(str_out_rect);
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		// 2. Shifting
		//	:: Square
		std::cout << "[GEN]: Shifting square..." << std::endl;
		Rect square(0, 0, square_in_w, square_in_h);
		int steps_side_max = img_in_w - square.w;
		int steps_down_max = img_in_h - square.h;
		if (TEST) steps_side_max = 3;	// TEST
		for (int i = 0; i <= steps_down_max; ++i)
		{
			for (int j = 0; j <= steps_side_max; ++j) {
				std::string ls_in = gen_img_square(square, img_in_w, img_in_h);
				//if (_display) str_print(ls_in, img_in_w); // DISPLAY
				PRINT(ls_in, img_in_w);
				vec_in.push_back(ls_in);
				vec_out.push_back(str_out_square);
				square.x++;
			}
			square.x = 0; square.y++;
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		//	:: Rect
		std::cout << "[GEN]: Shifting rect..." << std::endl;
		Rect rect(0, 0, rect_in_w, rect_in_h);
		steps_side_max = img_in_w - rect.w;
		steps_down_max = img_in_h - rect.h;
		if (TEST) steps_side_max = 3; // TEST
		for (int i = 0; i <= steps_down_max; ++i)
		{
			for (int j = 0; j <= steps_side_max; ++j) {
				std::string ls_in = gen_img_square(rect, img_in_w, img_in_h);
				//if (_display) str_print(ls_in, img_in_w); // DISPLAY
				PRINT(ls_in, img_in_w);
				vec_in.push_back(ls_in);
				vec_out.push_back(str_out_rect);
				rect.x++;
			}
			rect.x = 0; rect.y++;
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		// 3. Shifting + Noise
		noise_repeat = 5;
		if (TEST) { noise_repeat = 1; } // TEST
		//	:: Square
		std::cout << "[GEN]: Shifting+Noise square..." << std::endl;
		square = Rect(0, 0, square_in_w, square_in_h);
		steps_side_max = img_in_w - square.w;
		steps_down_max = img_in_h - square.h;
		if (TEST) steps_side_max = 3; // TEST
		for (int i = 0; i <= steps_down_max; ++i)
		{
			for (int j = 0; j <= steps_side_max; ++j) {
				std::string ls_in = gen_img_square(square, img_in_w, img_in_h);
				for (int k = 0; k < noise_repeat; ++k) { // <- noise
					std::string noise = str_noise(ls_in);
					//if (_display) str_print(noise, img_in_w); // DISPLAY
					PRINT(noise, img_in_w);
					vec_in.push_back(noise);
					vec_out.push_back(str_out_square);
				}
				square.x++;
			}
			square.x = 0; square.y++;
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		//	:: Rect
		std::cout << "[GEN]: Shifting+Noise rect..." << std::endl;
		rect = Rect(0, 0, rect_in_w, rect_in_h);
		steps_side_max = img_in_w - rect.w;
		steps_down_max = img_in_h - rect.h;
		if (TEST) steps_side_max = 3; // TEST
		for (int i = 0; i <= steps_down_max; ++i)
		{
			for (int j = 0; j <= steps_side_max; ++j) {
				std::string ls_in = gen_img_square(rect, img_in_w, img_in_h);
				for (int k = 0; k < noise_repeat; ++k) { // <- noise
					std::string noise = str_noise(ls_in);
					//if (_display) str_print(noise, img_in_w); // DISPLAY
					PRINT(noise, img_in_w);
					vec_in.push_back(noise);
					vec_out.push_back(str_out_rect);
				}
				rect.x++;
			}
			rect.x = 0; rect.y++;
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();


		// 4. Inverse + NOISE
		if (TEST) {
			noise_repeat = 5;	// TEST
			//	:: Square
			std::cout << "[GEN]: NOISE + INVERSE square..." << std::endl;
			for (int i = 0; i < noise_repeat; ++i) {
				std::string str = str_noise(str_in_square);
				str = str_inverse(str);
				PRINT(str, img_in_w);
				vec_in.push_back(str);
				vec_out.push_back(str_out_square);
			}
			std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

			//	:: Rect
			std::cout << "[GEN]: NOISE + INVERSE rect..." << std::endl;
			for (int i = 0; i < noise_repeat; ++i) {
				std::string str = str_noise(str_in_rect);
				str = str_inverse(str);
				PRINT(str, img_in_w);
				vec_in.push_back(str);
				vec_out.push_back(str_out_rect);
			}
			std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();
		}
		//1. Shifting with resizing (from 3x3 to 13x13)
		
		/*std::cout << "[GEN]: Shifting + resizing square..." << std::endl;
		for (int k = 3; k <= img_in_w; ++k)
		{
			Rect square(0, 0, k, k);
			int steps_side_max = img_in_w - square.w;
			int steps_down_max = img_in_h - square.h;
			for (int i = 0; i <= steps_down_max; ++i)
			{
				for (int j = 0; j <= steps_side_max; ++j) {
					std::string ls_in = gen_img_square(square, img_in_w, img_in_h);
					vec_in.push_back(ls_in);
					vec_out.push_back(str_out_square);
					square.x++;
				}
				square.x = 0; square.y++;
			}
		}*/
		
		CLOSE();

		std::cout << "[GEN]: DONE. LS were added: " << vec_in.size() << std::endl;
	}

	// To CSV File
	void convert_to_csv(std::string filename, std::string divider = ",", bool use_excel_format = false) {
		std::cout << "[GEN] Converting to CSV data (filename: " << filename << ")\n";
		std::ofstream outfile(filename);
		int sz = vec_in.size();
		for (int i = 0; i < sz; ++i)
		{
			// INPUT VECTOR
			auto str = vec_in.at(i);
			for (auto it = str.cbegin(); it != str.cend(); ++it) {
				outfile << *it << divider;
			}

			// OUTPUT VECTOR
			str = vec_out.at(i);
			for (auto it = str.cbegin(); it != str.cend(); ++it) {
				outfile << *it << divider;
			}
			outfile << "\n";
			//if (use_excel_format) outfile << "\"=\"\"\"";
			//outfile << vec_in.at(i) << divider << vec_out.at(i); // << std::endl;
			//if (use_excel_format) outfile << "\"\"\"\n";
			//else outfile << "\n";
		}
		outfile.close();
		std::cout << "[GEN] Converting DONE";
	}

	// Load PNG
	unsigned char* load_image(std::string filename, int &width, int &height, int &numComponents, int desiredChannels = 3) {
		std::cout << "[GEN] Loading image... (filename: " << filename << ")\n";
		unsigned char* data = stbi_load((filename).c_str(), &width, &height, &numComponents, desiredChannels);
		std::cout << "[GEN] Image loaded: " << width << "/" << height << " ochan=" << numComponents << "\n";
		return data;
	}

	// Get Image Pixel
	Pixel image_get_pixel(stbi_uc* image, size_t imageWidth, size_t x, size_t y, size_t channels_cnt) {
		Pixel output;
		const stbi_uc* p = image + (channels_cnt * (y * imageWidth + x)); // 4
		output.r = p[0];
		output.g = p[1];
		output.b = p[2];
		//*output.a = p[3];
		return output;
	}

	// Image To 0-1 sqn
	std::string image_to_str(stbi_uc* image, size_t imageWidth, size_t imageHeight, size_t channel_count) {
		std::cout << "[GEN] Converting image to string of 0-1 ...\n";
		std::string str;
		Pixel pix;
		for (int i = 0; i < imageHeight; ++i) {
			for (int j = 0; j < imageWidth; ++j)
			{
				//std::cout << "\tGet pixel at " << i << ", " << j << " ... ";
				pix = image_get_pixel(image, imageWidth, j, i, channel_count);
				//std::cout << "rgb = " << (int)pix.r << " " << (int)pix.g << " " << (int)pix.b << "\n";
				if (pix.is_white()) str.append(SYM_WHITE); else str.append(SYM_BLACK);
			}
			#ifdef IMGGEN_DISPLAY
			str.append("\n");
			#endif
		}
		#ifdef IMGGEN_DISPLAY
		std::cout << str << "\n\n";
		#endif

		str_print(str, imageWidth);

		std::cout << "[GEN] Converting DONE\n";
		return str;
	}

	// Noise
	std::string str_noise(std::string str, int noise_pixel = 8) {
		int _str_len = str.length();
		std::string _cc;
		for (int i = 0; i < noise_pixel; ++i) {
			int _pos = std::rand() % _str_len; // 0 - STRING LENGTH
			char _c = str.at(_pos); // i
			_cc = (_c == CHAR_WHITE) ? SYM_BLACK : SYM_WHITE;
			str.replace(_pos, 1, _cc);
		}
		return str;
	}

	// Inverse
	std::string str_inverse(std::string str) {
		int _str_len = str.length();
		std::string _cc;
		for (int i = 0; i < _str_len; ++i) {
			char _c = str.at(i);
			_cc = (_c == CHAR_WHITE) ? SYM_BLACK : SYM_WHITE;
			str.replace(i, 1, _cc);
		}
		return str;
	}

	// 1 -> 2
	std::string str_one2(std::string str) {
		int _str_len = str.length();
		std::string _cc;
		for (int i = 0; i < _str_len; ++i) {
			char _c = str.at(i);
			if (_c == CHAR_BLACK) {
				str.replace(i, 1, "2");
			}
		}
		return str;
	}

	std::string str_one2_white(std::string str) {
		int _str_len = str.length();
		std::string _cc;
		for (int i = 0; i < _str_len; ++i) {
			char _c = str.at(i);
			if (_c == CHAR_WHITE) {
				str.replace(i, 1, "2");
			}
		}
		return str;
	}

	// Shifting
	std::string str_shift(std::string str, int shift_hor, int shift_ver, int max_w, int max_h) {
		// HORIZONTAL SHIFTING
		if (shift_hor > 0) { // SHIFT RIGHT
			for (int i = 0; i < shift_hor; ++i) { str.insert(0, SYM_WHITE); str.pop_back(); }
		}
		else if (shift_hor < 0) { // SHIFT LEFT
			for (int i = 0; i < abs(shift_hor); ++i) { str.push_back(CHAR_WHITE); str.erase(0, 1); }
		}

		// VERTICAL SHIFTING
		if (shift_ver != 0) {
			std::string _str_white;
			for (int i = 0; i < max_w; ++i) _str_white.push_back(CHAR_WHITE);

			if (shift_ver > 0) { // DOWN
				int _last_str_pos = max_w * (max_h - 1) - 1;
				for (int i = 0; i < shift_ver; ++i) { str.insert(0, _str_white); str.erase(_last_str_pos, max_w); }
			}
			else if (shift_ver < 0) { // UP
				for (int i = 0; i < abs(shift_ver); ++i) { str.append(_str_white); str.erase(0, max_w); }
			}
		}
		
		return str;
	}

	void test() {
		int width = 0, height = 0, original_channel_count = 0;
		unsigned char* img = load_image("images/test-img.png", width, height, original_channel_count);
		image_to_str(img, width, height, original_channel_count);
	}

	void test1() {
		int width = 0, height = 0, original_channel_count = 0;
		std::string base_pth = "images/";
		std::string rus_pth = base_pth + "rus-";
		std::string eng_pth = base_pth + "eng-";
		const int letters_cnt = 11;
		std::string rus_ltr[letters_cnt] = {"b", "v", "g", "d", "j", "z", "k", "l", "m", "n", "p"};
		std::string eng_ltr[letters_cnt] = { "comma", "d", "u", "l", "dcomma", "p", "r", "k", "v", "y", "g" };

		std::vector<std::string> img_in;
		std::vector<std::string> img_out;

		unsigned char* img;
		// :: Rus img
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(rus_pth + rus_ltr[i] + ".png", width, height, original_channel_count);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_in.push_back(str);
		}

		// :: ENG img
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(eng_pth + eng_ltr[i] + ".png", width, height, original_channel_count);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_in.push_back(str);
		}
	}

	void test2() {
		#ifdef DISPLAY
		#define FILED
		#ifdef FILED
				std::ofstream outfile("A_output.txt"); // "output.txt"
		#define PRINT(str, w) str_print_file(str, w, outfile)
		#define CLOSE() outfile.close()
		#else
		#define PRINT(str, w) str_print_file(str, w)
		#define CLOSE() 
		#endif
		#else
		#define PRINT(str, w)
		#define CLOSE()
		#endif

		int width = 0, height = 0, original_channel_count = 0, desired_channels = 4;
		std::string base_pth = "images/letter_set_1/";
		std::string rus_pth = base_pth + "InAlfabet";
		std::string eng_pth = base_pth + "OutAlfabet";
		const int letters_cnt = 10;
		std::string numbers[letters_cnt] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

		std::vector<std::string> img_in;
		std::vector<std::string> img_out;

		unsigned char* img;
		// :: Rus img
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(rus_pth + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_in.push_back(str);
		}

		// :: ENG img
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(eng_pth + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_out.push_back(str);
		}

		std::cout << "[GEN]: IN IMG LOADED:" << img_in.size() << std::endl;
		std::cout << "[GEN]: OUT IMG LOADED:" << img_out.size() << std::endl << std::endl;

		//const bool _display = false;
		int _cur_sz = 0;

		// 1. Noise
		int repeat_noise = 20;
		if (TEST) repeat_noise = 3;
		std::cout << "[GEN]: NOISE letters... (" << repeat_noise << " per letter)" << std::endl;
		int img_cnt = img_in.size();
		for (int i = 0; i < img_cnt; ++i) {
			for (int j = 0; j < repeat_noise; ++j) {
				std::string noise = str_noise(img_in.at(i));
				PRINT(noise, img_in_w);
				vec_in.push_back(noise);
				vec_out.push_back(img_out.at(i));
			}
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		// 2. Shifting
		std::cout << "[GEN]: SHIFT letters..." << std::endl;
		Rect letter_rect(4, 3, 7, 7); // 3, 4, 7, 7
		for (int i = 0; i < img_cnt; ++i) {
			int steps_side_max = img_in_w - letter_rect.w;
			int steps_down_max = img_in_h - letter_rect.h;
			if (TEST) { steps_down_max = 1; } // TEST
			std::string zz_img = str_shift(img_in.at(i), -letter_rect.x, -letter_rect.y, img_in_w, img_in_h);
			for (int j = 0; j <= steps_down_max; ++j)
			{
				for (int k = 0; k <= steps_side_max; ++k) {
					std::string ls_in = str_shift(zz_img, k, j, img_in_w, img_in_h);
					PRINT(ls_in, img_in_w); // DISPLAY
					vec_in.push_back(ls_in);
					vec_out.push_back(img_out.at(i));
				}
			}
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		// 3. Shifting + noise
		std::cout << "[GEN]: SHIFT + NOISE letters..." << std::endl;
		repeat_noise = 3;
		if (TEST) { repeat_noise = 1; } // TEST
		for (int i = 0; i < img_cnt; ++i) {
			int steps_side_max = img_in_w - letter_rect.w;
			int steps_down_max = img_in_h - letter_rect.h;
			if (TEST) { steps_side_max /= 2; steps_down_max = 1; } // TEST
			std::string zz_img = str_shift(img_in.at(i), -letter_rect.x, -letter_rect.y, img_in_w, img_in_h);
			for (int j = 0; j <= steps_down_max; ++j)
			{
				for (int k = 0; k <= steps_side_max; ++k) {
					std::string ls_in = str_shift(zz_img, k, j, img_in_w, img_in_h);
					for (int l = 0; l < repeat_noise; ++l) {
						std::string noise = str_noise(ls_in);
						PRINT(noise, img_in_w); // DISPLAY
						vec_in.push_back(noise);
						vec_out.push_back(img_out.at(i));
					}
				}
			}
		}
		std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();

		//4. NOISE + INVERSE
		if (TEST) {
			int repeat_noise = 2;
			std::cout << "[GEN]: NOISE + INVERSE letters... (" << repeat_noise << " per letter)" << std::endl;
			int img_cnt = img_in.size();
			for (int i = 0; i < img_cnt; ++i) {
				for (int j = 0; j < repeat_noise; ++j) {
					std::string noise = str_noise(img_in.at(i));
					noise = str_inverse(noise);
					PRINT(noise, img_in_w);
					vec_in.push_back(noise);
					vec_out.push_back(img_out.at(i));
				}
			}
			std::cout << "[GEN]: Added: " << vec_in.size() - _cur_sz << std::endl; _cur_sz = vec_in.size();
		}

		std::cout << "[GEN]: DONE. LS were added: " << vec_in.size() << std::endl;

		// TEST SHIFT
		/*std::string shift = str_shift(img_in.at(0), -2, -2, 14, 14);
		str_print(shift, 14);*/
	}

	void test3() {
		int width = 0, height = 0, original_channel_count = 0, desired_channels = 4;
		std::string base_pth = "images/letter_set_2_1/";
		std::string rus_pth = base_pth + "rus-";
		std::string eng_pth = base_pth + "eng-";
		const int letters_cnt = 10;
		std::string rus_ltr[letters_cnt] = { "a", "e", "yo", "i", "o", "u", "y", "ae", "yu", "ia" };
		std::string eng_ltr[letters_cnt] = { "f", "t", "`", "b", "j", "e", "s", "\'", ".", "z"};

		// 14x14 7x7

		std::vector<std::string> img_in;
		std::vector<std::string> img_out;

		// Output file
		std::ofstream outfile("I_output.txt");

		unsigned char* img;
		// :: Rus img
		outfile << "* RUS DEFAULT *\n";
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(rus_pth + rus_ltr[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_in.push_back(str);
			//
			str_print_file(str, width, outfile);
		}

		// :: Rus img - italic
		outfile << "\n* RUS ITALIC *\n";
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(rus_pth + rus_ltr[i] + "-italic.png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_in.push_back(str);
			//
			str_print_file(str, width, outfile);
		}

		// :: ENG img x 2
		outfile << "* ENG DEFAULT *\n";
		desired_channels = 3;
		for (int i = 0; i < letters_cnt * 2; ++i) {
			std::string _sgn = eng_ltr[i % letters_cnt];
			desired_channels = (_sgn == "t" || _sgn == "j" || _sgn == "." || _sgn == "f") ? 4 : 3;
			img = load_image(eng_pth + eng_ltr[i % letters_cnt] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			img_out.push_back(str);
			//
			if(i < letters_cnt)
			str_print_file(str, width, outfile);
		}

		outfile << "\n* CORRUPTED LETTERS *\n";

		std::cout << "[GEN]: IN IMG LOADED:" << img_in.size() << std::endl;
		std::cout << "[GEN]: OUT IMG LOADED:" << img_out.size() << std::endl << std::endl;

		const bool _display = true;	

		// 1. Noise
		std::cout << "[GEN]: NOISE letters..." << std::endl;
		int repeat_noise = 10;
		int img_cnt = img_in.size();
		for (int i = 0; i < img_cnt; ++i) {
			for (int j = 0; j < repeat_noise; ++j) {
				std::string noise = str_noise(img_in.at(i));
				if (_display) str_print_file(noise, img_in_w, outfile);
				vec_in.push_back(noise);
				vec_out.push_back(img_out.at(i));
			}
		}
		std::cout << "[GEN]: NOISE DONE. Cur size:" << vec_in.size() << std::endl;

		// 2. Shifting
		std::cout << "[GEN]: SHIFT letters..." << std::endl;
		Rect letter_rect(1, 1, 12, 12); // 3, 4, 7, 7
		for (int i = 0; i < img_cnt; ++i) {
			int steps_side_max = img_in_w - letter_rect.w;
			int steps_down_max = img_in_h - letter_rect.h;
			std::string zz_img = str_shift(img_in.at(i), -letter_rect.x, -letter_rect.y, img_in_w, img_in_h);
			for (int j = 0; j <= steps_down_max; ++j)
			{
				for (int k = 0; k <= steps_side_max; ++k) {
					std::string ls_in = str_shift(zz_img, k, j, img_in_w, img_in_h);
					if (_display) str_print_file(ls_in, img_in_w, outfile); // DISPLAY
					vec_in.push_back(ls_in);
					vec_out.push_back(img_out.at(i));
				}
			}
		}
		std::cout << "[GEN]: SHIFT DONE. Cur size:" << vec_in.size() << std::endl;

		// 3. Shifting + noise
		std::cout << "[GEN]: SHIFT + NOISE letters..." << std::endl;
		repeat_noise = 3;
		for (int i = 0; i < img_cnt; ++i) {
			int steps_side_max = img_in_w - letter_rect.w;
			int steps_down_max = img_in_h - letter_rect.h;
			std::string zz_img = str_shift(img_in.at(i), -letter_rect.x, -letter_rect.y, img_in_w, img_in_h);
			for (int j = 0; j <= steps_down_max; ++j)
			{
				for (int k = 0; k <= steps_side_max; ++k) {
					std::string ls_in = str_shift(zz_img, k, j, img_in_w, img_in_h);
					for (int l = 0; l < repeat_noise; ++l) {
						std::string noise = str_noise(ls_in);
						if (_display) str_print_file(noise, img_in_w, outfile); // DISPLAY
						vec_in.push_back(noise);
						vec_out.push_back(img_out.at(i));
					}
				}
			}
		}
		std::cout << "[GEN]: SHIFT + NOISE DONE. Cur size:" << vec_in.size() << std::endl;

		// 4. NOISE + INVERSE
		if (TEST) {
			int repeat_noise = 4;
			std::cout << "[GEN]: NOISE + INVERSE letters...(" << repeat_noise << " per letter)" << std::endl;
			int img_cnt = img_in.size();
			for (int i = 0; i < img_cnt; ++i) {
				for (int j = 0; j < repeat_noise; ++j) {
					std::string noise = str_noise(img_in.at(i));
					noise = str_inverse(noise);
					if (_display) str_print_file(noise, img_in_w, outfile);
					vec_in.push_back(noise);
					vec_out.push_back(img_out.at(i));
				}
			}
			std::cout << "[GEN]: NOISE + INVERSE DONE. Cur size:" << vec_in.size() << std::endl;
		}

		std::cout << "[GEN]: DONE. LS were added: " << vec_in.size() << std::endl;
		outfile.close();
	}

	void test_n() {
		int width = 0, height = 0, original_channel_count = 0, desired_channels = 3;
		std::string base_pth = "images/letter_test_n/";
		std::string rect_pth = base_pth + "rect";
		std::string square_pth = base_pth + "square";
		const int letters_cnt = 19;
		std::string numbers[letters_cnt] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };

		//std::vector<std::string> img_in;
		//::vector<std::string> img_out;
		unsigned char* img;

		// :: Square img
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(square_pth + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			if (i == 0) str = str_one2(str);
			if (i == 18) str = str_one2_white(str);
			vec_in.push_back(str);
		}
		// :: Rect img
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(rect_pth + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			if (i == 0) str = str_one2(str);
			if (i == 18) str = str_one2_white(str);
			vec_in.push_back(str);
		}
		// :: Out img
		for (int i = 0; i < letters_cnt; ++i) {
			//vec_out.push_back(vec_in.at(0));	// squares
			img = load_image(square_pth + "_out.png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_out.push_back(str);
		}
		for (int i = 0; i < letters_cnt; ++i) {
			//vec_out.push_back(vec_in.at(letters_cnt));	// rects
			img = load_image(rect_pth + "_out.png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_out.push_back(str);
		}

		// Replace 1-2 samples
		//vec_in[0] = str_one2(vec_in.at(0)); // for square
		//vec_in[letters_cnt] = str_one2(vec_in.at(letters_cnt)); // for rect
	}

	void test_a() {
		int width = 0, height = 0, original_channel_count = 0, desired_channels = 4;
		std::string base_pth = "images/letter_test_a/";
		std::string eng_pth = "images/letter_set_1/OutAlfabet";
		const int letters_cnt = 10;
		std::string numbers[letters_cnt] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

		unsigned char* img;

		// :: 1na2
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "1na2/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			str = str_one2(str);
			vec_in.push_back(str);
		}
		// :: Chess
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "Chess/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_in.push_back(str);
		}
		// :: DownRightAngle
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "DownRightAngle/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_in.push_back(str);
		}
		// :: LeftsideClear
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "LeftsideClear/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_in.push_back(str);
		}
		// :: Reverse180
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "Reverse180/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_in.push_back(str);
		}
		// :: RightsideClear
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "RightsideClear/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_in.push_back(str);
		}
		// :: UpperLeftAngle
		for (int i = 0; i < letters_cnt; ++i) {
			img = load_image(base_pth + "UpperLeftAngle/InAlfabet" + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
			std::string str = image_to_str(img, width, height, original_channel_count);
			vec_in.push_back(str);
		}

		// :: ENG img
		for(int j = 0; j < 7; ++j)
			for (int i = 0; i < letters_cnt; ++i) {
				img = load_image(eng_pth + numbers[i] + ".png", width, height, original_channel_count, desired_channels);
				std::string str = image_to_str(img, width, height, original_channel_count);
				vec_out.push_back(str);
			}

		// New
		img = load_image(base_pth + "1na2/InAlfabet" + numbers[0] + ".png", width, height, original_channel_count, desired_channels);
		std::string str = image_to_str(img, width, height, original_channel_count);
		str = str_one2_white(str);
		vec_in.push_back(str);

		img = load_image(base_pth + "1na2/InAlfabet" + numbers[1] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		str = str_one2_white(str);
		vec_in.push_back(str);

		img = load_image(base_pth + "UpperLeftAngle/InAlfabet" + numbers[8] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		str = str_one2_white(str);
		vec_in.push_back(str);

		img = load_image(base_pth + "UpperLeftAngle/InAlfabet" + numbers[9] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		str = str_one2_white(str);
		vec_in.push_back(str);

		//out
		img = load_image(eng_pth + numbers[0] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		vec_out.push_back(str);
		img = load_image(eng_pth + numbers[1] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		vec_out.push_back(str);
		img = load_image(eng_pth + numbers[8] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		vec_out.push_back(str);
		img = load_image(eng_pth + numbers[9] + ".png", width, height, original_channel_count, desired_channels);
		str = image_to_str(img, width, height, original_channel_count);
		vec_out.push_back(str);
	}
};

