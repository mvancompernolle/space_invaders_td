#include "Entity.h"

Entity::Entity( unsigned components ) : componentTypes(components), keyboard(this) {
}

Entity::~Entity() {
}
