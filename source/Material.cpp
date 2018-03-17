#include "Material.hpp"

#include "TextureManager.hpp"

Material * Material::activeMaterial;

Material::Material()
{

}

void Material::AddTexture(const TextureType type, const char * path)
{
	Texture t = { 0,type, path };
	t.id = TextureManager::GetTexture(t.path);
	textures.push_back(t);
}