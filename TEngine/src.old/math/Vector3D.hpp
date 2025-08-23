/**
 * @file Vector3D.hpp
 * @author Tristan de Brault
 * @brief Représentation d'un vecteur 3 doubles
 * @version 0.1
 * @date 2021-09-01
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP
#include <math.h>

#include <iostream>

using namespace std;

namespace GEngine {
/**
 *
 * @brief Représentation d'un vecteur ayant comme composantes 3 doubles
 * @class Vecteur3D
 */
class Vector3D {
   public:
    double x;  ///< Composante horizontal du vecteur
    double y;  ///< Composante vertical du vecteur
    double z;  ///< Composante en profondeur du vecteur

    /**
     * @param x Composante horizontal du vecteur
     * @param y Composante vertical du vecteur
     * @param z Composante en profondeur du vecteur
     */
    Vector3D(double x = 0, double y = 0, double z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /**
     * @brief Retourne la norme du vecteur
     * @return La norme du vecteur
     */
    double GetNorm() {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    /**
     * @brief Normalise un vecteur (sa norme devient égal à 1)
     */
    void Normalize() {
        double norm = this->GetNorm();
        if (norm != 0) {
            this->x /= norm;
            this->y /= norm;
            this->z /= norm;
        }
    }

    Vector3D GetNormalized() {
        Vector3D copy = Vector3D(x, y, z);
        copy.Normalize();
        return copy;
    }

    /**
     * @brief Effectue une somme vectorielle
     *
     * @param v Le vecteur à additionner
     * @return Le somme des 2 vecteurs
     */
    Vector3D operator+(const Vector3D& v) {
        return Vector3D(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    void operator+=(const Vector3D& v) {
        Vector3D v2 = *this + v;
        x = v2.x;
        y = v2.y;
        z = v2.z;
    }

    /**
     * @brief Effectue une différence vectorielle
     *
     * @param v Le vecteur à soustraire
     * @return Le différence des 2 vecteurs
     */
    Vector3D operator-(const Vector3D& v) {
        return Vector3D(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    /**
     * @brief Effectue le calcul du produit scalaire
     *
     * @param v Le vecteur à multiplier
     * @return Le produit scalaire des 2 vecteurs
     */
    double operator*(const Vector3D& v) {
        return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
    }

    /**
     * @brief Effectue le produit par un scalaire
     *
     * @param s Le scalaire par lequel un multiplie notre norme
     * @return Le nouveau vecteur multiplié par le scalaire
     */
    Vector3D operator*(double s) {
        return Vector3D(this->x * s, this->y * s, this->z * s);
    }

    /**
     * @brief Effectue la division par un scalaire
     *
     * @param s Le scalaire par lequel on divise notre norme
     * @return Le nouveau vecteur divisé par le scalaire
     */
    Vector3D operator/(double s) {
        return Vector3D(this->x / s, this->y / s, this->z / s);
    }

    /**
     * @brief Effectue le produit vectorielle
     *
     * @param v Le vecteur sur lequel on veut effectuer le produit vectorielle
     * @return Le vecteur orthogonale des 2 vecteurs
     */
    Vector3D operator%(const Vector3D& v) {
        return Vector3D((this->y * v.z) - (this->z * v.y), (this->z * v.x) - (this->x * v.z), (this->x * v.y) - (this->y * v.x));
    }

    /**
     * @brief Retourne l'angle entre 2 vecteurs
     *
     * @param v Le vecteur avec lequel on veut calculer l'angle
     * @return L'angle entre les 2 vecteurs
     */
    double GetAngle(Vector3D& v) {
        return acos((*this * v) / (this->GetNorm() * v.GetNorm()));
    }

    /**
     * @brief Inverse le vecteur
     *
     */
    Vector3D Inverse() {
        this->x = -this->x;
        this->y = -this->y;
        this->z = -this->z;
        return *this;
    }

    string toString() {
        string yo = "X :" + to_string(x) + " Y:" + to_string(y) + " Z:" + to_string(z);
        cout << yo << endl;
        return yo;
    }

    Vector3D NoY() {
        return Vector3D(this->x, 0.0, this->z);
    }

    Vector3D JustY() {
        return Vector3D(0.0, this->y, 0.0);
    }

    Vector3D JustX() {
        return Vector3D(x, 0, 0);
    }

    Vector3D JustZ() {
        return Vector3D(0, 0, z);
    }

    Vector3D BiggestDimension() {
        Vector3D copy = Vector3D(x, y, z);
        double absX = abs(x);
        double absY = abs(y);
        double absZ = abs(z);

        if (absX < absY || absX < absZ) {
            copy.x = 0;
        }

        if (absY < absX || absY < absZ) {
            copy.y = 0;
        }

        if (absZ < absX || absZ < absY) {
            copy.z = 0;
        }

        return copy;
    }

    static inline Vector3D Zero() {
        return Vector3D(0.0, 0.0, 0.0);
    }

    static inline Vector3D Identity() {
        return Vector3D(1.0, 1.0, 1.0);
    }

    static inline Vector3D Right() {
        return Vector3D(1.0, 0.0, 0.0);
    }

    static inline Vector3D Left() {
        return Vector3D(-1.0, 0.0, 0.0);
    }

    static inline Vector3D Up() {
        return Vector3D(0.0, 1.0, 0.0);
    }

    static inline Vector3D Down() {
        return Vector3D(0.0, -1.0, 0.0);
    }

    static inline Vector3D Back() {
        return Vector3D(0.0, 0.0, 1.0);
    }

    static inline Vector3D Front() {
        return Vector3D(0.0, 0.0, -1.0);
    }
};
}  // namespace GEngine
#endif