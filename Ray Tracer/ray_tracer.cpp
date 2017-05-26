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
#include"hit_record.h"
#include"enum_classes.h"
#include"logger.h"


/*
macros for enable and 
disable functionality,better than
if-else statements
*/

//#define ANTI_ALIAS
#define CORRECT_GAMMA


ray_tracer::ray_tracer(
	int pImageWidth,
	int pImageHeight)
	:
	mImageWidth(pImageWidth),
	mImageHeight(pImageHeight)

{

	mCamera.mPosition = vec3(0.0f);
	mLight.mPosition = vec3(0.0f, 10.0f, 10.0f);

	mRandInst = random::getInstance();
	mFrameBuffer = new unsigned char[mImageWidth*mImageHeight * 3];
	mMaxThreads = std::thread::hardware_concurrency();
	mThreads = new std::thread[mMaxThreads];

	float planeY = -.8;
	auto getY = [&](float pY,float radius)->float { return  pY + radius; };

	mObjects.push_back(new plane(vec3(0.0f, 1.0f, 0.0f),vec3(0.0f,planeY,0.0f),vec3(.8,0.8,0.8),Material::DIFFUSE,ShapeID::PLANE));
	mObjects.push_back(new sphere(vec3(0.0f, getY(planeY,.5), -1.0f), .5f, vec3(1.0f, 0.0f, 0.0f), Material::REFLECTIVE, ShapeID::SPHERE));
	mObjects.push_back(new sphere(vec3(-1.5f, getY(planeY, .5), -1.5f), .5f, vec3(0.0, 1.0, 0.0), Material::REFLECTIVE, ShapeID::SPHERE));
	mObjects.push_back(new sphere(vec3(1.5f, getY(planeY, .5), -1.5f), 0.5f, vec3(0.0f, 0.0f, 1.0f), Material::REFLECTIVE, ShapeID::SPHERE));
	mObjects.push_back(new sphere(vec3(-.7f, getY(planeY, .1), -.7f), .1f, vec3(0.0, 1.0, 1.0), Material::DIFFUSE, ShapeID::SPHERE));
	mObjects.push_back(new sphere(vec3(.7f, getY(planeY, .1), -.7f), 0.1f, vec3(1.0f, 0.0f, 1.0), Material::DIFFUSE, ShapeID::SPHERE));

	

	mAspectRatio = float(mImageWidth) / float(mImageHeight);
	mFOV = 90.0f;
	mTanHalfFOV = tan(to_radians(mFOV/float(2.0)));

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
	return(ray(mCamera.mPosition, vec3(x, y, z).normalize()));
}


ray ray_tracer::compute_ray(int row, int col) {
	float x = (2.0f*((col+.5f) / static_cast<float>(mImageWidth)) - 1.0f);
	float y = 1.0f - 2.0f*((row + .5f) / static_cast<float>(mImageHeight));
	x *= mAspectRatio * mTanHalfFOV;
	y *= mTanHalfFOV;
	float z = -1.0f;
	return(ray(mCamera.mPosition,vec3(x,y,z).normalize()));
}



vec3 ray_tracer::compute_color(const ray& r,int depth) {
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

	vec3 vHitPoint = hitRec.hitPoint;
	vec3 lightDir = (mLight.mPosition - vHitPoint).normalize();
	vec3 viewDir = (vHitPoint - mCamera.mPosition).normalize();

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
		

			vec3 finalColor = (closestObject->get_color()*max(0.0f, dot(hitRec.normal, lightDir)));
			
			if (isInShadow)
				return finalColor*.02;
			else {
				

				Material objectMat = closestObject->get_material_type();

				if (Material::DIFFUSE == objectMat || depth > MAX_RAY_DEPTH)
					return finalColor;
				else if(Material::REFLECTIVE == objectMat) {
					float kr = .8f; //how much to blend?
					vec3 reflected = reflect(hitRec.normal,viewDir).normalize();
					ray reflectedRay(vHitPoint, reflected);
					vec3 refColor = compute_color(reflectedRay, depth + 1);
					//blending the colors
					return (1 - kr)*refColor + (kr*finalColor);
				}
			}
				
			
			}
	
	
	vec3 d = r.mDirection;
	d.make_it_unit();
	float y = 0.5f*(d.y + 1.0f);
	return (1.0f - y)*vec3(1.0f, 1.0f, 1.0f) + y*vec3(.5f, 0.7f, 1.0f);
	
}


void ray_tracer::put_pixel(int threadIndex)
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

void ray_tracer::updateCameraPos(Direction direction, float dt)
{
	float speed = 4.0f;

	switch (direction) {

	/*case Direction::UP:
		mCamera.mPosition = mCamera.mPosition + (vec3::UP * speed * dt);
		break;
	case Direction::DOWN:
		mCamera.mPosition = mCamera.mPosition + (vec3::DOWN * speed * dt);*/
		break;
	case Direction::LEFT:
		mCamera.mPosition = mCamera.mPosition + (vec3::LEFT * speed * dt);
		break;
	case Direction::RIGHT:
		mCamera.mPosition = mCamera.mPosition + (vec3::RIGHT * speed * dt);
		break;
	case Direction::FORWARD:
		mCamera.mPosition = mCamera.mPosition + (vec3::FORWARD * speed * dt);
		break;
	case Direction::BACKWARD:
		mCamera.mPosition = mCamera.mPosition + (vec3::BACKWARD * speed * dt);
		break;
	}

}


void ray_tracer::updateLightPos(Direction direction, float dt)
{
	float speed = 7.0f;

	switch (direction) {

	case Direction::UP:
		mLight.mPosition = mLight.mPosition + (vec3::UP * speed * dt);
		break;
	case Direction::DOWN:
		mLight.mPosition = mLight.mPosition + (vec3::DOWN * speed * dt);
		break;
	case Direction::LEFT:
		mLight.mPosition = mLight.mPosition + (vec3::LEFT * speed * dt);
		break;
	case Direction::RIGHT:
		mLight.mPosition = mLight.mPosition + (vec3::RIGHT * speed * dt);
		break;
	case Direction::FORWARD:
		mLight.mPosition = mLight.mPosition + (vec3::FORWARD * speed * dt);
		break;
	case Direction::BACKWARD:
		mLight.mPosition = mLight.mPosition + (vec3::BACKWARD * speed * dt);
		break;
	}

}

