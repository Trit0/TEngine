/**
 * @file Observer_Observable.hpp
 * @author Trito
 * @brief 
 * @version 0.1
 * @date 2021-09-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef OBSERVER_OBSERVABLE_HPP
#define OBSERVER_OBSERVABLE_HPP

#include <list>

using namespace std;

namespace GEngine {
    /**
     * @brief Classe représentant un observateur
     * 
     */
    class Observer{
        public:
            /**
             * @brief Appelé lorsque qu'un évènement se produit dans l'observé
             * 
             */
            virtual void Notification() = 0;
    };

    /**
     * @brief Classe représentant un observé
     * 
     */
    class Observable{
        private:
            list<Observer*> observers; //Liste des observateurs abonnés à l'observé 
        public:
            /**
             * @brief Rajoute un observateur à la liste d'observateur
             * 
             * @param myOberserver Observateur à rajouté
             */
            void AddObserver(Observer* myOberserver){
                this->observers.push_back(myOberserver);
            }

            /**
             * @brief Enlève un observateur de la liste d'observateur
             * 
             * @param myOberserver Observateur à retiré
             */
            void RemoveObserver(Observer* myOberserver){
                this->observers.remove(myOberserver);
            }

            /**
             * @brief Notifie tous les observateurs dans la liste des observateurs
             * 
             */
            void Notify(){
                for (auto o : observers){
                    o->Notification();
                }
            }
    };  
}
#endif
