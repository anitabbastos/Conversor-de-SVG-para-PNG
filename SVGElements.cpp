#include "SVGElements.hpp"
//adicionado por mim
#include <vector>
using namespace std;

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->


    //adionado por mim
    //ellipse classes virtuais
    void Ellipse::translate(const Point& t){
        center = center.translate(t);
    }
    void Ellipse::rotate(const Point &origin, int degrees){
        center = center.rotate(origin,degrees);
    }
    void Ellipse::scale(const Point &origin, int v){
        center = center.scale(origin,v);
        radius.x *= v;
        radius.y *= v;
    }

    SVGElement* Ellipse::duplication() const{
        return new Ellipse(fill,center,radius);
    }


    //circle
    Circle::Circle(const Color &fill, 
                    const Point &center, 
                    int radius)
        : Ellipse(fill,center,{radius,radius})
    {
    }

    SVGElement* Circle::duplication() const{
        return new Circle(*this);
    }


    //polyline
    Polyline::Polyline(const Color &stroke, 
                        const vector <Point> &points)
        : stroke(stroke), points(points)
    {
    }
    
    void Polyline::draw(PNGImage &img) const
    {
        for (size_t i=0; i+1<points.size();i++){ //para desenhar linha a linha
            img.draw_line(points[i],points[i+1],stroke);
        }
    }
    void Polyline::translate(const Point& t){
        for (Point& p : points){ //para cada ponto
            p=p.translate(t);
        }
    }
    void Polyline::rotate(const Point &origin, int degrees){
        for (Point& p : points){
            p=p.rotate(origin,degrees);
        }
    }
    void Polyline::scale(const Point &origin, int v){
        for (Point& p : points){
            p=p.scale(origin,v);
        }
    }

    SVGElement* Polyline::duplication() const{
        return new Polyline(stroke,points);
    }


    //line
    Line::Line(const Color &stroke, 
                const Point &start, 
                const Point &end)
        : Polyline(stroke, vector<Point>{start,end})
    {
    }

    SVGElement* Line::duplication() const{
        return new Line(*this);
    }


    //polygon
    Polygon::Polygon(const Color &fill, 
                    const vector <Point> &points)
        : fill(fill), points(points)
    {
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
    void Polygon::translate(const Point& t){
        for (Point& p : points){
            p=p.translate(t);
        }
    }
    void Polygon::rotate(const Point &origin, int degrees){
        for (Point& p : points){
            p=p.rotate(origin,degrees);
        }
    }
    void Polygon::scale(const Point &origin, int v){
        for (Point& p : points){
            p=p.scale(origin,v);
        }
    }

    SVGElement* Polygon::duplication() const{
        return new Polygon(fill,points);
    }


    //rect
    Rect::Rect(const Color& fill, const Point& coordinates, int width, int height)
        : Polygon(fill, vector<Point>{coordinates, //upper left
                                    {coordinates.x + width - 1, coordinates.y}, //upper right
                                    {coordinates.x + width - 1, coordinates.y + height - 1}, //lower right
                                    {coordinates.x,coordinates.y + height - 1} //lower left
                                    })
    {
    }

    SVGElement* Rect::duplication() const{
        return new Rect(*this);
    }


    //group
    Group::Group(const vector<SVGElement*>& elements)
        : group_elements(elements)
    {
    }

    Group::~Group()
    {
        for (SVGElement* e : group_elements){
            delete e;
        }
    }

    void Group::draw(PNGImage& img) const 
    {
        for (SVGElement* element : group_elements) {
            element->draw(img); //para cada elemento do grupo desenho esse elemento na img
        }
    }
    void Group::translate(const Point& t) {
        for (SVGElement* element : group_elements) {
            element->translate(t);
        }
    }
    void Group::rotate(const Point& origin, int degrees) {
        for (SVGElement* element : group_elements) {
            element->rotate(origin, degrees);
        }
    }
    void Group::scale(const Point& origin, int v) {
        for (SVGElement* element : group_elements) {
            element->scale(origin, v);
        }
    }

    SVGElement* Group::duplication() const {
        vector<SVGElement*> elements;

        for (SVGElement* element : group_elements) {
            elements.push_back(element->duplication());
        }

        return new Group(elements);
    }
}