#include "ray_tracer.h"
#include"sphere.h"
#include"ray.h"
#include"vec3.h"
#include"random.h"
#include"plane.h"
#include"sphere.h"
#include"math.h"
#include"global_settings.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"


#define USE_THREADS
//#define DEBUG_NORMALS



ray_tracer::ray_tracer(int pImageWidth, int pImageHeight,int pMaxRecDepth)
	:
	mImageWidth(pImageWidth),
	mImageHeight(pImageHeight),
	mMaxRayDepth(pMaxRecDepth),
	mCameraPosition(0.0f),
	mLightPosition(0.0f,10.0f,10.0f),
	mCurrectRecDepth(0)
{
	auto rand = random::getInstance();
	mFrameBuffer = new unsigned char[mImageWidth*mImageHeight*3];
	preCompRays = new ray[mImageWidth*mImageHeight];
	mMaxThreads = std::thread::hardware_concurrency();
	mThreads = new std::thread[mMaxThreads];

	
	mSpheres.push_back(new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, vec3(.5,.5,.5),global_settings::MATERIAL::REFLECTIVE));
	mSpheres.push_back(new sphere(
		vec3(0.0f, 0.0f, -1.0f), 
		.5f, vec3(1.0f, 0.0f, 0.0f),
		global_settings::MATERIAL::REFLECTIVE));

	mSpheres.push_back(new sphere(vec3(-2.0f, 0.0f, -2.0f), .5f, vec3(0.0, 1.0, 0.0), global_settings::MATERIAL::REFLECTIVE));
	mSpheres.push_back(new sphere(vec3(2.0f, 0.0f, -2.0f), 0.5f, vec3(0.0f, 0.0f, 1.0f), global_settings::MATERIAL::REFLECTIVE));
	
	mAspectRatio = float(mImageWidth) / float(mImageHeight);
	mFOV = 90.0f;

	for (int row = 0; row < mImageHeight; row++) {
		for (int col = 0; col < mImageWidth; col++) {
			preCompRays[row*mImageWidth + col] = compute_ray(row,col);
		}
	}
	
}

ray_tracer::~ray_tracer()
{
	if (mFrameBuffer != nullptr)
		delete[] mFrameBuffer;
	
	delete[] preCompRays;
	delete[] mThreads;
}




ray ray_tracer::compute_ray(int row, int col) {

	float x = (2.0f*((col+.5f) / mImageWidth) - 1.0f);
	float y = 1.0f - 2.0f*((row + .5f) / mImageHeight);
	x *= mAspectRatio * tan(to_radians(mFOV/2.0f));
	y *= tan(to_radians(mFOV/2.0f));
	float z = -1.0f;
	return(ray(mCameraPosition,vec3(x,y,z).normalize()));
}



vec3 ray_tracer::compute_color(const ray& r,int depth) {

	double t = 0.0f;
	vec3 normal; 
	double closestSoFar = global_settings::MAXFLOAT;
	sphere* closestSphere = nullptr;

	

	/* checking if ray r is intersecting with any of the sphere in the scene*/
	for (auto sphere : mSpheres) {
		if (sphere->hit(r, t,normal, 0.01, closestSoFar)) {
			if (t < closestSoFar){
				closestSoFar = t;
				closestSphere = sphere;
			}
		}
	}


	/*
	if ray intersecting any sphere then 
	shoot shadow ray to the light direction
	and check interesection of shadow ray with each 
	sphere. if shadowed then color the current pixel black,
	otherwise color it with sphere color.
	*/

	vec3 vHitPoint = r.point_at_parameter(closestSoFar);
	vec3 lightDir = (mLightPosition - vHitPoint).normalize();
	vec3 viewDir = (vHitPoint - mCameraPosition).normalize();

	if (closestSphere != nullptr) 
	{
			ray shadowRay(vHitPoint, lightDir);

			bool isInShadow = false;
			for (auto sphere : mSpheres) {
				if (sphere->hitOrNot(shadowRay, 0.01, global_settings::MAXFLOAT)) 
				{
					isInShadow = true;
					break;
				}
			}
			
			

			if (isInShadow)
				return vec3(0.0f);
			else {
				vec3 finalColor = (closestSphere->get_color()*max(0.0f, dot(normal, lightDir)));
			
				
				global_settings::MATERIAL objectMat = closestSphere->mat;
				if (objectMat == global_settings::MATERIAL::DIFFUSE || depth > mMaxRayDepth)
					return finalColor;
				if(objectMat == global_settings::MATERIAL::REFLECTIVE) {
					float kr = .5f;
					vec3 reflected = reflect(normal,viewDir).normalize();
					ray reflectedRay(vHitPoint, reflected);
					vec3 refColor = compute_color(reflectedRay, depth + 1);
					//blending the colors
					return (1 - kr)*finalColor + (kr*refColor);
				}
			}
				
			
			}
	
	
	vec3 d = r.mDirection;
	d.make_it_unit();
	float y = 0.5f*(d.y + 1.0f);
	return (1.0f - y)*vec3(1.0f, 1.0f, 1.0f) + y*vec3(.5f, 0.7f, 1.0f);
	
}


void ray_tracer::put_pixel_with_thread(int threadIndex)
{

	for (int row = 0; row <mImageHeight; ++row)
		{
			for (int col = (threadIndex / float(mMaxThreads))*mImageWidth;
				col < ((threadIndex + 1) / float(mMaxThreads))*mImageWidth;
				++col)
			{
				ray r = compute_ray(row,col);
				vec3 color = compute_color(r,0);
				
				int index = ((row * mImageWidth) + col) * 3;
				mFrameBuffer[index] = 255.99*color.x;
				mFrameBuffer[index + 1] = 255.99*color.y;
				mFrameBuffer[index + 2] = 255.99*color.z;
			}
	}

}




void ray_tracer::put_pixel() {
	
	for (int row = 0; row <mImageHeight; ++row)
	{
		for (int col = 0; col <mImageWidth; ++col)
		{

			ray r = compute_ray(row, col);
			vec3 color = compute_color(r,0);

			int index = ((row * mImageWidth) + col) * 3;
			mFrameBuffer[index] = int(255.99*color.x);
			mFrameBuffer[index + 1] = int(255.99*color.y);
			mFrameBuffer[index + 2] = int(255.99*color.z);
		}
	}
}

void ray_tracer::render()
{

#ifdef USE_THREADS

	for (int threadIndex = 0; threadIndex < mMaxThreads; ++threadIndex) {
		mThreads[threadIndex] = std::thread(&ray_tracer::put_pixel_with_thread, this, threadIndex);
	}

	for (int threadIndex = 0; threadIndex < mMaxThreads; ++threadIndex) {
		if (mThreads[threadIndex].joinable())
			mThreads[threadIndex].join();
	}

#else
	put_pixel(); //uses single thread

#endif

	stbi_write_bmp("result.bmp",mImageWidth,mImageHeight,3,mFrameBuffer);
}

void ray_tracer::updateCameraPos(int key, float dt)
{
	float speed = 1.0f;

	switch (key) {
	case global_settings::KEYS::UP:
		mCameraPosition = mCameraPosition + (vec3::UP * speed * dt);
		break;
	case global_settings::KEYS::DOWN:
		mCameraPosition = mCameraPosition + (vec3::DOWN * speed * dt);
		break;
	case global_settings::KEYS::LEFT:
		mCameraPosition = mCameraPosition + (vec3::LEFT * speed * dt);
		break;
	case global_settings::KEYS::RIGHT:
		mCameraPosition = mCameraPosition + (vec3::RIGHT * speed * dt);
		break;
	case global_settings::KEYS::FORWARD:
		mCameraPosition = mCameraPosition + (vec3::FORWARD * speed * dt);
		break;
	case global_settings::KEYS::BACKWARD:
		mCameraPosition = mCameraPosition + (vec3::BACKWARD * speed * dt);
		break;
	}

}


void ray_tracer::updateLightPos(int key, float dt)
{
	float speed = 1.0f;

	switch (key) {
	case global_settings::KEYS::UP:
		mLightPosition = mLightPosition + (vec3::UP * speed * dt);
		break;
	case global_settings::KEYS::DOWN:
		mLightPosition = mLightPosition + (vec3::DOWN * speed * dt);
		break;
	case global_settings::KEYS::LEFT:
		mLightPosition = mLightPosition + (vec3::LEFT * speed * dt);
		break;
	case global_settings::KEYS::RIGHT:
		mLightPosition = mLightPosition + (vec3::RIGHT * speed * dt);
		break;
	case global_settings::KEYS::FORWARD:
		mLightPosition = mLightPosition + (vec3::FORWARD * speed * dt);
		break;
	case global_settings::KEYS::BACKWARD:
		mLightPosition = mLightPosition + (vec3::BACKWARD * speed * dt);
		break;
	}

}

