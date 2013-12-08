#ifndef __KERNEL_JAMES_H
#define __KERNEL_JAMES_H

struct Ray
{
	float4 e;
	float4 d;
};

// Holds the material properties of a surface
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float spec_power;
};

struct ShadeRecord
{
	float tMin;
	bool hit;
	float4 intersection;
	float4 normal;
	struct Material mat;
};


constant struct Sphere
{
	float4 center;
	float radius;
};

//Helper function for intersecting rays with spheres
void intersectsSphere(struct Sphere s, struct Ray ray, struct ShadeRecord *record)
{
	//Sphere properties
	float t;
	float4 temp = ray.e - s.center;
	float a = dot(ray.d, ray.d);
	float b = dot(temp, ray.d) * 2.0;
	float c = dot(temp, temp) - s.radius * s.radius;
	float disc = b * b - 4 * a * c;

	// If false, the ray does not intersect, shade background colors
	if (disc < 0.0)
		return;
	else
	{
		float e = sqrt(disc);
		float denom = 2.0 * a;
		t = - (b + e) / denom;

		if (t < record->tMin && t > 0)
		{
			record->tMin = t;
			record->hit = true;
			record->normal = normalize(temp + t * ray.d);
			record->intersection = ray.e + t * ray.d;
		}

		t = (-b + e) / denom;
		
		if (t < record->tMin && t > 0)
		{
			record->tMin = t;
			record->hit = true;
			record->normal = normalize(temp + t * ray.d);
			record->intersection = ray.e + t * ray.d;
		}
	}

} 
#endif // Header include