#include "../include/harmonics.h"

int harmonics_conditions(int n, int m, int l)
{
	return zernike_conditions(n, m) && l >= -m && l <= m;
}

int harmonics_nummoments(int ord, int rep, int spin)
{
	int count = 0;

	for (int n = 0; n <= ord; n++)
		for (int m = 0; m <= rep; m++)
			for (int l = 0; l <= spin; l++)
				if (harmonics_conditions(n, m, l))
					count++;

	return count;
}

real harmonics_norm(int m, int l)
{
	real num = ((2.0 * m) + 1.0) * calc_factorial(m - l);
	real den = (4.0 * CALC_PI * calc_factorial(m + l));
	real norm = sqrt(num / den);
	
	return norm;
}

real harmonics_legendrepoly(int m, int l, real x)
{
	real p1 = pow(-1.0, l) * pow(2.0, m) * pow(1.0 - x*x, l/2.0);
	real p2 = 0.0;
	
	real i1 = 0.0;
	real i2 = 0.0;
	real i3 = 0.0;
	for (int k = l; k <= m; k++) {
		i1 = (calc_factorial(k) / calc_factorial(k - l)) * pow(x, k-l);
		i2 = calc_binom_coeff(m, k);
		i3 = calc_binom_coeff(ceil((m + k - 1.0)/2.0), m);
		
		p2 += i1 * i2 * i3;
	}
	
	return p1 * p2;
}

real harmonics_harmonic_odd(int m, int l, real theta, real phi)
{
	real sh = harmonics_norm(m, l) *
	          harmonics_legendrepoly(m, l, cos(theta)) *
	          sin(l * phi);
	
	return sh;
}

real harmonics_harmonic_even(int m, int l, real theta, real phi)
{
	real sh = harmonics_norm(m, l) *
	          harmonics_legendrepoly(m, l, cos(theta)) *
	          cos(l * phi);
	
	return sh;
}

real harmonics_harmonic_mag(int m, int l, real theta)
{
	real sh = harmonics_norm(m, l) *
	          harmonics_legendrepoly(m, l, cos(theta));
	
	return sh;
}

real harmonics_harmonic_full(int m, int l, real theta, real phi)
{
	real sh = harmonics_norm(m, l) *
	          harmonics_legendrepoly(m, l, cos(theta)) *
	          (cos(l * phi) + sin(l * phi));
	
	return sh;
}

real harmonics_moment_odd(int n, int m, int l, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	
	real cx = 0.0;
	real cy = 0.0;
	real cz = 0.0;
	real d = 0.0;
	real dist = 0.0;
	real theta = 0.0;
	real phi = 0.0;
	real radpoly = 0.0;
	real harmonic = 0.0;
	real moment = 0.0;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		cz = set->point->z - centroid->z;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		phi = zernike_zenith(cz, r);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = harmonics_harmonic_odd(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(&centroid);
	
	return (3.0 * moment) / (4.0 * CALC_PI);
}

real harmonics_moment_even(int n, int m, int l, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	
	real cx = 0.0;
	real cy = 0.0;
	real cz = 0.0;
	real d = 0.0;
	real dist = 0.0;
	real theta = 0.0;
	real phi = 0.0;
	real radpoly = 0.0;
	real harmonic = 0.0;
	real moment = 0.0;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		cz = set->point->z - centroid->z;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		phi = zernike_zenith(cz, r);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = harmonics_harmonic_even(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(&centroid);
	
	return (3.0 * moment) / (4.0 * CALC_PI);
}

real harmonics_moment_mag(int n, int m, int l, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	
	real cx = 0.0;
	real cy = 0.0;
	real d = 0.0;
	real dist = 0.0;
	real theta = 0.0;
	real radpoly = 0.0;
	real harmonic = 0.0;
	real moment = 0.0;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = harmonics_harmonic_mag(m, l, theta);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(&centroid);
	
	return (3.0 * moment) / (4.0 * CALC_PI);
}

real harmonics_moment_full(int n, int m, int l, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	
	real cx = 0.0;
	real cy = 0.0;
	real cz = 0.0;
	real d = 0.0;
	real dist = 0.0;
	real theta = 0.0;
	real phi = 0.0;
	real radpoly = 0.0;
	real harmonic = 0.0;
	real moment = 0.0;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		cz = set->point->z - centroid->z;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		phi = zernike_zenith(cz, r);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = harmonics_harmonic_full(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(&centroid);
	
	return (3.0 * moment) / (4.0 * CALC_PI);
}

struct dataframe *harmonics_cloud_moments_odd(struct cloud *cloud)
{
	int s = harmonics_nummoments(HARMON_ORD, HARMON_REP, HARMON_SPIN);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	real moment = 0.0;
	
	int col = 0;
	for (int n = 0; n <= HARMON_ORD; n++) {
		for (int m = 0; m <= HARMON_REP; m++) {
			for (int l = 0; l <= HARMON_SPIN; l++) {
				if (harmonics_conditions(n, m, l)) {
					moment = harmonics_moment_odd(n, m, l, r, cloud);
					dataframe_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct dataframe *harmonics_cloud_moments_even(struct cloud *cloud)
{
	int s = harmonics_nummoments(HARMON_ORD, HARMON_REP, HARMON_SPIN);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	real moment = 0.0;
	
	int col = 0;
	for (int n = 0; n <= HARMON_ORD; n++) {
		for (int m = 0; m <= HARMON_REP; m++) {
			for (int l = 0; l <= HARMON_SPIN; l++) {
				if (harmonics_conditions(n, m, l)) {
					moment = harmonics_moment_even(n, m, l, r, cloud);
					dataframe_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct dataframe *harmonics_cloud_moments_mag(struct cloud *cloud)
{
	int s = harmonics_nummoments(HARMON_ORD, HARMON_REP, HARMON_SPIN);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	real moment = 0.0;
	
	int col = 0;
	for (int n = 0; n <= HARMON_ORD; n++) {
		for (int m = 0; m <= HARMON_REP; m++) {
			for (int l = 0; l <= HARMON_SPIN; l++) {
				if (harmonics_conditions(n, m, l)) {
					moment = harmonics_moment_mag(n, m, l, r, cloud);
					dataframe_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct dataframe *harmonics_cloud_moments_full(struct cloud *cloud)
{
	int s = harmonics_nummoments(HARMON_ORD, HARMON_REP, HARMON_SPIN);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	real moment = 0.0;
	
	int col = 0;
	for (int n = 0; n <= HARMON_ORD; n++) {
		for (int m = 0; m <= HARMON_REP; m++) {
			for (int l = 0; l <= HARMON_SPIN; l++) {
				if (harmonics_conditions(n, m, l)) {
					moment = harmonics_moment_full(n, m, l, r, cloud);
					dataframe_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

