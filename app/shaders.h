//
// Created by larsm on 06.09.2023.
//

#ifndef PROG2002_SHADERS_H
#define PROG2002_SHADERS_H

#include "string"

const std::string gridVertexShader = R"(
    #version 460 core

    layout (location = 0) in vec2 position;

    out vec3 vs_tcoords;
    out vec2 positions;
    out vec3 Normal;

    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modMat;
    uniform vec3 u_normals;


    void main()
    {
        gl_Position = u_projMat * u_viewMat * u_modMat * vec4(position.x, 0.0f, position.y, 1.0f);
        mat4 viewModMat = u_viewMat * u_modMat;
        vs_tcoords = vec3(u_modMat * vec4(position.x,0.0f,position.y,0.0f));
        positions = position;
        Normal = u_normals;
    }
)";

const std::string gridFragmentShader = R"(
	#version 460 core

    #define M_PI 3.141592653589

    in vec3 vs_tcoords;
    in vec2 positions;
    in vec3 Normal;

    out vec4 fragColor;

    uniform vec3 u_color1 = vec3(0.0); // (usually) black tiles
    uniform vec3 u_color2 = vec3(1.0); // (usually) white tiles
    uniform int u_texture;
    uniform uint u_highlightedLow;
    uniform uint u_highlightedHigh;

    //Lighting
    uniform float u_ambientStrength;
    uniform vec3 u_lightSourcePosition;
    uniform float u_diffuseStrength;
    uniform vec3 u_lightColor;
    uniform vec3 u_cameraPosition;
    uniform float u_specularStrength;

    layout(binding=1) uniform sampler2D u_lightTextureSampler;

    bool isBitSet(uint value, int bitIndex) {
        // Create a bitmask with the specified bitIndex set
        uint bitmask = 1u << bitIndex;

        // Use bitwise AND to check if the specified bit is set
        return (value & bitmask) != 0;
    }

    void main()
    {
           // Get the tile coordinates
        int tileX = int(mod(positions.x * 8.0, 8.0));
        int tileY = int(mod(positions.y * 8.0, 8.0));

        // Use alternating colors for even and odd tiles
        vec3 color;
        if ((tileX + tileY) % 2 == 0) {
            // Even tile
            fragColor = vec4(0.0, 0.0, 0.0, 0.0); // Black
        } else {
            // Odd tile
            fragColor = vec4(1.0, 1.0, 1.0, 1.0); // White
        }

        // Highlight the tile if it is selected
        uint tileIndex = uint(tileX * 8 + tileY);
        if (isBitSet(u_highlightedLow, int(tileIndex)) || isBitSet(u_highlightedHigh, int(tileIndex))) {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red
        }

        //Ambient color
        vec3 ambient = u_ambientStrength * u_lightColor;

        // Apply texture if enabled
        if (u_texture == 1) {
            fragColor = mix(fragColor, texture(u_lightTextureSampler, positions),  fragColor == vec4(u_color2, 1) ? 0.9f : 0.5f);
        }


        // Diffuse reflection
        vec3 norm = normalize(Normal);
        vec3 lightDirection = normalize(u_lightSourcePosition - vs_tcoords.xyz);
        float diffuseStrength = max(dot(norm, lightDirection), 0.0f) * u_diffuseStrength;
        vec3 diffuse = diffuseStrength * u_lightColor;

        // Specular reflection
        vec3 viewDir = normalize(u_cameraPosition - vs_tcoords.xyz);
        vec3 reflectedLight = reflect(-lightDirection, norm);
        float specFactor = pow(max(dot(viewDir, reflectedLight), 0.0f), 64);
        float spec = specFactor * u_specularStrength;
        vec3 specular = spec * u_lightColor;

        // Combine results
        vec3 fRGB = vec3(fragColor.r,fragColor.g,fragColor.b);
        fRGB *= specular + ambient + diffuse;
        fragColor = vec4(fRGB,fragColor.z);
    }
)";

std::string cubeFragmentShader = R"(
    #version 460 core

    in vec3 vs_texPos;
    in vec3 vs_fragPosition;
    in vec3 Normal;

    out vec4 finalColor;

    uniform vec4 u_cubeColor;
    uniform int u_texture;
    uniform float u_ambientStrength;
    uniform bool u_whitePiece;

    uniform vec3 u_cameraPosition;
    uniform float u_specularStrength;

    uniform vec3 u_lightSourcePosition;
    uniform float u_diffuseStrength;
    uniform vec3 u_lightColor;
    uniform int u_shine;

    layout(binding = 2) uniform samplerCube u_darkWoodCube;
    layout(binding = 3) uniform samplerCube u_lightWoodCube;

    void main()
    {
        //Ambient color
        vec3 ambient = u_ambientStrength * u_lightColor;

        //Textures
        if(u_texture == 1)
        {
            finalColor = mix(u_cubeColor,texture(u_whitePiece ? u_lightWoodCube : u_lightWoodCube, vs_texPos),u_whitePiece ? 0.8 : 0.4);
        } else {
            finalColor = vec4(u_cubeColor);
        }

        //Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDirection = normalize(u_lightSourcePosition - vs_fragPosition);
        float diffuseStrength = max(dot(norm, lightDirection),0) * u_diffuseStrength;
        vec3 diffuse = diffuseStrength * u_lightColor;

        //Specular
        vec3 viewDir = normalize(u_cameraPosition - vs_fragPosition);
        vec3 reflectedLight = reflect(-lightDirection, norm);
        float specFactor = pow(max(dot(viewDir, reflectedLight), 0.0f), u_shine);
        float spec = specFactor * u_specularStrength;
        vec3 specular = spec * u_lightColor;

        //Combine results
        vec3 fRGB = vec3(finalColor.r,finalColor.g,finalColor.b);
        fRGB *= specular + ambient + diffuse;
        finalColor = vec4(fRGB,finalColor.z);
    }
)";

std::string cubeVertexShader = R"(
    #version 460 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 normals;

    out vec3 vs_texPos;
    out vec3 vs_fragPosition;
    out vec3 Normal;

    uniform mat4 u_viewProjMat;
    uniform mat4 u_model;

    void main(){
        gl_Position = u_viewProjMat * u_model * vec4(position,1.0f);
        vs_texPos = position;
        Normal = normals;
        vs_fragPosition = vec3(u_model * vec4(position, 1.0f));
    }
)";

#endif //PROG2002_SHADERS_H
