#include "gameObject.h"

GameObject::GameObject(float x, float y, int w, int h)
    : box(x, y, w, h), boundary(-1, -1, -1, -1), boundaryStatus(INSIDE_LIMITS) {}

GameObject::~GameObject() {}

float GameObject::getX() const {
    return box.x;
}

float GameObject::getY() const {
    return box.y;
}

float GameObject::getCenterX() const {
    return box.center.x;
}

float GameObject::getCenterY() const {
    return box.center.y;
}

int GameObject::getWidth() const {
    return box.w;
}

int GameObject::getHeight() const {
    return box.h;
}

bool GameObject::collidedWith(const GameObject* other) const {
    // Vì overlaps không được khai báo const trong shapes.h, sử dụng const_cast để gọi nó.
    return const_cast<Rectangle&>(box).overlaps(other->box);
}

void GameObject::placeOnTop(const GameObject* other) {
    // Sử dụng const_cast để truyền đối số cho placeOnTop
    box.placeOnTop(const_cast<Rectangle*>(&other->box));
}

void GameObject::setBoundary(const Rectangle& newBoundary) {
    boundary = newBoundary;
}

BoundaryStatus GameObject::actOnBoundaries() {
    if (boundary.leftmost != -1 && box.leftmost < boundary.leftmost) {
        box.x = boundary.leftmost;
        box.update();
        return OFF_LEFT;
    }
    if (boundary.rightmost != -1 && box.rightmost > boundary.rightmost) {
        box.x = boundary.rightmost - box.w;
        box.update();
        return OFF_RIGHT;
    }
    if (boundary.top != -1 && box.top < boundary.top) {
        box.y = boundary.top;
        box.update();
        return OFF_TOP;
    }
    if (boundary.bottom != -1 && box.bottom > boundary.bottom) {
        box.y = boundary.bottom - box.h;
        box.update();
        return OFF_BOTTOM;
    }
    return INSIDE_LIMITS;
}
