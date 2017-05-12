# Field with Radius and Step

```C++
typedef double Value;
typedef double Coord;
typedef field::Field<Value, Coord> Field;

size_t radius = 5;
double step = 0.345;
Field field(radius, step);

for (auto i: field) { // enumerate through all points 
    auto p = i.coord; // access to point coordinate
    auto distance = sqrt(p.x*p.x + p.y*p.y);
    *i = distance;    // read/write access to field value
}

auto cellSize = 20;
Display("distance", cellSize).diplay(field, Display::Wait);
```

![](https://github.com/nut-code-monkey/field/blob/master/Screen%20Shot.png?raw=true)
