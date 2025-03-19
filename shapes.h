#ifndef SHAPES_H_DEFINED
#define SHAPES_H_DEFINED

/// Điểm
class Point {
public:
    float x;
    float y;

    Point();
    Point(float x, float y);
};

/// Hình chữ nhật
struct Rectangle {
    float x, y; ///< Tọa độ góc trên bên trái
    int w, h;   ///< Chiều rộng và chiều cao

    // Các alias(bí danh) để dễ dàng truy cập cạnh hình chữ nhật
    float top, bottom, leftmost, rightmost;

    Point center, leftCenter, rightCenter;
    Point topLeft, topRight, topCenter;
    Point bottomLeft, bottomRight, bottomCenter;

    Rectangle();
    Rectangle(float x, float y, int w, int h);
    Rectangle(const Point& topLeft, const Point& bottomRight); ///< Constructor từ hai điểm góc đối diện
    Rectangle(Rectangle* other);                         ///< Constructor sao chép.

    Rectangle& operator=(const Rectangle& other);   //giúp sao chép một hình chữ nhật này sang hình chữ nhật khác.

    void copy(Rectangle* other); ///< Sao chép dữ liệu từ hình chữ nhật khác
    void addX(float dx); ///< Di chuyển hình chữ nhật theo trục X
    void addY(float dy); ///< Di chuyển hình chữ nhật theo trục Y
    void setX(float newX); ///< Đặt lại tọa độ X
    void setY(float newY); ///< Đặt lại tọa độ Y
    void stretch(float scale); ///< Co giãn hình chữ nhật theo tỷ lệ
    void stretch(float scaleX, float scaleY); ///< Co giãn theo hai chiều riêng biệt
    void update(); ///< Cập nhật lại các điểm quan trọng sau khi thay đổi giá trị
    bool overlaps(Rectangle other) ; ///< Kiểm tra xem hai hình chữ nhật có giao nhau không
    bool overlaps(Rectangle* other);
    void placeOnTop(Rectangle* other); ///< Di chuyển hình chữ nhật lên trên một hình chữ nhật khác
};

/// Hình tròn
struct Circle {
    float x, y; ///< Tọa độ tâm
    int r; ///< Bán kính

    float centerX, centerY; ///< Tọa độ tâm được lưu trữ để dễ truy cập

    Circle(float x, float y, int r); ///< Constructor với tọa độ và bán kính
    void addX(float dx); ///< Di chuyển hình tròn theo trục X
    void addY(float dy); ///< Di chuyển hình tròn theo trục Y
};

#endif /* SHAPES_H_DEFINED */


