#include "Render.h"

Render::Render(std::string _filename, int width, int height, int font_size, int ox, int oy)
{
	filename = _filename + ".html";
	
	this->width = width;
	this->height = height;

	offset_x = ox;
	offset_y = oy;

	f.open(filename.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	
	//printf("%s\n", filename.c_str());
	if (!f.is_open())
	{
		printf("im not open\n");
		return;
	}

	f << "<!DOCTYPE html><html><body><div style=\"width: 100%;text-align:center;\"><canvas id=\"myCanvas\" width=\"" + std::to_string(width + (offset_x * 2)) + "\" height=\"" + std::to_string(height + (offset_y * 2)) + "\" style=\"border:1px solid #d3d3d3;\">Your browser does not support the canvas element.</canvas></div><script>var canvas = document.getElementById(\"myCanvas\");var ctx = canvas.getContext(\"2d\");ctx.font = \" " + std::to_string(font_size) + "px Arial\";";
	f << "fs = []; map = 0;";
	f << "window.addEventListener('keydown',this.check,false);function check(e) {var code = e.keyCode;";
	f << "if ((code == 13 || code == 39) && map < fs.length - 2) { map++; fs[map]() };"; // enter or right
	f << "if (code == 37 && map > 0) {map--; fs[map]();}"; // left
	f << "if (code == 32 || code == 35) { map = fs.length - 2; fs[map](); }"; // space or end
	f << "if (code == 36) { map = 0; fs[map](); } }"; // home
	numberOfMaps = 0;
	newMap();
}

Render::~Render()
{
	close_file();
}

void Render::line(int x1, int y1, int x2, int y2)
{
	f << "ctx.moveTo(" + std::to_string(x1 + offset_x) + ", " + std::to_string(y1 + offset_y) + ");";
	f << "ctx.lineTo(" + std::to_string(x2 + offset_x) + ", " + std::to_string(y2 + offset_y) + ");";
	f << "ctx.stroke();";
}

void Render::point(int x, int y, float r, std::string t)
{
	f << "ctx.beginPath();";
	f << "ctx.arc(" + std::to_string(x + offset_x) + ", " + std::to_string(y + offset_y) + ", " + std::to_string(r) + ", 0, 2 * Math.PI);";
	f << "ctx.stroke();";
	text(x + 5, y - 5, t);
}

void Render::text(int x, int y, std::string t)
{
	/*if (x < 5) x = 5;
	if (y < 5) y = 5;
	if (x > width - 5) x = width - 5;
	if (x > height - 5) y = height - 5;
	*/
	f << "ctx.fillText(\" " + t + " \", " + std::to_string(x + offset_x) + ", " + std::to_string(y + offset_y) + ");";
}

void Render::close_file()
{
	if (f.is_open())
	{
		//printf("closing file\n");
		closeMap();
		f << "fs[0]();</script></body></html>";
		f.close();
	}
}

void Render::open_html()
{
	close_file();
	system(filename.c_str());
}

void Render::newMap()
{
	if(numberOfMaps) closeMap();
	f << "function f" << numberOfMaps << "() {";
	f << "ctx.clearRect(0, 0, canvas.width, canvas.height);";
	text(-30, -30, "Page " + std::to_string(numberOfMaps));
	numberOfMaps++;
}

void Render::closeMap()
{
	f << "} fs.push(f" << numberOfMaps - 1 << ");";
}

/*
37 esq
39 dir
32 esp
36 hom
35 end
38 cim
40 bai
*/
