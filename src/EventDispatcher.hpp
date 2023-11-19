/**
 * @file EventDispatcher.hpp
 * @author Trito
 * @brief Dispatch des events
 * @version 0.1
 * @date 2021-09-22
 * @copyright Copyright (c) 2021
 */
#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP

#include <map>

#include "EventContainer.hpp"
#include "design/Observer_Observable.hpp"

namespace GEngine {
/**
 * @brief Classe servant à abonner, désabonner et notifier des observateurs d'évènement se produisant dans les observés
 *
 */
class EventDispatcher {
   private:
    std::map<unsigned int, Observable> events;  // Dictionnaire des différents évènements observés et d'un id pour les trouvés

   public:
    /**
     * @brief Abonne un observateur à un type d'évènement
     *
     * @param eventType Type d'évènement auquel l'obervateur s'abonne
     * @param observer Observateur qui s'abonne
     */
    void Bind(unsigned int eventType, Observer* observer) {
        events[eventType].AddObserver(observer);
    }

    /**
     * @brief Désabonne un observateur à un type d'évènement
     *
     * @param eventType Type d'évènement auquel l'obervateur se désabonne
     * @param observer Observateur qui se désabonne
     */
    void Unbind(unsigned int eventType, Observer* observer) {
        events[eventType].RemoveObserver(observer);
    }

    /**
     * @brief Notifie qu'un évènement s'est produit, tous ses observateurs
     *
     */
    void Dispatch() {
        if (events.find(EventContainer::GetType()) != events.end()) {
            events[EventContainer::GetType()].Notify();
        }
    }
};
}  // namespace GEngine

#endif