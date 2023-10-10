#version 330 core
layout (location = 0) in vec3 aPos;   // позиция атрибута, содержащего переменные координат, устанавливается в 0
layout (location = 1) in vec3 aColor; // позиция атрибута, содержащего переменные цвета, устанавливается в 1
layout (location = 2) in vec2 aTexCoord; // координаты текстуры
  
out vec3 ourColor; // на выходе – значение цвета, передаваемое во фрагментный шейдер
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor; // присваиваем переменной ourColor значение цвета, которое мы получили из данных вершины
    TexCoord = aTexCoord;
}