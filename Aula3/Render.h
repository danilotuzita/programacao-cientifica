#pragma once

#include <string>
#include <fstream>

class Render
{
private:
	std::string filename;
	std::fstream f;
	int offset_x, offset_y;
	int width, height;
public:
	Render(std::string _filename="index", int width=720, int height=480, int font_size=10, int ox=10, int oy=10);
	~Render();
	void line(int x1, int y1, int x2, int y2);
	void point(int x, int y, float r, std::string t="");
	void text(int x, int y, std::string t);
	void close_file();
	void open_html();
};

