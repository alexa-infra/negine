#pragma once 

#include "app/glutapp.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"

class GlutSampleWindow : public GlutWindow {
private:
    base::opengl::Texture* texture_;
    base::opengl::Program* program_;
public:
	GlutSampleWindow(i32 width = 640, i32 height = 480); 
	virtual ~GlutSampleWindow();
protected:
	virtual void OnDisplay(void);
	virtual void OnReshape(i32 width, i32 height);
	virtual void OnMotion(i32 x, i32 y);
};

