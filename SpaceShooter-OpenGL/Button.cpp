#include "Button.h"
#include "Engine.h"

void Button::create(const glm::vec2& position, const glm::vec2& size, string text, function<void()> callback)
{
    this->position = glm::vec3(position, 0.0f);
    this->size = glm::vec3(size, 1.0f);
    this->text = text;
    this->callback = callback;

    shared_ptr<Texture> whiteTexture = Engine::getTexture("../Data/Textures/white-tex.png");
    if (!whiteTexture)
    {
        std::cout << "Couldn't load healthbar texture!" << std::endl;
        return;
    }
    sprite = Sprite::create(*whiteTexture, position, size);
}

void Button::update(float deltaTime)
{
    hover = isMouseOver();
}

void Button::render()
{
    glm::vec4 color = backgroundColor;
    if (hover)
    {
        color = hoverColor;
    }
    if (pressed)
    {
        color = clickColor;
    }

    sprite->setColor(color);
    sprite->setPosition(position);
    sprite->setSize(size);
    sprite->draw();

    glm::vec2 textPos = glm::vec2(position.x - size.x / 8.0f, position.y - size.y / 8.0f);
    Engine::getInstance().renderText(text, textPos, 0.5f, glm::vec3(0, 0, 0));
}

void Button::onMouseButtonDown(int button)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && hover)
    {
        pressed = true;
    }
}

void Button::onMouseButtonUp(int button)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (pressed && hover)
        {
            callback();
        }
        pressed = false;
    }
}

bool Button::isMouseOver()
{
    const glm::vec2& position = sprite->getPosition();
    const glm::vec2& size = sprite->getSize();

    glm::vec2 mousePosition = Engine::getMousePositionInUISpace();

    glm::vec2 topLeft     = position - size / 2.0f;
    glm::vec2 bottomRight = position + size / 2.0f;

    if (mousePosition.x >= topLeft.x && mousePosition.x <= bottomRight.x &&
        mousePosition.y >= topLeft.y && mousePosition.y <= bottomRight.y)
    {
        return true;
    }

    return false;
}