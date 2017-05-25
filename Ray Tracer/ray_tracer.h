#ifndef INC_RAY_TRACER_H
#define INC_RAY_TRACER_H

#include<thread>
#include<vector>

#include"vec3.h"
#include"ray.h"
#include"object.h"
#include"sphere.h"
#include"global_settings.h"
#include"random.h"
#include<memory>



class ray_tracer {
	
	//compile-time constants
	static const int MAX_RAY_DEPTH = 1;
	static const int MAX_AA_SAMPLES = 50;

	std::shared_ptr<random> mRandInst;
	std::thread* mThreads;
	int mMaxThreads;
	int mImageWidth;
	int mImageHeight;
	float mAspectRatio;
	float mFOV;
	float mTanHalfFOV;
	unsigned char* mFrameBuffer;
	vec3 mLightPosition;
	std::vector<sphere*> mSpheres;

	//states
	vec3 mCameraPosition;

public:

	
	
	ray_tracer(int pImageWidth,
		int pImageHeight);

	~ray_tracer();
	ray compute_ray(int row, int col);
	ray compute_ray_for_aa(float row,float col);
	vec3 compute_color(const ray& r,int depth);
	void put_pixel_with_thread(int threadIndex);
	void put_pixel();
	void render();
	void updateLightPos(int key, float dt);
	void updateCameraPos(int key, float dt);
};

#endif