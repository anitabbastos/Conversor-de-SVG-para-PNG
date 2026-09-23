//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
//adicionado por mim
#include <vector>
using namespace std;

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;

        //classes virtuais + class duplication
        virtual void translate(const Point& t) = 0; 
        virtual void rotate(const Point &origin, int degrees) = 0;
        virtual void scale(const Point &origin, int v) = 0; 

        virtual SVGElement* duplication() const = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

        //adicionado por mim
        void translate(const Point& t) override;
        void rotate(const Point &origin, int degrees) override;
        void scale(const Point &origin, int v) override;

        SVGElement* duplication() const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };

    //circle
    class Circle : public Ellipse
    {
    public:
        Circle(const Color &fill, const Point &center, int radius);

        SVGElement* duplication() const override;
    };


    //polyline
    class Polyline : public SVGElement
    {   
    public:
        Polyline(const Color &stroke, const vector <Point> &points); //stroke = cor da linha

        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point &origin, int degrees) override;
        void scale(const Point &origin, int v) override;

        SVGElement* duplication() const override;

    private:
        Color stroke;
        vector <Point> points;
    };
    

    //line
    class Line : public Polyline
    {
    public:
        Line(const Color &stroke, const Point &start, const Point &end);

        SVGElement* duplication() const override;
    };


    //polygon
    class Polygon : public SVGElement
    {
    public:
        Polygon(const Color &fill, const vector <Point> &points);

        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point &origin, int degrees) override;
        void scale(const Point &origin, int v) override;

        SVGElement* duplication() const override;

    private:
        Color fill;
        vector <Point> points;
    };


    //rect
    class Rect : public Polygon 
    {
    public:
        Rect(const Color& fill, const Point& xy, int width, int height);

        SVGElement* duplication() const override;

    };

    
    //group
    class Group : public SVGElement 
    {
    public:
        Group(const vector<SVGElement*>& elements);
        ~Group();
        
        void draw(PNGImage &img) const override;
        virtual void translate(const Point& t) override;
        virtual void rotate(const Point &origin, int degrees) override;
        virtual void scale(const Point &origin, int v) override;

        SVGElement* duplication() const override;
    
    private:
        vector<SVGElement*> group_elements;
    };

}
#endif
