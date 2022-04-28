#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include "math_base.h"
#include "matrix.h"

namespace rt3{

// wrapper class for structured point values
template <typename T, int size> class StructuredValues {
protected:
  array<T, size> values;

public:
  StructuredValues(){ values = array<T,size>();}
  StructuredValues(const StructuredValues &clone) = default;

  template <typename _T, int _size>
  friend StructuredValues<_T, _size> customMult(const Matrix4x4 &matrix, const StructuredValues<_T, _size> &element);


  StructuredValues(const vector<T> &_values){
    assert(values.size() == size);
    copy_n(_values.begin(), _values.size(), values.begin());
  }  

  T& operator[]( const int i ){
    assert(i < size);
    return values[i];
  }

  T at(int i) const{
    assert(i < size);
    return values[i];
  }

  StructuredValues operator*(real_type t) const{
      StructuredValues v(*this);
      for(auto &x : v.values) x *= t;
      return v;
  }

  inline T x() const{ return values[0]; }
  inline T y() const{ return values[1]; }
  inline T z() const{ 
    assert(size == 3);
    return values[2]; 
  }

  string toString() const{
    std::stringstream ss; ss << "( ";
    for(int i = 0; i < size; ++i){
      ss << values[i] << " ";
    }
    ss << ")";
    return ss.str();
  }

};

template<typename T, int size> struct Vector : 
    public StructuredValues<T, size>{
      
  private:
    using StructuredValues<T, size>::values;
  public:
  using StructuredValues<T, size>::StructuredValues;

  
  Vector():StructuredValues<T,size>(){}
  Vector(const Vector &clone):StructuredValues<T,size>(clone){}
  Vector(const vector<T> &_values):StructuredValues<T,size>(_values){}

  Vector(StructuredValues<T,size> base):StructuredValues<T,size>(base){}
    
  Vector operator*(real_type t) const{
    return StructuredValues<T,size>::operator*(t);
  }

  real_type operator*(const Vector &u) const{
    real_type prod = 0;
    for(int i = 0; i < size; ++i){
      prod += this->at(i) * u.at(i);
    }
    return prod;
  }

  Vector normalize() const{
    Vector v(*this);
    real_type norm = v.getNorm();
    return v * (1.0 / norm);
  }

  Vector abs() const{
    Vector v(*this);
    for(int i = 0; i < size; ++i){
      v[i] = fabs(v[i]);
    }
    return v;
  }

  Vector cross(const Vector &x) const{
    Vector v(*this);
    v[0] = this->at(1) * x.at(2) - this->at(2) * x.at(1);
    v[1] = this->at(2) * x.at(0) - this->at(0) * x.at(2);
    v[2] = this->at(0) * x.at(1) - this->at(1) * x.at(0);
    return v;
  }

  real_type getNorm() const{
    real_type norm = 0;
    for(auto x : values){
      norm += x * x;
    }
    return sqrt(norm);
  }
};


template<typename T, int size> struct Point : 
    public StructuredValues<T, size>{

  using StructuredValues<T, size>::StructuredValues;

  Point():StructuredValues<T,size>(){}
  Point(const Point &clone):StructuredValues<T,size>(clone){}
  Point(const vector<T> &_values):StructuredValues<T,size>(_values){}
  Point(StructuredValues<T,size> base):StructuredValues<T,size>(base){}

  Point operator*(real_type t) const{
    return StructuredValues<T,size>::operator*(t);
  }

};


template<typename T, int size>
Vector<T,size> operator-(const Point<T,size>& x, const Point<T,size>& y){
    Vector<T,size> v;
    for(int i = 0; i < size; ++i){
      v[i] = x.at(i) - y.at(i);
    }
    // std::cout << "- " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}


template<typename T, int size>
Point<T, size> operator+(const Point<T, size> x, const Vector<T,size> y){
    Point<T, size> v(x);
    for(int i = 0; i < size; ++i){
        v[i] += y.at(i);
    }
    // std::cout << "+ " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}

template<typename T, int size>
Point<T, size> operator+(const Point<T, size> x, const Point<T,size> y){
    Point<T, size> v(x);
    for(int i = 0; i < size; ++i){
        v[i] += y.at(i);
    }
    // std::cout << "+ " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}


template<typename T, int size>
Vector<T, size> operator+(const Vector<T, size> &x, const Vector<T,size> &y){
    Vector<T, size> v;
    for(int i = 0; i < size; ++i){
        v[i] = x.at(i) + y.at(i);
    }
    // std::cout << "+ " << x.toString() << " " << y.toString() << " " << v.toString() << std::endl;
    return v;
}

// Point aliases
using Point3i = Point<int, 3>;
using Point2i = Point<int, 2>;
using Point2f = Point<real_type, 2>;
using Point3f = Point<real_type, 3>;

// Vector aliases
using Vector3f = Vector<real_type, 3>;
using Vector3i = Vector<int, 3>;


// List of points
using ListPoint3f = std::vector<Point3f>;

class Normal3f : public Vector3f {
public:
  Normal3f():Vector3f(){}
  Normal3f(const vector<real_type> &_values):Vector3f(_values){}
  Normal3f(Vector3f base):Vector3f(base){}

};

class Ray {
public:
    Ray (const Point3f& _o, const Vector3f& _d ) : o{_o}, d{_d.normalize()} {}
    Point3f o; //!< origin
    Vector3f d; //!< direction
    Point3f operator()(real_type t) const { 
      return o + d * t; 
    }
};

}


#endif