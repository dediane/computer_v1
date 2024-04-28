#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    float a; // Coefficient for X^2
    float b; // Coefficient for X^1
    float c; // Coefficient for X^0
} Polynomial;

Polynomial parseEquation(const char* eq) {
    Polynomial p = {0, 0, 0};
    float coeff = 0;
    int exp = 0;
    char op;
    int side = 1; // 1 for left of '=', -1 for right

    while (*eq) {
        if (sscanf(eq, " %c %f * X^%d ", &op, &coeff, &exp) == 3) {
            coeff *= (op == '-') ? -1 : 1;
            coeff *= side;
            if (exp == 2) p.a += coeff;
            else if (exp == 1) p.b += coeff;
            else if (exp == 0) p.c += coeff;
        } else if (sscanf(eq, " %c %f * X^%d ", &op, &coeff, &exp) == 2) {
            coeff *= (op == '-') ? -1 : 1;
            coeff *= side;
            if (exp == 1) p.b += coeff;
            else if (exp == 0) p.c += coeff;
        } else if (sscanf(eq, " %c %f ", &op, &coeff) == 2) {
            coeff *= (op == '-') ? -1 : 1;
            coeff *= side;
            p.c += coeff;
        }

        if (*eq == '=') {
            side = -1; // Switch to the right side of the equation
        }
        while (*eq && *eq != ' ') eq++; // Skip to next term
        while (*eq == ' ') eq++; // Skip spaces
    }
    return p;
}

void solveQuadratic(Polynomial p) {
    float discriminant = p.b * p.b - 4 * p.a * p.c;
    if (discriminant > 0) {
        float root1 = (-p.b + sqrt(discriminant)) / (2 * p.a);
        float root2 = (-p.b - sqrt(discriminant)) / (2 * p.a);
        printf("Discriminant is strictly positive, the two solutions are:\n%.6f\n%.6f\n", root1, root2);
    } else if (discriminant == 0) {
        float root = -p.b / (2 * p.a);
        printf("Discriminant is zero, the solution is:\n%.6f\n", root);
    } else {
        printf("No real solutions.\n");
    }
}

void solveLinear(Polynomial p) {
    if (p.b != 0) {
        float root = -p.c / p.b;
        printf("The solution is:\n%.6f\n", root);
    } else {
        printf("No solution or infinite solutions.\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s 'equation'\n", argv[0]);
        return 1;
    }

    Polynomial p = parseEquation(argv[1]);

    printf("Reduced form: ");
    if (p.a != 0) printf("%.1f * X^2 ", p.a);
    if (p.b != 0) printf("%+.1f * X^1 ", p.b);
    if (p.c != 0) printf("%+.1f * X^0 ", p.c);
    printf("= 0\n");

    int degree = (p.a != 0) ? 2 : (p.b != 0) ? 1 : (p.c != 0) ? 0 : -1;
    printf("Polynomial degree: %d\n", degree);

    switch (degree) {
        case 2:
            solveQuadratic(p);
            break;
        case 1:
            solveLinear(p);
            break;
        case 0:
            printf("Every real number is a solution.\n");
            break;
        default:
            printf("The polynomial degree is strictly greater than 2, I can't solve.\n");
            break;
    }

    return 0;
}