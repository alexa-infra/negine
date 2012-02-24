/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#error Depricated header SpriteGroup.h

#include "renderer/sprite.h"
#include "renderer/vertexbuffer.h"

#include <vector>
#include <stack>

namespace base {
namespace opengl {

class SpriteGroup {
public:
    std::vector<Sprite*> sprites;
    std::vector<Sprite*> active_sprites;
    std::stack<u32> free_sprites_index;

    VertexBuffer* buffer;

    SpriteGroup(u32 sprite_count) {
        buffer = new VertexBuffer;
        for (u32 i=0; i<sprite_count; i++) {
            Sprite* s = new Sprite;
            sprites.push_back(s);
            free_sprites_index.push(i);
        }
        Vertex* v = new Vertex[sprite_count * 4];
        Face* f = new Face[sprite_count * 2];

        Sprite sp;
        for (u32 i=0, j=0; i<sprite_count*2; )
        {
            sp.fill_position(v, j);
            j = sp.init_vertex(v, j);
            i = sp.init_faces(f, i);
        }

        buffer->SetData(v, sprite_count * 4, f, sprite_count * 2);
        delete f;
        delete v;
    }

    ~SpriteGroup() {
        for (u32 i=0; i<sprites.size(); i++)
            delete sprites[i];
        delete buffer;
    }

    Sprite* AddSprite() {
        u32 index = free_sprites_index.top();
        free_sprites_index.pop();

        Sprite* s = sprites[index];
        active_sprites.push_back(s);

        return s;
    }

    Sprite* AddSprite(const Vector2& position, const Vector4& color, f32 size, f32 angle) {
        Sprite* s = AddSprite();

        s->rectange.position = position;
        s->rectange.size = Vector2(size);
        s->rectange.angle = angle;
        s->color = color;

        return s;
    }

    void RemoveSprite(const Sprite* s) {
        std::vector<Sprite*>::iterator it;
        for (it = active_sprites.begin(); it != active_sprites.end(); ++it) {
            if (*it == s) {
                active_sprites.erase(it);
                break;
            }
        }
        u32 index = 0;
        for (it = sprites.begin(); it != sprites.end(); ++it, ++index) {
            if (*it == s)
                break;
        }
        free_sprites_index.push(index);
    }

    void Draw(AttributeBinding& binding) {
        Vertex* v = buffer->Lock();
        
        std::vector<Sprite*>::iterator it;
        int index = 0;
        for (it = active_sprites.begin(); it != active_sprites.end(); ++it) {
            Sprite* s = *it;
            index = s->fill_position(v, index);
        }

        buffer->Unlock();

        buffer->Draw(binding, 0, active_sprites.size() * 2);
    }
};

}
}