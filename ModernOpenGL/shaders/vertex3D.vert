#version 330 core
layout (location = 0) in vec3 aPos;   // позиция атрибута, содержащего переменные координат, устанавливается в 0
layout (location = 1) in vec2 aTexCoord; // координаты текстуры
  
out vec3 ourColor; // на выходе – значение цвета, передаваемое во фрагментный шейдер
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = vec3(1.0); // присваиваем переменной ourColor значение цвета, которое мы получили из данных вершины
    TexCoord = aTexCoord;
}