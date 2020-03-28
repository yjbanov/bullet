What if regular unannotated classes are supported and are always in the temporary heap?

Classes annotated to be persistent are the only ones that survive.

Plus structs, of course - non-null embedded objects.

Sketch:

```
class Point implements Struct {
  Point({this.x = 0, this.y = 0});

  final double x;
  final double y;

  int get hashCode => hashValues(x, y);

  bool operator==(Object other) {
    return other is Point && x == other.x && y == other.y;
  }
}

class TempRect {
  Point topLeft = Point();
  Point bottomRight = Point();
}

class Rect implements Persistent {
  Rect();
}
```
