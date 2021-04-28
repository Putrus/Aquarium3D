#version 330

//Zmienne jednorodne
uniform mat4 P; //macierz rzutowania
uniform mat4 V; //macierz widoku
uniform mat4 M; //macierz modelu

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=1) in vec4 normal; //wektor normalny w przestrzeni modelu
layout (location=2) in vec2 texCoord0; //wspolrzedne teksturowania

//Zmienne interpolowane opisane są w mainie
out vec4 l;
out vec4 n;
out vec4 v; 
out vec2 iTexCoord0; //wspolrzedne tekstury obiektu

void main(void) {
    vec4 lp = vec4(10, 50, -10, 1); //pozycyja światła, przestrzeń świata
    l = normalize(V * lp - V*M*vertex); //wektor do światła w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
	iTexCoord0 = texCoord0; //wspolrzedne teksturowania ktore przekazujemy do fragment shadera
    gl_Position=P*V*M*vertex; //pozycja przeksztalcona do 3D
}
