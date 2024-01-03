#version 330


uniform sampler2D textureMap0; //uchwyt/ID do tekstury obiektu

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//zmienne interpolowane, które przekazaliśmy w vertex shaderze
in vec4 n;
in vec4 l;
in vec4 v;
in vec2 iTexCoord0;

void main(void) {

	//Znormalizowane interpolowane wektory, trzeba tak zrobić bo niekiedy przekazanie wektorów z vertex shadera do fragment powoduje, że nie są znormalizowane
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);   

	//Parametry powierzchni
	//vec4 kd = vec4(1,0,0,1);
	vec4 kd = texture(textureMap0, iTexCoord0); //kolor obiektu
	vec4 ks = vec4(0,0.7,1,1); //kolor otoczenia

	kd = mix(kd, ks, 0.2); //kolor obiektu(czyli mix koloru otoczenia i koloru obiektu)
	ks = vec4(1,1,1,1); //kolor swiatla
	
	//Obliczenie modelu oświetlenia
	
	float nl = clamp(dot(mn, ml), 0, 1); //iloczyn wektoru normalnego i wektora z jakim pada swiatlo
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50); //iloczyn wektora odbitego i wierzcholka
	
	
	//dla metalu texColor.rgb, a dla innych ks
	//wzor na Model Phonga
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0); 
}
