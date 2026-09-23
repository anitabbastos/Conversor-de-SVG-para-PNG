# Conversor-de-SVG-para-PNG
Desenvolvimento de uma aplicação em C++ capaz de interpretar ficheiros SVG e convertê-los em imagens PNG, utilizando programação orientada a objetos, hierarquias de classes, parsing de elementos SVG e transformações geométricas.

## Group elements

Identify all group elements (numbers and names).

- up202506456 Anita Brandão de Bastos
- up202508237 Maria Beatriz Carvas Dias
- up202506219 Mariana Cancela Morim


## Accomplished tasks

Brief summary of what you implemented.

Task 1: Read an <svg> tree
Na função readSVG, foi criado um ciclo for para percorrer todos os elementos filhos do elemento principal <svg>. Para cada filho, foi obtido o seu nome, permitindo identificar o tipo de elemento SVG a ler.
Inicialmente, foi tratado o caso da ellipse, aproveitando a classe Ellipse já existente. Foram lidos os seus atributos, criado um objeto com new Ellipse, e o respetivo ponteiro foi guardado no vetor svg_elements.

Task 2: Add support to read geometrical elements
Começámos por criar classes para o resto dos elementos: circle, polyline, line, polygon e rect, tendo em atenção que circle pode ser um caso especial de ellipse, line pode ser um caso especial de polyline e rect pode ser um caso especial de polygon.
Guiámo-nos pela class Ellipse que já existia no projeto.
Dentro do ciclo for feito na Task 1 fomos acrescentando vários else if para reconhecer cada tipo de elemento. Em cada caso, foram lidos os atributos correspondentes do SVG, criada a respetiva classe com new, e guardado o ponteiro no vetor svg_elements. (também foi implementado o método draw() em cada classe)

Task 3: Add support for element transformations
Depois de analisar o ficheiro Point.hpp, adicionámos cada um dos métodos de transformação (translate, rotate e scale) à classe SVGElement. Implementamos cada método nas classes: Ellipse, Polyline, Polygon.
Acabámos por criar uma função auxiliar apply_transformations no ficheiro readSVG, que lê os atributos transform e transform-origin e aplica as transformações ao elemento antes de este ser guardado.

Task 4: Add support for groups (<g> elements)
Criámos a classe Group onde implementámos os métodos draw, translate, rotate e scale.
Para suportar grupos no readSVG.cpp criámos a função read_elements, que recebe um elemento pai e percorre todos os seus filhos. Depois colocámos dentro dessa função toda a lógica de leitura dos elementos SVG. Assim, a mesma função serve para ler os filhos do <svg> e também os filhos de um <g>.

Task 5: Add support for element duplication (<use> elements)
Foi criado um map chamado ids, que associa cada id ao respetivo ponteiro para SVGElement. Sempre que um elemento SVG é criado, verificámos se tem o atributo id.
Também adicionámos à classe base SVGElement isto:
    virtual SVGElement* duplication() const = 0;
e implementámos nas classes derivadas para permitir criar uma cópia independente de qualquer elemento SVG.
Quando encontramos um <use>, lemos o atributo href, removemos o #, procuramos o elemento original no mapa ids e criámos uma cópia através de duplication(). Depois aplicamos à cópia as transformações próprias do <use> e guardamos essa cópia no vetor svg_elements.
