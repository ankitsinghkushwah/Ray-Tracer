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


/*
macros for enable and 
disable functionality,better than
if-else statements
*/

#define USE_THREADS
//#define ANTI_ALIAS
#define CORRECT_GAMMA


ray_tracer::ray_tracer(
	int pImageWidth,
	int pImageHeight)
	:
	mImageWidth(pImageWidth),
	mImageHeight(pImageHeight)

{

	mCameraPosition = vec3(0.0f);
	mLightPosition = vec3(0.0f, 10.0f, 10.0f);

	mRandInst = random::getInstance();
	mFrameBuffer = new unsigned char[mImageWidth*mImageHeight * 3];
	mMaxThreads = std::thread::hardware_concurrency();
	mThreads = new std::thread[mMaxThreads];


	mSpheres.push_back(new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, vec3(.3, .3, .3), global_settings::MATERIAL::REFLECTIVE));
	mSpheres.push_back(new sphere(vec3(0.0f, 0.0f, -1.0f), .5f, vec3(.6f, 0.0f, 0.0f), global_settings::MATERIAL::REFLECTIVE));
	mSpheres.push_back(new sphere(vec3(-1.5f, 0.0f, -1.5f), .5f, vec3(0.0, .6, 0.0), global_settings::MATERIAL::REFLECTIVE));
	mSpheres.push_back(new sphere(vec3(1.5f, 0.0f, -1.5f), 0.5f, vec3(0.0f, 0.0f, 1.0f), global_settings::MATERIAL::REFLECTIVE));
	mSpheres.push_back(new sphere(vec3(-.7f, -0.4f, -.7f), .1f, vec3(0.0, .6, .6), global_settings::MATERIAL::DIFFUSE));
	mSpheres.push_back(new sphere(vec3(.7f, -0.4f, -.7f), 0.1f, vec3(.6f, 0.0f, .6f), global_settings::MATERIAL::DIFFUSE));

	mAspectRatio = float(mImageWidth) / float(mImageHeight);
	mFOV = 90.0f;
	mTanHalfFOV = tan(to_radians(mFOV/double(2.0)));

}

ray_tracer::~ray_tracer()
{
	if (mFrameBuffer != nullptr)
		delete[] mFrameBuffer;
	delete[] mThreads;
}



ray ray_tracer::compute_ray_for_aa(float row, float col) {
	float x = (2.0f*((col + .5f) /static_cast<float>(mImageWidth)) - 1.0f);
	float y = 1.0f - 2.0f*((row + .5f) / static_cast<float>(mImageHeight));
	x *= mAspectRatio * mTanHalfFOV;
	y *= mTanHalfFOV;
	float z = -1.0f;
	return(ray(mCameraPosition, vec3(x, y, z).normalize()));
}


ray ray_tracer::compute_ray(int row, int col) {
	float x = (2.0f*((col+.5f) / static_cast<float>(mImageWidth)) - 1.0f);
	float y = 1.0f - 2.0f*((row + .5f) / static_cast<float>(mImageHeight));
	x *= mAspectRatio * mTanHalfFOV;
	y *= mTanHalfFOV;
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
			bool isInShadow = false;

				ray shadowRay(vHitPoint, lightDir);
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
				if (objectMat == global_settings::MATERIAL::DIFFUSE || depth > MAX_RAY_DEPTH)
					return finalColor;
				if(objectMat == global_settings::MATERIAL::REFLECTIVE) {
					float kr = .2f;
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



				vec3 finalColor(0.0f);
				ray r;

#ifdef ANTI_ALIAS
				for (int sample = 0; sample < MAX_AA_SAMPLES; sample++) {
					float u = col + mRandInst->real(0,1);
					float v = row + mRandInst->real(0,1);
					r = compute_ray_for_aa(v, u);
					finalColor += compute_color(r, 0);
				}
					finalColor /= float(MAX_AA_SAMPLES);
#else
				r = compute_ray(row,col);
				finalColor = compute_color(r,0);
#endif
				
#ifdef CORRECT_GAMMA
					finalColor = vec3(sqrt(finalColor.r), sqrt(finalColor.g), sqrt(finalColor.b));
#endif
				
				int index = ((row * mImageWidth) + col) * 3;
				mFrameBuffer[index] = 255.99*finalColor.r;
				mFrameBuffer[index + 1] = 255.99*finalColor.g;
				mFrameBuffer[index + 2] = 255.99*finalColor.b;
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
	float speed = 4.0f;

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
	float speed = 7.0f;

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

