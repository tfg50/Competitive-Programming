struct PT {
	typedef double T;
	T x, y;
	PT(T x = 0, T y = 0) : x(x), y(y){}
	PT operator +(const PT &p) const { return PT(x+p.x,y+p.y); }
	PT operator -(const PT &p) const { return PT(x-p.x,y-p.y); }
	PT operator *(T c)         const { return PT(x*c,y*c);     }
	PT operator /(double c)    const { return PT(x/c,y/c);     }
	T operator *(const PT &p)  const { return x*p.x+y*p.y;     }
	T operator %(const PT &p)  const { return x*p.y-y*p.x;     }
	double operator !()        const { return sqrt(x*x+y*y);   }
	double operator ^(const PT &p) const { return atan2(*this%p, *this*p);}
};
