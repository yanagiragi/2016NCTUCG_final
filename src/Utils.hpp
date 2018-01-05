#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace Utils{

	template<typename Out> void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	void screenshot_ppm_DepthPow(const char *filename, unsigned int width, unsigned int height, GLfloat *pixels, GLuint tex, float powFactor) {
		size_t i, j, k, cur;
		const size_t format_nchannels = 1;
		FILE *f = fopen(filename, "w");
		fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
		glGetTextureImage(tex, 0, GL_DEPTH_COMPONENT, GL_FLOAT, sizeof(GLfloat) * width * height, pixels);
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				cur = format_nchannels * ((height - i - 1) * width + j);
				float p = pow((pixels)[cur], powFactor);
				int pInt = p * 255.0;
				fprintf(f, "%3d %3d %3d ", pInt, pInt, pInt);
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}

	void screenshot_ppm_Depth(const char *filename, unsigned int width, unsigned int height, GLfloat *pixels, GLuint tex) {
		size_t i, j, k, cur;
		const size_t format_nchannels = 1;
		FILE *f = fopen(filename, "w");
		fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
		glGetTextureImage(tex, 0, GL_DEPTH_COMPONENT, GL_FLOAT, sizeof(GLfloat) * width * height, pixels);
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				cur = format_nchannels * ((height - i - 1) * width + j);
				float p = (pixels)[cur];
				int pInt = p * 255.0;
				fprintf(f, "%3d %3d %3d ", pInt, pInt, pInt);
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}

	void screenshot_ppm_Depth(const char *filename, unsigned int width, unsigned int height, GLfloat *pixels) {
		size_t i, j, k, cur;
		const size_t format_nchannels = 1;
		FILE *f = fopen(filename, "w");
		fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				cur = format_nchannels * ((height - i - 1) * width + j);
				float p = (pixels)[cur];
				int pInt = p * 255.0;
				fprintf(f, "%3d %3d %3d ", pInt, pInt, pInt);
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}

	void screenshot_ppm_RGB(const char *filename, unsigned int width, unsigned int height, GLubyte *pixels) {
		size_t i, j, k, cur;
		const size_t format_nchannels = 3;
		FILE *f = fopen(filename, "w");
		fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
		pixels = (GLubyte*)realloc(pixels, format_nchannels * sizeof(GLubyte) * width * height);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		int sum = 0;
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				cur = format_nchannels * ((height - i - 1) * width + j);
				fprintf(f, "%3d %3d %3d ", (pixels)[cur], (pixels)[cur + 1], (pixels)[cur + 2]);
				sum += (int)(pixels)[cur] + (int)(pixels)[cur + 1] + (int)(pixels)[cur + 2];
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}

};

#endif // !_UTILS_HPP