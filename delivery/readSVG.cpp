
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

//adicionado por mim
#include <vector>
#include <sstream>
#include <string>
#include <map>

namespace svg
{
    //começa aqui
    void apply_transformations(XMLElement* child, SVGElement* element) {
        const char* transformA = child->Attribute("transform"); //vai buscar o atributo "transform" do elemento XML
        if (transformA == nullptr){
            return; 
        }
        Point origin = {0,0};

        const char* originA = child->Attribute("transform-origin"); //vai buscar o atributo "transform-origin" do elemento XML

        if (originA != nullptr) { //ver se tem alguma operação em transform-origin
            stringstream ss(originA);
            int x;
            int y; 
            ss >> x >> y;

            origin = {x,y};
        }

        string transform = transformA;
        
        for (char& c : transform) {
            if (c == '(' || c == ')' || c == ',') {
                c = ' '; 
            }
        }

        stringstream ss(transform);
        string transformation; 

        while (ss >> transformation) {
            if (transformation == "translate") {
                int x; //deslocamento em x
                int y; //descolamento em y

                ss >> x >> y;

                Point p = {x, y};

                element->translate(p); 
            }

            else if (transformation == "rotate") {
                int degrees; //angulo da rotação

                ss >> degrees; 

                element->rotate(origin, degrees); //roda o elemento à volta da origem definida
            }

            else if (transformation == "scale") {
                int v; //fator de escala

                ss >> v;

                element->scale(origin, v);
            }
        }
    }

    void read_elements(XMLElement* parent, vector<SVGElement*>& elements, map<string, SVGElement*>& ids){ //parent é por exemplo <svg> ou <g> (um elemento que tem filhos)
        for (XMLElement *child = parent->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
            string element_name = child->Name();// encontro o elemento e o nome dele
            
            //ellipse
            if (element_name == "ellipse"){
                const char* fill_color = child->Attribute ("fill"); //li a cor
                int cx = child->IntAttribute("cx"); 
                int cy = child->IntAttribute("cy"); //li o centro
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry"); //li o raio
                
                Color fill = parse_color(fill_color); //converte a cor que veio em texto para um objeto Color
                Point center = {cx,cy}; 
                Point radius = {rx,ry};
                
                SVGElement* element = new Ellipse(fill, center, radius);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id"); //vai buscar o atributo "id" do elemento XML
                    if (idA != nullptr) { //verifica se o elemento tem mesmo um id
                        ids[idA] = element; //guarda o elemento no mapa ids, a chave é o id do SVG e o valor é o ponteiro para o elemento. isto permite encontrar este elemento mais tarde pelo seu id.
                    }
            }


            //circle
            else if (element_name == "circle"){
                const char* fill_color = child->Attribute ("fill"); 
                int cx = child->IntAttribute("cx"); 
                int cy = child->IntAttribute("cy");
                int r = child->IntAttribute("r"); 
                
                Color fill = parse_color(fill_color);
                Point center = {cx,cy};
                //raio já está criado
                
                SVGElement* element = new Circle(fill, center, r);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id");
                    if (idA != nullptr) {
                        ids[idA] = element;
                    }
            }


            //polyline 
            else if (element_name == "polyline"){
                const char* stroke_color = child->Attribute("stroke");
                const char* pointsA = child->Attribute("points");

                Color stroke = parse_color(stroke_color); 
                vector<Point> points; //cria vetor onde guardar os points
                string points_string = pointsA; //converte const char* para string para facilitar a alteração do texto
                
                for (char& c : points_string) {
                    if (c == ',') {
                        c = ' ';
                    }
                }

                stringstream ss(points_string);
                int x;
                int y;

                while (ss >> x >> y){
                    points.push_back({x, y});
                }

                SVGElement* element = new Polyline(stroke, points);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id");
                    if (idA != nullptr) {
                        ids[idA] = element;
                    }
            }


            //line
            else if (element_name == "line"){
                const char* stroke_color = child->Attribute ("stroke");
                int x1 = child->IntAttribute("x1"); 
                int y1 = child->IntAttribute("y1"); //li o ponto onde começa a linha
                int x2 = child->IntAttribute("x2"); 
                int y2 = child->IntAttribute("y2"); //li o ponto onde termina a linha

                Color stroke = parse_color(stroke_color);
                Point start = {x1, y1};
                Point end = {x2, y2};

                SVGElement* element = new Line(stroke,start,end);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id");
                    if (idA != nullptr) {
                        ids[idA] = element;
                    }
            }


            //polygon
            else if (element_name == "polygon") {
                const char* fill_color = child->Attribute("fill");
                const char* pointsA = child->Attribute("points");

                Color fill = parse_color(fill_color);
                vector<Point> points;
                string points_string = pointsA;
                
                for (char& c : points_string) {
                    if (c == ',') {
                        c = ' ';
                    }
                }

                stringstream ss(points_string);
                int x;
                int y;

                while (ss >> x >> y){
                    points.push_back({x, y});
                }

                SVGElement* element = new Polygon(fill, points);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id");
                    if (idA != nullptr) {
                        ids[idA] = element;
                    }
            }


            //rect
            else if (element_name == "rect"){
                const char* fill_color = child->Attribute("fill");
                int x = child->IntAttribute("x");
                int y = child->IntAttribute("y");
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");

                Color fill = parse_color(fill_color);
                Point coordinates = {x, y};

                SVGElement* element = new Rect(fill, coordinates, width, height);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id");
                    if (idA != nullptr) {
                        ids[idA] = element;
                    }
            }


            //<g>
            else if (element_name == "g") {
                vector<SVGElement*> group_elements;

                read_elements(child, group_elements, ids);

                SVGElement* element = new Group(group_elements);
                apply_transformations(child, element);
                elements.push_back(element);

                const char* idA = child->Attribute("id");
                    if (idA != nullptr) {
                        ids[idA] = element;
                    }
            }


            //<use>
            else if (element_name == "use") {
                const char* hrefA = child->Attribute("href"); //href indica qual elemento já existente vai ser reutilizado

                if (hrefA == nullptr){
                    return;
                }

                string href = hrefA;
                href.erase(0, 1); //remove o primeiro caractere, isto porque no map ids o id está guardado sem o símbolo #

                SVGElement* original = ids[href];

                if (original == nullptr){
                    return;
                }

                SVGElement* copy = original->duplication(); //cria uma cópia do elemento original

                apply_transformations(child, copy);
                elements.push_back(copy);
                const char* idA = child->Attribute("id");

                if (idA != nullptr) {
                    ids[idA] = copy;
                }
                    
            }
        }
    }

//já estava escrito
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");

        map<string, SVGElement*> ids;
        read_elements(xml_elem, svg_elements, ids);
    }
}