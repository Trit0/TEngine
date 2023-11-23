#ifndef COLOR_HPP
#define COLOR_HPP

namespace GEngine {
class Color {
   private:
    /**
     * Variante rouge
     */
    double r;

    /**
     * Variante verte
     */
    double g;

    /**
     * Variante bleu
     */
    double b;

    /**
     * Variante alpha
     */
    double a;

   public:
    Color(double r, double g, double b, double a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        this->r = r / 255.0;
        this->g = g / 255.0;
        this->b = b / 255.0;
        this->a = a / 255.0;
    }

    Color(const Color& color) {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
        this->a = color.a;
    }

    Color(unsigned int hex) {
        unsigned char* hexPtr = (unsigned char*)&hex;
        r = hexPtr[3] / 255.0;
        g = hexPtr[2] / 255.0;
        b = hexPtr[1] / 255.0;
        a = hexPtr[0] / 255.0;
    }

    Color() {
        r = 1.0;
        g = 1.0;
        b = 1.0;
        a = 1.0;
    }

    /**
     * \brief Accesseur de la variante rouge
     * \return Variante rouge
     */
    inline double GetR() const {
        return r;
    }

    /**
     * \brief Accesseur de la variante verte
     * \return Variante verte
     */
    inline double GetG() const {
        return g;
    }

    /**
     * \brief Accesseur de la variante bleu
     * \return Variante bleu
     */
    inline double GetB() const {
        return b;
    }

    /**
     * \brief Accesseur de la variante alpha
     * \return Variante alpha
     */
    inline double GetA() const {
        return a;
    }

    inline Color WithA(double a) {
        return Color(r, g, b, a);
    }

    /**
     * \inherit
     */
    inline string getType() {
        return "Color";
    }

    inline bool operator==(const Color& other) const {
        return other.r == r && other.b == b && other.a == a && other.g == g;
    }

    inline bool operator!=(const Color& other) const {
        return !(*this == other);
    }
};

class Colors {
   public:
    static inline Color AQUA() {
        return Color(0.0, 1.0, 1.0, 1.0);
    }
    static inline Color BLACK() {
        return Color(0.0, 0.0, 0.0, 1.0);
    }
    static inline Color BLUE() {
        return Color(0.0, 0.0, 1.0, 1.0);
    }
    static inline Color GREEN() {
        return Color(0.0, 0.5, 0.0, 1.0);
    }
    static inline Color GREY() {
        return Color(0.5, 0.5, 0.5, 1.0);
    }
    static inline Color LIME() {
        return Color(0.0, 1.0, 0.0, 1.0);
    }
    static inline Color MAROON() {
        return Color(0.5, 0.0, 0.0, 1.0);
    }
    static inline Color NAVY() {
        return Color(0.0, 0.0, 0.5, 1.0);
    }
    static inline Color OLIVE() {
        return Color(0.5, 0.5, 0.0, 1.0);
    }
    static inline Color ORANGE() {
        return Color(1.0, 0.5, 0.0, 1.0);
    }
    static inline Color PINK() {
        return Color(1.0, 0.5, 1.0, 1.0);
    }
    static inline Color PURPLE() {
        return Color(1.0, 0.0, 1.0, 1.0);
    }
    static inline Color RED() {
        return Color(1.0, 0.0, 0.0, 1.0);
    }
    static inline Color TEAL() {
        return Color(0.0, 0.5, 0.5, 1.0);
    }
    static inline Color WHITE() {
        return Color(1.0, 1.0, 1.0, 1.0);
    }
    static inline Color YELLOW() {
        return Color(1.0, 1.0, 0.0, 1.0);
    }
};

}  // namespace GEngine

#endif
