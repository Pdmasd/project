#ifndef GAMEOBJECT_H_DEFINED
#define GAMEOBJECT_H_DEFINED

#include "shapes.h"

/// Xác định vị trí của đối tượng so với ranh giới
enum BoundaryStatus {
    INSIDE_LIMITS,
    OFF_TOP, OFF_BOTTOM, OFF_LEFT, OFF_RIGHT
};

/// Lớp trừu tượng cho tất cả các đối tượng trong game
class GameObject {
public:
    GameObject(float x = 0.0f, float y = 0.0f, int w = 0, int h = 0);
    virtual ~GameObject();

    /// Cập nhật trạng thái của đối tượng
    virtual void update(float dt) = 0;

    /// Vẽ đối tượng lên màn hình
    virtual void render(float cameraX, float cameraY) = 0;

    /// Kiểm tra va chạm với đối tượng khác
    bool collidedWith(const GameObject* other) const;

    /// Đặt đối tượng này lên trên đối tượng khác (chỉ theo trục Y)
    void placeOnTop(const GameObject* other);

    /// Đặt ranh giới di chuyển cho đối tượng
    void setBoundary(const Rectangle& boundary);

    /// Kiểm tra xem đối tượng có vượt khỏi ranh giới không
    BoundaryStatus actOnBoundaries();

    // Getter
    float getX() const;
    float getY() const;
    float getCenterX() const;
    float getCenterY() const;
    int getWidth() const;
    int getHeight() const;

    /// Hộp va chạm của đối tượng
    Rectangle box;

    /// Ranh giới di chuyển của đối tượng
    Rectangle boundary;
    BoundaryStatus boundaryStatus;
};

#endif /* GAMEOBJECT_H_DEFINED */
