#ifndef PT_H
#define PT_H
#include "Biblioteca/template_basics.hpp"

template<class T = int64_t>
struct PT {
    T x, y;
    PT(T _x = 0, T _y = 0) : x(_x), y(_y){}
    PT operator +(const PT &p) const { return PT(x+p.x,y+p.y); }
    PT operator -(const PT &p) const { return PT(x-p.x,y-p.y); }
    PT operator *(T c)         const { return PT(x*c,y*c);     }
    PT operator /(double c)    const { return PT(x/c,y/c);     }
    T operator *(const PT &p)  const { return x*p.x+y*p.y;     }
    T operator %(const PT &p)  const { return x*p.y-y*p.x;     }
    double operator !()        const { return sqrt(x*x+y*y);   }
    double operator ^(const PT &p) const { return atan2(*this%p, *this*p);}
    bool operator < (const PT &p) const { return x != p.x ? x < p.x : y < p.y; }
    bool operator == (const PT &p)const { return x == p.x && y == p.y; }

    friend std::ostream& operator << (std::ostream &os, const PT &p) {
        return os << p.x << ' ' << p.y;
    }
    friend std::istream& operator >> (std::istream &is, PT &p) {
        return is >> p.x >> p.y;
    }
};

#endif
