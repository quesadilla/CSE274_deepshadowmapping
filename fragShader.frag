
varying vec4 diffuse,ambientGlobal, ambient, ecPos;
varying vec3 normal,halfVector;
 
void main()
{
	float dist;
    vec3 n,halfV, lightDir;
    float NdotL,NdotHV;
    vec4 color = ambientGlobal;
    float att,spotEffect;
     
    /* a fragment shader can't write a varying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);
     
    // Compute the light direction
    lightDir = vec3(gl_LightSource[1].position-ecPos);
     
    /* compute the distance to the light source to a varying variable*/
    dist = length(lightDir);
 
    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n,normalize(lightDir)),0.0);
 
    if (NdotL > 0.0) {
     
        spotEffect = dot(normalize(gl_LightSource[1].spotDirection), normalize(-lightDir));
        if (spotEffect > gl_LightSource[1].spotCosCutoff) {
            spotEffect = pow(spotEffect, gl_LightSource[1].spotExponent);
            att = spotEffect / (gl_LightSource[1].constantAttenuation +
                    gl_LightSource[1].linearAttenuation * dist +
                    gl_LightSource[1].quadraticAttenuation * dist * dist);
                 
            color += att * (diffuse * NdotL + ambient);
         
             
            halfV = normalize(halfVector);
            NdotHV = max(dot(n,halfV),0.0);
            color += att * gl_FrontMaterial.specular * gl_LightSource[1].specular * pow(NdotHV,gl_FrontMaterial.shininess);
        }
    }
 
    gl_FragColor = color;
}