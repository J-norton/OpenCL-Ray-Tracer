#include "Header.cl"

#define NUM_SPHERES 5

void shadePixel(struct Ray ray, float *pixel, int frame_number)
{
	//Used in shading
	float t;
	struct Material mat = { (float4 ) (0, 0, 0, 0), (float4 ) (0, 0, 0, 0),
	(float4 ) (0, 0, 0, 0), 0.0};
	struct ShadeRecord rec = {FLT_MAX, false, (float4 ) (0, 0, 0, 0),
	(float4 ) (0, 0, 0, 0), mat};

	struct Sphere surfaces[NUM_SPHERES];

	surfaces[0] = (struct Sphere) { ( float4 ) (0, 0, -10 - (frame_number / 5), 0), 2 };
	surfaces[1] = (struct Sphere) { ( float4 ) (2, 2, -12 - (frame_number / 5), 0), 1 };
	surfaces[2] = (struct Sphere) { ( float4 ) (4, 4, -13 - (frame_number / 5), 0), 1 };
	surfaces[3] = (struct Sphere) { ( float4 ) (6, 2, -15 - (frame_number / 5), 0), 1 };
	surfaces[4] = (struct Sphere) { ( float4 ) (8, 0, -22 - (frame_number / 5), 0), 1 };

	for (int i = 0; i < NUM_SPHERES; i++)
		intersectsSphere(surfaces[i], ray, &rec);

	// If false, the ray does not intersect, shade background colors
	if (!rec.hit)
	{
		pixel[0] = fabs(ray.d.y);
		pixel[1] = fabs( ray.d.y);
		pixel[2] = fabs( ray.d.y);
	/*
		pixel[0] = 0;
		pixel[1] = 0;
		pixel[2] = 0;
		*/
	}
	else
	{
		//Now the normal and intersection are here, shade the pixel
		float4 light = (float4) (-3, 2, -12, 0);
		//light = normalize(light);
		//rec.intersection = normalize(rec.intersection);
		
		float4 pts_to_light = light - rec.intersection;

		pts_to_light = normalize(pts_to_light);
		rec.normal = normalize(rec.normal);
		float shade_angle = max( (float) 0.0, (float) dot(pts_to_light, rec.normal) );
		//Ambient component
		shade_angle += 0.03;

		float gamma_correct_val = powr( shade_angle, (float) 1.0f / 2.2f);

		pixel[0] = gamma_correct_val;
		pixel[1] = gamma_correct_val;
		pixel[2] = gamma_correct_val;
	}
}

kernel void traceSphere(image2d_t framebuffer,
const uint height, const uint width, int framenumber)
{
	int y = get_global_id(0);
	int x = get_global_id(1);

	// Index of the R-channel of this kernel's pixel
	int pix_index = y * width * 3 + x * 3;
	// Compute the ray
	float window_width = 0.2 / width;
	float window_height = 0.2 / height;
	struct Ray r;
	r.e = (float4) (0, 0, 0, 0);
	r.d = (float4) (-0.1 + window_width * x, -0.1 + window_height * y, -0.2, 0);
	r.d = normalize(r.d);
	float temp[3] = {0.0f, 0.0f, 0.0f};

	shadePixel(r, temp, framenumber);
	write_imagef(framebuffer, (int2 ) (x, y), (float4 ) (temp[0], temp[1], temp[2], 1) );
}
