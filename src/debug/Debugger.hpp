#pragma once

#include <map>
#include <string>

#include "../Text.hpp"
#include "../core/RenderComponent.hpp"
#include "../design/Singleton.hpp"
#include "../math/Vector3D.hpp"
#include "../resources/Color.hpp"

namespace GEngine {
class Debugger : public Singleton<Debugger> {
   private:
    std::map<std::string, Text*> prints;
    std::map<std::string, RenderComponent*> renderables;
    bool drawGrid = false;
    bool drawOnlyAxis = false;

   protected:
    void DrawGrid() {
        glDisable(GL_TEXTURE_2D);
        Vector3D leftMax = Vector3D::Left() * 5000;
        Vector3D rightMax = Vector3D::Right() * 5000;
        DrawLine(leftMax, rightMax, Color(1.0, 0.0, 0.0, 1.0));
        if (!drawOnlyAxis) {
            for (int i = -100; i < 100; i++) {
                leftMax.z = i;
                rightMax.z = i;
                DrawLine(leftMax, rightMax, Color(1.0, 1.0, 1.0, 0.3));
            }
        }

        Vector3D downMax = Vector3D::Down() * 5000;
        Vector3D upMax = Vector3D::Up() * 5000;
        DrawLine(downMax, upMax, Color(0.0, 1.0, 0.0, 1.0));

        Vector3D backMax = Vector3D::Back() * 5000;
        Vector3D frontMax = Vector3D::Front() * 5000;
        DrawLine(backMax, frontMax, Color(0.0, 0.0, 1.0, 1.0));
        if (!drawOnlyAxis) {
            for (int i = -100; i < 100; i++) {
                backMax.x = i;
                frontMax.x = i;
                DrawLine(backMax, frontMax, Color(1.0, 1.0, 1.0, 0.3));
            }
        }
        glEnable(GL_TEXTURE_2D);
    }

   public:
    void Set(std::string key, std::string value) {
        if (prints.find(key) == prints.end()) {
            prints[key] = new Text(value, "../res/monogram.ttf", 20);
        } else {
            prints[key]->SetText(value);
        }
    }

    void DrawPrints() {
        double height = 0;
        for (auto print : prints) {
            print.second->Draw(0.0, height);
            height += print.second->GetFontHeight();
        }
    }

    void SetRenderComponent(std::string key, Transform* transform, RenderComponent* component) {
        component->SetGameObjectTransform(transform);
        component->Init();
        transform->Notify();
        if (renderables.find(key) == renderables.end()) {
            renderables[key] = component;
        } else {
            RenderComponent* old = renderables[key];
            Transform* oldTransform = old->GetGameObjectTransform();
            old->Destroy();
            delete oldTransform;
            delete old;
            renderables[key] = component;
        }
    }

    void DrawRenderables() {
        if (drawGrid) {
            DrawGrid();
        }

        for (auto component : renderables) {
            component.second->Render(RenderHandler::GetInstance().GetRenderConfig());
        }
    }

    void DrawLine(Vector3D start, Vector3D end, Color color) {
        glBegin(GL_LINES);
        glColor4d(color.GetR(), color.GetG(), color.GetB(), color.GetA());

        glVertex3d(start.x, start.y, start.z);
        glVertex3d(end.x, end.y, end.z);

        glColor4d(1.0, 1.0, 1.0, 1.0);
        glEnd();
    }

    void SetDrawGrid(bool drawGrid) {
        this->drawGrid = drawGrid;
    }

    void SetDrawAxisOnly(bool drawAxisOnly) {
        this->drawOnlyAxis = drawAxisOnly;
    }
};
}  // namespace GEngine
