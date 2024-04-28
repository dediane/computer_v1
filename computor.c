#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DEGREE 10  

void update_coefficients(char *token, double sign, double coeffs[], int *degree) {
    double coeff = atof(token); 
    token = strtok(NULL, "* ");  
    int exp = 0; 
    if (token && strstr(token, "X^")) {
        token = strchr(token, '^') + 1;
        exp = atoi(token);
    }
    coeffs[exp] += sign * coeff; 
    if (exp > *degree) *degree = exp;  
}

void parse_coefficients(char *equation, double coeffs[], int *degree, double sign) {
    char *token = strtok(equation, " ");
    while (token) {
        if (strcmp(token, "+") == 0) {
            sign = 1.0;  
        } else if (strcmp(token, "-") == 0) {
            sign = -1.0; 
        } else {
            update_coefficients(token, sign, coeffs, degree);
        }
        token = strtok(NULL, " ");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./computor \"equation\"\n");
        return 1;
    }

    char buffer[1024];  // Buffer pour copier l'équation
    strncpy(buffer, argv[1], sizeof(buffer));

    double coeffs[MAX_DEGREE + 1] = {0};  
    int degree = 0; 

    char *left = strtok(buffer, "=");
    char *right = strtok(NULL, "=");

    // Process left and right sides
    parse_coefficients(left, coeffs, &degree, 1.0); 
    parse_coefficients(right, coeffs, &degree, -1.0);

    printf("Reduced form:");
    for (int i = 0; i <= degree; i++) {
        if (i == 0 || coeffs[i] != 0) {
            printf(" %+.1f * X^%d", coeffs[i], i); 
        }
    }
    printf(" = 0\n");
    printf("Polynomial degree: %d\n", degree);

   //On resout l'équation selon le degré que l'on a trouvé
    if (degree == 0) {

        printf("Every real number is a solution.\n");
    }
    if (degree == 1) {
        double solution = -coeffs[0] / coeffs[1];
        printf("The solution is:\n%.5f\n", solution);
    } else if (degree == 2) {
        double a = coeffs[2], b = coeffs[1], c = coeffs[0];
        double discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            double x1 = (-b + sqrt(discriminant)) / (2 * a);
            double x2 = (-b - sqrt(discriminant)) / (2 * a);
            printf("Discriminant is strictly positive, the two solutions are:\n%.6f\n%.6f\n", x2, x1);
        } else if (discriminant == 0) {
            double x = -b / (2 * a);
            printf("Discriminant is zero, the only solution is:\n%.6f\n", x);
        } else {
            printf("Discriminant is negative, there are no real solutions.\n");
        }
    } else if (degree > 2) {
        printf("The polynomial degree is strictly greater than 2, I can't solve.\n");
    }
    return 0;
}