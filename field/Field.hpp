//
//  Field.hpp
//
//  Copyright © 2017 Max Lunin. All rights reserved.
//

#ifndef Field_hpp
#define Field_hpp

#include <vector>

namespace field {
    
    template <typename T>
    T linear_map(T x, T in_min, T in_max, T out_min, T out_max){
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    
    template <typename T>
    T linear_map(T x, T in_min, T in_max){
        return linear_map(x, in_min, in_max, 0, 1);
    }

    template <typename Value, typename Coord>
    class Field{
    public:
        typedef std::vector<Value> Array;
        typedef ptrdiff_t Int;
        typedef size_t UInt;
    private:
        UInt _radius;
        Coord _step;
        UInt _size;
        Array _field;
    public:
        Field(const Field& f)
        : _radius(f._radius)
        , _step(f._step)
        , _size(f._size)
        , _field(f._field) {};
        
        Field(Field&& f)
        : _radius(f._radius)
        , _step(f._step)
        , _size(f._size)
        , _field(std::move(f._field)) {};
        
        Field(UInt radius, Coord step)
        : _radius(radius)
        , _step(step)
        , _size(2*radius+1)
        , _field(Array(_size * _size, 0)) {}
        
        Field newSameSize(){
            Field newField(_radius, _step);
            return std::move(newField);
        }
        
        Field& operator=(Field&& other) {
            _radius = other._radius;
            _step = other._step;
            _size = other._size;
            _field = std::move(other._field);
            return *this;
        }
        
        inline
        UInt step() const {
            return _step;
        }
        
        inline
        UInt radius() const {
            return _radius;
        }
        
        inline
        UInt size() const {
            return _size;
        }
        
        inline
        const Array& field() const {
            return _field;
        }
        
        inline
        Array& field() {
            return _field;
        }
        
        inline
        Coord from() const {
            return -static_cast<Coord>(_radius);
        }
        
        inline
        Coord to() const {
            return static_cast<Coord>(_radius);
        }
        
        struct Point{
            Coord x, y;
            Point(Coord x = 0, Coord y = 0): x(x), y(y) {}
            
            inline
            Point operator+(const Point& p) const {
                return Point(x+p.x, y+p.y);
            }
            
            Point operator+=(const Point& p){
                x+=p.x;
                y+=p.y;
                return *this;
            }
            
            Point& operator+=(Coord p){
                x+=p;
                y+=p;
                return *this;
            }
            
            inline
            bool operator==(const Point& p) const {
                return x==p.x && y==p.y;
            }
            
            inline
            bool operator!=(const Point& p) const {
                return !(*this == p);
            }
        };
        
        struct IteratorValue{
            Point coord;
        private:
            typename Array::iterator _it;
        public:
            IteratorValue(Coord x, Coord y, typename Array::iterator it): coord(Point{x, y}), _it(it) {}
            IteratorValue(const IteratorValue& i) : coord(i.coord), _it(i._it) {}
            
            IteratorValue& operator=(const IteratorValue& i) {
                coord = i.coord;
                _it = i._it;
                return *this;
            };
            
            Value& operator *(){
                return *_it;
            }
            
            const Value& operator *() const {
                return *_it;
            }
            
            inline
            bool operator < (const IteratorValue& i) const {
                return *_it < *i._it;
            }
        };
        
        struct Iterator : public std::iterator<std::forward_iterator_tag, IteratorValue> {
            typedef IteratorValue value_type;
            
            Iterator(typename Array::iterator it, UInt size, Coord step, UInt i, UInt j):
            _it(it), size(size), step(step), i(i), j(j)
            {}
            
            Iterator(const Iterator&) = default;
            Iterator(Iterator&&) = default;
            Iterator& operator = (const Iterator&) = default;
            
            Iterator& operator ++ () {
                ++_it;
                ++j;
                if(j >= size){
                    ++i;
                    j = 0;
                }
                return *this;
            }
            
            inline
            bool operator != (const Iterator& other) const { return _it != other._it; }
            
            inline
            bool operator == (const Iterator& other) const {
                return _it == other._it;
            }
            
            IteratorValue operator * () const {
                Coord from = (((Coord)(size-1))/2.) * step;
                Coord y = (Coord)i*step - from;
                Coord x = (Coord)j*step - from;
                return IteratorValue{x, y, _it};
            }
            
        private:
            typename Array::iterator _it;
            size_t size, i, j;
            Coord step;
        };
        
        
        
        
        
        Field& max_of(const Field& other){
            for (UInt i = 0; i < _field.size(); ++i) {
                _field[i] += other.field()[i];
            }
            return *this;
        }
        
        Field& operator += (const Field &f){
            for (UInt i = 0; i < _field.size(); ++i) {
                _field[i] += f.field()[i];
            }
            return *this;
        }
        
        Field& setZero(){
            for(auto& i: _field){
                i = 0.;
            }
            return *this;
        }
        
        Field& cut(Value from = 0, Value to = 1){
            for(auto& i: _field){
                i = std::min(i, to);
                i = std::max(i, from);
            }
            return *this;
        }
        
        Field& normalize(Value from = 0, Value to = 1){
            auto mm = std::minmax_element(std::begin(_field), std::end(_field));
            Value in_min = *mm.first;
            Value in_max = *mm.second;
            
            if(in_min == in_max){ 
                return *this;
            }
            
            for(auto& i: _field){
                i = linear_map(i, in_min, in_max, from, to);
            }
            return *this;
        }

        inline
        const Value& operator ()(UInt y, UInt x) const {
            return _field.at(y * _size + x);
        }
        
        inline
        Value& operator ()(UInt y, UInt x){
            return _field.at(y * _size + x);
        }
        
        Value& operator ()(const Point& p){
            Coord from = ((Coord)(_size-1))/2.;
            UInt i = from + round(p.y / _step);
            UInt j = from + round(p.x / _step);
            return operator()(i, j);
        }
        
        Iterator begin() {
            return Iterator(_field.begin(), _size, _step, 0, 0);
        }
        
        Iterator end() {
            return Iterator(_field.end(), _size, _step, _size, _size);
        }
        
        
        static const IteratorValue& MinValue(const IteratorValue& a, const IteratorValue& b) {
            return a.value < b.value ? a : b;
        }

        static const IteratorValue& MaxValue(const IteratorValue& a, const IteratorValue& b) {
            return a.value > b.value ? a : b;
        }
    };

    typedef Field<double, double> FieldDouble;
    typedef Field<float, float> FieldFloat;
}

#endif
