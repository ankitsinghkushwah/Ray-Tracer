#include "ray_tracer.h"
#include"sphere.h"
#include"ray.h"
#include"vec4.h"
#include"random.h"
#include"plane.h"
#include"sphere.h"
#include"math.h"
#include"global_settings.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#include"hit_record.h"
#include"enum_classes.h"
#include"logger.h"


//#define ANTI_ALIAS
#define CORRECT_GAMMA


ray_tracer::ray_tracer(
	int pImageWidth,
	int pImageHeight)
	:
	mImageWidth(pImageWidth),
	mImageHeight(pImageHeight)

{

	mCamera.mPosition = vec4(0.0f,0.0f,3.0f);
	mLight.mPosition = vec4(0.0f, 10.0f, 10.0f);
	mRandInst = random::getInstance();
	mFrameBuffer = new unsigned char[mImageWidth*mImageHeight * 3];
	mMaxThreads = std::thread::hardware_concurrency();
	mThreads = new std::thread[mMaxThreads];
	float planeY = -1.0;
	auto getY = [&](float pY,float radius)->float { return  pY + radius; };
	//plane
	mObjects.push_back(new plane(vec4(0.0f, 1.0f, 0.0f),
		vec4(0.0f,planeY,0.0f),
		vec4(.1,.1,.1),
		Material::REFLECTIVE,
		ShapeID::PLANE,.1f));
	//all the spheres
	mObjects.push_back(new sphere(vec4(0.0f, getY(planeY,.5), -1.0f), .5f, vec4(1.0f, 0.0f, 0.0f), Material::REFLECTIVE, ShapeID::SPHERE,.4f));
	mObjects.push_back(new sphere(vec4(-2.5f, getY(planeY, 1.0), -1.5f), 1.0f, vec4(1.0, 1.0, 1.0), Material::REFLECTIVE, ShapeID::SPHERE,.8f));
	mObjects.push_back(new sphere(vec4(2.5f, getY(planeY, 1.0), -1.5f), 1.0f, vec4(0.0f, 0.0f, 1.0f), Material::REFLECTIVE, ShapeID::SPHERE,.4f));
	mObjects.push_back(new sphere(vec4(-.7f, -.3, -.7f), .3f, vec4(0.0, 1.0, 1.0), Material::DIFFUSE, ShapeID::SPHERE,0.0f));
	mObjects.push_back(new sphere(vec4(.7f, -.3, -.7f), .3f, vec4(1.0f, 0.0f, 1.0), Material::DIFFUSE, ShapeID::SPHERE,0.0f));
	mObjects.push_back(new sphere(vec4(-2.5f, getY(planeY, .4), 0), .4f, vec4(1.0f, 1.0f, 1.0f), Material::DIFFUSE, ShapeID::SPHERE, .4f));
	mObjects.push_back(new sphere(vec4(2.5f, getY(planeY, .4), 0), .4f, vec4(.3, .6, .2), Material::DIFFUSE, ShapeID::SPHERE, .8f));

	mAspectRatio = float(mImageWidth) / float(mImageHeight);
	mCamera.mFOV = 90.0f;
	mTanHalfFOV = tan(to_radians(mCamera.mFOV/float(2.0)));
}

ray_tracer::~ray_tracer()
{
	if (mFrameBuffer != nullptr)
		delete[] mFrameBuffer;
	if(mThreads != nullptr)
		delete[] mThreads;
}


/* Computes ray more precisely for better anti aliasing */
ray ray_tracer::compute_ray_for_AA(float row, float col) {
	float x = (2.0f*((col + .5f) /static_cast<float>(mImageWidth)) - 1.0f);
	float y = 1.0f - 2.0f*((row + .5f) / static_cast<float>(mImageHeight));
	x *= mAspectRatio * mTanHalfFOV;
	y *= mTanHalfFOV;
	float z = -1.0f;
	return(ray(mCamera.mPosition, vec4(x, y, z).normalize()));
}


ray ray_tracer::compute_ray(int row, int col) {
	float x = (2.0f*((col+.5f) / static_cast<float>(mImageWidth)) - 1.0f);
	float y = 1.0f - 2.0f*((row + .5f) / static_cast<float>(mImageHeight));
	x *= mAspectRatio * mTanHalfFOV;
	y *= mTanHalfFOV;
	float z = -1.0f;
	return(ray(mCamera.mPosition,vec4(x,y,z).normalize()));
}


/* computes the final pixel color*/
vec4 ray_tracer::compute_color(const ray& r,int depth) {
	float closestSoFar = global_settings::MAXFLOAT;
	object* closestObject = nullptr;
	hit_record hitRec;
	

	/* checking if ray r is intersecting with any of the object in the scene*/
	for (auto object : mObjects) {
		if (object->hit(r,0.01,closestSoFar,hitRec)) {
			if (hitRec.t < closestSoFar){
				closestSoFar = hitRec.t;
				closestObject = object;
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

	vec4 vHitPoint = hitRec.hitPoint;
	vec4 lightDir = (mLight.mPosition - vHitPoint).normalize();
	vec4 viewDir = (vHitPoint - mCamera.mPosition).normalize();

	if (closestObject != nullptr)
	{
		bool isInShadow = false;

		
			ray shadowRay(vHitPoint, lightDir);
			for (auto object : mObjects)
			{
				if (object->hit_or_miss(shadowRay, 0.01, global_settings::MAXFLOAT))
				{
					isInShadow = true;
					break;
				}
			}
		
			vec4 finalColor = (closestObject->get_color()*max(0.0f, dot(hitRec.normal, lightDir)));
			
			if (isInShadow)
				return vec4(0.0f);
			else {
				Material objectMat = closestObject->get_material_type();
				float kr = closestObject->get_ref_fact();
				if (Material::DIFFUSE == objectMat || depth > MAX_RAY_DEPTH)
					return finalColor;
				else if(Material::REFLECTIVE == objectMat) {
					vec4 reflected = reflect(hitRec.normal,viewDir).normalize();
					ray reflectedRay(vHitPoint, reflected);
					vec4 refColor = compute_color(reflectedRay, depth + 1);
					//blending the colors
					return (1 - kr)*finalColor + (kr*refColor);
				}
			}
				
			
			}
	
	/* 
	if the ray did't intersect with any 
	object then return the interpolated 
	background color
	*/
	return vec4(0.2f,.5,.6);
	vec4 d = r.mDirection;
	d.make_it_unit();
	float y = 0.5f*(d.y + 1.0f);
	return (1.0f - y)*vec4(1.0f, 1.0f, 1.0f) + y*vec4(.2f, 0.4f, 1.0f);
}


void ray_tracer::put_pixel(int threadIndex)
{

	/* some fancy stuff. This doesnt matter*/
	static float angle = 0.0f;
	static float waveAngle = 0.0f;
	angle += .6f;
	waveAngle += 5.0f;
	mObjects[1]->mPosition.y = 0.4*sin(to_radians(angle));
	mObjects[4]->mPosition.x = mObjects[1]->mPosition.x + 1.0f*sin(to_radians(angle));
	mObjects[4]->mPosition.y = mObjects[1]->mPosition.y + .2*sin(to_radians(waveAngle));
	mObjects[4]->mPosition.z = mObjects[1]->mPosition.z + 1.0f*cos(to_radians(angle));
	mObjects[5]->mPosition.x = mObjects[1]->mPosition.x + 1.0f*sin(to_radians(1-angle));
	mObjects[5]->mPosition.y = mObjects[1]->mPosition.y + .2*sin(to_radians(waveAngle));
	mObjects[5]->mPosition.z = mObjects[1]->mPosition.z + 1.0f*cos(to_radians(1-angle));


	/* loop which goes through every single pixel and determine its color*/
	for (int row = 0; row <mImageHeight; ++row)
		{
			for (int col = (threadIndex / float(mMaxThreads))*mImageWidth;
				col < ((threadIndex + 1) / float(mMaxThreads))*mImageWidth;
				++col)
			{

				vec4 finalColor(0.0f);
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
					finalColor = vec4(sqrt(finalColor.r), sqrt(finalColor.g), sqrt(finalColor.b));
#endif
				
				int index = ((row * mImageWidth) + col) * 3;
				mFrameBuffer[index] = 255.99*finalColor.r;
				mFrameBuffer[index + 1] = 255.99*finalColor.g;
				mFrameBuffer[index + 2] = 255.99*finalColor.b;
			}
	}

}


//renders pixel color to the framebuffer
void ray_tracer::render()
{
	for (int threadIndex = 0; threadIndex < mMaxThreads; ++threadIndex) {
		mThreads[threadIndex] = std::thread(&ray_tracer::put_pixel, this, threadIndex);
	}
	for (int threadIndex = 0; threadIndex < mMaxThreads; ++threadIndex) {
		if (mThreads[threadIndex].joinable())
			mThreads[threadIndex].join();
	}
	stbi_write_bmp("result.bmp",mImageWidth,mImageHeight,3,mFrameBuffer);
}


//updates the camera position
void ray_tracer::updateCameraPos(Direction direction, float dt)
{
	float speed = 4.0f;

	switch (direction) {
	case Direction::LEFT:
		mCamera.mPosition = mCamera.mPosition + (vec4::LEFT * speed * dt);
		break;
	case Direction::RIGHT:
		mCamera.mPosition = mCamera.mPosition + (vec4::RIGHT * speed * dt);
		break;
	case Direction::FORWARD:
		mCamera.mPosition = mCamera.mPosition + (vec4::FORWARD * speed * dt);
		break;
	case Direction::BACKWARD:
		mCamera.mPosition = mCamera.mPosition + (vec4::BACKWARD * speed * dt);
		break;
	}

}



