#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "Ray.h"

class aabb {
    public:
        aabb() {}
        aabb(const Point3& a, const Point3& b) { minimum = a; maximum = b;}

        Point3 min() const {return minimum; }
        Point3 max() const {return maximum; }

        inline bool hit(const Ray& r, double t_min, double t_max) const;

        Point3 minimum;
        Point3 maximum;
};

inline bool aabb::hit(const Ray& r, double t_min, double t_max) const {
    for (int a = 0; a < 3; a++) {
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}

#endif // AABB_H
