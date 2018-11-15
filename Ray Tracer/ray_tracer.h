#ifndef INC_RAY_TRACER_H
#define INC_RAY_TRACER_H

//C++ headers
#include<thread>
#include<vector>
#include<memory>

//custom headers
#include"vec4.h"
#include"ray.h"
#include"object.h"
#include"sphere.h"
#include"global_settings.h"
#include"random.h"
#include"camera.h"
#include"light.h"
#include"plane.h"


struct alignas(16) ray_tracer {
public:
	ray_tracer(int pImageWidth,
		int pImageHeight);
	~ray_tracer();
	ray compute_ray(int row, int col);
	ray compute_ray_for_AA(float row,float col);
	vec4 compute_color(const ray& r,int depth);
	void put_pixel(int threadIndex);
	void render();
	void updateCameraPos(Direction direction, float dt);
private:
	std::shared_ptr<random> mRandInst;
	std::thread* mThreads;
	int mMaxThreads;
	int mImageWidth;
	int mImageHeight;
	float mAspectRatio;
	float mTanHalfFOV;
	unsigned char* mFrameBuffer;
	std::vector<object*> mObjects;
	sphere mSpheres[7];
	plane mPlane;
	camera mCamera;
	light mLight;
	//compile-time constants
	static const int MAX_RAY_DEPTH = 1;
	static const int MAX_AA_SAMPLES = 50;
};

#endif