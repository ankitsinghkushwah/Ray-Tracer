#ifndef INC_RAY_TRACER_H
#define INC_RAY_TRACER_H

#include<thread>
#include<vector>

#include"vec3.h"
#include"ray.h"
#include"object.h"
#include"sphere.h"
#include"global_settings.h"



class ray_tracer {
	
	std::thread* mThreads;
	int mMaxThreads;
	int mImageWidth;
	int mImageHeight;
	float mAspectRatio;
	float mFOV;
	unsigned char* mFrameBuffer;
	vec3 mLightPosition;
	std::vector<sphere*> mSpheres;

	int mMaxRayDepth;
	int mCurrectRecDepth;

	vec3 mCameraPosition;

	ray* preCompRays;
	

public:

	
	
	ray_tracer(int pImageWidth,int pImageHeight,int pMaxRecDepth);
	~ray_tracer();
	ray compute_ray(int row,int col);
	vec3 compute_color(const ray& r,int depth);
	void put_pixel_with_thread(int threadIndex);
	void put_pixel();
	void render();
	void updateLightPos(int key, float dt);
	void updateCameraPos(int key, float dt);
};

#endif