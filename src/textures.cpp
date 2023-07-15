#include "textures.hh"

#include <GL/glew.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
    : t_render_id(0), t_filepath(path), t_localbuffer(nullptr), t_width(0), t_height(0), t_bpp(0) {

  glGenTextures(1, &t_render_id);
  glBindTexture(GL_TEXTURE_2D, t_render_id);

  stbi_set_flip_vertically_on_load(true);
  t_localbuffer = stbi_load(path.c_str(), &t_width, &t_height, &t_bpp, STBI_rgb_alpha);

  if (!t_localbuffer) {
    std::cout << "[ERROR] Fail to load texture " << t_filepath << std::endl;
  } else {
    std::cout << "[LOG] set texture " << t_render_id << ", path: " << path << std::endl;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_localbuffer);
  unbind();
};

Texture::~Texture() {
  glDeleteTextures(1, &t_render_id);

  if (t_localbuffer)
    stbi_image_free(t_localbuffer);
}

void Texture::bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, t_render_id);
};

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };
