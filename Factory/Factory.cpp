#include <algorithm>
#include <vector>
#include <iostream>
#include <numeric>
#include <thread>
#include <future>
#include <exception>
#include <stdexcept>
#include <memory>
#include <map>
#include <string>
#include <functional>

class Image{
public:
	virtual ~Image() = default ;
	virtual void printImage() const = 0 ;
} ;
class BitmapImage : public Image {
public:
	void printImage() const override { std::cout << "bitmap" << std::endl ; }
} ;
class PngImage : public Image {
	public:
	void printImage() const override { std::cout << "pngmap" << std::endl ; }
} ;
class JpgImage : public Image {
	public:
	void printImage() const override { std::cout << "jpgmap" << std::endl ; }
} ;


struct IImageFactory 
{
	virtual ~IImageFactory() = default ;
	virtual std::shared_ptr<Image> Create( std::string_view type ) = 0 ;
} ;

struct ImageFactory : public IImageFactory
{
	std::shared_ptr<Image> Create( std::string_view type ) override {

	auto it = mapping.find(type.data()) ;
	if ( it != mapping.end() ) {
		return it->second() ;
	}
	return nullptr ;
	}
private:
		static std::map<std::string, std::function<std::shared_ptr<Image>()>> mapping ;
} ;

		std::map<std::string, std::function<std::shared_ptr<Image>()>> ImageFactory::mapping
		{
			{ "bmp", []() {return std::make_shared<BitmapImage>(); } },
			{ "png", []() {return std::make_shared<PngImage>(); } },
			{ "jpg", []() {return std::make_shared<JpgImage>(); } }
		} ;

int main() {

	auto imageFactory = ImageFactory() ;

	auto bitmap = imageFactory.Create("bmp") ;
	bitmap->printImage() ;
}